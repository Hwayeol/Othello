#include "ai.h"

// a , b �� �� ū ���� ��ȯ
int max(int a, int b) {
	return a > b ? a : b;  // ���̸� a, �����̸� b�� ��ȯ
}

// a,b �� ���� ���� ��ȯ
int min(int a, int b) {
	return a < b ? a : b; // ���̸� b, �����̸� a�� ��ȯ
}

// AI ��Ȳ�Ǵ�ó�� �˰���
// AI�� �������� �Ǵ��ϰ� ��.
// AI�� �� �ٵϵ��� �������� �̵�� ���ظ� ���.
int heuristic(char board[][MAX_BOARD_LEN], int n, char aiPiece) {
	int count = 0;
	int i = 0, j = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (board[i][j] == aiPiece)
				count++;
			else if (board[i][j] == oppositePiece(aiPiece))
				count--;
		}
	}
	return count;
}

// minimax �˰���� ���� ��Ÿ ����ġ�⸦ �̿��� AI�� �ΰ����� �ý��� �˰���
// minimax �˰��� : AI�� �� �� �ִ� ��� ����� ���� Ȯ��
// Alpha beta pruning : ���ʿ��� ������ ������ ���� ��Ÿ ����ġ�� �˰���
int minimax(char board[][MAX_BOARD_LEN], int n, char aiPiece, char turn, int depth, int alpha, int beta) {
	// ��� �˰����� �˻簡 ������ �� ���� ���� ���������� ��ȯ
	if (depth == 0)
		return heuristic(board, n, aiPiece);

	// Checks for base cases. If game is over, the following status values are returned:
	// ������ ������ ������ ���� ���� ��ȯ��.
	char gameStatus = checkGameStatus(board, n);
	if (gameStatus == aiPiece)			// AI-WIN
		return INT_MAX; 
	else if (gameStatus == oppositePiece(aiPiece))			// AI-LOOSE
		return INT_MIN;
	else if (gameStatus == TIE)			// ���º�
		return 0;

	// ������ �������ΰ���� �˰���
	int bestScore = (aiPiece == turn) ? INT_MIN : INT_MAX;
	// AI ���̸� min ���� ���� , �ƴϸ� max�� ����

	bool hasValidMove = false;
	// �� ���� ��ȿ�� ������ Ȯ���ϴ� �Լ�

	bool continueSearch = true;
	// ���� ��� ã�� �˻��ϴ� �Լ�
	int i = 0, j = 0;

	for (i = 0; i < n && continueSearch; i++) {
		for (j = 0; j < n && continueSearch; j++) {
			if (validMove(board, n, i, j, turn)) {
				hasValidMove = true;

				// ���ο� ���带 ����� ���� ���� ��� �� Ž��.
				char tempBoard[MAX_BOARD_LEN][MAX_BOARD_LEN];
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
						tempBoard[k][l] = board[k][l];
					}
				}
				tempBoard[i][j] = turn;
				flipEnemyPieces(tempBoard, n, i, j, turn);

				// �� ���忡�� ���� ������ �����.
				int tempScore = minimax(tempBoard, n, aiPiece, oppositePiece(turn), depth - 1, alpha, beta);

				// AI�� ���� �ִ�ȭ
				if (turn == aiPiece) {
					bestScore = max(bestScore, tempScore); //int_min ���� ��.
					alpha = max(alpha, tempScore);
				}
				// �÷��̾� �Ͽ� ���� ������ �ּ�ȭ��.
				else {
					bestScore = min(bestScore, tempScore);  //int _max ���� ��.
					beta = min(beta, tempScore);
				}

				// �ʿ���� ��
				if (alpha >= beta)
					continueSearch = false;
			}
		}
	}
	// ��ȿ�� �̵��� ������ ��� �÷��̾�� ��ȯ
	if (!hasValidMove) {
		return minimax(board, n, aiPiece, oppositePiece(turn), depth, alpha, beta);
	}

	return bestScore;
}

// �ּ��� �˰����� �˻����� ������ �̵� ��ġ�� �ľ��ϴ� �˰��� 
bool aiFindMove(char board[][MAX_BOARD_LEN], int n, int *row, int *col, char aiPiece) {
	bool foundMove = false;
	int bestScore = INT_MIN;
	int i = 0, j = 0, k = 0, l = 0;

	for (i = 0; i < n; i++) {
		for ( j = 0; j < n; j++) {

			if (validMove(board, n, i, j, aiPiece)) {
				// �� ���� ���带 ����� �����ӵ��� ���
				char tempBoard[MAX_BOARD_LEN][MAX_BOARD_LEN];
				for (k = 0; k < n; k++) {
					for (l = 0; l < n; l++) {
						tempBoard[k][l] = board[k][l];
					}
				}
				tempBoard[i][j] = aiPiece;
				flipEnemyPieces(tempBoard, n, i, j, aiPiece);

				// �� ��ġ�� ��ȿ�� ��ġ���� ���.
				int tempScore = minimax(tempBoard, n, aiPiece, oppositePiece(aiPiece), SEARCH_DEPTH - 1, INT_MIN, INT_MAX);

				// ���� ���� ������ ������ ��ġ���� ���.
				if (tempScore >= bestScore) {
					*row = i;
					*col = j;
					bestScore = tempScore;
					foundMove = true;
				}
			}
		}
	}
	// ��ȿ�� ��ġ�� ã���� �� ���� ����.
	return foundMove;
}