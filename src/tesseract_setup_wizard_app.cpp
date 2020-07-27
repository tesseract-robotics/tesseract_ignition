
#ifndef Q_MOC_RUN
  #include <ignition/gui/qt.h>
  #include <ignition/gui/Application.hh>
  #include <ignition/gui/MainWindow.hh>
#endif

//////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Increase verboosity so we see all messages
  ignition::common::Console::SetVerbosity(4);

  // Create app
  ignition::gui::Application app(_argc, _argv);

  // Set orginization information for QSettings
  app.setOrganizationName("Southwest Research Institute");
  app.setOrganizationDomain("SwRI");

  const char *env = std::getenv("TSW_CONFIG_PATH");
  std::string tsw_config_path = (env) ? std::string(env) :
      TSW_CONFIG_PATH;

  if (!app.LoadConfig(tsw_config_path))
    return 1;

  // Customize main window
  auto win = app.findChild<ignition::gui::MainWindow *>()->QuickWindow();
  win->setProperty("title", "Tesseract Setup Wizard!");

  // Run window
  app.exec();

  return 0;
}
