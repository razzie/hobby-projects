#include <iostream>
#include <time.h>
#include "labyrinth.h"

using namespace std;

/*Labyrinth::Labyrinth(ifstream *file) {
    //ennél a konstruktornál nem generálódik labirintus, hanem fájlból kell betölteni

}*/

Labyrinth::Labyrinth(int SOR, int OSZLOP, int fal1, int fal2, int fal3, bool isNegyzet) {
    int i,j;
    this->SOR = SOR;
    this->OSZLOP = OSZLOP;
    LV = (int**)malloc(sizeof(int)*(SOR+1)); // vízszintes
    LF = (int**)malloc(sizeof(int)*(SOR+1)); // függõleges
    LP = (int**)malloc(sizeof(int)*(SOR)); // padló
    for (i=0;i<SOR+1;i++) {
        LV[i] = (int*)malloc(sizeof(int)*(OSZLOP+1));
        LF[i] = (int*)malloc(sizeof(int)*(OSZLOP+1));
        if (i<SOR) LP[i] = (int*)malloc(sizeof(int)*(OSZLOP));
    }
    for (i=0;i<SOR+1;i++) {
        for (j=0;j<OSZLOP+1;j++) {
            LV[i][j] = 0;
            LF[i][j] = 0;
            if (i<SOR && j<OSZLOP) LP[i][j] = 0;
        }
    }

    //srand(time(NULL));
    srand(clock());
    genWall(OSZLOP,SOR,fal1);
    genFloor(OSZLOP,SOR,fal1);
    genL(0,0,OSZLOP,SOR,fal1);
    if(fal2!=0)
        if (isNegyzet) genDiffNegyzet(fal2);
        else if(fal3!=0)genDiffKor2(fal2,fal3);
        else genDiffKor(fal2);
}

void Labyrinth::Debug() {
    int i,j;
    for (i=0;i<SOR+1;i++){
        for (j=0;j<OSZLOP+1;j++){
            //if (i<=SOR && LV[i][j]==1) cout << "_"; else cout << " ";
            //if (j<=OSZLOP && LF[i][j]==1) cout << "|"; else cout << " ";
            cout << (LV[i][j]>0?"_":" ");
            cout << (LF[i][j]>0?"|":" ");
        }
        cout << endl;
    }
}

void Labyrinth::genL(int vKezd,int fKezd,int vBef,int fBef,int wallNum){

    if (vBef-vKezd<2 || fBef-fKezd<2) return;
    int i,atl;
    int temp=rand()%(fBef-fKezd-1)+fKezd+1;
    int temp2=rand()%(vBef-vKezd-1)+vKezd+1;
    int t1,t2,t3,t4;
    atl=(int)((fBef+fKezd)/2);
    temp=(int)((temp+atl)/2);                   //(temp-atl)/2+atl=(temp+atl)/2  az intervallum
    atl=(int)((vBef+vKezd)/2);                         //meretenek felere csokkentese atlag vsltozasa nelkul
    temp2=(int)((temp2+atl)/2);

    for (i=vKezd+1;i<=vBef;i++) LV[temp][i]=wallNum;  //vizszintes felosztas
    for (i=fKezd+1;i<=fBef;i++) LF[i][temp2]=wallNum; //fuggoleges felosztas

    t1=rand()%(temp2-vKezd)+vKezd+1;
    t2=rand()%(vBef-temp2)+temp2+1;
    t3=rand()%(temp-fKezd)+fKezd+1;
    t4=rand()%(fBef-temp)+temp+1;

    i=rand()%4;
    if (i==0); else LV[temp][t1]=0;
    if (i==1); else LV[temp][t2]=0;       //   ajtok szama egy felosztasnal 3
    if (i==2); else LF[t3][temp2]=0;
    if (i==3); else LF[t4][temp2]=0;


    genL(vKezd,fKezd,temp2,temp,wallNum);
    genL(temp2,fKezd,vBef,temp,wallNum);
    genL(vKezd,temp,temp2,fBef,wallNum);
    genL(temp2,temp,vBef,fBef,wallNum);

}

