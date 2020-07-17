#include "game.h"

// 게임 보드 출력 함수
void printBoard(char board[][MAX_BOARD_LEN], int n) {
	printf("  ");
	for (int i = 0; i < n; i++) {
		printf("  %2c ", 'a' + i);
	}
	printf("\n");
	for (int i = 0; i < n; i++) {
		printf("  %c ", 'a' + i);
		for (int j = 0; j < n; j++) {
			printf(" %c", board[i][j]);
			printf(" l ");
		}
		printf("\n");
		printf("\n");
	}
	printf("\n");
}

// 입력한 값이 보드 내의 범위에 있는지 확인 있으면 true, 없으면 false 값
bool positionInBounds(int n, int row, int col) {
	return (row >= 0 && col >= 0 && row < n && col < n);   
	//행과 열이 0보다 크거나 같고, 보드판의 크기보다 작을때
}

// 주어진 위치가 뒤집을 수 있는 양을 측정
int possibleFlipsInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
	int count = 0;

	row += deltaRow;
	col += deltaCol;

	// 착수한 곳에 돌이 있는지 없는지 확인
	if (!positionInBounds(n, row, col) || board[row][col] == U || board[row][col] == colour)
		return 0;

	// 두번째 위치가 범위 내에 있는지 확인
	do {
		row += deltaRow;
		col += deltaCol;
		count++;

		if (board[row][col] == U)
			return 0;
		if (board[row][col] == colour)
			return count;
	} while (positionInBounds(n, row, col));

	return 0;
}

// 상대방 돌을 몇개 넘길 수 있는지 체크
int possibleFlips(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour) {
	int count = 0;
	int i = 0, j = 0;

	// 상대방 돌이 있는지 없는지를 검토 
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)  		// 없으면 continue
				continue;
			
			//위  뒤집을 수 있는 양을 체크 후 count에 누적
			count += possibleFlipsInDirection(board, n, row, col, colour, i, j);
			
		}
	}
	//누적된 count 값을 리턴
	return count;
}

// 선택한 위치에 착수를 할수 있는지 없는지 확인
bool validMove(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour){

	// 입력한 값에 돌이 없으면 true 값을 리턴
	return board[row][col] == U && possibleFlips(board, n, row, col, colour) > 0;
}

// 착수한 곳의 반대방향에 돌이 있는지 없는지 체크
void flipInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
	// 돌과 돌 사이에 돌이 몇개 있는지 체크 후 뒤집기
	do {
		board[row][col] = colour;

		row += deltaRow;
		col += deltaCol;
	} while (board[row][col] != colour && board[row][col] != U);
}

// 상대방의 돌을 뒤집는 함수. 함수타입이 bool형식이라  있으면 true, 아니면 false값을 반환함.
bool flipEnemyPieces(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour) {
	bool flipped = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;

			// 뒤집을 돌이 1개 이상 있는지 확인
			if (possibleFlipsInDirection(board, n, row, col, colour, i, j) > 0) {
				flipInDirection(board, n, row, col, colour, i, j);
				flipped = true;
			}
		}
	}
	// 하나 이상의 돌을 뒤집을 수 있으면 true값이 담겨져있고, 아니면 false 값을 반환.
	return flipped;
}

// 뒤집은 돌을 화면에 출력  ex) 검은색 -> 흰색
char oppositePiece(char turn) {
	return turn == B ? W : B;
}

/* B - Black wins
 * W - White wins
 * TIE - 무승부
 * ONGOING - 진행
 */
//게임이 종료되었는지 확인
char checkGameStatus(char board[][MAX_BOARD_LEN], int n){
	int blackFlips = 0, whiteFlips = 0;			
	int blackPieces = 0, whitePieces = 0;		

	for(int i=0; i<n;i++){
		for(int j=0; j<n;j++){
			if(board[i][j] == U){
				blackFlips += possibleFlips(board, n ,i, j, B); // 더 이상 뒤집을 곳이 있는지 없는지 확인
				whiteFlips += possibleFlips(board, n ,i, j, W);
			}
			// 각각의 돌이 몇개 있는지를 확인
			else if(board[i][j] == B) {
				blackPieces ++;
			}
			else if(board[i][j] == W){
				whitePieces ++;
			}
		}
	}
	
	// 모든 조각이 뒤집을 수 없으면 게임을 끝냄.
	if(blackFlips == 0 && whiteFlips == 0){
		if(blackPieces > whitePieces)
			return B;
		else if(whitePieces > blackPieces)
			return W;
		else
			return TIE;
	}else
		return ONGOING;
}