#include "game.h"

void guid() {
	int key = 0;
	while (1) {

		gotoxy(30, 6);
		printf("게임 규칙\n");
		gotoxy(5, 8);
		printf(" ● 상대방 돌을 양쪽에서 포위할 수 있는곳에 놓아야 한다.");
		gotoxy(1, 10);
		printf(" ● 착수할 곳이 없는 경우에는 차례가 자동적으로 상대방에게 넘어가게 된다.");
		gotoxy(8, 12);
		printf(" ● 게임이 끝났을 때 돌이 많이 있는 플레이어가 승자가 된다. ");
		gotoxy(8, 14);
		printf(" ● 만일 돌의 개수가 같을 경우는 무승부가 된다.");

		if (kbhit()) { //키입력받음 
			key = getch();
			if (key == ESC)
				exit(); // ESC키면 종료 
			else
				break; //아니면 그냥 계속 진행 
		}
		gotoxy(22,20);
		printf(" < PRESS ANY KEY TO START > ");
		Sleep(50);
		gotoxy(25,22);
		printf("                            ");
	}
	system("cls");
}