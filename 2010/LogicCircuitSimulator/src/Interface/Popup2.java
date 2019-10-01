/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import Interface.Forms.Rename;
import Interface.Forms.Message;
import java.awt.event.*;
import javax.swing.JPopupMenu;
import javax.swing.JMenuItem;

public class Popup2 extends JPopupMenu {
    private int menuX, menuY;

    public Popup2() {
        JMenuItem Setup = add("Setup");
        Setup.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/setup.png")));
        Setup.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                Renderer r = ((Renderer)getInvoker());
                if (r.selected.size() == 1) {
                    r.addSavepoint();
                    r.selected.get( 0 ).setup();
                } else {
                    Message.dropWarning("Can not setup more than one items at a time :(");
                }
            }
        });

        JMenuItem Rename = add("Rename");
        Rename.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/name.png")));
        Rename.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                Renderer r = ((Renderer)getInvoker());
                if (r.selected.size() == 1) {
                    r.addSavepoint();
                    Program.Gate gate = r.selected.get( 0 );
                    new Rename(gate);
                } else {
                    Message.dropWarning("Can not rename more than one items at a time :(");
                }
            }
        });

        add( new JPopupMenu.Separator() );

        JMenuItem Cut = add("Cut");
        Cut.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/cut.png")));
        Cut.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).cutSelected();
            }
        });

        JMenuItem Copy = add("Copy");
        Copy.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/copy.png")));
        Copy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).copySelected();
            }
        });

        JMenuItem Paste = add("Paste");
        Paste.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/paste.png")));
        Paste.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).paste();
            }
        });

        JMenuItem Delete = add("Törlés");
        Delete.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/delete.png")));
        Delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).deleteSelected();
            }
        });

        add( new JPopupMenu.Separator() );

        JMenuItem SelectAll = add("Select All");
        SelectAll.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/selection.png")));
        SelectAll.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).selectAll();
            }
        });

        JMenuItem Arrange = add("Auto Arrange");
        Arrange.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/arrange.png")));
        Arrange.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                ((Renderer)getInvoker()).autoArrange(true);
            }
        });

        add( new JPopupMenu.Separator() );

        JMenuItem Module = add("Convert to module");
        Module.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/module_in.png")));
        Module.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                Renderer r = (Renderer)getInvoker();
                r.addSavepoint();
                r.project().addGate(new Program.Gates.Module( r.selected ), menuX, menuY);
                r.project().removeGates(r.selected);
                r.selected.clear();
            }
        });

        JMenuItem Module2 = add("Extract module");
        Module2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/module_out.png")));
        Module2.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                Renderer r = (Renderer)getInvoker();
                if (r.selected.size() == 1) {
                    Program.Gate module = r.selected.get(0);
                    if (module instanceof Program.Gates.Module) {
                        r.addSavepoint();
                        r.project().getList().addAll( ((Program.Gates.Module)module).getList() );
                        r.project().removeGates(r.selected);
                        r.selected.clear();
                    } else {
                        Message.dropWarning("The choosen item is not a module :(");
                    }
                } else {
                    Message.dropWarning("There are multiple items selected :(");
                }
            }
        });
    }

    public void setXY(int x, int y) {
        menuX = x;
        menuY = y;
    }
}
