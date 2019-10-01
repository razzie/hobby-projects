/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import java.awt.event.*;

public class MinimapListener implements MouseListener, MouseMotionListener {
    private java.awt.Panel m = null;

    public MinimapListener() {
        this.m = Interface.Forms.Minimap.minimap;
        m.addMouseListener(this);
        m.addMouseMotionListener(this);
    }

    public void mouseClicked(MouseEvent e) {
    }
    public void mouseEntered(MouseEvent e) {
    }
    public void mouseExited(MouseEvent e) {
    }
    public void mousePressed(MouseEvent e) { // egy egérgomb lenyomásakor meghívja a Renderer megfelelő metódusát
        Renderer r = (Renderer)(((Interface.Forms.Window.w).Workspace).getSelectedComponent());
        r.minimapClick(e.getX(), e.getY());
    }
    public void mouseReleased(MouseEvent e) {
    }
    public void mouseDragged(MouseEvent e) { // 'húzás' eseménynél meghívja a Renderer megfelelő metódusát
        Renderer r = (Renderer)(((Interface.Forms.Window.w).Workspace).getSelectedComponent());
        r.minimapClick(e.getX(), e.getY());
    }
    public void mouseMoved(MouseEvent e) {
    }
}
