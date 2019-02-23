#pragma once
#include <score/command/AggregateCommand.hpp>
#include <score/plugins/application/GUIApplicationPlugin.hpp>
namespace DawImport
{
class ApplicationPlugin : public QObject, public score::GUIApplicationPlugin
{
public:
  ApplicationPlugin(const score::GUIApplicationContext& app);

private:
  score::GUIElements makeGUIElements() override;

  QAction* m_generate{};

  void generate();
};

inline const CommandGroupKey& CommandFactoryName()
{
  static const CommandGroupKey key{"Loop"};
  return key;
}
class GenerateAScore final : public score::AggregateCommand
{
  SCORE_COMMAND_DECL(CommandFactoryName(), GenerateAScore, "Generate a score")
};
}
