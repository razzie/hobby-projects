/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program;

import java.util.ArrayList;
import java.awt.event.*;
import javax.swing.JTabbedPane;
import Interface.Renderer;

public class Simulate implements ActionListener { // szimulálás osztály
    private JTabbedPane Workspace;

    public Simulate(JTabbedPane Workspace) {
        this.Workspace = Workspace;
    }

    @Override
    public void actionPerformed(ActionEvent evt) { // akutális munkalapon lévő elemek szimulálása (időzítő hívja meg)
        ArrayList<Gate> gates;
        Renderer r = (Renderer)(Workspace.getSelectedComponent());
        if ( r != null ) {
            gates = r.project().getList();
            for ( Gate g : gates ) g.Simulate();
        }
    }
}
