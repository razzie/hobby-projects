/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program;

import java.util.ArrayList;

final public class Foot { // áramköri elemhez tartozó láb
    private boolean state, input, needInputCheck; // state: állapot, input: típus (bemenet vagy kimenet)
    private Foot pair; // a lábhoz kapcsolt másik láb (alapértelmezetten null)
    private Gate gate; // ennek a kapunak vagyok egy lába

    public Foot(Gate gate) { // konstruktor a gazda kapu megadásával
        state = false;
        pair = null;
        this.gate = gate;
        needInputCheck = true;
    }
    public Foot(Gate gate, Foot pair) { // konstruktor a gazda kapu és a kapcsolat (másik láb) megadásával
        state = false;
        this.gate = gate;
        set(pair);
        needInputCheck = true;
    }
    @Override
    protected void finalize() {
        set(null); // kapcsolat törlése
    }
    private void inputCheck() { // típus beállítása
        input = true;
        for ( Foot f : gate.getInputs() ) if (f == this) return;
        input = false;
        for ( Foot f : gate.getOutputs() ) if (f == this) return;
        needInputCheck = false;
    }

    public void set(Foot pair) { // kapcsolat beállítása másik lábhoz (kölcsönös beállítás)
        if (this.pair != pair) {
            Foot oldpair = this.pair;
            this.pair = pair;
            if (oldpair != null) {
                oldpair.set(null);
            }
            if (pair != null) {
                pair.set(this);
                pair.state(state);
            } else {
                state = false;
            }
        }
    }
    public void state(boolean state) { // állapot beállítása (ha van kapcsolat, akkor a másik lábat is állítja)
        this.state = state;
        if (pair != null && pair.state() != state) pair.state(state);
    }
    public boolean state() { // állapot lekérdezése
        return state;
    }
    public Foot pair() { // a hozzámkapcsolt másik láb lekérdezése
        return pair;
    }
    public void gate(Gate gate) { // gazda kapu beállítása
        if ( this.gate != gate ) needInputCheck = true;
        this.gate = gate;
    }
    public Gate gate() { // gazda kapu lekérdezése
        return gate;
    }
    public boolean isInput() { // típus lekérdezése (igaz, ha bemenet)
        return input;
    }
    public int index() { // megadja, hogy a gazda kapunak hanyadik indexű lába vagyok
        if (gate != null) {
            if ( needInputCheck ) inputCheck();
            int index = 0;
            ArrayList<Foot> foots = input ? gate.getInputs() : gate.getOutputs();
            for ( Foot f : foots ) {
                if (f == this) return index;
                index++;
            }
        }
        return 0;
    }
    public Interface.Point getCoords() { // koordináták lekérdezése
        if (gate != null) {
            if ( needInputCheck ) inputCheck();
            int index = 0;
            ArrayList<Foot> foots = input ? gate.getInputs() : gate.getOutputs();
            for ( Foot f : foots ) {
                if (f == this) return input ? gate.getInputXY(index) : gate.getOutputXY(index);
                index++;
            }
        }
        return new Interface.Point(0,0);
    }
    public void Draw(java.awt.Graphics g) { // láb és lehetséges kapcsolat rajzolása
        Interface.Point p1, p2;
        g.setColor(state ? Interface.Theme.wirecolor_on : Interface.Theme.wirecolor);
        p1 = getCoords();
        if (input) g.drawRect(p1.x()-2, p1.y()-1, 2, 2);
        else g.drawRect(p1.x(), p1.y()-1, 2, 2);

        if (pair != null && !input) {
            p2 = pair.getCoords();
            Wire.Draw(g, p1, p2);
        }
    }
}
