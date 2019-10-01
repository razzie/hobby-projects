/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program;

import java.util.ArrayList;

public class History { // előzménykezelő osztály
    private class Savepoint { // alosztály elemlista-másolatok tárolására
        ArrayList<Gate> gates;
        Savepoint(ArrayList<Gate> gates) {
            this.gates = Gate.copy(gates);
        }
    }
    private ArrayList<Savepoint> saves = new ArrayList(); // előzménylista (listamásolatok listája)
    private Project p; // gazda project (ennek az elemlistáját másolom)
    private int current; // jelenlegi pozíció az előzménylistában

    public History(Project p) { // konstruktor projekt megadásával (később nem változtatható)
        this.p = p;
        current = 0;
    }

    public void addSavepoint() { // új előzmény pont létrehozása
        p.unsave(); // jelzés a projektnek hogy módosítva lett
        saves.add( current, new Savepoint(p.getList()) );
        current++; // jelenlegi pozíció állítása az új előzmény állapotra
    }
    public void free() { // előzmények törlése
        saves.clear();
        current = 0;
    }

    public boolean undo() { // visszavonás
        if ( current > 0 ) { // ha létezik előzmény
            if ( current == saves.size() ) { // ha visszalépek egy előző állapotra, a mostanit elmentem (az ismét gomb miatt)
                addSavepoint();
                current--;
            }
            p.gates = saves.get(current-1).gates; // előző állapot betöltése az előzményből
            current--; // pozíció változtatása az előzménylistában
            return true;
        }
        return false;
    }
    public boolean redo() { // ismét
        if ( current < saves.size()-1 ) { // ha létezik frissebb előzmény
            current++; // pozíció mozgatása a frissebb előzményre
            p.gates = saves.get(current).gates; // frissebb előzmény betöltése
            return true;
        }
        return false;
    }
}
