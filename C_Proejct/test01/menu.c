#include "game.h"

void menu() {
	int key = 0;

	while (kbhit())
		getch();  //���ۿ� �ִ� Ű���� ����

	gotoxy(20,7);
	printf("+--------------------------+");
	gotoxy(20,8); 
	printf("     |  O T H E L L O  |          ");
	gotoxy(20,9); 
	printf("+--------------------------+");

	gotoxy(20,10);
	printf("   ESC �� ������ ���� ���� ");

	while (1) {
		if (kbhit()) { //Ű�Է¹��� 
			key = getch();
			if (key == ESC)
				exit(); // ESCŰ�� ���� 
			else 
				break; //�ƴϸ� �׳� ��� ���� 
		}
		gotoxy(20,12);
		printf(" < PRESS ANY KEY TO START > ");
		Sleep(50);
		gotoxy(20,13);
		printf("                            ");
	}
	system("cls");

	guid();
}