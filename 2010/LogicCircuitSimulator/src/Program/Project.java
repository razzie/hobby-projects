/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program;

import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;
import Program.Gates.*;

public class Project {
    private String name; // projekt neve
    private static int projectID; // project azonosító új projekt esetén
    private boolean saved; // jelzi, hogy a projekt mentve van-e
    ArrayList<Gate> gates; // elemlista
    private java.io.File file; // fájl azonosító

    public Project() { // konstruktor új projekt esetére
        gates = new ArrayList();
        projectID++;
        name = "Project "+projectID; // alapértelmezett név
        file = null;
        saved = false;
    }
    private Project(String name, ArrayList<Gate> gates, java.io.File file) { // privát konstruktor projekt betöltéséhez
        this.gates = gates;
        this.name = name;
        this.file = file;
        saved = true;
    }

    public String name() { // projekt nevének lekérdezése
        return name;
    }
    public void rename(String name) { // projekt átnevezése
        this.name = name;
    }
    public void unsave() { // jelzés a projektnek, hogy módosítva lett
        saved = false;
    }
    public boolean saved() { // megadja, hogy a projekt mentve van-e
        return saved;
    }

    public void addGate(Gate gate, int x, int y) { // elem hozzáadása az elemlistához a megadott x,y koordinátákkal
        gate.setCoords(x, y);
        gates.add(gate);
    }
    public void removeGate(Gate gate) { // elem eltávolítása au elemlistából
        gate.clear(); // elem kapcsolatainak törlése
        gates.remove(gate);
    }
    public void removeGates(ArrayList<Gate> list) { // egyszerre több elem eltávolítása
        for ( Gate gate : list ) removeGate(gate);
    }

    public ArrayList<Gate> getList() { // projekt elemlistájának lekérdezésa
        return gates;
    }

    public boolean save() { // projekt mentése
        return Save(this, file);
    }
    public boolean saveAs() { // projekt mentése másként
        return Save(this, null);
    }

    public void optimize() { // elemlista optimalizált rendezése
        gates = Gate.optimize(gates);
    }
    public void reset() { // elemlista logikai alapállapotba helyezése
        reset(gates);
    }
    private void reset(ArrayList<Gate> list) { // allista alapállapotba helyezése (rekurzív)
        for ( Gate g : list ) {
            if ( g instanceof Button ) ((Button)g).setState(false);
            else if ( g instanceof Module ) reset( ((Module)g).getList() );
            for ( Foot f : g.getInputs() ) f.state(false);
            for ( Foot f : g.getOutputs() ) f.state(false);
        }
    }

    // Elemlista fájlba írása (statikus). Bemenetek: filewriter, elemlista, elemlisták listája (modulok miatt)
    private static void saveCircuit(FileWriter fw, ArrayList<Gate> gates, ArrayList<ArrayList<Gate>> lists) throws IOException {
                ArrayList<Foot> footlist = new ArrayList();
                String info;
                // egy teljes láb-lista létrehozása
                for ( Gate g : gates ) {
                    footlist.addAll(g.getInputs());
                    footlist.addAll(g.getOutputs());
                }
                fw.write(gates.size()+","+footlist.size()+";\r\n");
                for ( Foot foot : footlist ) fw.write( foot.state()+","+footlist.indexOf(foot.pair())+";\r\n");
                // az elemek fájlba írása
                for ( Gate g : gates ) {
                    info = "";
                    if(g instanceof Button) info = ((Button)g).ButtonType()+",";
                    else if(g instanceof Module) info = lists.indexOf( ((Module)g).getList() )+",";
                    else if(g instanceof Timer) info = ((Timer)g).getTimerOn()+","+((Timer)g).getTimerOff()+",";
                    else if(g instanceof Display) info = ((Display)g).getRows()+","+((Display)g).getColumns()+",";
                    fw.write(g.type()+","+info+g.name()+","+g.getX()+","+g.getY()+","+g.getInputs().size()+","+g.getOutputs().size()+",");
                    for ( Foot foot : g.getInputs() ) fw.write( footlist.indexOf(foot)+"," ); // a lábak azonosítása a láb-lista index alapján
                    for ( Foot foot : g.getOutputs() ) fw.write( footlist.indexOf(foot)+"," );
                    fw.write(";\r\n");
                }
    }
    // Szöveges alapján elemlista létrehozása (statikus). Bemenetek: scanner (szöveg feldolgozása), elemlisták listája (modulok miatt)
    private static ArrayList<Gate> loadCircuit(Scanner s, ArrayList<ArrayList<Gate>> lists) {
        ArrayList<Gate> gates = new ArrayList();
        ArrayList<Foot> foots = new ArrayList();
        Scanner s2 = new Scanner(s.next());
        s2.useDelimiter(",");
        int i,j;
        int gnum = s2.nextInt(); // elemek számának kiolvasása
        int fnum = s2.nextInt(); // lábak számának kiolvasása
        for (i=0;i<fnum;i++) foots.add(new Foot(null)); // láb-lista felépítése
        for (i=0;i<fnum;i++) { // lábak összekötése
            s2 = new Scanner(s.next());
            s2.useDelimiter(",");
            Foot f = foots.get(i);
            f.state( s2.nextBoolean() );
            int pair = s2.nextInt();
            f.set( pair==-1 ? null : foots.get(pair) );
        }
        // elem lista felépítése
        for (i=0;i<gnum;i++) {
           s2 = new Scanner(s.next());
           s2.useDelimiter(",");
           String type=s2.next();
           Gate g=(null);
           if (type.equals("And")) g = new And();
           if (type.equals("Nand")) g = new Nand();
           else if (type.equals("Button"))
               g = new Button(s2.next().equals("PushButton")? Button.ButtonType.PushButton:Button.ButtonType.ToggleButton );
           else if (type.equals("Led")) g = new Led();
           else if (type.equals("Module")) g = new Module( lists.get(s2.nextInt()) );
           else if (type.equals("Not")) g = new Not();
           else if (type.equals("Or")) g = new Or();
           else if (type.equals("Nor")) g = new Nor();
           else if (type.equals("Timer")) {
               g = new Timer();
               ((Timer)g).setTimer(s2.nextInt(), s2.nextInt());
           }
           else if (type.equals("Wirepoint")) g = new Wirepoint();
           else if (type.equals("Xor")) g = new Xor();
           else if (type.equals("Nxor")) g = new Nxor();
           else if (type.equals("Display")) {
               g = new Display();
               ((Display)g).setSize(s2.nextInt(), s2.nextInt());
           }
           else if (type.equals("SSD")) g = new SSD();
           else if (type.equals("JK")) g = new JK();
           g.rename(s2.next());
           g.setCoords(s2.nextInt(),s2.nextInt());
           int inputN=s2.nextInt(),outputN=s2.nextInt();
           g.setInputNum(inputN);g.setOutputNum(outputN);
           // bemenet lábak hozzárendelése a jelenlegi elemhez
           for (j=0;j<inputN;j++) {
               Foot f = foots.get(s2.nextInt());
               g.getInputs().set(j, f);
               f.gate(g);
           }
           // kimenet lábak hozzárendelése a jelenlegi elemhez
           for (j=0;j<outputN;j++) {
               Foot f = foots.get(s2.nextInt());
               g.getOutputs().set(j, f);
               f.gate(g);
           }
           gates.add(g);

        }
        return gates;
    }

