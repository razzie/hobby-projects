/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

import Program.Project;
import Interface.Renderer;
import Interface.Theme;
import javax.swing.Timer;

public class Window extends javax.swing.JFrame {
    Timer timer;
    public static Window w = null;

    public Window() {
        initComponents();
        setIconImage((new javax.swing.ImageIcon(getClass().getResource("/Icons/icon.png"))).getImage());
        timer = new Timer(50, new Program.Simulate(Workspace));
        timer.start();
        new Timer(40, new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Renderer r = (Renderer)Workspace.getSelectedComponent();
                if ( r != null ) r.paint();
            }
        }).start();
        NewProjectActionPerformed(null);
        setVisible(true);
        w = this;
    }

    @Override
    public void dispose() {
        for ( java.awt.Component c : Workspace.getComponents() ) {
            Workspace.setSelectedComponent(c);
            if ( ((Renderer)c).project().saved() ) {
                Workspace.remove(c);
            } else {
                new CloseProject(Workspace);
            }
        }
        if ( Workspace.getComponentCount() == 0 ) System.exit(0);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        Workspace = new javax.swing.JTabbedPane();
        MenuBar = new javax.swing.JMenuBar();
        ProjectMenu = new javax.swing.JMenu();
        NewProject = new javax.swing.JMenuItem();
        OpenProject = new javax.swing.JMenuItem();
        SaveProject = new javax.swing.JMenuItem();
        SaveProjectAs = new javax.swing.JMenuItem();
        jSeparator10 = new javax.swing.JPopupMenu.Separator();
        Rename = new javax.swing.JMenuItem();
        CloseProject = new javax.swing.JMenuItem();
        jSeparator3 = new javax.swing.JPopupMenu.Separator();
        Exit = new javax.swing.JMenuItem();
        EditMenu = new javax.swing.JMenu();
        Undo = new javax.swing.JMenuItem();
        Redo = new javax.swing.JMenuItem();
        jSeparator4 = new javax.swing.JPopupMenu.Separator();
        Cut = new javax.swing.JMenuItem();
        Copy = new javax.swing.JMenuItem();
        Paste = new javax.swing.JMenuItem();
        Delete = new javax.swing.JMenuItem();
        jSeparator5 = new javax.swing.JPopupMenu.Separator();
        SelectAll = new javax.swing.JMenuItem();
        Arrange = new javax.swing.JMenuItem();
        Simulation = new javax.swing.JMenu();
        Reset = new javax.swing.JMenuItem();
        jSeparator9 = new javax.swing.JPopupMenu.Separator();
        SimOn = new javax.swing.JMenuItem();
        SimOff = new javax.swing.JMenuItem();
        jSeparator6 = new javax.swing.JPopupMenu.Separator();
        SimSpeed = new javax.swing.JMenuItem();
        ColorMenu = new javax.swing.JMenu();
        Themes = new javax.swing.JMenu();
        DefaultTheme = new javax.swing.JMenuItem();
        DarkTheme = new javax.swing.JMenuItem();
        jSeparator8 = new javax.swing.JPopupMenu.Separator();
        LoadTheme = new javax.swing.JMenuItem();
        SaveTheme = new javax.swing.JMenuItem();
        jSeparator7 = new javax.swing.JPopupMenu.Separator();
        BgColor = new javax.swing.JMenuItem();
        Selection = new javax.swing.JMenuItem();
        GateColor = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JPopupMenu.Separator();
        WireColor = new javax.swing.JMenuItem();
        WireColorOn = new javax.swing.JMenuItem();
        jSeparator2 = new javax.swing.JPopupMenu.Separator();
        LedColor = new javax.swing.JMenuItem();
        LedColorOn = new javax.swing.JMenuItem();
        ButtonColor = new javax.swing.JMenuItem();
        ButtonColorOn = new javax.swing.JMenuItem();
        ModuleColor = new javax.swing.JMenuItem();
        HelpMenu = new javax.swing.JMenu();
        MiniMap = new javax.swing.JMenuItem();
        jSeparator12 = new javax.swing.JPopupMenu.Separator();
        Help = new javax.swing.JMenuItem();
        About = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Logic Circuit Simulator");
        setLocationByPlatform(true);
        setMinimumSize(new java.awt.Dimension(400, 300));
        setName("Window"); // NOI18N

        Workspace.setBorder(javax.swing.BorderFactory.createEmptyBorder(3, 2, 1, 0));
        Workspace.setAlignmentX(0.0F);
        Workspace.setAlignmentY(0.0F);
        Workspace.setFocusable(false);

        ProjectMenu.setText("Project");

        NewProject.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_N, java.awt.event.InputEvent.CTRL_MASK));
        NewProject.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/new.png"))); // NOI18N
        NewProject.setText("New");
        NewProject.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                NewProjectActionPerformed(evt);
            }
        });
        ProjectMenu.add(NewProject);

        OpenProject.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_O, java.awt.event.InputEvent.CTRL_MASK));
        OpenProject.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/open.png"))); // NOI18N
        OpenProject.setText("Open");
        OpenProject.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                OpenProjectActionPerformed(evt);
            }
        });
        ProjectMenu.add(OpenProject);

        SaveProject.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_S, java.awt.event.InputEvent.CTRL_MASK));
        SaveProject.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/save.png"))); // NOI18N
        SaveProject.setText("Save");
        SaveProject.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SaveProjectActionPerformed(evt);
            }
        });
        ProjectMenu.add(SaveProject);

        SaveProjectAs.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/save_as.png"))); // NOI18N
        SaveProjectAs.setText("Save As");
        SaveProjectAs.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SaveProjectAsActionPerformed(evt);
            }
        });
        ProjectMenu.add(SaveProjectAs);
        ProjectMenu.add(jSeparator10);

        Rename.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/rename.png"))); // NOI18N
        Rename.setText("Rename");
        Rename.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                RenameActionPerformed(evt);
            }
        });
        ProjectMenu.add(Rename);

        CloseProject.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_Q, java.awt.event.InputEvent.CTRL_MASK));
        CloseProject.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/close.png"))); // NOI18N
        CloseProject.setText("Close");
        CloseProject.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CloseProjectActionPerformed(evt);
            }
        });
        ProjectMenu.add(CloseProject);
        ProjectMenu.add(jSeparator3);

        Exit.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F4, java.awt.event.InputEvent.ALT_MASK));
        Exit.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/exit.png"))); // NOI18N
        Exit.setText("Exit");
        Exit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ExitActionPerformed(evt);
            }
        });
        ProjectMenu.add(Exit);

        MenuBar.add(ProjectMenu);

        EditMenu.setText("Edit");

        Undo.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_Z, java.awt.event.InputEvent.CTRL_MASK));
        Undo.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/undo.png"))); // NOI18N
        Undo.setText("Undo");
        Undo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                UndoActionPerformed(evt);
            }
        });
        EditMenu.add(Undo);

        Redo.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_Y, java.awt.event.InputEvent.CTRL_MASK));
        Redo.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/redo.png"))); // NOI18N
        Redo.setText("Redo");
        Redo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                RedoActionPerformed(evt);
            }
        });
        EditMenu.add(Redo);
        EditMenu.add(jSeparator4);

        Cut.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_X, java.awt.event.InputEvent.CTRL_MASK));
        Cut.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/cut.png"))); // NOI18N
        Cut.setText("Cut");
        Cut.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CutActionPerformed(evt);
            }
        });
        EditMenu.add(Cut);

        Copy.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_C, java.awt.event.InputEvent.CTRL_MASK));
        Copy.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/copy.png"))); // NOI18N
        Copy.setText("Copy");
        Copy.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CopyActionPerformed(evt);
            }
        });
        EditMenu.add(Copy);

        Paste.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_V, java.awt.event.InputEvent.CTRL_MASK));
        Paste.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/paste.png"))); // NOI18N
        Paste.setText("Paste");
        Paste.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                PasteActionPerformed(evt);
            }
        });
        EditMenu.add(Paste);

        Delete.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_DELETE, 0));
        Delete.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/delete.png"))); // NOI18N
        Delete.setText("Delete");
        Delete.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                DeleteActionPerformed(evt);
            }
        });
        EditMenu.add(Delete);
        EditMenu.add(jSeparator5);

        SelectAll.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_A, java.awt.event.InputEvent.CTRL_MASK));
        SelectAll.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/selection.png"))); // NOI18N
        SelectAll.setText("Select All");
        SelectAll.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SelectAllActionPerformed(evt);
            }
        });
        EditMenu.add(SelectAll);

        Arrange.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_SPACE, 0));
        Arrange.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/arrange.png"))); // NOI18N
        Arrange.setText("Auto Arrange");
        Arrange.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ArrangeActionPerformed(evt);
            }
        });
        EditMenu.add(Arrange);

        MenuBar.add(EditMenu);

        Simulation.setText("Simulation");

        Reset.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F7, 0));
        Reset.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/reset.png"))); // NOI18N
        Reset.setText("Reset");
        Reset.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ResetActionPerformed(evt);
            }
        });
        Simulation.add(Reset);
        Simulation.add(jSeparator9);

        SimOn.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F5, 0));
        SimOn.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/start_sim.png"))); // NOI18N
        SimOn.setText("Enable");
        SimOn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SimOnActionPerformed(evt);
            }
        });
        Simulation.add(SimOn);

        SimOff.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F6, 0));
        SimOff.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/stop_sim.png"))); // NOI18N
        SimOff.setText("Disable");
        SimOff.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SimOffActionPerformed(evt);
            }
        });
        Simulation.add(SimOff);
        Simulation.add(jSeparator6);

        SimSpeed.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F5, java.awt.event.InputEvent.CTRL_MASK));
        SimSpeed.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/speed.png"))); // NOI18N
        SimSpeed.setText("Speed");
        SimSpeed.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SimSpeedActionPerformed(evt);
            }
        });
        Simulation.add(SimSpeed);

        MenuBar.add(Simulation);

        ColorMenu.setText("Colors");

        Themes.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/theme.png"))); // NOI18N
        Themes.setText("Themes");

        DefaultTheme.setText("Default");
        DefaultTheme.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                DefaultThemeActionPerformed(evt);
            }
        });
        Themes.add(DefaultTheme);

        DarkTheme.setText("Dark");
        DarkTheme.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                DarkThemeActionPerformed(evt);
            }
        });
        Themes.add(DarkTheme);
        Themes.add(jSeparator8);

        LoadTheme.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/theme_load.png"))); // NOI18N
        LoadTheme.setText("Open theme");
        LoadTheme.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                LoadThemeActionPerformed(evt);
            }
        });
        Themes.add(LoadTheme);

        SaveTheme.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/theme_save.png"))); // NOI18N
        SaveTheme.setText("Save theme");
        SaveTheme.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SaveThemeActionPerformed(evt);
            }
        });
        Themes.add(SaveTheme);

        ColorMenu.add(Themes);
        ColorMenu.add(jSeparator7);

        BgColor.setText("Project background");
        BgColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                BgColorActionPerformed(evt);
            }
        });
        ColorMenu.add(BgColor);

        Selection.setText("Selection");
        Selection.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SelectionActionPerformed(evt);
            }
        });
        ColorMenu.add(Selection);

        GateColor.setText("Gate");
        GateColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                GateColorActionPerformed(evt);
            }
        });
        ColorMenu.add(GateColor);
        ColorMenu.add(jSeparator1);

        WireColor.setText("Unpowered wire");
        WireColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                WireColorActionPerformed(evt);
            }
        });
        ColorMenu.add(WireColor);

        WireColorOn.setText("Powered wire");
        WireColorOn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                WireColorOnActionPerformed(evt);
            }
        });
        ColorMenu.add(WireColorOn);
        ColorMenu.add(jSeparator2);

        LedColor.setText("Unpowered LED");
        LedColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                LedColorActionPerformed(evt);
            }
        });
        ColorMenu.add(LedColor);

        LedColorOn.setText("Powered LED");
        LedColorOn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                LedColorOnActionPerformed(evt);
            }
        });
        ColorMenu.add(LedColorOn);

        ButtonColor.setText("Unpowered button");
        ButtonColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ButtonColorActionPerformed(evt);
            }
        });
        ColorMenu.add(ButtonColor);

        ButtonColorOn.setText("Powered button");
        ButtonColorOn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ButtonColorOnActionPerformed(evt);
            }
        });
        ColorMenu.add(ButtonColorOn);

        ModuleColor.setText("Module");
        ModuleColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ModuleColorActionPerformed(evt);
            }
        });
        ColorMenu.add(ModuleColor);

        MenuBar.add(ColorMenu);

        HelpMenu.setText("Help");

        MiniMap.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/minimap.png"))); // NOI18N
        MiniMap.setText("Minimap");
        MiniMap.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                MiniMapActionPerformed(evt);
            }
        });
        HelpMenu.add(MiniMap);
        HelpMenu.add(jSeparator12);

        Help.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F1, 0));
        Help.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Icons/help.png"))); // NOI18N
        Help.setText("Help");
        Help.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                HelpActionPerformed(evt);
            }
        });
        HelpMenu.add(Help);

        About.setText("About");
        About.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                AboutActionPerformed(evt);
            }
        });
        HelpMenu.add(About);

        MenuBar.add(HelpMenu);

        setJMenuBar(MenuBar);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(Workspace, javax.swing.GroupLayout.DEFAULT_SIZE, 521, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(Workspace, javax.swing.GroupLayout.DEFAULT_SIZE, 365, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void AboutActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_AboutActionPerformed
        new About();
    }//GEN-LAST:event_AboutActionPerformed

    private void HelpActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_HelpActionPerformed
        String helpfile = "documentation.pdf";
        try
        {
            java.awt.Desktop dt = java.awt.Desktop.getDesktop();
            java.io.File document = new java.io.File(helpfile);
            System.out.println(document.getAbsoluteFile());
            dt.open( document );

        }
        catch (Exception e)
        {
            Message.dropError("Could not open: " + helpfile);
        }
    }//GEN-LAST:event_HelpActionPerformed

    private void NewProjectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_NewProjectActionPerformed
        Program.Project p = new Project();
        Renderer r = new Renderer(p, timer);
        Workspace.addTab(p.name(), r);
        Workspace.setSelectedIndex(Workspace.getTabCount()-1);
    }//GEN-LAST:event_NewProjectActionPerformed

    private void CloseProjectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CloseProjectActionPerformed
        if (Workspace.getSelectedComponent() != null) {
            Interface.Renderer r = (Interface.Renderer)(Workspace.getSelectedComponent());
            if ( r.project().saved() ) {
                Workspace.remove( r );
            } else {
                new CloseProject(Workspace);
            }
        }
    }//GEN-LAST:event_CloseProjectActionPerformed

    private void BgColorActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_BgColorActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.bgcolor);
        java.awt.Color color = ColorChooser.showDialog(this, "Project background", Theme.bgcolor);
        if (color != null) Theme.bgcolor = color;
    }//GEN-LAST:event_BgColorActionPerformed

    private void WireColorOnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_WireColorOnActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.wirecolor_on);
        java.awt.Color color = ColorChooser.showDialog(this, "Powered wire", Theme.wirecolor_on);
        if (color != null) Theme.wirecolor_on = color;
    }//GEN-LAST:event_WireColorOnActionPerformed

    private void LedColorOnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_LedColorOnActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.ledcolor_on);
        java.awt.Color color = ColorChooser.showDialog(this, "Powered LED", Theme.ledcolor_on);
        if (color != null) Theme.ledcolor_on = color;
    }//GEN-LAST:event_LedColorOnActionPerformed

    private void SelectionActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SelectionActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.selection);
        java.awt.Color color = ColorChooser.showDialog(this, "Selection", Theme.selection);
        if (color != null) Theme.selection = color;
    }//GEN-LAST:event_SelectionActionPerformed

    private void OpenProjectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_OpenProjectActionPerformed
        Program.Project p = Program.Project.Load();
        if ( p != null ) {
            Workspace.addTab(p.name(), new Renderer(p, timer));
            Workspace.setSelectedIndex(Workspace.getTabCount()-1);
        }
    }//GEN-LAST:event_OpenProjectActionPerformed

    private void SaveProjectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SaveProjectActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.project().save();
    }//GEN-LAST:event_SaveProjectActionPerformed

    private void CutActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CutActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.cutSelected();
    }//GEN-LAST:event_CutActionPerformed

    private void CopyActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CopyActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.copySelected();
    }//GEN-LAST:event_CopyActionPerformed

    private void PasteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_PasteActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.paste();
    }//GEN-LAST:event_PasteActionPerformed

    private void DeleteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_DeleteActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.deleteSelected();
    }//GEN-LAST:event_DeleteActionPerformed

    private void SelectAllActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SelectAllActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.selectAll();
    }//GEN-LAST:event_SelectAllActionPerformed

    private void ExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ExitActionPerformed
        dispose();
    }//GEN-LAST:event_ExitActionPerformed

    private void WireColorActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_WireColorActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.wirecolor);
        java.awt.Color color = ColorChooser.showDialog(this, "Unpowered wire", Theme.wirecolor);
        if (color != null) Theme.wirecolor = color;
    }//GEN-LAST:event_WireColorActionPerformed

    private void LedColorActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_LedColorActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.ledcolor);
        java.awt.Color color = ColorChooser.showDialog(this, "Unpowered LED", Theme.ledcolor);
        if (color != null) Theme.ledcolor = color;
    }//GEN-LAST:event_LedColorActionPerformed

    private void ButtonColorActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ButtonColorActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.buttoncolor);
        java.awt.Color color = ColorChooser.showDialog(this, "Unpowered button", Theme.buttoncolor);
        if (color != null) Theme.buttoncolor = color;
    }//GEN-LAST:event_ButtonColorActionPerformed

    private void ButtonColorOnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ButtonColorOnActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.buttoncolor_on);
        java.awt.Color color = ColorChooser.showDialog(this, "Powered button", Theme.buttoncolor_on);
        if (color != null) Theme.buttoncolor_on = color;
    }//GEN-LAST:event_ButtonColorOnActionPerformed

    private void SimOnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SimOnActionPerformed
        timer.start();
    }//GEN-LAST:event_SimOnActionPerformed

    private void SimOffActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SimOffActionPerformed
        timer.stop();
    }//GEN-LAST:event_SimOffActionPerformed

    private void GateColorActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_GateColorActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.gatecolor);
        java.awt.Color color = ColorChooser.showDialog(this, "Gate", Theme.gatecolor);
        if (color != null) Theme.gatecolor = color;
    }//GEN-LAST:event_GateColorActionPerformed

    private void ModuleColorActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ModuleColorActionPerformed
        javax.swing.JColorChooser ColorChooser = new javax.swing.JColorChooser(Theme.modulecolor);
        java.awt.Color color = ColorChooser.showDialog(this, "Module", Theme.modulecolor);
        if (color != null) Theme.modulecolor = color;
    }//GEN-LAST:event_ModuleColorActionPerformed

    private void SimSpeedActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SimSpeedActionPerformed
        new SpeedSetup(timer);
    }//GEN-LAST:event_SimSpeedActionPerformed

    private void DefaultThemeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_DefaultThemeActionPerformed
        Theme.Default();
    }//GEN-LAST:event_DefaultThemeActionPerformed

    private void DarkThemeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_DarkThemeActionPerformed
        Theme.Dark();
    }//GEN-LAST:event_DarkThemeActionPerformed

    private void LoadThemeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_LoadThemeActionPerformed
        Theme.Load();
    }//GEN-LAST:event_LoadThemeActionPerformed

    private void SaveThemeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SaveThemeActionPerformed
        Theme.Save();
    }//GEN-LAST:event_SaveThemeActionPerformed

    private void UndoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_UndoActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.undo();
    }//GEN-LAST:event_UndoActionPerformed

    private void RedoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_RedoActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.redo();
    }//GEN-LAST:event_RedoActionPerformed

    private void ArrangeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ArrangeActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.autoArrange(true);
    }//GEN-LAST:event_ArrangeActionPerformed

    private void RenameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_RenameActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) new NewProjectName( r.project() , Workspace );
    }//GEN-LAST:event_RenameActionPerformed

    private void ResetActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ResetActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) {
            r.addSavepoint();
            r.project().reset();
        }
    }//GEN-LAST:event_ResetActionPerformed

    private void SaveProjectAsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SaveProjectAsActionPerformed
        Renderer r = (Renderer)Workspace.getSelectedComponent();
        if ( r != null ) r.project().saveAs();
    }//GEN-LAST:event_SaveProjectAsActionPerformed

    private void MiniMapActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_MiniMapActionPerformed
        if ( Minimap.minimap == null ) new Minimap();
    }//GEN-LAST:event_MiniMapActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem About;
    private javax.swing.JMenuItem Arrange;
    private javax.swing.JMenuItem BgColor;
    private javax.swing.JMenuItem ButtonColor;
    private javax.swing.JMenuItem ButtonColorOn;
    private javax.swing.JMenuItem CloseProject;
    private javax.swing.JMenu ColorMenu;
    private javax.swing.JMenuItem Copy;
    private javax.swing.JMenuItem Cut;
    private javax.swing.JMenuItem DarkTheme;
    private javax.swing.JMenuItem DefaultTheme;
    private javax.swing.JMenuItem Delete;
    private javax.swing.JMenu EditMenu;
    private javax.swing.JMenuItem Exit;
    private javax.swing.JMenuItem GateColor;
    private javax.swing.JMenuItem Help;
    private javax.swing.JMenu HelpMenu;
    private javax.swing.JMenuItem LedColor;
    private javax.swing.JMenuItem LedColorOn;
    private javax.swing.JMenuItem LoadTheme;
    private javax.swing.JMenuBar MenuBar;
    private javax.swing.JMenuItem MiniMap;
    private javax.swing.JMenuItem ModuleColor;
    private javax.swing.JMenuItem NewProject;
    private javax.swing.JMenuItem OpenProject;
    private javax.swing.JMenuItem Paste;
    private javax.swing.JMenu ProjectMenu;
    private javax.swing.JMenuItem Redo;
    private javax.swing.JMenuItem Rename;
    private javax.swing.JMenuItem Reset;
    private javax.swing.JMenuItem SaveProject;
    private javax.swing.JMenuItem SaveProjectAs;
    private javax.swing.JMenuItem SaveTheme;
    private javax.swing.JMenuItem SelectAll;
    private javax.swing.JMenuItem Selection;
    private javax.swing.JMenuItem SimOff;
    private javax.swing.JMenuItem SimOn;
    private javax.swing.JMenuItem SimSpeed;
    private javax.swing.JMenu Simulation;
    private javax.swing.JMenu Themes;
    private javax.swing.JMenuItem Undo;
    private javax.swing.JMenuItem WireColor;
    private javax.swing.JMenuItem WireColorOn;
    public javax.swing.JTabbedPane Workspace;
    private javax.swing.JPopupMenu.Separator jSeparator1;
    private javax.swing.JPopupMenu.Separator jSeparator10;
    private javax.swing.JPopupMenu.Separator jSeparator12;
    private javax.swing.JPopupMenu.Separator jSeparator2;
    private javax.swing.JPopupMenu.Separator jSeparator3;
    private javax.swing.JPopupMenu.Separator jSeparator4;
    private javax.swing.JPopupMenu.Separator jSeparator5;
    private javax.swing.JPopupMenu.Separator jSeparator6;
    private javax.swing.JPopupMenu.Separator jSeparator7;
    private javax.swing.JPopupMenu.Separator jSeparator8;
    private javax.swing.JPopupMenu.Separator jSeparator9;
    // End of variables declaration//GEN-END:variables

}
