#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> //bool, true, false �� ����ִ�  ��� ����
#include <ctype.h>    //���� ���ǿ� �´��� �˻��ϰ� ��ȯ�ϴ� ������� (���׿�����)
#include <windows.h>
#include <mmsystem.h>  // �̵�� ��� ���� �������
#pragma comment(lib, "winmm.lib") // laysound �Լ� ȣ���� ���� ���̺귯�� ���� �ε�
#include "ai.h"
#include "game.h"

//�������� ���� : sql.c ���� ������ �������� main.c.���� Ȱ���ϱ� ���� ����.
extern char id[50];
extern char buffer[100];
extern int choose;
extern conn;  //sql ����
extern *connection;       // sql�� ���� ���� ������ �ϴ� ����
extern *sql_result; //sql ��� �޴� ����ü ����
extern sql_row;   //������� �����ϴ� ����ü

int main(void) {
	int n = 0, i = 0, j = 0;  //���� ���� ũ�⸦ �����ϴ� ����
	char board[MAX_BOARD_LEN][MAX_BOARD_LEN];  //������ ũ�� ���� 
	char playerPiece, aiPiece;  //�÷��̾�� AI�� ��
	char turn;   //���� Ȯ���� �ϴ� ����
	int tie = 0, win = 0, lose = 0;
	char choice = 0;
	int a = 0;

	system("mode con cols=75 lines=30");
	menu();

	//sql();   //sql�� ����

	//����� �б���
	back:

	// ���� ������
	while (1) {
		printf("(8x8 ������ : 1,  ����� ���� : 2  ) : ");
		scanf("%d", &a);
		if (a == 1) {
			n = 8;
			break;
		}
		else if (a == 2) {
			printf("\n�������� ���̸� ������ �ּ���. : \n");
			printf("  (�ּ� 4, �ִ� : 10 ¦�� ������ ������ ) ");
			while (!scanf("%d", &n) || n % 2 != 0 || n < 4 || n > MAX_BOARD_LEN)  //�Է��� ����� ���Դ��� �ȵ��Դ��� Ȯ��
				printf("���� �ùٸ��� �ʽ��ϴ�. �ٽ� �Է��ϼ���. : ");
			break;
		}
		else {
			printf("�߸� �Է��ϼ̽��ϴ�.\n");
		}

	}

	// ���� �ʱ�ȭ
	// �Է¹��� n�� ���� �ݺ����鼭 ���� ����
	for (i = 0; i < n; i++)			// ���� ���� ���� ��� �������� ǥ��
		for (j = 0; j < n; j++)
			board[i][j] = U;

	board[n / 2 - 1][n / 2 - 1] = W;		// �ʱ⿡ ���۵Ǵ� �� ��ġ  ����.
	board[n / 2 - 1][n / 2] = B;
	board[n / 2][n / 2 - 1] = B;
	board[n / 2][n / 2] = W;
	turn = B;						// ���� ���� : �������� ����

	// �÷��̾� �� ����
	while (1) {
		printf("���� ������ �����Ͻðڽ��ϱ�? : (B/W): ");
		scanf(" %c", &playerPiece);
		
		if (playerPiece == 'b' || playerPiece == 'w' || playerPiece == B || playerPiece == W) {
			break;
		}
		else {
			printf("�߸� �Է��ϼ̽��ϴ�.");
		}
	}

	system("cls");

	PlaySound(TEXT(Music), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);  //�뷡���

	playerPiece = toupper(playerPiece);  //�Է°��� �ҹ��ڸ� �빮�ڷ� ��ȯ�ϴ� �Լ�

	aiPiece = oppositePiece(playerPiece);  //�÷��̾� ���� �ݴ� ������ AI���� ����
	printf("\n���� ����\n");
	printBoard(board, n);  //������ ���

	// ���� ����
	char result;   //����� ����
	int aiMoves = 0;  //AI�� ������
	double intervalSum = 0;  //AI�� ��� �ҿ�ð�
	int blackPieces = 0;
	int whitePieces = 0;

	while ((result = checkGameStatus(board, n)) == ONGOING) {  
		// ������ �������ϰ�� WHILE������ ���ѷ���

		//ȭ�鿡 ������ ���� � �ִ��� Ȯ���� ���
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
		printf("������ �� : %d, �� �� �� : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;   //�ʱ�ȭ�� ���� ������ ���� ��� ������.

		// ��ǻ�� ���� �� ���
		if (turn == aiPiece) {
			int row, col;

			// Ÿ�̸� ����   �ð� ������ ���� �˰��� 
			//AI�� �ּ��� ���� �����ϴ� ������ �ð��� ����ϴ� �ð����� �Լ�.
			LARGE_INTEGER frequency;
			LARGE_INTEGER start;
			LARGE_INTEGER end;
			double interval;

			QueryPerformanceFrequency(&frequency);    //1�ʴ� �����Ǵ� ī��Ʈ �� ����
			QueryPerformanceCounter(&start); // ���� �� ī��Ʈ ����

			bool aiCanMove = aiFindMove(board, n, &row, &col, aiPiece);  //AI �� ���� �˰��� ���� ���� �� ������ ����

			//   �ð� ���� �˰��� ��
			QueryPerformanceCounter(&end); //�ڵ� ���� �� ī��Ʈ ����
			interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;  // AI�� ���� �˾Ƴ��µ� �ɸ� �ð�

			if (aiCanMove) {
				//ȭ�� �����
				system("cls");

				printf("��ǻ�Ͱ� ������ �ڸ� %c at %c %c.\n", aiPiece, row + 'a', col + 'a');
				printf("��ǻ�Ͱ� ����� �ð� (s): %f\n", interval);
				flipEnemyPieces(board, n, row, col, aiPiece);   //������ ���� �������� ���� ������ �� �ִ��� Ȯ���� ������.				
				printBoard(board, n);  //���� ������ �� ���Ӻ��� ��� ���

				intervalSum += interval;  //��ǻ�Ͱ� ����� �ð��� ����
				aiMoves++; // AI�� ������ Ƚ�� ���
			}

			else
				printf("��ǻ�Ͱ� (%c) ������ ���� �����ϴ�. ���� ��ŵ�մϴ�.\n\n", aiPiece);

			turn = playerPiece;
		}

		// �÷��̾� ����
		else if (turn == playerPiece) {
			bool playerCanMove = false;  //�÷��̾ ������ ���� �ִ��� ������ Ȯ��

			for (i = 0; i < n && !playerCanMove; i++) {   
				//�Է��� ���� �� ��ŭ �ݺ� �ϸ� board[i][j]�� �÷��̾ ���� �������� Ȯ��
				for (j = 0; j < n && !playerCanMove; j++) {
					if (validMove(board, n, i, j, playerPiece)) 
						//���� true�� ��� �÷��̾� ���� �������� �Ѿ.
						playerCanMove = true;   
				}
			}

			// �÷��̾� ����
			if (playerCanMove) {  //�÷��̾ ������ ���� ������ �Լ� ����
				char inputRow, inputCol;
				int row, col;
				bool validInput = false;
				
				printf("������ ���� �����ϼ���. %c (��,��) : ", playerPiece);

				while (!validInput) {   //�Է��� ���� ������ �Ҽ� �ִ��� ������ Ȯ��
					scanf(" %c%c", &inputRow, &inputCol);

					row = tolower(inputRow) - 'a';    //�Է��� �빮���϶� �ҹ��ڷ� ��ȯ��.
					col = tolower(inputCol) - 'a';

					if (positionInBounds(n, row, col) && validMove(board, n, row, col, playerPiece)){
						//�Է��� ���� ���� �� ���̸�, ���� �� �� �ִ� ������ Ȯ��
						validInput = true;   
						//������ ���� ������ while ������ �������� ���â�� ��Ÿ��.
					}
					else
						printf("������ ���� ������ �Դϴ�. �ٽ� �����ϼ���. (��,��) : ");
				}
				printf("\n");
				flipEnemyPieces(board, n, row, col, playerPiece); 
				//AI�� �� �� ������ �� �ִ� ���� ã�� ������.
				printBoard(board, n);

			}
			else {
				printf("�÷��̾ (%c) ���̻� �� ���� �����ϴ�. ���� ��ŵ�մϴ�.\n\n", playerPiece);
			}

			turn = aiPiece;
		}
	}

	// ��� �Ǻ�
	if (result == TIE){
		printf("���º�!\n\n");
		printf("������ �� : %d, �� �� �� : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;
		tie++;     //���º� ���� 1 �߰���.
	}
	else if (result == playerPiece){
		printf("�÷��̾� (%c) �¸�!\n\n", result);
		printf("������ �� : %d, �� �� �� : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;
		win++;   //��,�� ���� ������.
	}
	else if (result == aiPiece){
		printf("��ǻ�� (%c) �¸�!\n\n", result);
		printf("������ �� : %d, �� �� �� : %d \n\n", blackPieces, whitePieces);
		blackPieces = 0, whitePieces = 0;
		lose++;   //��,�� ���� ������.
	}

	printf("	��ǻ���� ��� ��� �ð�: %f\n\n", intervalSum / aiMoves);
	
	while (1) {
		printf("�ٽ� �Ͻðڽ��ϱ� ? : (y or n)");
		scanf(" %c", &choice);
		
		if (choice == 'y' || choice == 'Y') {
			// �ٽ� �ϱ⸦ ������ main�Լ� �� sql ���� �������� �Ѿ.
			goto back;
			system("cls");
		}
		else if (choice == 'n' || choice == 'N') {

			printf("\n������ ��� ���� ���� : %d��, %d��, %d��\n", win, lose, tie);

			//choose�� sql.c���� �α��� �� �Ѿ���� ����.
			//��ȸ���̸� �� ������ ������ �ʴ´�.
			if (choose == 2) {
				
				//������ ��� ����� SQL�� �ѱ�.
				if (win > 0) {
					sprintf(buffer, "update score set win = win + 1 where id = '%s'",id);
					mysql_query(connection, buffer);  //������ ����� SQL �����Ϳ� �����.
				}
				if (lose > 0) {
					sprintf(buffer, "update score set lose = lose + 1 where id = '%s'", id);
					mysql_query(connection, buffer); 
				}
				if (tie > 0) {
					sprintf(buffer, "update score set tie = tie + 1 where id = '%s'", id);
					mysql_query(connection, buffer); 
				}
				printf("\n��� ����� ����Ǿ����ϴ�.\n");
			}

			Sleep(2000);
			//��ŷ�� �����ִ� �Լ�
			system("cls");
			//rank();

			mysql_close(&conn);      //sql ���� ����

			printf("\n���� ����.\n\n");
			return 0;
		}
		else {
			printf("�߸� �Է��ϼ̽��ϴ�.\n");
		}
	}
}
