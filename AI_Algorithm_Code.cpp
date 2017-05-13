// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI �ڵ� �ۼ� ���]

1. char info[]�� �迭 �ȿ�					"TeamName:�ڽ��� ����,Department:�ڽ��� �Ҽ�"					������ �ۼ��մϴ�.
( ���� ) Teamname:�� Department:�� �� ���� �մϴ�.
"�ڽ��� ����", "�ڽ��� �Ҽ�"�� �����ؾ� �մϴ�.

2. �Ʒ��� myturn() �Լ� �ȿ� �ڽŸ��� AI �ڵ带 �ۼ��մϴ�.

3. AI ������ �׽�Ʈ �Ͻ� ���� "���� �˰����ȸ ��"�� ����մϴ�.

4. ���� �˰��� ��ȸ ���� �����ϱ⿡�� �ٵϵ��� ���� ��, �ڽ��� "����" �� �� �˰����� �߰��Ͽ� �׽�Ʈ �մϴ�.



[���� �� �Լ�]
myturn(int cnt) : �ڽ��� AI �ڵ带 �ۼ��ϴ� ���� �Լ� �Դϴ�.
int cnt (myturn()�Լ��� �Ķ����) : ���� �� �� �־��ϴ��� ���ϴ� ����, cnt�� 1�̸� ���� ���� ��  �� ����  �δ� ��Ȳ(�� ��), cnt�� 2�̸� �� ���� ���� �δ� ��Ȳ(�� ��)
int  x[0], y[0] : �ڽ��� �� ù �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
int  x[1], y[1] : �ڽ��� �� �� �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
void domymove(int x[], int y[], cnt) : �� ������ ��ǥ�� �����ؼ� ���


//int board[BOARD_SIZE][BOARD_SIZE]; �ٵ��� �����Ȳ ��� �־� �ٷλ�� ������. ��, ���������ͷ� ���� �������
// ������ ���� ��ġ�� �ٵϵ��� ������ �ǰ��� ó��.

boolean ifFree(int x, int y) : ���� [x,y]��ǥ�� �ٵϵ��� �ִ��� Ȯ���ϴ� �Լ� (������ true, ������ false)
int showBoard(int x, int y) : [x, y] ��ǥ�� ���� ���� �����ϴ��� �����ִ� �Լ� (1 = �ڽ��� ��, 2 = ����� ��, 3 = ��ŷ)


<-------AI�� �ۼ��Ͻ� ��, ���� �̸��� �Լ� �� ���� ����� �������� �ʽ��ϴ�----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

#include <iostream>
#include <limits>
#include <queue>
// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:�̰ܺ�����,Department:���հ����б�" };

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