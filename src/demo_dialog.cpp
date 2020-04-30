#include <iostream>

#include <ignition/common/Console.hh>

#ifndef Q_MOC_RUN
  #include <ignition/gui/qt.h>
  #include <ignition/gui/Application.hh>
  #include <ignition/gui/Dialog.hh>
#endif

//////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Increase verboosity so we see all messages
  ignition::common::Console::SetVerbosity(4);

  // Initialize app
  ignition::gui::Application app(_argc, _argv,
      ignition::gui::WindowType::kDialog);

  // Load plugins / config
  if (!app.LoadPlugin("TesseractScene3D"))
  {
    return 1;
  }

  // TODO(chapulina): fix multiple dialogs
  // app.LoadPlugin("TopicEcho");

  // Run dialogs
  app.exec();

  std::cout << "After run" << std::endl;

  return 0;
}
