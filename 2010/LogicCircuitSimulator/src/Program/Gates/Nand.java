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

public class Nand extends Gate {
    public Nand() {
        super(Type.Nand);
        setInputNum(2);
        setOutputNum(1);
    }
    
    public void Simulate() {
        for (Foot f : inputs) {
            if (f.state() == false) {
                setOutputs(true);
                return;
            }
        }
        setOutputs(false);
    }
    public Gate copy() {
        Gate gate = new Nand();
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
        int height = inputs.size() * 12;
        return (height>32) ? height : 32;
    }
    public void Draw(Graphics g) {
        int height = getHeight();
        g.setColor(Interface.Theme.gatecolor);
        g.fillArc(x-24, y, 48, height, -90, 180);
        g.setColor(Interface.Theme.wirecolor);
        g.drawArc(x-24, y, 48, height, -90, 180);
        g.drawLine(x, y, x, y+height);
        g.drawOval(x+24, y+(height/2)-4, 8, 8);
    }
    public Point getInputXY(int index) {
        int height = getHeight() / inputs.size();
        return new Point( x , y + height*index + height/2 );
    }
    public Point getOutputXY(int index) {
        int height = getHeight();
        return new Point( x+32 , y + height/2 );
    }
}
