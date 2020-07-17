#ifndef GAME_H
#define GAME_H
#define  _CRT_SECURE_NO_WARNINGS

#define B 'B'			// ������
#define W 'W'			// �����
#define U ' '			// �Ƕ���
#define TIE 'T'			// ���º�
#define ONGOING 'O'		// ongoing
#define ESC 27

#define MAX_BOARD_LEN 10   //���� �� �ִ� ����
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
//���� ���
bool positionInBounds(int n, int row, int col);
//��ġ Ȯ��
int possibleFlipsInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol);
// ������ �� �ִ� ���� ���� Ȯ��
int possibleFlips(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
// ������ ���� �� �� ������ �� �ִ��� Ȯ��
bool validMove(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
// �Է��� ���� ������ �� �� �ִ��� ������ Ȯ��
void flipInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol);
// ������ ���� �ݴ���⿡ ���� �ִ��� �������� Ȯ��
bool flipEnemyPieces(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
// �� ������
char oppositePiece(char turn);
// ������ ���� ȭ�鿡 ���
char checkGameStatus(char board[][MAX_BOARD_LEN], int n);
// ���� ���� ����Ȯ�� �� ����
#endif