/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Gate;
import java.awt.Graphics;

public class Timer extends Gate {
    private int msOn, msOff;
    
    public Timer() {
        super(Type.Timer);
        setOutputNum(1);
        msOn = msOff = 500;
    }

    public void setTimer(int msOn, int msOff) {
        this.msOn = msOn;
        this.msOff = msOff;
    }
    public int getTimerOn() {
        return msOn;
    }
    public int getTimerOff() {
        return msOff;
    }

    public void Simulate() {
        long ms = System.currentTimeMillis() % (msOn + msOff);
        if (ms >= 0 && ms < msOn) {
            setOutputs(true);
        } else {
            setOutputs(false);
        }
    }
    public Gate copy() {
        Gate gate = new Timer();
        gate.setInputNum(this.inputs.size());
        gate.setOutputNum(this.outputs.size());
        gate.setCoords(x, y);
        gate.rename(name);
        ((Timer)gate).setTimer(msOn, msOff);
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
        g.fillRect(x, y, 32, 32);
        g.setColor(Interface.Theme.wirecolor);
        g.drawLine(x+4, y+20, x+12, y+20);
        g.drawLine(x+12, y+12, x+12, y+20);
        g.drawLine(x+12, y+12, x+20, y+12);
        g.drawLine(x+20, y+12, x+20, y+20);
        g.drawLine(x+20, y+20, x+28, y+20);
        g.drawRect(x, y, 32, 32);
    }
    public Point getInputXY(int index) {
        return new Point( x , y+16 );
    }
    public Point getOutputXY(int index) {
        return new Point( x+32 , y+16 );
    }
}
