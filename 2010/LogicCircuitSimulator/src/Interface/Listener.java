/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import java.awt.event.*;

class Listener implements MouseListener, MouseMotionListener { // egér és billentyű eseményeket kezelő osztály (Rendererhez tartozik)
    private Renderer r; // Renderer osztály azonosítója
    private Popup pmenu; // 1-es számú felugró menü (új elemek beszúrása stb)
    private Popup2 pmenu2; // 2-es számú felugró menü (a többi akcióhoz)

    Listener(Renderer r) {
        this.r = r;
        r.addMouseListener(this);
        r.addMouseMotionListener(this);
        pmenu = new Popup();
        pmenu2 = new Popup2();
    }

    public void mouseClicked(MouseEvent e) {
    }
    public void mouseEntered(MouseEvent e) {
    }
    public void mouseExited(MouseEvent e) {
    }
    public void mousePressed(MouseEvent e) { // egy egérgomb lenyomásakor meghívja a Renderer megfelelő metódusát
        r.mousepress(e.getX(), e.getY(), e.getClickCount(), e.isControlDown());
    }
    public void mouseReleased(MouseEvent e) { // egy egérgomb felengedésekor meghívja a Renderer megfelelő metódusát (vagy előhoz egy felugró menüt)
        int x = e.getX(), y = e.getY();
        if ( e.isPopupTrigger() ) {
            if (r.selected.size() > 0) { // ha vannak kijelölt elemek
                pmenu2.setXY(x+r.displayX, y+r.displayY);
                pmenu2.show(r, x, y);
            } else {
                pmenu.setXY(x+r.displayX, y+r.displayY);
                pmenu.show(r, x, y);
            }
        }
        r.mouserelease(e.getX(), e.getY(), e.isControlDown(), e.isShiftDown());
    }
    public void mouseDragged(MouseEvent e) { // 'húzás' eseménynél meghívja a Renderer megfelelő metódusát
        r.mousedrag(e.getX(), e.getY());
    }
    public void mouseMoved(MouseEvent e) {
    }
}
