// Samsung Go Tournament Form C Connect6Algo Header (g++-4.8.3)

// <--------------- 이 Code를 수정하면  작동하지 않을 수 있습니다 ------------------>

#pragma once
extern int width, height;
extern int terminateAI;
extern int board[20][20];
extern int lastMove[2][2];

extern unsigned s_time;

extern int isFree(int x, int y);
extern int showBoard(int x, int y);
extern void init();
extern void restart();
extern void mymove(int x[], int y[], int cnt = 2);
extern void opmove(int x[], int y[], int cnt = 2);
extern void block(int, int);
extern int setLine(char *fmt, ...);
extern void domymove(int x[], int y[], int cnt = 2);
extern double evaluation(int x, int y);
extern int howManyOpStonesAround(int x, int y);
extern bool Attack(int x[2], int y[2], int cnt);
void myturn(int cnt = 2);
char info[];