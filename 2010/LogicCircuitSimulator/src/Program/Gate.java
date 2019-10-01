/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program;

import java.util.ArrayList;
import Program.Gates.*;

public abstract class Gate { // áramköri elem (absztrakt osztály, csak származtatással ahsználható)
    public enum Type { Button, And, Nand, Or, Nor, Not, Xor, Nxor, Wirepoint, Led, Display, SSD, Timer, Module, JK };
    private final Type type; // az elem típusa
    protected String name; // az elem neve a munkalapon
    protected ArrayList<Foot> inputs, outputs; // be- és kimenet lábak listái
    protected int x, y; // koordináták a munkalapon
    private boolean processed; // <- pointless

    public Gate(Type type) { // konstruktor a típus megadásával (a típus később nem változtatható meg)
        name = "";
        inputs = new ArrayList();
        outputs = new ArrayList();
        this.type = type;
        x = y = 0;
    }
    @Override
    public void finalize() {
        clear();
    }
    @Override
    public String toString() {
        return "("+type+", "+x+", "+y+") "+name;
    }

    public Type type() { // típus lekérdezése
        return type;
    }

    public String name() { // név lekérdezése
        return name;
    }
    public void rename(String name) { // elem átnevezése
        this.name = name;
    }

    public void setInputNum(int inputNum) { // bemenetek számának állítása
        int i, size = inputs.size();
        if (inputNum > size) {
            // több bemenet esetén a bemenet lábak listájának bővítése
            for (i=size;i<inputNum;i++) inputs.add(new Foot(this));
        } else if (inputNum < size) {
            // kevesebb bemenet esetén a lista utolsó lábainak törlése
            for (i=size;i>inputNum;i--) {
                inputs.get(i-1).set(null);
                inputs.remove(i-1);
            }
        }
    }
    public void setOutputNum(int outputNum) { // kimenetek számának állítása
        int i, size = outputs.size();
        if (outputNum > size) {
            // több kimenet esetén a bemenet lábak listájának bővítése
            for (i=size;i<outputNum;i++) outputs.add(new Foot(this));
        } else if (outputNum < size) {
            // kevesebb kimenet esetén a lista utolsó lábainak törlése
            for (i=size;i>outputNum;i--) {
                outputs.get(i-1).set(null);
                outputs.remove(i-1);
            }
        }
    }
    public ArrayList<Foot> getInputs() { // bemeneti lábak listájának lekérdezése
        return inputs;
    }
    public ArrayList<Foot> getOutputs() { // kimeneti lábak listájának lekérdezése
        return outputs;
    }
    public void setOutputs(boolean state) { // a kimeneti lábak állapotát állítja 'state' állapotra
        for (Foot f : outputs) f.state(state);
    }

    public void clear() { // be- és kimenetek eltávolítása (tipikusan az elem törlése előtt)
        for ( Foot f : inputs ) f.set(null);
        for ( Foot f : outputs ) f.set(null);
    }
    public void setup() { // megfelelő beállító ablak hívása
        if ( this instanceof Timer ) new Interface.Forms.TimerSetup((Timer)this);
        else if ( this instanceof Display ) new Interface.Forms.DisplaySetup((Display)this);
        else if ( this instanceof JK ) new Interface.Forms.JKsetup((JK)this);
        else if ( this instanceof Module || this instanceof SSD ) Interface.Forms.Message.dropWarning("Ez az elem nem állítható be :(");
        else new Interface.Forms.FootSetup(this);
    }
    public boolean extendableInputs() {
        switch (type) {
            case Button:
            case Timer:
            case Display:
            case JK:
            case SSD: return false;
            default: return true;
        }
    }
    public boolean extendableOutputs() {
        switch (type) {
            case Display:
            case JK:
            case SSD: return false;
            default: return true;
        }
    }
    public int defaultInputNum() {
        switch (type) {
            case Led:
            case Wirepoint: return 1;
            case And:
            case Nand:
            case Or:
            case Nor:
            case Xor:
            case Nxor: return 2;
            case JK: return 3;
            default: return 0;
        }
    }
    public int defaultOutputNum() {
        switch (type) {
            case Button:
            case Timer:
            case Wirepoint:
            case And:
            case Nand:
            case Or:
            case Nor:
            case Xor:
            case Nxor: return 1;
            case JK: return 2;
            default: return 0;
        }
    }

