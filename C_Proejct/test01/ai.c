#include "ai.h"

// a , b 둘 중 큰 값을 반환
int max(int a, int b) {
	return a > b ? a : b;  // 참이면 a, 거짓이면 b를 반환
}

// a,b 중 작은 값을 반환
int min(int a, int b) {
	return a < b ? a : b; // 참이면 b, 거짓이면 a를 반환
}

// AI 상황판단처리 알고리즘
// AI를 동적으로 판단하게 함.
// AI가 둔 바둑돌을 기준으로 이득과 손해를 계산.
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

// minimax 알고리즘과 알파 베타 가지치기를 이용한 AI의 인공지능 시스템 알고리즘
// minimax 알고리즘 : AI가 둘 수 있는 모든 경우의 수를 확인
// Alpha beta pruning : 불필요한 연산을 버리는 알파 베타 가지치기 알고리즘
int minimax(char board[][MAX_BOARD_LEN], int n, char aiPiece, char turn, int depth, int alpha, int beta) {
	// 모든 알고리즘의 검사가 끝나면 그 값을 게임 스테이지에 반환
	if (depth == 0)
		return heuristic(board, n, aiPiece);

	// Checks for base cases. If game is over, the following status values are returned:
	// 게임이 끝나면 다음과 같은 값이 반환됨.
	char gameStatus = checkGameStatus(board, n);
	if (gameStatus == aiPiece)			// AI-WIN
		return INT_MAX; 
	else if (gameStatus == oppositePiece(aiPiece))			// AI-LOOSE
		return INT_MIN;
	else if (gameStatus == TIE)			// 무승부
		return 0;

	// 게임이 진행중인경우의 알고리즘
	int bestScore = (aiPiece == turn) ? INT_MIN : INT_MAX;
	// AI 턴이면 min 값을 저장 , 아니면 max값 저장

	bool hasValidMove = false;
	// 그 수가 유효한 수인지 확인하는 함수

	bool continueSearch = true;
	// 값을 계속 찾아 검색하는 함수
	int i = 0, j = 0;

	for (i = 0; i < n && continueSearch; i++) {
		for (j = 0; j < n && continueSearch; j++) {
			if (validMove(board, n, i, j, turn)) {
				hasValidMove = true;

				// 새로운 보드를 만들어 적의 예상 경로 를 탐색.
				char tempBoard[MAX_BOARD_LEN][MAX_BOARD_LEN];
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
						tempBoard[k][l] = board[k][l];
					}
				}
				tempBoard[i][j] = turn;
				flipEnemyPieces(tempBoard, n, i, j, turn);

				// 새 보드에서 예상 점수를 계산함.
				int tempScore = minimax(tempBoard, n, aiPiece, oppositePiece(turn), depth - 1, alpha, beta);

				// AI턴 점수 최대화
				if (turn == aiPiece) {
					bestScore = max(bestScore, tempScore); //int_min 값이 들어감.
					alpha = max(alpha, tempScore);
				}
				// 플레이어 턴에 대한 점수를 최소화함.
				else {
					bestScore = min(bestScore, tempScore);  //int _max 값이 들어감.
					beta = min(beta, tempScore);
				}

				// 필요없는 수
				if (alpha >= beta)
					continueSearch = false;
			}
		}
	}
	// 유효한 이동이 없으면 상대 플레이어에게 전환
	if (!hasValidMove) {
		return minimax(board, n, aiPiece, oppositePiece(turn), depth, alpha, beta);
	}

	return bestScore;
}

// 최소의 알고리즘의 검색으로 최적의 이동 위치를 파악하는 알고리즘 
bool aiFindMove(char board[][MAX_BOARD_LEN], int n, int *row, int *col, char aiPiece) {
	bool foundMove = false;
	int bestScore = INT_MIN;
	int i = 0, j = 0, k = 0, l = 0;

	for (i = 0; i < n; i++) {
		for ( j = 0; j < n; j++) {

			if (validMove(board, n, i, j, aiPiece)) {
				// 새 게임 보드를 만들어 움직임들을 계산
				char tempBoard[MAX_BOARD_LEN][MAX_BOARD_LEN];
				for (k = 0; k < n; k++) {
					for (l = 0; l < n; l++) {
						tempBoard[k][l] = board[k][l];
					}
				}
				tempBoard[i][j] = aiPiece;
				flipEnemyPieces(tempBoard, n, i, j, aiPiece);

				// 그 위치가 유효한 위치인지 계산.
				int tempScore = minimax(tempBoard, n, aiPiece, oppositePiece(aiPiece), SEARCH_DEPTH - 1, INT_MIN, INT_MAX);

				// 가장 높은 점수가 최적의 위치임을 계산.
				if (tempScore >= bestScore) {
					*row = i;
					*col = j;
					bestScore = tempScore;
					foundMove = true;
				}
			}
		}
	}
	// 유효한 위치를 찾으면 그 값을 착수.
	return foundMove;
}