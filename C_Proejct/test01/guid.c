#include "game.h"

void guid() {
	int key = 0;
	while (1) {

		gotoxy(30, 6);
		printf("���� ��Ģ\n");
		gotoxy(5, 8);
		printf(" �� ���� ���� ���ʿ��� ������ �� �ִ°��� ���ƾ� �Ѵ�.");
		gotoxy(1, 10);
		printf(" �� ������ ���� ���� ��쿡�� ���ʰ� �ڵ������� ���濡�� �Ѿ�� �ȴ�.");
		gotoxy(8, 12);
		printf(" �� ������ ������ �� ���� ���� �ִ� �÷��̾ ���ڰ� �ȴ�. ");
		gotoxy(8, 14);
		printf(" �� ���� ���� ������ ���� ���� ���ºΰ� �ȴ�.");

		if (kbhit()) { //Ű�Է¹��� 
			key = getch();
			if (key == ESC)
				exit(); // ESCŰ�� ���� 
			else
				break; //�ƴϸ� �׳� ��� ���� 
		}
		gotoxy(22,20);
		printf(" < PRESS ANY KEY TO START > ");
		Sleep(50);
		gotoxy(25,22);
		printf("                            ");
	}
	system("cls");
}