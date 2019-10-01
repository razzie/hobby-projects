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

public class Wirepoint extends Gate {
    private boolean lastState;

    public Wirepoint() {
        super(Type.Wirepoint);
        setInputNum(1);
        setOutputNum(1);
        lastState = false;
    }

    public void Simulate() {
        for (Foot f : inputs) {
            if (f.state() == true) {
                setOutputs(true);
                lastState = true;
                return;
            }
        }
        setOutputs(false);
        lastState = false;
    }
    public Gate copy() {
        Gate gate = new Wirepoint();
        gate.setInputNum(this.inputs.size());
        gate.setOutputNum(this.outputs.size());
        gate.setCoords(x, y);
        gate.rename(name);
        return gate;
    }

    public int getWidth() {
        return 16;
    }
    public int getHeight() {
        return 16;
    }
    public void Draw(Graphics g) {
        g.setColor( lastState ? Interface.Theme.wirecolor_on : Interface.Theme.wirecolor );
        g.fillOval(x+5, y+5, 6, 6);
    }
    public Point getInputXY(int index) {
        return new Point( x+4 , y+8 );
    }
    public Point getOutputXY(int index) {
        return new Point( x+12 , y+8 );
    }
}
