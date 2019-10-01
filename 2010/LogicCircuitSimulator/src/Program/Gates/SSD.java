/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Gate;
import java.awt.Graphics;

public class SSD extends Gate {
    private int state;

    public SSD() {
        super(Type.SSD);
        setInputNum(4);
        state = 0;
    }
    
    public void Simulate() {
        int x1 = inputs.get(0).state()==true ? 1 : 0;
        int x2 = inputs.get(1).state()==true ? 2 : 0;
        int x3 = inputs.get(2).state()==true ? 4 : 0;
        int x4 = inputs.get(3).state()==true ? 8 : 0;
        state = x1 + x2 + x3 + x4;
    }
    public Gate copy() {
        Gate gate = new SSD();
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
        return 48;
    }
    public void Draw(Graphics g) {
        int i = state;
        g.setColor(Interface.Theme.modulecolor);
        g.fillRect(x, y, 32, 48);
        g.setColor(Interface.Theme.wirecolor);
        g.drawRect(x, y, 32, 48);
        g.setColor(segment(1,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+7, y+1, 18, 2);
        g.setColor(segment(2,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+4, y+4, 2, 18);
        g.setColor(segment(3,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+26, y+4, 2, 18);
        g.setColor(segment(4,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+7, y+23, 18, 2);
        g.setColor(segment(5,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+4, y+26, 2, 18);
        g.setColor(segment(6,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+26, y+26, 2, 18);
        g.setColor(segment(7,state) ? Interface.Theme.ledcolor_on : Interface.Theme.modulecolor);
        g.fillRect(x+7, y+45, 18, 2);
}
    public Point getInputXY(int index) {
        return new Point( x , y+6+(index*12) );
    }
    public Point getOutputXY(int index) {
        return new Point( x+32 , y+24 );
    }

    private boolean segment(int segment, int i) {
        switch (segment) {
            case 1: return (i==0||i==2||i==3||i==5||i==6||i==7||i==8||i==9||i==10||i==14||i==15);
            case 2: return (i==0||i==4||i==5||i==6||i==8||i==9||i==10||i==11||i==14||i==15);
            case 3: return (i==0||i==1||i==2||i==3||i==4||i==7||i==8||i==9||i==10||i==13);
            case 4: return (i==2||i==3||i==4||i==5||i==6||i==8||i==9||i==10||i==11||i==12||i==13||i==14||i==15);
            case 5: return (i==0||i==2||i==6||i==8||i==10||i==11||i==12||i==13||i==14||i==15);
            case 6: return (i==0||i==1||i==3||i==4||i==5||i==6||i==7||i==8||i==9||i==10||i==11||i==13);
            case 7: return (i==0||i==2||i==3||i==5||i==6||i==8||i==9||i==11||i==12||i==13||i==14);
        }
        return false;
    }
}
