#include "game.h"

// ���� ���� ��� �Լ�
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

// �Է��� ���� ���� ���� ������ �ִ��� Ȯ�� ������ true, ������ false ��
bool positionInBounds(int n, int row, int col) {
	return (row >= 0 && col >= 0 && row < n && col < n);   
	//��� ���� 0���� ũ�ų� ����, �������� ũ�⺸�� ������
}

// �־��� ��ġ�� ������ �� �ִ� ���� ����
int possibleFlipsInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
	int count = 0;

	row += deltaRow;
	col += deltaCol;

	// ������ ���� ���� �ִ��� ������ Ȯ��
	if (!positionInBounds(n, row, col) || board[row][col] == U || board[row][col] == colour)
		return 0;

	// �ι�° ��ġ�� ���� ���� �ִ��� Ȯ��
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

// ���� ���� � �ѱ� �� �ִ��� üũ
int possibleFlips(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour) {
	int count = 0;
	int i = 0, j = 0;

	// ���� ���� �ִ��� �������� ���� 
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)  		// ������ continue
				continue;
			
			//��  ������ �� �ִ� ���� üũ �� count�� ����
			count += possibleFlipsInDirection(board, n, row, col, colour, i, j);
			
		}
	}
	//������ count ���� ����
	return count;
}

// ������ ��ġ�� ������ �Ҽ� �ִ��� ������ Ȯ��
bool validMove(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour){

	// �Է��� ���� ���� ������ true ���� ����
	return board[row][col] == U && possibleFlips(board, n, row, col, colour) > 0;
}

// ������ ���� �ݴ���⿡ ���� �ִ��� ������ üũ
void flipInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
	// ���� �� ���̿� ���� � �ִ��� üũ �� ������
	do {
		board[row][col] = colour;

		row += deltaRow;
		col += deltaCol;
	} while (board[row][col] != colour && board[row][col] != U);
}

// ������ ���� ������ �Լ�. �Լ�Ÿ���� bool�����̶�  ������ true, �ƴϸ� false���� ��ȯ��.
bool flipEnemyPieces(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour) {
	bool flipped = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;

			// ������ ���� 1�� �̻� �ִ��� Ȯ��
			if (possibleFlipsInDirection(board, n, row, col, colour, i, j) > 0) {
				flipInDirection(board, n, row, col, colour, i, j);
				flipped = true;
			}
		}
	}
	// �ϳ� �̻��� ���� ������ �� ������ true���� ������ְ�, �ƴϸ� false ���� ��ȯ.
	return flipped;
}

// ������ ���� ȭ�鿡 ���  ex) ������ -> ���
char oppositePiece(char turn) {
	return turn == B ? W : B;
}

/* B - Black wins
 * W - White wins
 * TIE - ���º�
 * ONGOING - ����
 */
//������ ����Ǿ����� Ȯ��
char checkGameStatus(char board[][MAX_BOARD_LEN], int n){
	int blackFlips = 0, whiteFlips = 0;			
	int blackPieces = 0, whitePieces = 0;		

	for(int i=0; i<n;i++){
		for(int j=0; j<n;j++){
			if(board[i][j] == U){
				blackFlips += possibleFlips(board, n ,i, j, B); // �� �̻� ������ ���� �ִ��� ������ Ȯ��
				whiteFlips += possibleFlips(board, n ,i, j, W);
			}
			// ������ ���� � �ִ����� Ȯ��
			else if(board[i][j] == B) {
				blackPieces ++;
			}
			else if(board[i][j] == W){
				whitePieces ++;
			}
		}
	}
	
	// ��� ������ ������ �� ������ ������ ����.
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