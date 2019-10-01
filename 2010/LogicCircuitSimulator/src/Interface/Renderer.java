/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import java.awt.Graphics;
import java.util.ArrayList;
import Program.Gate;
import Program.Foot;
import Program.Wire;
import java.awt.Color;

public class Renderer extends java.awt.Panel {
    public static boolean debug = false;
    static ArrayList<Gate> clipboard = new ArrayList(); // statikus vágólap
    ArrayList<Gate> selected; // kiválasztott elemek
    private int mouseX, mouseY, selX, selY; // egérkoordináták és kijelölés kijelölés koordináták
    private int moveX, moveY; // scrollozáshoz szükséges értékek
    private int foot_errormsg_counter; // változó az összekötésnél felmerülő többszöri hibaüzenet elkerülésére
    int displayX, displayY; // képernyő koordináták (scrollozásnál)
    private int max_x, max_y; // legnagyobb szélesség és magasság értékek
    private boolean selecting, drag, isFootInput;
    private Foot footToConnect; // kijelölt láb
    private Program.Project project; // a Rendererhez tartozó Project osztály
    private javax.swing.Timer timer; // a szimuláció időzítője
    private Program.History history; // előzménykezelő osztály

    public Renderer(Program.Project project, javax.swing.Timer timer) {
        this.project = project;
        this.timer = timer;
        selected = new ArrayList();
        footToConnect = null;
        foot_errormsg_counter = 0;
        displayX = displayY = moveX = moveY = max_x = max_y = 0;
        selecting = drag = false;
        new Listener(this);
        history = new Program.History(project);
    }

    public Program.Project project() { // projekt azonosító lekérdezése
        return project;
    }

    @Override // munkalap kirajzolása
    public void update(Graphics g) {
        ArrayList<Gate> list = project.getList(); // elemlista
        java.awt.Dimension dim = size();
        java.awt.Image buffer = createImage(dim.width<1 ? 1 : dim.width, dim.height<1 ? 1 : dim.height);
        Graphics gr = buffer.getGraphics();
        ((java.awt.Graphics2D)gr).setRenderingHint(java.awt.RenderingHints.KEY_ANTIALIASING, java.awt.RenderingHints.VALUE_ANTIALIAS_ON);
        // képtörlés
        gr.setColor(Theme.bgcolor);
        gr.fillRect(0, 0, getWidth(), getHeight());
        // kapuk rajzolása
        gr.translate(-displayX, -displayY); // scrollozás
        max_x = max_y = 0;
        if ( list.size() > 0 ) {
            for ( Gate gate : list ) {
                gate.Draw(gr);
                if ( !gate.name().equals("") ) { // az elem nevének kiírása
                    gr.setColor(Theme.wirecolor);
                    gr.drawString(gate.name(), gate.getX()+(gate.getWidth()-gate.name().length()*5)/2, gate.getY()-10);
                }
                if ( debug ) { // debug mód
                    gr.setColor(Color.RED);
                    gr.drawString((list.indexOf(gate)+1)+".", gate.getX(), gate.getY()+gate.getHeight()+10);
                }
                // be- és kimenet lábak és ezek kapcsolatainak rajzolása
                for ( Foot f : gate.getInputs() ) f.Draw(gr);
                for ( Foot f : gate.getOutputs() ) f.Draw(gr);
                if ( gate.getX() > max_x ) max_x = gate.getX();
                if ( gate.getY() > max_y ) max_y = gate.getY();
            }
            gr.setColor(Theme.selection);
            if ( footToConnect != null ) { // kiválasztott láb kijelölésénak rajzolása
                Point p = footToConnect.getCoords();
                gr.drawOval(p.x()-4, p.y()-4, 8, 8);
            }
            // kiválasztott elemek keretezése
            for ( Gate gate : selected ) {
                gr.drawRect(gate.getX()-5, gate.getY()-5, gate.getWidth()+10, gate.getHeight()+10);
                if ( debug ) gr.drawString((selected.indexOf(gate)+1)+".", gate.getX(), gate.getY()+gate.getHeight()+20); // debug mód
            }
        }
        // kijelölés téglalap rajzolása
        if ( selecting ) {
            gr.setColor(Theme.selection);
            gr.drawLine(selX, selY, selX, mouseY+displayY);
            gr.drawLine(selX, mouseY+displayY, mouseX+displayX, mouseY+displayY);
            gr.drawLine(mouseX+displayX, mouseY+displayY, mouseX+displayX, selY);
            gr.drawLine(mouseX+displayX, selY, selX, selY);
        }
        gr.translate(displayX, displayY); // statikus elemek
        // szimulálás jelző négyzet
        gr.setColor( timer.isRunning() ? Theme.wirecolor_on : Theme.wirecolor );
        gr.fillRect(5, 5, 10, 10);
        gr.setColor( timer.isRunning() ? Theme.wirecolor : Theme.wirecolor_on );
        gr.drawRect(5, 5, 10, 10);
        // esetleges scrollozás
        displayX += moveX;
        displayY += moveY;
        if ( displayX < 0 ) displayX = 0;
        if ( displayY < 0 ) displayY = 0;
        // kirajzolás (double buffer módszer)
        g.drawImage(buffer, 0, 0, this);
        buffer.flush();
        if ( Interface.Forms.Minimap.minimap != null ) drawMinimap(); // ha a minimap be van kapcsolva akkor rajzolja ki
    }
    @Override
    public void paint(Graphics g) {
        update(g);
    }
    public void paint() { // kívülről általában meghívott rajzoló metódus
        paint(getGraphics());
    }

