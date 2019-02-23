#include "score_addon_dawimport.hpp"

#include <score/plugins/FactorySetup.hpp>

#include <DawImport/ApplicationPlugin.hpp>
#include <score_addon_dawimport_commands_files.hpp>

score_addon_dawimport::score_addon_dawimport() {}

score_addon_dawimport::~score_addon_dawimport() {}

score::GUIApplicationPlugin* score_addon_dawimport::make_guiApplicationPlugin(
    const score::GUIApplicationContext& app)
{
  return new DawImport::ApplicationPlugin{app};
}

std::pair<const CommandGroupKey, CommandGeneratorMap>
score_addon_dawimport::make_commands()
{
  using namespace DawImport;
  std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{
      CommandFactoryName(), CommandGeneratorMap{}};

  ossia::for_each_type<
#include <score_addon_dawimport_commands.hpp>
      >(score::commands::FactoryInserter{cmds.second});

  return cmds;
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_dawimport)
