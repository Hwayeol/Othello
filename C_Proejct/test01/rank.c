#pragma once
#include "game.h"
#include <my_global.h>
#include <mysql.h>

char id[50];   //���̵�
char buffer[100];   //����
int choose;
MYSQL conn;  //sql ����
MYSQL *connection;       // sql�� ���� ���� ������ �ϴ� ����
MYSQL_RES *sql_result; //sql ��� �޴� ����ü ����
MYSQL_ROW sql_row;   //������� �����ϴ� ����ü

void rank() {
	int y = 10, count = 1;
	gotoxy(28, 6);
	printf("�� �ְ��� ���ڡ�");

	// ���� �κ��� �����Ϳ��� win �׸� ���� ���������� �̴� sql ��ɾ�
	sprintf(buffer, "select * from score order by win desc");
	mysql_query(&conn, buffer); // SQL������ �θ� ���� ���ۿ� ����
	sql_result = mysql_store_result(connection); //sql ���� ����� �����ͼ� ����.

	gotoxy(18, y);

	//top 3������ id�� ��,���� Ƚ���� ������.
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL && count <= 3) {
		printf("TOP %d ID : %s      %d��  %d�� \n", count,sql_row[0], atoi(sql_row[1]),atoi(sql_row[2]));
		gotoxy(18, y = y+2);

		count++;
	}

	printf("\n\n\n");
	Sleep(5000);
}