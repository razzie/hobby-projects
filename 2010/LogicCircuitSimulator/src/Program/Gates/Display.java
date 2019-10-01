/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Program.Gates;

import Interface.Point;
import Program.Gate;
import java.awt.Graphics;

public class Display extends Gate {
    private int rows, columns;
    private boolean[] states;

    public Display() {
        super(Type.Display);
        setInputNum(9);
        rows = columns = 3;
        states = new boolean[9];
    }

    public void setSize(int rows, int columns) {
        this.rows = rows;
        this.columns = columns;
        states = new boolean[ rows * columns ];
        setInputNum( rows * columns );
    }
    public int getRows() {
        return rows;
    }
    public int getColumns() {
        return columns;
    }

    public void Simulate() {
        int i, size = rows*columns;
        for (i=0;i<size;i++) states[i] = getInputs().get(i).state();
    }
    public Gate copy() {
        Gate gate = new Display();
        gate.setInputNum(this.inputs.size());
        gate.setOutputNum(this.outputs.size());
        gate.setCoords(x, y);
        gate.rename(name);
        ((Display)gate).setSize(rows, columns);
        return gate;
    }

    public int getWidth() {
        return columns * 12;
    }
    public int getHeight() {
        return rows * 24;
    }
    public void Draw(Graphics g) {
        int i,j;
        g.setColor(Interface.Theme.ledcolor_on);
        for (i=0;i<rows;i++) {
            for (j=0;j<columns;j++) {
                g.setColor( states[i*columns+j] ? Interface.Theme.ledcolor_on : Interface.Theme.ledcolor );
                g.fillRect(x+j*12, y+i*12, 12, 12);
                g.setColor( Interface.Theme.wirecolor );
                g.drawRect(x+j*12, y+i*12, 12, 12);
            }
        }
    }
    public Point getInputXY(int index) {
        return new Point ( x + (index%columns)*12 + 7 , y + rows*12 + (index/columns)*12 + 7 );
    }
    public Point getOutputXY(int index) {
        return new Point( x , y );
    }
}