    // elemlisták rekurzív feltérképezése és egy nagy listába másolása (a modulok mentése miatt)
    private static void examine(ArrayList<Gate> gates, ArrayList<ArrayList<Gate>> lists) {
        for ( Gate g : gates ) {
            if ( g instanceof Module ) {
                ArrayList<Gate> list = ((Module)g).getList();
                examine( list , lists );
                lists.add( list );
            }
        }
    }
    public static boolean Save(Project p, java.io.File f) { // statikus projekt mentés
        if ( f == null ) { // ha nincs megadva fájl, akkor behoz egy mentés ablakot
            javax.swing.JFileChooser FileChooser = new javax.swing.JFileChooser();
            FileChooser.setDialogTitle("Save project");
            if ( FileChooser.showSaveDialog(null) != FileChooser.APPROVE_OPTION ) return false;
            f = FileChooser.getSelectedFile();
        }
        try {
            if ( f != p.file ) p.file = f;
            // mentés algoritmus
            FileWriter fw = new FileWriter(f);
            ArrayList<ArrayList<Gate>> lists = new ArrayList();
            examine(p.gates, lists);
            lists.add(p.gates);
            fw.write(p.name+","+lists.size()+";\r\n");
            for ( ArrayList<Gate> list : lists ) saveCircuit( fw , list , lists ); // elemlisták fájlba írása
            fw.close();
            p.saved = true;
            return true;
        }
        catch (Exception e) {
            Interface.Forms.Message.dropError("Error saving project :(");
        }
        return false;
    }
    public static Project Load() { // statikus projekt megnyitás
        javax.swing.JFileChooser FileChooser = new javax.swing.JFileChooser();
        FileChooser.setDialogTitle("Open project");
        if ( FileChooser.showOpenDialog(null) == FileChooser.APPROVE_OPTION ) {
            try {
                java.io.File f = FileChooser.getSelectedFile();
                // megnyitás algoritmus
                Scanner s = new Scanner(f);
                s.useDelimiter(";\r\n");
                Scanner s2 = new Scanner(s.next());
                s2.useDelimiter(",");
                String name = s2.next();
                ArrayList<ArrayList<Gate>> lists = new ArrayList();
                int i, cnum = s2.nextInt();
                for (i=0;i<cnum;i++) lists.add( loadCircuit( s , lists ) ); // elemlisták betöltése
                return new Project(name, lists.get(cnum-1), f); // projekt létrehozása, a projekt elemlistája a nagy listában szereplő utolsó lista lesz
            }
            catch (Exception e) {
                Interface.Forms.Message.dropError("Error opening project :(");
                System.out.println(e.getClass());
                e.printStackTrace();
            }
        }
        return null; // hiba esetén null értékkel tér vissza
    }
}
