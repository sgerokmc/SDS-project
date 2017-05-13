// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

#include <iostream>
#include <limits>
#include <queue>
// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:이겨보시조,Department:성균관대학교" };

const double dweight[4] = { 1.0, 1.00000181862, 1.00000363725, 1.00000726562 };
const long long weight[5] = { 4096, 2048, 1024, 512, 256 };

const int dx[4][2][6] = {
	{ { -1, -2, -3, -4, -5, -6 }, { 1, 2, 3, 4, 5, 6} }, // Horizontal
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } }, // Vertical
	{ { -1, -2, -3, -4, -5, -6 }, { 1, 2, 3, 4, 5, 6 } }, // Diagonal topleft-bottomright
	{ { -1, -2, -3, -4, -5, -6 }, { 1, 2, 3, 4, 5, 6 } } //  Diagonal bottomleft-topright
};
int a;
const int dy[4][2][6] = {
	{ { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 } },// H
	{ { -1, -2, -3, -4, -5, -6 }, { 1, 2, 3, 4, 5, 6 } }, // V
	{ { -1, -2, -3, -4, -5, -6 }, { 1, 2, 3, 4, 5, 6 } }, // D t-b
	{ { 1, 2, 3, 4, 5, 6 }, { -1, -2, -3, -4, -5, -6 } } // D b-t
};

struct candidates {
	double E;
	int x, y;
};

bool operator<(candidates t, candidates u) {
	return t.E < u.E;
}

void myturn(int cnt) {
	int x[2], y[2];

	if (!Attack(x, y, cnt)) {
		if (cnt == 1) {
			if (isFree(9, 9)) {
				x[0] = 9;
				y[0] = 9;
			}
			else {
				std::priority_queue<candidates> candidates_queue;
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						if (isFree(j, i)) {
							double E = evaluation(j, i);
							int OpStones = howManyOpStonesAround(j, i);
							E = E * dweight[OpStones];
							candidates c;
							c.E = E;
							c.x = j;
							c.y = i;
							candidates_queue.push(c);
						}
					}
				}

				x[0] = candidates_queue.top().x;
				y[0] = candidates_queue.top().y;
			}
			domymove(x, y, cnt);
		}
		else {
			for (int t = 0; t < 2; t++) {
				std::priority_queue<candidates> candidates_queue;
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						if (isFree(j, i)) {
							double E = evaluation(j, i);
							int OpStones = howManyOpStonesAround(j, i);
							E = E * dweight[OpStones];
							candidates c;
							c.E = E;
							c.x = j;
							c.y = i;
							candidates_queue.push(c);
						}
					}
				}

				x[t] = candidates_queue.top().x;
				y[t] = candidates_queue.top().y;
				board[x[t]][y[t]] = 1;
			}
			board[x[0]][y[0]] = 0;
			board[x[1]][y[1]] = 0;
			domymove(x, y, cnt);
		}
	}
	else {
		domymove(x, y, cnt);
	}
}

bool isInBoard(int x, int y) {
	return (x >= 0 && x < 19 && y >= 0 && y < 19);
}

double evaluation(int x, int y) {
	if (!isFree(x, y)) return -1;
	double E = 0, Ed = 1;
	int boardState;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 5; k++) {
				int nx = x + dx[i][j][k];
				int ny = y + dy[i][j][k];
				boardState = showBoard(nx, ny);
				if (boardState == 1 || boardState == 3) {
					break;
				} else if (isFree(nx, ny)) {
					Ed = Ed * 2;
				}
				else if (boardState == 2) {
					Ed = Ed * weight[k];
				}

			}
		}
		E = E + Ed;
		Ed = 1;
	}
	return E;
}

int howManyOpStonesAround(int x, int y) {
	int result = 3;
	int boardState;
	bool finished = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 5; k++) {
				boardState = showBoard(x + dx[i][j][k], y + dy[i][j][k]);
				if (boardState == 1 || boardState == 3) {
					finished = true;
					result--;
					break;
				}
			}
			if (finished) break;
		}
		finished = false;
	}
	
	return (result < 0? 0 : result);
}


bool Attack(int x[2], int y[2], int cnt) {
	int numOfStones;
	int boardState;
	int nx, ny;
	bool finished = false;
	for (int yy = 0; yy < 19; yy++) {
		for (int xx = 0; xx < 19; xx++) {
			if (showBoard(xx,yy) == 1) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 2; j++) {
						std::vector<candidates> v;
						finished = false;
						numOfStones = 1;
						for (int k = 0; k < 5; k++) {
							nx = xx + dx[i][j][k];
							ny = yy + dy[i][j][k];
							boardState = showBoard(nx, ny);

							if (boardState == 0 && isFree(nx, ny)) {
								candidates c;
								c.x = nx;
								c.y = ny;
								v.push_back(c);
							}
							else if (boardState == 2 || boardState == 3) {
								finished = true;
								break;
							}
							else if (boardState == 1) {
								numOfStones++;
							}
						}
						nx = xx + dx[i][j][5];
						ny = yy + dy[i][j][5];
						if (showBoard(nx, ny) == 1) continue;
						nx = xx + dx[i][(j == 0 ? 1 : 0)][0];
						ny = yy + dy[i][(j == 0 ? 1 : 0)][0];
						if (showBoard(nx, ny) == 1) continue;
						if (finished) continue;
						else if (numOfStones == 5) {
							x[0] = v.back().x;
							y[0] = v.back().y;
							for (int p = 0; p < 19; p++) {
								for (int q = 0; q < 19; q++) {
									if (isFree(p, q)) {
										x[1] = p;
										y[1] = q;
										return true;
									}
								}
							}
						}
						else if (numOfStones == 4 && cnt == 2) {
							x[0] = v[0].x; y[0] = v[0].y;
							x[1] = v[1].x; y[1] = v[1].y;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}