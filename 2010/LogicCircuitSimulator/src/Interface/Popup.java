/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import java.awt.event.*;
import javax.swing.JPopupMenu;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import Program.Gate;
import Program.Gates.*;

public class Popup extends JPopupMenu {
    private int menuX, menuY;

    public Popup() {
        JMenuItem Button1 = add("Toggle Button");
        Button1.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Button(Button.ButtonType.ToggleButton));
            }
        });

        JMenuItem Button2 = add("Push Button");
        Button2.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Button(Button.ButtonType.PushButton));
            }
        });

        JMenuItem And = add("AND gate");
        And.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new And());
            }
        });

        JMenuItem Or = add("OR gate");
        Or.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Or());
            }
        });

        JMenuItem Not = add("NOT gate");
        Not.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Not());
            }
        });

        JMenuItem Xor = add("XOR gate");
        Xor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Xor());
            }
        });

        JMenuItem Led = add("LED");
        Led.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Led());
            }
        });

        JMenuItem Timer = add("Timer");
        Timer.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Timer());
            }
        });

        JMenu submenu = new JMenu("Others");
        add(submenu);

        JMenuItem Nand = submenu.add("NAND gate");
        Nand.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Nand());
            }
        });

        JMenuItem Nor = submenu.add("NOR gate");
        Nor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Nor());
            }
        });

        JMenuItem Nxor = submenu.add("NXOR gate");
        Nxor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Nxor());
            }
        });

        JMenuItem Wirepoint = submenu.add("Junction");
        Wirepoint.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Wirepoint());
            }
        });

        JMenuItem Display = submenu.add("Display");
        Display.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new Display());
            }
        });

        JMenuItem SSD = submenu.add("SSD (Seven Segment Display)");
        SSD.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new SSD());
            }
        });

        submenu.add ( new JPopupMenu.Separator() );

        JMenuItem JK = submenu.add("JK flip-flop");
        JK.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                addGate(new JK());
            }
        });

        add ( new JPopupMenu.Separator() );

        JMenuItem Paste = add("Paste");
        Paste.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/paste.png")));
        Paste.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).paste();
            }
        });
    }

    private void addGate(Gate gate) {
        Renderer r = (Renderer)getInvoker();
        r.addSavepoint();
        r.project().addGate(gate, menuX, menuY);
    }

    public void setXY(int x, int y) {
        menuX = x;
        menuY = y;
    }
}
