/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

import java.awt.Color;
import java.io.*;
import java.util.Scanner;

public class Theme {
    public static Color bgcolor,selection,gatecolor,modulecolor,wirecolor,wirecolor_on,ledcolor,ledcolor_on,buttoncolor,buttoncolor_on;
    static {
        Default(); // inicializáláskor az alapértelmezett (Default) témát állítja be
    }

    public static void Default() { // alapértelmezett téma
        bgcolor = Color.WHITE;
        selection = Color.BLUE;
        gatecolor = Color.WHITE;
        modulecolor = Color.DARK_GRAY;
        wirecolor = Color.BLACK;
        wirecolor_on = Color.ORANGE;
        ledcolor = Color.GRAY;
        ledcolor_on = Color.RED;
        buttoncolor = Color.LIGHT_GRAY;
        buttoncolor_on = Color.GREEN;
    }
    public static void Dark() { // sötét téma
        bgcolor = Color.BLACK;
        selection = Color.WHITE;
        gatecolor = Color.GRAY;
        modulecolor = Color.DARK_GRAY;
        wirecolor = Color.DARK_GRAY;
        wirecolor_on = Color.ORANGE;
        ledcolor = Color.GRAY;
        ledcolor_on = Color.ORANGE;
        buttoncolor = Color.LIGHT_GRAY;
        buttoncolor_on = Color.BLUE;
    }

    private static String getColor(Color c) { // szín szöveges kóddá konvertálása
        return c.getRed()+","+c.getGreen()+","+c.getBlue();
    }
    private static Color makeColor(String rgb) { // szöveges kód színné konvertálása
        Scanner s = new Scanner(rgb);
        s.useDelimiter(",");
        return new Color(s.nextInt(), s.nextInt(), s.nextInt());
    }

    public static void Load() { // téma betöltése
        javax.swing.JFileChooser FileChooser = new javax.swing.JFileChooser();
        FileChooser.setDialogTitle("Open theme");
        if ( FileChooser.showOpenDialog(null) == FileChooser.APPROVE_OPTION ) {
            try {
                java.io.File f = FileChooser.getSelectedFile();
                // megnyitás algoritmus
                Scanner s = new Scanner(f);
                s.useDelimiter(";");
                bgcolor = makeColor(s.next());
                selection = makeColor(s.next());
                gatecolor = makeColor(s.next());
                modulecolor = makeColor(s.next());
                wirecolor = makeColor(s.next());
                wirecolor_on = makeColor(s.next());
                ledcolor = makeColor(s.next());
                ledcolor_on = makeColor(s.next());
                buttoncolor = makeColor(s.next());
                buttoncolor_on = makeColor(s.next());
                s.close();
            }
            catch (Exception e) {
                Interface.Forms.Message.dropError("Error opening theme :(");
            }
        }
    }
    public static void Save() { // téma mentése
        javax.swing.JFileChooser FileChooser = new javax.swing.JFileChooser();
        FileChooser.setDialogTitle("Save theme");
        if ( FileChooser.showSaveDialog(null) == FileChooser.APPROVE_OPTION ) {
            try {
                java.io.File f = FileChooser.getSelectedFile();
                // mentés algoritmus
                FileWriter fw = new FileWriter(f);
                fw.write(getColor(bgcolor)+";");
                fw.write(getColor(selection)+";");
                fw.write(getColor(gatecolor)+";");
                fw.write(getColor(modulecolor)+";");
                fw.write(getColor(wirecolor)+";");
                fw.write(getColor(wirecolor_on)+";");
                fw.write(getColor(ledcolor)+";");
                fw.write(getColor(ledcolor_on)+";");
                fw.write(getColor(buttoncolor)+";");
                fw.write(getColor(buttoncolor_on)+"");
                fw.close();
            }
            catch (Exception e) {
                Interface.Forms.Message.dropError("Error saving theme :(");
            }
        }
    }
}
