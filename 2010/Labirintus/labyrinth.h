#ifndef _LABYRINTH_H_
#define _LABYRINTH_H_

#include <fstream>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int SOR;
    int OSZLOP;
    int **LV;
    int **LF;
    int **LP;
} LStruct;

class Labyrinth {
    private:
        int SOR, OSZLOP;
        int **LV, **LF, **LP;
        void genWall(int oszlop,int sor,int wall);
        void genFloor(int oszlop,int sor,int floor);
        void genDiffNegyzet(int diff);
        void genDiffKor(int diff);
        void genDiffKor2(int diff,int diff2);
        void genL(int vKezd,int fKezd,int vBef,int fBef,int wallNum);

    public:
        //Labyrinth(std::ifstream *file); // ifstream : írásra
        Labyrinth(int SOR, int OSZLOP, int fal1, int fal2, int fal3, bool isNegyzet);
        void Debug();
        void makeHoles();
        //bool Save(std::ofstream *file); // ofstream : olvasásra
        LStruct get();
        ~Labyrinth();
};

#endif