    final public static ArrayList<Gate> copy(ArrayList<Gate> list) { // statikus elemlista másoló
        ArrayList<Gate> newlist = new ArrayList();
        for ( Gate g : list ) newlist.add(g.copy()); // elemek másolatainak létrehozása az új listában (még nincsenek kapcsolatok)
        int i, j, indexOfPair, size = list.size(), size2;
        Gate g1, g2;
        Foot f1, f2;
        // az új lista bejárása
        for (i=0;i<size;i++) {
            g1 = list.get(i);
            g2 = newlist.get(i);
            // bemenetek bejárása a régi kapcsolatok helyreállítása miatt
            size2 = g1.getInputs().size();
            for (j=0;j<size2;j++) {
                f1 = g1.getInputs().get(j);
                f2 = g2.getInputs().get(j);
                // csak akkor állítja vissza a kapcsolatot ha a mutatott elem is a másolandó elemek között volt
                if ( f1.pair() != null && list.contains(f1.pair().gate()) ) {
                    indexOfPair = list.indexOf(f1.pair().gate());
                    f2.set( newlist.get(indexOfPair).getOutputs().get( f1.pair().index() ) );
                }
                f2.state( f1.state() );
            }
            // kimenetek bejárása a régi kapcsolatok helyreállítása miatt
            size2 = g1.getOutputs().size();
            for (j=0;j<size2;j++) {
                f1 = g1.getOutputs().get(j);
                f2 = g2.getOutputs().get(j);
                // csak akkor állítja vissza a kapcsolatot ha a mutatott elem is a másolandó elemek között volt
                if ( f1.pair() != null && list.contains(f1.pair().gate()) ) {
                    indexOfPair = list.indexOf(f1.pair().gate());
                    f2.set( newlist.get(indexOfPair).getInputs().get( f1.pair().index() ) );
                }
                f2.state( f1.state() );
            }
        }
        return newlist;
    }
    public static ArrayList<Gate> optimize(ArrayList<Gate> list) { // statikus elemlista optimalizáló
        ArrayList<Gate> optimized = new ArrayList();
        int i, size = list.size(); // régi lista mérete
        boolean found = false;
        for ( Gate g : list ) {
            g.processed = false;
            if ( g instanceof Button || g instanceof Timer ) { // kapcsolók és időzítők (bemenetek) hozzáadása az új listához
                optimized.add(g);
                found  = true;
                g.processed = true;
            }
        }
        if ( !found ) { // ha nincs bemenet az eredeti listában
            optimized.add(list.get(0));
            list.get(0).processed = true;
        }
        for (i=0; i<size; i++) { // az új listának ugyanannyi eleme lesz, mint a réginek, ezért annak a mérete használható
            Gate g = optimized.get(i);
            for ( Foot f : g.getOutputs() ) { // kimenet lábak bejárása
                Gate pg = (f.pair()!=null) ? (f.pair().gate()) : null;
                if ( !( pg == null ||
                        pg.processed ||
                        pg instanceof Led ||
                        pg instanceof SSD ||
                        pg instanceof Display) ) { // az elemhez kapcsolt más elemek hozzáadása az új listához
                    optimized.add( pg );
                    pg.processed = true;
                }
            }
            if ( optimized.size()==(i+1) && i<size-1 ) { // ha nincs következő elem a listában és még nem a lista végén vagyok
                for ( Gate x : list )
                    if ( !x.processed ) { // egy (még nem feldolgozott) elem hozzáadása az új listához
                        optimized.add(x);
                        x.processed = true;
                        break;
                    }
            }
        }
        return optimized;
    }

    public abstract void Simulate(); // az elem kimeneteinek kiértékelése
    public abstract Gate copy(); // egy másolatot hoz létre az elemről

    public boolean clicked(int mx, int my, int border) { // megvizsgálja, hogy az adott koordináták az elemen helyezkednek-e el (border: tűréshatár)
        if ( mx>=x-border && mx<=x+getWidth()+border*2 && my>=y-border && my<=y+getHeight()+border*2 ) return true;
        else return false;
    }
    public boolean inside(int x1, int y1, int x2, int y2, int border) { // megvizsgálja, hogy az elem a megadott kijelölésen belül van-e (border: tűréshatár)
        if ( x2 < x1 ) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        if ( y2 < y1 ) {
            int tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        if ( x-border>=x1 && y-border>=y1 && x+getWidth()+border<=x2 && y+getHeight()+border<=y2 ) return true;
        else return false;
    }
    public void setCoords(int x, int y) { // koordináták beállítása
        this.x = x;
        this.y = y;
    }
    public int getX() { // x koordináta lekérdezése
        return x;
    }
    public int getY() { // y koordináta lekérdezése
        return y;
    }
    public abstract int getWidth(); // az elem szélességének lekérdezése
    public abstract int getHeight(); // az elem magasságának lekérdezése
    public abstract void Draw(java.awt.Graphics g); // az elem kirajzolása
    public abstract Interface.Point getInputXY(int index); // a megadott számú bemenet láb koordinátái (kezdőérték: 0)
    public abstract Interface.Point getOutputXY(int index); // a megadott számú kimenet láb koordinátái (kezdőérték: 0)
}