    public void addSavepoint() { // új előzmény létrehozása
        history.addSavepoint();
    }
    public void undo() { // visszavonás
        if ( history.undo() ) selected.clear();
    }
    public void redo() { // ismét
        if ( history.redo() ) selected.clear();
    }

    public void cutSelected() { // kiválasztott elemek kivágása
        copySelected();
        deleteSelected();
    }
    public void copySelected() { // kiválasztott elemek másolása
        clipboard = Gate.copy(selected);
        for ( Gate g : clipboard ) g.setCoords(g.getX()-displayX, g.getY()-displayY);
    }
    public void paste() { // vágólap tartalmának beillesztése
        selected.clear();
        if ( clipboard.size() > 0 ) {
            history.addSavepoint();
            for ( Gate g : clipboard ) {
                project.getList().add(g);
                selected.add(g);
                g.setCoords(g.getX()+15+displayX, g.getY()+15+displayY);
            }
            copySelected(); // másolás esetén lehetővé teszi a többszöri beillesztést
        }
    }
    public void deleteSelected() { // kiválasztott elemek törlése
        if ( selected.size() > 0 ) {
            history.addSavepoint();
            project.removeGates( selected );
            selected.clear();
        }
        if ( footToConnect != null ) { // kiválasztott láb kijelölésénak törlése
            unconnectFoots(footToConnect);
            footToConnect = null;
        }
    }
    public void selectAll() { // összes elem kijelölése
        selected.clear();
        selected.addAll(project.getList());
        footToConnect = null;
    }
    public void selectNone() { // megszűnteti az összes kijelölést
        selected.clear();
    }

    public void autoArrange(boolean usermode) { // automatikus elrendezés
        final int r = 6; // hatósugár
        int size = selected.size(); // kijelölt elemek száma
        int x, y, av_x = 0, av_y = 0, max_x = 0, max_y = 0;
        if ( size > 0 ) {
            for ( Gate g : selected ) {
                x = g.getX();
                y = g.getY();
                // vezetékek egyenesítése
                for ( Foot f : g.getInputs() ) arrange(f, r);
                for ( Foot f : g.getOutputs() ) arrange(f, r);
                // szándékos elrendezésnél egyéb adatok számítása
                if ( usermode ) {
                    av_x += x;
                    av_y += y;
                    if ( x > max_x ) max_x = x;
                    if ( y > max_y ) max_y = y;
                }
            }
            if ( usermode ) { // akkor igaz, amikor a felhasználó szándékosan hívta meg a rendező metódust
                av_x /= size; // átlagok kiszámítása
                av_y /= size;
                if ( max_x - av_x < max_y - av_y ) {
                    // függőleges elrendezés
                    class sort implements java.util.Comparator { // összehasonlító osztály (a rendezéshez)
                        public int compare(Object o1, Object o2) { return ((Gate)o1).getY() - ((Gate)o2).getY(); }
                    }
                    java.util.Collections.sort(selected, new sort()); // y koordináta szerinti rendezés
                    int i = 0, min = selected.get(0).getY(), max = selected.get(size-1).getY();
                    double diff = ( max - min ) / ( size - 1 ); // elemek egymás közti távolságátlaga
                    for ( Gate g : selected ) g.setCoords( av_x , (int)((i++)*diff)+min ); // az elemek valódi rendezése
                } else {
                    // vízszintes elrendezés
                    class sort implements java.util.Comparator { // összehasonlító osztály (a rendezéshez)
                        public int compare(Object o1, Object o2) { return ((Gate)o1).getX() - ((Gate)o2).getX(); }
                    }
                    java.util.Collections.sort(selected, new sort()); // x koordinánta szerinti rendezés
                    int i = 0, min = selected.get(0).getX(), max = selected.get(size-1).getX();
                    double diff = ( max - min ) / ( size - 1 ); // elemek egymás közti távolságátlaga
                    for ( Gate g : selected ) g.setCoords( (int)((i++)*diff)+min , av_y ); // az elemek valódi rendezése
                }
            }
        } else if ( usermode == true ) {
            Interface.Forms.Message.dropWarning("No selected items :(");
        }
    }
    private boolean arrange(Foot f, int r) { // adott lábhoz tartozó láb vezetékének egyenesítése, r hatósugárral
        if ( f != null && f.pair() != null ) {
            int d_x = f.getCoords().x() - f.pair().getCoords().x();
            int d_y = f.getCoords().y() - f.pair().getCoords().y();
            if ( d_x*d_x <= r*r ) { // függőleges egyenesítés
                f.gate().setCoords( f.gate().getX()-d_x , f.gate().getY() );
                return true;
            }
            if ( d_y*d_y <= r*r ) { // vízszintes egyenesítés
                f.gate().setCoords( f.gate().getX() , f.gate().getY()-d_y );
                return true;
            }
        }
        return false;
    }

