#pragma once
#include "game.h"
#include <my_global.h>
#include <mysql.h>

char id[50];   //아이디
char buffer[100];   //버퍼
int choose;
MYSQL conn;  //sql 선언
MYSQL *connection;       // sql로 부터 연결 설정을 하는 변수
MYSQL_RES *sql_result; //sql 결과 받는 구조체 선언
MYSQL_ROW sql_row;   //행단위를 저장하는 구조체

void rank() {
	int y = 10, count = 1;
	gotoxy(28, 6);
	printf("★ 최고의 승자★");

	// 점수 부분의 데이터에서 win 항목에 대해 높은순으로 뽑는 sql 명령어
	sprintf(buffer, "select * from score order by win desc");
	mysql_query(&conn, buffer); // SQL문으로 부른 값을 버퍼에 저장
	sql_result = mysql_store_result(connection); //sql 연결 결과를 가저와서 저장.

	gotoxy(18, y);

	//top 3까지의 id와 승,패의 횟수를 보여줌.
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL && count <= 3) {
		printf("TOP %d ID : %s      %d승  %d패 \n", count,sql_row[0], atoi(sql_row[1]),atoi(sql_row[2]));
		gotoxy(18, y = y+2);

		count++;
	}

	printf("\n\n\n");
	Sleep(5000);
}