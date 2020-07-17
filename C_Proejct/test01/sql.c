#include "game.h"
#include <my_global.h>
#include <mysql.h>

#define DB_HOST "localhost"
#define DB_User "root"
#define DB_passwd "qlalfqjsgh"
#define DB_name "test3"
#define CHOP(x) x[strlen(x) - 1] = ' '   //엔터키 없애는 방법

char id[50];   //아이디
char buffer[100];   //버퍼
int choose;
int win = 0, lose = 0, tie = 0;
MYSQL conn;  //sql 선언
MYSQL *connection;       // sql로 부터 연결 설정을 하는 변수
MYSQL_RES *sql_result; //sql 결과 받는 구조체 선언
MYSQL_ROW sql_row;   //행단위를 저장하는 구조체

void sql() {

	char passwd[50] = { 0 };
	int i = 0;
	int count = 0;
	mysql_init(&conn);  //초기화

	connection = mysql_real_connect(&conn, DB_HOST, DB_User, DB_passwd, DB_name, 3307, (char*)NULL, 0);
	// 데이터베이스 접속

	back:

	printf("로그인 하시겠습니까 ? ");
	printf("\n (1 : 회원가입, 2 : 로그인 , 3 : 비회원)");
	scanf(" %d", &choose);

	// 회원가입
	if (choose == 1) {

		mysql_query(connection, "select * from log");   // db 불러오기
		sql_result = mysql_store_result(connection);

		mysql_free_result(sql_result);   //출력하는 sql

		scanf("%*c");   //초반 엔터값을 없애줌.
		printf(" ID :  ");
		fgets(id, 50, stdin);  // 입력받은 값을 파일로 저장하거나 getch처럼 올릴수도 있음  stdin : 모니터로 보냄.
		CHOP(id);  //입력할때 발생하는 엔터키가 사라짐.

		sprintf(buffer, "select * from log");    //SQL문으로 log 의 값들을 불러옴.
		mysql_query(&conn, buffer); // SQL문으로 부른 값을 버퍼에 저장
		sql_result = mysql_store_result(connection); //sql 연결 결과를 가저와서 저장.

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {   //이전에 만들었던 id, pass 확인
			if (strcmp(id, sql_row[0]) == 0) {	// 중복아이디는 오류로 판단하여 back 지점으로 되돌림.
				if (strcmp(id, sql_row[0]) != 0) {  //중복이 아니면 비밀번호 입력구문으로 넘어감.
					break;
				}
				printf("아이디 중복! \n");
				goto back;
			}
		}
		mysql_free_result(sql_result);   //출력하는 sql

		printf(" PASSWD : ");
		fgets(passwd, 50, stdin);
		CHOP(passwd);

		//회원가입 성공시 sql 테이블에 저장
		sprintf(buffer, "insert into log values" "('%s', '%s')", id, passwd);
		mysql_query(connection, buffer);  //입력받은 값을 SQL에 저장

		//가입한 ID의 스코어를 0으로 초기화
		sprintf(buffer, "insert into score values" "('%s',0,0,0)", id, win, lose, tie);
		mysql_query(connection, buffer);  //입력받은 값을 SQL에 저장

		sql_result = mysql_store_result(connection);    //결과 저장

		printf("회원가입 성공!\n\n");
		Sleep(1000);
		system("cls");
		goto back;
	}

	// 로그인
	else if (choose == 2) {		

		mysql_query(connection, "select * from log");   // db 불러오기
		sql_result = mysql_store_result(connection);

		mysql_free_result(sql_result);   //출력하는 sql

		scanf("%*c");   //초반 엔터값을 없애줌.
		printf(" ID :  ");
		fgets(id, 50, stdin);  // 입력받은 값을 파일로 저장하거나 getch처럼 올릴수도 있음  stdin : 모니터로 보냄.
		CHOP(id);  //입력할때 발생하는 엔터키가 사라짐. 

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {   //이전에 만들었던 id, pass 확인

			if (strcmp(id, sql_row[0]) == 0) {     // 아이디가 일치하면 비밀번호 입력.				

				printf("PASSWD : ");
				fgets(passwd, 50, stdin);
				CHOP(passwd);

				if (strcmp(passwd, sql_row[1]) == 0) {  //로그인 성공시 sql.c를 종료
					printf("로그인 성공! \n\n");
					
					//mysql에 명령어로 score 테이블의 내용을 보여줌.
					sprintf(buffer, "select * from score where id = '%s'", id);
					mysql_query(&conn, buffer);
					sql_result = mysql_store_result(connection);

					//입력받은 id의 데이터를 불러옴.
					//atoi로 문자열을 정수로 변환하여 출력.
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						printf(" ID : %s      %d승, %d무,%d패  \n\n\n", sql_row[0], atoi(sql_row[1]), atoi(sql_row[3]), atoi(sql_row[2]));
					}

					Sleep(1000);
					return 0;
				}

				else if (strcmp(passwd, sql_row[1]) != 0) {   //비밀번호가 일치하지 않으면 오류
					printf("비밀번호 오류.\n\n");
					goto back;
				}
			}
		}
		// sql테이블 내에 없으면 오류 문구 후 로그인 초기화면으로 되돌림.
		printf("아이디 오류!\n");
		goto back;
	}

	//비회원 플레이
	else if (choose == 3) {
		system("cls");
		printf("비회원으로 플레이합니다. \n\n");
		Sleep(500);

		return 0;
	}
	else {
		printf("다시 입력하세요.\n");
		goto back;
	}
}