    private boolean ConnectFoots(Gate g, int x, int y) { // lábakkal kapcsolatos műveletek egy elemen
        return ConnectFoots(g, x, y, true) || ConnectFoots(g, x, y, false);
    }
    private boolean ConnectFoots(Gate g, int x, int y, boolean isInput) { // lábakkal kapcsolatos műveletek
        ArrayList<Foot> foots = isInput ? g.getOutputs() : g.getInputs(); // a megfelelő láb-lista kiválasztása
        boolean returnState = false;
        for ( int i=0 ; i<foots.size() ; i++ ) {
            Foot f = foots.get(i);
            if ( f.getCoords().distSq(x,y) < 25 ) { // vizsgálat, hogy a lábra kattintottam-e
                if ( footToConnect == null ) { // ha eddig nem volt kijelölt láb, ez lesz kijelölve
                    footToConnect = f;
                    isFootInput = !isInput;
                } else if ( isFootInput == isInput ) { // ha a két láb típusa különböző (tehát pl kimenet-bemenet)
                    if ( footToConnect.gate() != f.gate() && f.pair() == null ) { // csak akkor csatlakoztat, ha a két láb nincs ugyanazon az elemen és a másik lábnak nincs kapcsolata
                        history.addSavepoint();
                        f.set(footToConnect);
                        footToConnect = null;
                        project.optimize(); // optimalizálás minden új összeköttetés után
                        return true;
                    }
                } else {
                    if ( footToConnect.gate() != f.gate() ) { // ha a két láb ugyanolyan típusú és nem azonos elem lábai akkor figyelmeztetés dobása
                        if ( foot_errormsg_counter == 0 ) { // egymás után sok figyelmeztetés dobásának kiszűrése
                            foot_errormsg_counter++;
                            Interface.Forms.Message.dropWarning("Can't connect :(");
                        }
                    } else if ( footToConnect != f ) { // az elem kát lábának felcserélése
                        history.addSavepoint();
                        foots.set(footToConnect.index(), f);
                        foots.set(i, footToConnect);
                    }
                }
                returnState = true;
            } else {
                foot_errormsg_counter = 0;
            }
        }
        return returnState;
    }
    private void unconnectFoots(Foot f) {
        boolean input = f.isInput();
        Gate g = f.gate();
        if ( (input ? g.extendableInputs() : g.extendableOutputs()) && (input ? (g.getInputs().size()>g.defaultInputNum()) : (g.getOutputs().size()>g.defaultOutputNum())) ) {
            ( input ? g.getInputs() : g.getOutputs() ).remove( f );
        }
        if ( f.pair() != null ) {
            Gate g2 = f.pair().gate();
            if ( (!input ? g2.extendableInputs() : g2.extendableOutputs()) && (!input ? (g2.getInputs().size()>g2.defaultInputNum()) : (g2.getOutputs().size()>g2.defaultOutputNum())) ) {
                ( !input ? g2.getInputs() : g2.getOutputs() ).remove( f.pair() );
            }
        }
        f.set(null);
    }

