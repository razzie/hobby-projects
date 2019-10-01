/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Gate;
import java.awt.Graphics;

public class Button extends Gate {
    public enum ButtonType { ToggleButton, PushButton };

    private boolean state, old_state, state_delay, isClicked;
    private ButtonType type;

    public Button(ButtonType type) {
        super(Type.Button);
        setOutputNum(1);
        this.type = type;
        isClicked = false;
        state = old_state = state_delay = false;
    }

    public void Click() {
        if (type == ButtonType.PushButton) {
            isClicked = true;
            state = true;
        } else {
            isClicked = true;
        }
    }
    public void Release() {
        if (type == ButtonType.PushButton) {
            isClicked = false;
            state = false;
        } else {
            if (isClicked) {
                isClicked = false;
                state = !state;
            }
        }
    }
    public void setState(boolean state) {
        this.state = state;
    }
    public boolean getState() {
        return state;
    }
    public ButtonType ButtonType() {
        return type;
    }

    public void Simulate() {
        setOutputs(old_state);
        if (!state_delay) state_delay = true;
        else {
            old_state = state;
            state_delay = false;
        }
    }
    public Gate copy() {
        Gate gate = new Button(type);
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
        g.fillRect(x, y, 32, 32);
        g.setColor(state ? Interface.Theme.buttoncolor_on : Interface.Theme.buttoncolor);
        g.fillRect(x+6, y+6, 20, 20);
        g.setColor(Interface.Theme.wirecolor);
        g.drawRect(x, y, 32, 32);
        g.drawRect(x+6, y+6, 20, 20);
        if (type == ButtonType.ToggleButton) g.drawRect(x+4, y+4, 24, 24);
    }
    public Point getInputXY(int index) {
        return new Point( x , y+16 );
    }
    public Point getOutputXY(int index) {
        return new Point( x+32 , y+16 );
    }
}
