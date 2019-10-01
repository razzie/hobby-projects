/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Foot;
import Program.Gate;
import java.util.ArrayList;
import java.awt.Graphics;

class Sort implements java.util.Comparator {
    public int compare(Object o1, Object o2) {
        if ( (o1 instanceof Button || o1 instanceof Led) && (o2 instanceof Button || o2 instanceof Led) ) {
            Gate g1 = (Gate)o1, g2 = (Gate)o2;
            if (g1.getY() == g2.getY()) {
                return g1.getX()-g2.getX();
            } else {
                return g1.getY()-g2.getY();
            }
        } else {
            return 0;
        }
    }
    public boolean equals(Object o1, Object o2) {
        if ( (o1 instanceof Button || o1 instanceof Led) && (o2 instanceof Button || o2 instanceof Led) ) {
            Gate g1 = (Gate)o1, g2 = (Gate)o2;
            if (g1.getY() == g2.getY() && g1.getX() == g2.getX()) return true;
            else return false;
        } else {
            return true;
        }
    }
}

public class Module extends Gate {
    private ArrayList<Gate> gates;

    public Module(final ArrayList<Gate> list) {
        super(Type.Module);
        gates = copy(list);
        java.util.Collections.sort(gates, new Sort());
        for ( Gate g : gates ) {
            switch ( g.type() ) {
                case Button:
                    inputs.add( new Foot(this) );
                    break;
                case Led:
                    outputs.add( new Foot(this) );
                    break;
            }
        }
    }

    public ArrayList<Gate> getList() {
        return gates;
    }

    public void Simulate() {
        // aláramkör kapcsolóinak beállítása
        int i = 0;
        for ( Gate g : gates ) {
            if ( g.type() == Type.Button ) {
                ((Button)g).setState( inputs.get(i).state() );
                i++;
            }
        }
        // szimuláció
        for ( Gate g : gates ) g.Simulate();
        // aláramkör LEDjeinek beállítása
        i = 0;
        for ( Gate g : gates ) {
            if ( g.type() == Type.Led ) {
                outputs.get(i).state( ((Led)g).getState() );
                i++;
            }
        }
    }
    public Gate copy() {
        Gate gate = new Module(gates);
        gate.setCoords(x, y);
        gate.rename(name);
        return gate;
    }

    public int getWidth() {
        return 24;
    }
    public int getHeight() {
        int inputH = inputs.size(), outputH = outputs.size();
        int height = ((inputH>outputH) ? inputH : outputH) * 12;
        return (height > 0) ? height : 12;
    }
    public void Draw(Graphics g) {
        int height = getHeight();
        g.setColor(Interface.Theme.modulecolor);
        g.fillRect(x, y, 24, height);
        g.setColor(Interface.Theme.wirecolor);
        g.drawRect(x, y, 24, height);
    }
    public Point getInputXY(int index) {
        int height = getHeight() / inputs.size();
        return new Point( x , y + height*index + height/2 );
    }
    public Point getOutputXY(int index) {
        int height = getHeight() / outputs.size();
        return new Point( x+24 , y + height*index + height/2 );
    }
}
