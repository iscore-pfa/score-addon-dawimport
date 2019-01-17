#pragma once
#include <score/plugins/qt_interfaces/CommandFactory_QtInterface.hpp>
#include <score/plugins/qt_interfaces/GUIApplicationPlugin_QtInterface.hpp>
#include <score/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>

class score_addon_dawimport final : public score::Plugin_QtInterface,
                                   public score::ApplicationPlugin_QtInterface,
                                   public score::CommandFactory_QtInterface
{
  SCORE_PLUGIN_METADATA(1, "aa132783-aba3-4e9f-b1f0-8d39c9878203")

public:
  score_addon_dawimport();
  ~score_addon_dawimport() override;

private:
  std::pair<const CommandGroupKey, CommandGeneratorMap>
  make_commands() override;

  score::GUIApplicationPlugin* make_guiApplicationPlugin(
          const score::GUIApplicationContext& app) override;
};
