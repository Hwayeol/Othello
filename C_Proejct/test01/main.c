#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> //bool, true, false 가 들어있는  헤더 파일
#include <ctype.h>    //문자 조건에 맞는지 검사하고 변환하는 헤더파일 (삼항연산자)
#include <windows.h>
#include <mmsystem.h>  // 미디어 제어를 위한 헤더파일
#pragma comment(lib, "winmm.lib") // laysound 함수 호출을 위한 라이브러리 파일 로드
#include "ai.h"
#include "game.h"

//전역변수 선언 : sql.c 에서 선언한 변수들을 main.c.에서 활용하기 위해 선언.
extern char id[50];
extern char buffer[100];
extern int choose;
extern conn;  //sql 선언
extern *connection;       // sql로 부터 연결 설정을 하는 변수
extern *sql_result; //sql 결과 받는 구조체 선언
extern sql_row;   //행단위를 저장하는 구조체

int main(void) {
	int n = 0, i = 0, j = 0;  //보드 판의 크기를 저장하는 변수
	char board[MAX_BOARD_LEN][MAX_BOARD_LEN];  //보드의 크기 선언 
	char playerPiece, aiPiece;  //플레이어와 AI의 돌
	char turn;   //턴의 확인을 하는 변수
	int tie = 0, win = 0, lose = 0;
	char choice = 0;
	int a = 0;

	system("mode con cols=75 lines=30");
	menu();

	//sql();   //sql문 실행

	//재시작 분기점
	back:

	// 보드 디자인
	while (1) {
		printf("(8x8 보드판 : 1,  사용자 지정 : 2  ) : ");
		scanf("%d", &a);
		if (a == 1) {
			n = 8;
			break;
		}
		else if (a == 2) {
			printf("\n보드판의 길이를 지정해 주세요. : \n");
			printf("  (최소 4, 최대 : 10 짝수 갯수의 보드판 ) ");
			while (!scanf("%d", &n) || n % 2 != 0 || n < 4 || n > MAX_BOARD_LEN)  //입력이 제대로 들어왔는지 안들어왔는지 확인
				printf("수가 올바르지 않습니다. 다시 입력하세요. : ");
			break;
		}
		else {
			printf("잘못 입력하셨습니다.\n");
		}

	}

	// 보드 초기화
	// 입력받은 n값 까지 반복돌면서 보드 생성
	for (i = 0; i < n; i++)			// 돌이 없는 판은 모두 공백으로 표시
		for (j = 0; j < n; j++)
			board[i][j] = U;

	board[n / 2 - 1][n / 2 - 1] = W;		// 초기에 시작되는 돌 위치  선점.
	board[n / 2 - 1][n / 2] = B;
	board[n / 2][n / 2 - 1] = B;
	board[n / 2][n / 2] = W;
	turn = B;						// 시작 순서 : 검은돌이 먼저

	// 플레이어 돌 선택
	while (1) {
		printf("무슨 색으로 시작하시겠습니까? : (B/W): ");
		scanf(" %c", &playerPiece);
		
		if (playerPiece == 'b' || playerPiece == 'w' || playerPiece == B || playerPiece == W) {
			break;
		}
		else {
			printf("잘못 입력하셨습니다.");
		}
	}

	system("cls");

	PlaySound(TEXT(Music), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);  //노래재생

	playerPiece = toupper(playerPiece);  //입력값이 소문자면 대문자로 변환하는 함수

	aiPiece = oppositePiece(playerPiece);  //플레이어 돌의 반대 색갈을 AI돌로 지정
	printf("\n시작 보드\n");
	printBoard(board, n);  //보드판 출력

	// 게임 루프
	char result;   //결과를 저장
	int aiMoves = 0;  //AI의 움직임
	double intervalSum = 0;  //AI의 계산 소요시간
	int blackPieces = 0;
	int whitePieces = 0;

	while ((result = checkGameStatus(board, n)) == ONGOING) {  
		// 게임이 진행중일경우 WHILE문으로 무한루프

		//화면에 각각의 돌이 몇개 있는지 확인후 출력
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (board[i][j] == B) {
					blackPieces++;
				}
				else if (board[i][j] == W) {
					whitePieces++;
				}
			}
		}
		printf("검은돌 수 : %d, 흰 돌 수 : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;   //초기화를 하지 않으면 값이 계속 더해짐.

		// 컴퓨터 착수 일 경우
		if (turn == aiPiece) {
			int row, col;

			// 타이머 시작   시간 측정을 위한 알고리즘 
			//AI가 최선의 수를 생각하는 동안의 시간을 계산하는 시간측정 함수.
			LARGE_INTEGER frequency;
			LARGE_INTEGER start;
			LARGE_INTEGER end;
			double interval;

			QueryPerformanceFrequency(&frequency);    //1초당 증가되는 카운트 수 측정
			QueryPerformanceCounter(&start); // 수행 전 카운트 측정

			bool aiCanMove = aiFindMove(board, n, &row, &col, aiPiece);  //AI 의 착수 알고리즘 값에 대한 참 거짓을 측정

			//   시간 측정 알고리즘 끝
			QueryPerformanceCounter(&end); //코드 수행 후 카운트 저장
			interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;  // AI가 수를 알아내는데 걸린 시간

			if (aiCanMove) {
				//화면 지우기
				system("cls");

				printf("컴퓨터가 착수한 자리 %c at %c %c.\n", aiPiece, row + 'a', col + 'a');
				printf("컴퓨터가 계산한 시간 (s): %f\n", interval);
				flipEnemyPieces(board, n, row, col, aiPiece);   //착수한 곳을 기준으로 돌을 뒤집을 수 있는지 확인후 뒤집음.				
				printBoard(board, n);  //돌을 뒤집은 후 게임보드 결과 출력

				intervalSum += interval;  //컴퓨터가 계산한 시간을 저장
				aiMoves++; // AI의 움직임 횟수 기록
			}

			else
				printf("컴퓨터가 (%c) 착수할 곳이 없습니다. 턴을 스킵합니다.\n\n", aiPiece);

			turn = playerPiece;
		}

		// 플레이어 시작
		else if (turn == playerPiece) {
			bool playerCanMove = false;  //플레이어가 착수할 곳이 있는지 없는지 확인

			for (i = 0; i < n && !playerCanMove; i++) {   
				//입력한 보드 수 만큼 반복 하며 board[i][j]에 플레이어가 착수 가능한지 확인
				for (j = 0; j < n && !playerCanMove; j++) {
					if (validMove(board, n, i, j, playerPiece)) 
						//값이 true인 경우 플레이어 착수 구문으로 넘어감.
						playerCanMove = true;   
				}
			}

			// 플레이어 착수
			if (playerCanMove) {  //플레이어가 착수할 곳이 있으면 함수 시작
				char inputRow, inputCol;
				int row, col;
				bool validInput = false;
				
				printf("착수할 곳을 선택하세요. %c (행,열) : ", playerPiece);

				while (!validInput) {   //입력한 값에 착수를 할수 있는지 없는지 확인
					scanf(" %c%c", &inputRow, &inputCol);

					row = tolower(inputRow) - 'a';    //입력이 대문자일때 소문자로 변환함.
					col = tolower(inputCol) - 'a';

					if (positionInBounds(n, row, col) && validMove(board, n, row, col, playerPiece)){
						//입력한 값이 보드 내 수이며, 착수 할 수 있는 곳인지 확인
						validInput = true;   
						//착수할 곳이 있으면 while 구문을 빠져나가 결과창에 나타냄.
					}
					else
						printf("허용되지 않은 움직임 입니다. 다시 선택하세요. (행,열) : ");
				}
				printf("\n");
				flipEnemyPieces(board, n, row, col, playerPiece); 
				//AI의 돌 중 뒤집을 수 있는 돌을 찾아 뒤집음.
				printBoard(board, n);

			}
			else {
				printf("플레이어가 (%c) 더이상 둘 곳이 없습니다. 턴을 스킵합니다.\n\n", playerPiece);
			}

			turn = aiPiece;
		}
	}

	// 결과 판별
	if (result == TIE){
		printf("무승부!\n\n");
		printf("검은돌 수 : %d, 흰 돌 수 : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;
		tie++;     //무승부 값을 1 추가함.
	}
	else if (result == playerPiece){
		printf("플레이어 (%c) 승리!\n\n", result);
		printf("검은돌 수 : %d, 흰 돌 수 : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;
		win++;   //승,패 값을 증가함.
	}
	else if (result == aiPiece){
		printf("컴퓨터 (%c) 승리!\n\n", result);
		printf("검은돌 수 : %d, 흰 돌 수 : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;
		lose++;   //승,패 값을 증가함.
	}

	printf("	컴퓨터의 평균 계산 시간: %f\n\n", intervalSum / aiMoves);
	
	while (1) {
		printf("다시 하시겠습니까 ? : (y or n)");
		scanf(" %c", &choice);
		
		if (choice == 'y' || choice == 'Y') {
			// 다시 하기를 누르면 main함수 중 sql 구문 다음으로 넘어감.
			goto back;
			system("cls");
		}
		else if (choice == 'n' || choice == 'N') {

			printf("\n유저의 경기 승패 집계 : %d승, %d패, %d무\n", win, lose, tie);

			//choose는 sql.c에서 로그인 시 넘어오는 구문.
			//비회원이면 이 구문을 지나지 않는다.
			if (choose == 2) {
				
				//유저의 경기 기록을 SQL로 넘김.
				if (win > 0) {
					sprintf(buffer, "update score set win = win + 1 where id = '%s'",id);
					mysql_query(connection, buffer);  //유저의 기록을 SQL 데이터에 기록함.
				}
				if (lose > 0) {
					sprintf(buffer, "update score set lose = lose + 1 where id = '%s'", id);
					mysql_query(connection, buffer); 
				}
				if (tie > 0) {
					sprintf(buffer, "update score set tie = tie + 1 where id = '%s'", id);
					mysql_query(connection, buffer); 
				}
				printf("\n경기 기록이 저장되었습니다.\n");
			}

			Sleep(2000);
			//랭킹을 보여주는 함수
			system("cls");
			//rank();

			mysql_close(&conn);      //sql 접속 종료

			printf("\n게임 종료.\n\n");
			return 0;
		}
		else {
			printf("잘못 입력하셨습니다.\n");
		}
	}
}
