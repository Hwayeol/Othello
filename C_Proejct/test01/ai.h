#ifndef AI_H
#define AI_H

#define SEARCH_DEPTH 8

#include "game.h"
#include <stdbool.h>
#include <limits.h>  // 자료형의 최대값과 최소값이 정의된 헤더 파일

int heuristic(char board[][MAX_BOARD_LEN], int n, char aiPiece);
// AI 상황판단처리 알고리즘

int minimax(char board[][MAX_BOARD_LEN], int n, char aiPiece, char turn, int depth, int alpha, int beta);
// minimax 알고리즘과 알파 베타 가지치기를 이용한 AI의 인공지능 시스템 알고리즘

bool aiFindMove(char board[][MAX_BOARD_LEN], int n, int *row, int *col, char aiPiece);
// 최소의 알고리즘의 검색으로 최적의 이동 위치를 파악하는 알고리즘 

#endif