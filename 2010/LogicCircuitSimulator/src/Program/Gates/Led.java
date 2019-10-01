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

public class Led extends Gate {
    private boolean state, old_state;

    public Led() {
        super(Type.Led);
        setInputNum(1);
        state = old_state = false;
    }
    
    public void setState(boolean state) {
        this.state = state;
    }
    public boolean getState() {
        return old_state;
    }

    public void Simulate() {
        old_state = state;
        for (Foot f : inputs) {
            if (f.state() == true) {
                setOutputs(true);
                state = true;
                return;
            }
        }
        setOutputs(false);
        state = false;
    }
    public Gate copy() {
        Gate gate = new Led();
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
        return 32;
    }
    public void Draw(Graphics g) {
        g.setColor(Interface.Theme.gatecolor);
        g.fillOval(x, y, 32, 32);
        g.setColor(state ? Interface.Theme.ledcolor_on : Interface.Theme.ledcolor);
        g.fillOval(x+4, y+4, 24, 24);
        g.setColor(Interface.Theme.wirecolor);
        g.drawOval(x, y, 32, 32);
        g.drawOval(x+4, y+4, 24, 24);
    }
    public Point getInputXY(int index) {
        return new Point( x , y+16 );
    }
    public Point getOutputXY(int index) {
        return new Point( x+32 , y+16 );
    }
}
