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

public class Xor extends Gate {
    public Xor() {
        super(Type.Xor);
        setInputNum(2);
        setOutputNum(1);
    }

    public void Simulate() {
        int counter = 0;
        for (Foot f : inputs) {
            if (f.state() == true) counter++;
            if (counter > 1) break;
        }
        setOutputs(counter == 1);
    }
    public Gate copy() {
        Gate gate = new Xor();
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
        g.fillArc(x-24, y, 56, height, -90, 180);
        g.setColor(Interface.Theme.bgcolor);
        g.fillArc(x-4, y, 16, height, -90, 180);
        g.setColor(Interface.Theme.wirecolor);
        g.drawArc(x-24, y, 56, height, -90, 180);
        g.drawArc(x-4, y, 16, height, -90, 180);
        g.drawArc(x-8, y, 16, height, -90, 180);
    }
    public Point getInputXY(int index) {
        int height = getHeight() / inputs.size();
        return new Point( x+1 , y + height*index + height/2 );
    }
    public Point getOutputXY(int index) {
        int height = getHeight();
        return new Point( x+32 , y + height/2 );
    }
}
