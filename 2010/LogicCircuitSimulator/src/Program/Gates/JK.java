/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Gate;
import java.awt.Graphics;

public class JK extends Gate {
    private boolean C, Q;

    public JK() {
        super(Type.JK);
        setInputNum(3);
        inputs.get(0).state(true);
        inputs.get(2).state(true);
        setOutputNum(2);
        C = false;
        Q = true;
    }

    public void Simulate() {
        boolean J = inputs.get(0).state(), K = inputs.get(2).state(), C_new = inputs.get(1).state();
        if ( C_new != C && !C_new ) {
            if ( J && K ) {
                Q = !Q;
            } else if ( J || K ) {
                Q = J;
            }
        }
        C = C_new;
        outputs.get(0).state(Q);
        outputs.get(1).state(!Q);
    }
    public Gate copy() {
        Gate gate = new JK();
        gate.setInputNum(3);
        gate.setOutputNum(2);
        gate.setCoords(x, y);
        gate.rename(name);
        return gate;
    }

    public int getWidth() {
        return 36;
    }
    public int getHeight() {
        return 54;
    }
    public void Draw(Graphics g) {
        g.setColor(Interface.Theme.gatecolor);
        g.fillRect(x, y, 36, 54);
        g.setColor(Interface.Theme.wirecolor);
        g.drawRect(x, y, 36, 54);
        g.drawString("J", x+3, y+13);
        g.drawString("C", x+3, y+31);
        g.drawString("K", x+3, y+49);
        g.drawString("Q", x+26, y+16);
        g.drawString("Q'", x+26, y+43);
    }
    public Point getInputXY(int index) {
        return new Point( x , y + index*18 + 9 );
    }
    public Point getOutputXY(int index) {
        return new Point( x+36 , y + index*27 + 13 );
    }
}
