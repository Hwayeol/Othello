#include "game.h"

void menu() {
	int key = 0;

	while (kbhit())
		getch();  //버퍼에 있는 키값을 버림

	gotoxy(20,7);
	printf("+--------------------------+");
	gotoxy(20,8); 
	printf("     |  O T H E L L O  |          ");
	gotoxy(20,9); 
	printf("+--------------------------+");

	gotoxy(20,10);
	printf("   ESC 를 누르면 게임 종료 ");

	while (1) {
		if (kbhit()) { //키입력받음 
			key = getch();
			if (key == ESC)
				exit(); // ESC키면 종료 
			else 
				break; //아니면 그냥 계속 진행 
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