void Labyrinth::genFloor(int oszlop,int sor, int floor){
    int i,j;
    for (i=0;i<sor;i++) {
        for (j=0;j<oszlop;j++) {
            LP[i][j] = floor;
        }
    }
}

void Labyrinth::genWall(int oszlop,int sor,int wall){
    int i;
    for (i=1;i<=oszlop;i++) {
        LV[0][i]=wall;LV[sor][i]=wall;
        }
    for (i=1;i<=sor;i++) {
        LF[i][0]=wall;LF[i][oszlop]=wall;
        }
    LF[(int)(SOR/2+1)][0]=0;
    LF[(int)(SOR/2+1)][OSZLOP]=0;
    LV[0][(int)(OSZLOP/2+1)]=0;
    LV[SOR][(int)(OSZLOP/2+1)]=0;

}

void Labyrinth::genDiffNegyzet(int diff) {//négyzet
    int begin_s=(int)(SOR/4+1);
    int begin_o=(int)(OSZLOP/4+1);
    int end_s=(int)(SOR/4*3+1);
    int end_o=(int)(OSZLOP/4*3+1);

    for (;begin_s<end_s;begin_s++)
        for (begin_o=(int)(OSZLOP/4+1);begin_o<end_o;begin_o++){
            if (LF[begin_s][begin_o]>0) LF[begin_s][begin_o]=diff;
            if (LV[begin_s][begin_o]>0) LV[begin_s][begin_o]=diff;
            LP[begin_s-1][begin_o-1] = diff;
        }

}
void Labyrinth::genDiffKor(int diff) {//kör
        int i,j;
        int center[1][2];
        int r=(int)(min(SOR,OSZLOP)/4);
        int r2=(int)(min(SOR,OSZLOP)/16*7);

        center[0][0]=(int)(SOR/2);
        center[0][1]=(int)(OSZLOP/2);

        for (i=1;i<SOR;i++)
            for (j=1;j<OSZLOP;j++){
                if((i-center[0][0])*(i-center[0][0])+
                   (j-center[0][1])*(j-center[0][1])<r*r){
                    if (LF[i][j]>0)LF[i][j]=diff;
                    if (LV[i][j]>0)LV[i][j]=diff;
                    LP[i-1][j-1]=diff;
                }
        }
}

void Labyrinth::genDiffKor2(int diff, int diff2) {//kör
        int i,j;
        int center[1][2];
        int r=(int)(min(SOR,OSZLOP)/4);
        int r2=(int)(min(SOR,OSZLOP)/16*7);

        center[0][0]=(int)(SOR/2);
        center[0][1]=(int)(OSZLOP/2);

        for (i=1;i<SOR;i++)
            for (j=1;j<OSZLOP;j++){
                if((i-center[0][0])*(i-center[0][0])+
                   (j-center[0][1])*(j-center[0][1])<r*r){
                    if (LF[i][j]>0)LF[i][j]=diff;
                    if (LV[i][j]>0)LV[i][j]=diff;
                    LP[i-1][j-1]=diff;
                }
                else
                if((i-center[0][0])*(i-center[0][0])+
                   (j-center[0][1])*(j-center[0][1])<r2*r2){
                    if (LF[i][j]>0)LF[i][j]=diff2;
                    if (LV[i][j]>0)LV[i][j]=diff2;
                    LP[i-1][j-1]=diff2;
                   }
        }
}

/*bool Labyrinth::Save(ofstream *file) {


    return true; // sikeres mentésnél
}*/

void Labyrinth::makeHoles() {
    LP[rand()%SOR][rand()%OSZLOP] += 5;
}

LStruct Labyrinth::get() {
    LStruct l = { SOR, OSZLOP, LV, LF, LP };
    return l;
}

Labyrinth::~Labyrinth() {
    for (int i=0;i<SOR+1;i++) {
        free(LV[i]);
        free(LF[i]);
        if (i<SOR) free(LP[i]);
    }
    free(LV);
    free(LF);
    free(LP);
}
