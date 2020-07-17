#ifndef AI_H
#define AI_H

#define SEARCH_DEPTH 8

#include "game.h"
#include <stdbool.h>
#include <limits.h>  // �ڷ����� �ִ밪�� �ּҰ��� ���ǵ� ��� ����

int heuristic(char board[][MAX_BOARD_LEN], int n, char aiPiece);
// AI ��Ȳ�Ǵ�ó�� �˰���

int minimax(char board[][MAX_BOARD_LEN], int n, char aiPiece, char turn, int depth, int alpha, int beta);
// minimax �˰���� ���� ��Ÿ ����ġ�⸦ �̿��� AI�� �ΰ����� �ý��� �˰���

bool aiFindMove(char board[][MAX_BOARD_LEN], int n, int *row, int *col, char aiPiece);
// �ּ��� �˰����� �˻����� ������ �̵� ��ġ�� �ľ��ϴ� �˰��� 

#endif