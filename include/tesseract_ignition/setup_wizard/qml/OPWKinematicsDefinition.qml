import QtQuick 2.4

OPWKinematicsDefinitionForm {
    addOPWKinematicsButton.onClicked: {
        var a1 = 0.0;
        var a2 = 0.0;
        var b = 0.0;
        var c1 = 0.0;
        var c2 = 0.0;
        var c3 = 0.0;
        var c4 = 0.0;
        var o1 = 0.0;
        var o2 = 0.0;
        var o3 = 0.0;
        var o4 = 0.0;
        var o5 = 0.0;
        var o6 = 0.0;
        var sc1 = 1;
        var sc2 = 1;
        var sc3 = 1;
        var sc4 = 1;
        var sc5 = 1;
        var sc6 = 1;


        if (a1TextField.acceptableInput)
            a1 = a1TextField.text;

        if (a2TextField.acceptableInput)
            a2 = a2TextField.text;

        if (bTextField.acceptableInput)
            b = bTextField.text;

        if (c1TextField.acceptableInput)
            c1 = c1TextField.text;

        if (c2TextField.acceptableInput)
            c2 = c2TextField.text;

        if (c3TextField.acceptableInput)
            c3 = c3TextField.text;

        if (c4TextField.acceptableInput)
            c4 = c4TextField.text;

        if (jointOffsetTextField.acceptableInput)
            o1 = jointOffsetTextField.text;

        if (jointOffsetTextField1.acceptableInput)
            o2 = jointOffsetTextField1.text;

        if (jointOffsetTextField2.acceptableInput)
            o3 = jointOffsetTextField2.text;

        if (jointOffsetTextField3.acceptableInput)
            o4 = jointOffsetTextField3.text;

        if (jointOffsetTextField4.acceptableInput)
            o5 = jointOffsetTextField4.text;

        if (jointOffsetTextField5.acceptableInput)
            o6 = jointOffsetTextField5.text;

        if (jointCorrectionSwitch.position === 1)
            sc1 = -1;

        if (jointCorrectionSwitch1.position === 1)
            sc2 = -1;

        if (jointCorrectionSwitch2.position === 1)
            sc3 = -1;

        if (jointCorrectionSwitch3.position === 1)
            sc4 = -1;

        if (jointCorrectionSwitch4.position === 1)
            sc5 = -1;

        if (jointCorrectionSwitch5.position === 1)
            sc6 = -1;


        TesseractSetupWizard.onAddGroupOPWKinematics(
                               opwKinematicsGroupNameComboBox.currentText, a1, a2, b, c1, c2, c3, c4,
                               o1, o2, o3, o4, o5, o6, sc1, sc2, sc3, sc4, sc5, sc6)
    }

    removeOPWKinematicsButton.onClicked: {
        TesseractSetupWizard.onRemoveGroupOPWKinematics(opwKinematicsTableView.currentRow)
    }
}