    void mousepress(int x, int y, int click, boolean ctrl) { // egérkattintás esetén
        boolean empty = true, emptyfoot = true;
        mouseX = x;
        mouseY = y;
        // elemlista bejárása
        for ( Gate g : project.getList() ) {
            if ( g.clicked(x+displayX, y+displayY, 6) ) { // megvizsgálja, hogy a jelenlegi elemre kattintottam-e (6 pixeles tűréshatárral)
                // kísérlet láb kijelölésére, vagy lábak csatlakoztatására
                emptyfoot = !ConnectFoots(g, x+displayX, y+displayY);
                // nyomógombra vagy kapcsolóra történő kattintás esetén
                if ( g instanceof Program.Gates.Button && g.clicked(x+displayX, y+displayY, -4) ) ((Program.Gates.Button)g).Click();
                // kiválasztott elem kijelölése, vagy kijelöltekhez adása
                if ( emptyfoot && !selected.contains(g) ) {
                    if ( !ctrl && click == 1 ) selected.clear();
                    boolean newConnection = false;
                    if ( footToConnect != null && footToConnect.gate() != g ) tryToConnect: {
                        for ( Foot f : isFootInput ? g.getOutputs() : g.getInputs() )
                            if ( f.pair() == null ) {
                                history.addSavepoint();
                                f.set(footToConnect);
                                footToConnect = null;
                                project.optimize();
                                newConnection = true;
                                break tryToConnect;
                            }
                        if ( isFootInput ? g.extendableOutputs() : g.extendableInputs() ) {
                            history.addSavepoint();
                            ( isFootInput ? g.getOutputs() : g.getInputs() ).add( new Foot(g,footToConnect) );
                            footToConnect = null;
                            project.optimize();
                        }
                    }
                    if ( !newConnection ) selected.add(g);
                }
                empty = false;
                break; // egy kattintásnál max 1 elemet kell lekezelni
            }
        }
        if ( (empty || !emptyfoot) && !selecting ) { // ha semmire sem kattintottam és nincs megkezdett kijelölés
            if (click > 1) selectAll(); // duplaklikk esetén összes elem kijelölése
            else { // különben kijelölés megkezdése
                selected.clear();
                selX = x+displayX;
                selY = y+displayY;
                selecting = true;
            }
        }
        if (emptyfoot) footToConnect = null; // a kijelölés eltávolítása az előzőleg kiválasztott lábról, ha nem kattintottam lábra
    }
    void mousedrag(int x, int y) { // húzás esetén
        if ( !drag ) { // elemek húzásának kezdete és előzményi pont létrehozása
            drag = true;
            history.addSavepoint();
        }
        java.awt.Dimension dim = size(); // munkalap mérete
        if ( !selecting ) {
            int toX, toY;
            x = ( x<0 ? 0 : ( x>dim.width ? dim.width : x ) );
            y = ( y<0 ? 0 : ( y>dim.height ? dim.height : y ) );
            // ha nincs megkezdett kijelölés, akkor az elemek mozgatása a munkalapon
            for ( Gate g : selected ) {
                toX = g.getX()+(x-mouseX);
                toY = g.getY()+(y-mouseY);
                //toX = ( toX<displayX ? displayX : ( toX>dim.width+displayX ? dim.width+displayX : toX ) );
                //toY = ( toY<displayY ? displayY : ( toY>dim.height+displayY ? dim.height+displayY : toY ) );
                toX = ( toX<displayX ? displayX : toX );
                toY = ( toY<displayY ? displayY : toY );
                g.setCoords( toX , toY ); // elem mozgatása az új koordinátákra
            }
        } else {
            // megkezdett kijelölés esetén
            for ( Gate g : project.getList() ) {
                // elemek kijelöléshez adása vagy elvétele
                if ( g.inside(selX, selY, x+displayX, y+displayY, -12) ) {
                    if ( !selected.contains(g) ) selected.add(g);
                } else {
                    selected.remove(g);
                }
                // ha kimentem a munkalapról, akkor scrollozás
                moveX = (x > dim.width) ? x-dim.width : ((x < 0) ? x : 0);
                moveY = (y > dim.height) ? y-dim.height : ((y < 0) ? y : 0);
            }
        }
        mouseX = x;
        mouseY = y;
    }
    void mouserelease(int x, int y, boolean ctrl, boolean shift) { // egérgomb felengedése
        moveX = moveY = 0;
        if ( selected.size() > 0 ) {
            // nyomógombon vagy kapcsolón történó gombnyomás felengedése (a kapcsoló nem feltétlenül vált alapállapotba)
            for( Gate g : selected )  if ( g instanceof Program.Gates.Button ) ((Program.Gates.Button)g).Release();
            autoArrange(false); // automatikus elrendezés, amit nem a felhasználó kért
            if ( shift && selected.size() == 1 ) {
                if ( ctrl ) { // ctrl+shift esetén az összes hasonló elem kijelölése
                    Gate.Type type = selected.get(0).type();
                    selected.clear();
                    for ( Gate g : project.getList() ) if ( g.type() == type ) selected.add(g);
                } else { // elem beállítás ablakának behozása
                    history.addSavepoint();
                    selected.get(0).setup();
                }
            }
        }
        selecting = false;
        drag = false;
    }

