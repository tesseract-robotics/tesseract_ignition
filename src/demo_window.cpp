
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

  // Load plugins / config
  if (!app.LoadPlugin("TesseractScene3D"))
  {
    return 1;
  }

  // Customize main window
  auto win = app.findChild<ignition::gui::MainWindow *>()->QuickWindow();
  win->setProperty("title", "Hello Window!");

  // Run window
  app.exec();

  std::cout << "After run" << std::endl;

  return 0;
}
