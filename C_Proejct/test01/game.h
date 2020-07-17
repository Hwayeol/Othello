#ifndef GAME_H
#define GAME_H
#define  _CRT_SECURE_NO_WARNINGS

#define B 'B'			// 검은돌
#define W 'W'			// 흰색돌
#define U ' '			// 판때기
#define TIE 'T'			// 무승부
#define ONGOING 'O'		// ongoing
#define ESC 27

#define MAX_BOARD_LEN 10   //게임 판 최대 길이
#define Music "TheFatRat - Unity .wav"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void gotoxy(int x, int y);
void menu();
void sql();
void guid();
void rank();
void printBoard(char board[][MAX_BOARD_LEN], int n);
//보드 출력
bool positionInBounds(int n, int row, int col);
//위치 확인
int possibleFlipsInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol);
// 뒤집을 수 있는 돌의 갯수 확인
int possibleFlips(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
// 상대방의 돌을 몇 개 뒤집을 수 있는지 확인
bool validMove(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
// 입력한 곳에 착수를 할 수 있는지 없는지 확인
void flipInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol);
// 착수한 곳의 반대방향에 돌이 있는지 없는지를 확인
bool flipEnemyPieces(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
// 돌 뒤집기
char oppositePiece(char turn);
// 뒤집은 돌을 화면에 출력
char checkGameStatus(char board[][MAX_BOARD_LEN], int n);
// 게임 종료 조건확인 및 종료
#endif