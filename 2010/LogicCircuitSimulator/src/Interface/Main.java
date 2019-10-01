/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import Interface.Forms.Message;
import Interface.Forms.Window;
import javax.swing.UIManager;

public class Main {

    public static void main(String[] args) {

        try {
            // az ablak elemeit a futtató rendszer témájára állítja
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        }
        catch (Exception e) {
            Message.dropError("Could not set OS look and feel :(");
        }

        Renderer.debug = false;

        // főablak megnyitása (ezzel indul a program)
        new Window();
    }

}
