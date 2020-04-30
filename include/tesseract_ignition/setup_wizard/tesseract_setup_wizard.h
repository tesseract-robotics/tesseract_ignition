#ifndef TESSERACT_IGNITION_TESSERACT_SETUP_WIZARD_H
#define TESSERACT_IGNITION_TESSERACT_SETUP_WIZARD_H

#ifndef Q_MOC_RUN
  #include <ignition/gui/qt.h>
  #include <ignition/gui/Plugin.hh>
  #include <QQuickWindow>
#endif

namespace tesseract_ignition
{
  namespace gui
  {
    namespace plugins
    {
      class TesseractSetupWizardPrivate;
      class TesseractSetupWizard : public ignition::gui::Plugin
      {
        Q_OBJECT
      public:

        TesseractSetupWizard();
        ~TesseractSetupWizard() override;

        void LoadConfig( const tinyxml2::XMLElement * _pluginElem) override;

      public slots:
        /**
         * @brief Callback trigged when the load button is pressed.
         * @param[in] _urdf_filepath The urdf file path.
         * @param[in] _srdf_filepath The srdf file path.
         */
        void onLoad(const QString &_urdf_filepath, const QString& _srdf_filepath);

      protected slots:
        void updateTesseract();

        /** @brief Used to filter scroll wheel events.
          * @param[in] _o Object that receives the event.
          * @param[in] _event Pointer to the event.
          * @return True if event was handled.
          */
        bool eventFilter(QObject *_o, QEvent *_event) override;

      private:
        /** @brief Pointer to private data */
        std::unique_ptr<TesseractSetupWizardPrivate> data_;
      };
    }
  }
}

#endif
