/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface;

public class Point { // egyszerű 2D pont osztály (koordinátákhoz)
    private int x, y;

    public Point(int x, int y) { // konstruktor a koordináták megadásával (később nem módosíthatóak)
        this.x = x;
        this.y = y;
    }

    public int x() { // x koordináta lekérdezése
        return x;
    }
    public int y() { // y koordináta lekérdezése
        return y;
    }
    public int distSq(int a, int b) { // távolságnégyzet egy másik ponttól
        return (a-x)*(a-x) + (b-y)*(b-y);
    }
}
