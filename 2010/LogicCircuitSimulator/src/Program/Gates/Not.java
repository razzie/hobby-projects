/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Foot;
import Program.Gate;
import java.awt.Graphics;

public class Not extends Gate {
    public Not() {
        super(Type.Not);
        setInputNum(1);
        setOutputNum(1);
    }
    
    public void Simulate() {
        for (Foot f : inputs) {
            if (f.state() == true) {
                setOutputs(false);
                return;
            }
        }
        setOutputs(true);
    }
    public Gate copy() {
        Gate gate = new Not();
        gate.setInputNum(this.inputs.size());
        gate.setOutputNum(this.outputs.size());
        gate.setCoords(x, y);
        gate.rename(name);
        return gate;
    }

    public int getWidth() {
        return 32;
    }
    public int getHeight() {
        return 24;
    }
    public void Draw(Graphics g) {
        java.awt.Polygon poly = new java.awt.Polygon();
        poly.addPoint(x, y);
        poly.addPoint(x+24, y+12);
        poly.addPoint(x, y+24);
        g.setColor(Interface.Theme.gatecolor);
        g.fillPolygon(poly);
        g.fillOval(x+24, y+8, 8, 8);
        g.setColor(Interface.Theme.wirecolor);
        g.drawPolygon(poly);
        g.drawOval(x+24, y+8, 8, 8);
    }
    public Point getInputXY(int index) {
        return new Point( x , y+12 );
    }
    public Point getOutputXY(int index) {
        return new Point( x+32 , y+12 );
    }
}
