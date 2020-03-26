#include <Media/ApplicationPlugin.hpp>
#include <Media/Commands/InsertVST.hpp>
#include <Media/Effect/VST/VSTEffectModel.hpp>
#include <Media/SynthChain/SynthChainModel.hpp>
#include <Media/AudioChain/AudioChainModel.hpp>
#include <Media/Sound/SoundModel.hpp>
#include <Midi/Commands/AddNote.hpp>
#include <Midi/MidiProcess.hpp>
#include <Scenario/Commands/CommandAPI.hpp>
#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>
#include <Scenario/Process/Algorithms/Accessors.hpp>
#include <Scenario/Process/Algorithms/ContainersAccessors.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <score/actions/Menu.hpp>
#include <score/actions/MenuManager.hpp>
#include <score/document/DocumentInterface.hpp>

#include <core/document/Document.hpp>

#include <QMenu>

#include <DawImport/ApplicationPlugin.hpp>

namespace DawImport
{
static void generateScore(
    const Scenario::ScenarioDocumentModel& root,
    const Scenario::ProcessModel& scenar,
    Scenario::Command::Macro& macro,
    const score::GUIApplicationContext& context)
{
  using namespace std::literals;

  // Création d'une structure de base
  constexpr double y = 0.02;
  auto& s1 = macro.createState(scenar, scenar.startEvent().id(), y);

  const auto& [t2, e2, s2] = macro.createDot(scenar, {2s, y});
  const auto& i1 = macro.createInterval(scenar, s1.id(), s2.id());

  // Ajoutons un premier process
  auto& midi = macro.createProcessInNewSlot<Midi::ProcessModel>(i1, {});

  std::vector<Midi::NoteData> notes;
  notes.emplace_back(0.2, 0.1, 64, 100);
  notes.emplace_back(0.2, 0.1, 68, 100);
  notes.emplace_back(0.2, 0.1, 72, 30);
  notes.emplace_back(0.4, 0.5, 70, 100);
  macro.submit(new Midi::ReplaceNotes(midi, notes, 55, 85, 2500ms));

  // Et un autre
  const auto& vsts
      = context.applicationPlugin<Media::ApplicationPlugin>().vst_infos;
  auto& fx_chain
      = macro.createProcessInNewSlot<Media::SynthChain::ProcessModel>(i1, {});

  // On va voir les VSTs qu'il y a sur le système
  for (const auto& vst : vsts)
  {
    // On ajoute le premier synthé qu'on trouve
    // (J'ai désactivé Carla car c'est un VST qui bug chez moi...)
    if (vst.isSynth && vst.isValid && !vst.prettyName.contains("Carla"))
    {
      macro.submit(new Media::InsertGenericEffect<Media::VST::VSTEffectModel>(
          fx_chain, QString::number(vst.uniqueID), 0));
      break;
    }
  }

  // On connecte un cable entre le midi et le vst
  {
    Process::CableData cable;
    cable.source = *midi.outlet;
    cable.sink = *fx_chain.inlet;
    cable.type = Process::CableType::ImmediateGlutton;
    macro.createCable(root, std::move(cable));
  }

  // Et un fichier son
  const auto& i2 = macro.createIntervalAfter(scenar, s2.id(), {5000ms, y});
  macro.createProcessInNewSlot<Media::Sound::ProcessModel>(
      i2, PLUGIN_SOURCE_DIR "/DawImport/resources/test.wav");

  macro.commit();
}

ApplicationPlugin::ApplicationPlugin(const score::GUIApplicationContext& app)
    : score::GUIApplicationPlugin{app}
{
  m_generate = new QAction{tr("Generate a score"), nullptr};
  connect(m_generate, &QAction::triggered, this, &ApplicationPlugin::generate);
}

score::GUIElements ApplicationPlugin::makeGUIElements()
{
  auto& m = context.menus.get().at(score::Menus::Export());
  QMenu* menu = m.menu();
  menu->addAction(m_generate);
  return {};
}

void ApplicationPlugin::generate()
{
  auto doc = currentDocument();
  if (!doc)
    return;
  Scenario::ScenarioDocumentModel& base
      = score::IDocument::get<Scenario::ScenarioDocumentModel>(*doc);

  const auto& baseInterval = base.baseScenario().interval();
  if (baseInterval.processes.size() == 0)
    return;

  auto firstScenario = dynamic_cast<Scenario::ProcessModel*>(
      &*baseInterval.processes.begin());
  if (!firstScenario)
    return;

  Scenario::Command::Macro m{new GenerateAScore, doc->context()};
  generateScore(base, *firstScenario, m, this->context);
}
}