    private void drawMinimap() { // minimap rajzolás
        java.awt.Dimension dim = size(); // rajzlap mérete
        java.awt.Dimension dim2 = Interface.Forms.Minimap.minimap.size(); // minimap mérete
        java.awt.Image buffer = createImage(dim2.width<1 ? 1 : dim2.width, dim2.height<1 ? 1 : dim2.height); // doublebuffer kép létrehozása
        Graphics gr = buffer.getGraphics();
        ((java.awt.Graphics2D)gr).setRenderingHint(java.awt.RenderingHints.KEY_ANTIALIASING, java.awt.RenderingHints.VALUE_ANTIALIAS_ON); // élsimítás
        gr.setColor(Theme.bgcolor);
        gr.fillRect(0, 0, dim2.width, dim2.height); // háttértörlés
        double sc_x = (double)(dim2.width-8) / (double)((max_x==0)?1:max_x); // vízszintes arány
        double sc_y = (double)(dim2.height-8) / (double)((max_y==0)?1:max_y); // föggőleges arány
        int x, y;
        for ( Gate gate : project.getList() ) {
            x = (int)( sc_x * (double)(gate.getX()) ); // egy elem minimap beli x koordinátája
            y = (int)( sc_y * (double)(gate.getY()) ); // egy elem minimap beli y koordinátája
            // elem típusától függően megfelelő szín kiválasztása
            if ( gate instanceof Program.Gates.Button || gate instanceof Program.Gates.Timer ) gr.setColor(Theme.buttoncolor_on);
            else if ( gate instanceof Program.Gates.Led || gate instanceof Program.Gates.SSD || gate instanceof Program.Gates.Display ) gr.setColor(Theme.ledcolor_on);
            else gr.setColor(Theme.gatecolor);
            gr.fillRect(x+2, y+2, 4, 4);
            gr.setColor(Theme.wirecolor);
            gr.drawRect(x+2, y+2, 4, 4);
            for ( Foot f : gate.getInputs() ) { // vezetékek rajzolása
                if (f.pair() != null) {
                    Interface.Point in = new Interface.Point(x+4, y+4);
                    Interface.Point out = new Interface.Point((int)( sc_x * (double)(f.pair().gate().getX()) )+4, (int)( sc_y * (double)(f.pair().gate().getY()) )+4);
                    Wire.Draw(gr, out, in);
                }
            }
        }
        // képernyő keret rajzolása
        gr.setColor(Theme.selection);
        gr.drawRect((int)((double)displayX*sc_x)+2, (int)((double)displayY*sc_y)+2, (int)((double)(dim.width)*sc_x)-4, (int)((double)(dim.height)*sc_y)-4);
        (Interface.Forms.Minimap.minimap.getGraphics()).drawImage(buffer, 0, 0, Interface.Forms.Minimap.minimap); // buffer vetítése
        buffer.flush();
    }
    void minimapClick(int x, int y) {
        java.awt.Dimension dim = size(); // rajzlap mérete
        java.awt.Dimension dim2 = Interface.Forms.Minimap.minimap.size(); // minimap mérete
        double sc_x = (double)(dim2.width-8) / (double)((max_x==0)?1:max_x); // vízszintes arány
        double sc_y = (double)(dim2.height-8) / (double)((max_y==0)?1:max_y); // föggőleges arány
        displayX = (int)( (double)x / sc_x - (dim.width / 2) );
        displayY = (int)( (double)y / sc_y - (dim.height / 2) );
    }
}
