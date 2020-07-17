#include "game.h"
#include <my_global.h>
#include <mysql.h>

#define DB_HOST "localhost"
#define DB_User "root"
#define DB_passwd "qlalfqjsgh"
#define DB_name "test3"
#define CHOP(x) x[strlen(x) - 1] = ' '   //����Ű ���ִ� ���

char id[50];   //���̵�
char buffer[100];   //����
int choose;
int win = 0, lose = 0, tie = 0;
MYSQL conn;  //sql ����
MYSQL *connection;       // sql�� ���� ���� ������ �ϴ� ����
MYSQL_RES *sql_result; //sql ��� �޴� ����ü ����
MYSQL_ROW sql_row;   //������� �����ϴ� ����ü

void sql() {

	char passwd[50] = { 0 };
	int i = 0;
	int count = 0;
	mysql_init(&conn);  //�ʱ�ȭ

	connection = mysql_real_connect(&conn, DB_HOST, DB_User, DB_passwd, DB_name, 3307, (char*)NULL, 0);
	// �����ͺ��̽� ����

	back:

	printf("�α��� �Ͻðڽ��ϱ� ? ");
	printf("\n (1 : ȸ������, 2 : �α��� , 3 : ��ȸ��)");
	scanf(" %d", &choose);

	// ȸ������
	if (choose == 1) {

		mysql_query(connection, "select * from log");   // db �ҷ�����
		sql_result = mysql_store_result(connection);

		mysql_free_result(sql_result);   //����ϴ� sql

		scanf("%*c");   //�ʹ� ���Ͱ��� ������.
		printf(" ID :  ");
		fgets(id, 50, stdin);  // �Է¹��� ���� ���Ϸ� �����ϰų� getchó�� �ø����� ����  stdin : ����ͷ� ����.
		CHOP(id);  //�Է��Ҷ� �߻��ϴ� ����Ű�� �����.

		sprintf(buffer, "select * from log");    //SQL������ log �� ������ �ҷ���.
		mysql_query(&conn, buffer); // SQL������ �θ� ���� ���ۿ� ����
		sql_result = mysql_store_result(connection); //sql ���� ����� �����ͼ� ����.

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {   //������ ������� id, pass Ȯ��
			if (strcmp(id, sql_row[0]) == 0) {	// �ߺ����̵�� ������ �Ǵ��Ͽ� back �������� �ǵ���.
				if (strcmp(id, sql_row[0]) != 0) {  //�ߺ��� �ƴϸ� ��й�ȣ �Է±������� �Ѿ.
					break;
				}
				printf("���̵� �ߺ�! \n");
				goto back;
			}
		}
		mysql_free_result(sql_result);   //����ϴ� sql

		printf(" PASSWD : ");
		fgets(passwd, 50, stdin);
		CHOP(passwd);

		//ȸ������ ������ sql ���̺� ����
		sprintf(buffer, "insert into log values" "('%s', '%s')", id, passwd);
		mysql_query(connection, buffer);  //�Է¹��� ���� SQL�� ����

		//������ ID�� ���ھ 0���� �ʱ�ȭ
		sprintf(buffer, "insert into score values" "('%s',0,0,0)", id, win, lose, tie);
		mysql_query(connection, buffer);  //�Է¹��� ���� SQL�� ����

		sql_result = mysql_store_result(connection);    //��� ����

		printf("ȸ������ ����!\n\n");
		Sleep(1000);
		system("cls");
		goto back;
	}

	// �α���
	else if (choose == 2) {		

		mysql_query(connection, "select * from log");   // db �ҷ�����
		sql_result = mysql_store_result(connection);

		mysql_free_result(sql_result);   //����ϴ� sql

		scanf("%*c");   //�ʹ� ���Ͱ��� ������.
		printf(" ID :  ");
		fgets(id, 50, stdin);  // �Է¹��� ���� ���Ϸ� �����ϰų� getchó�� �ø����� ����  stdin : ����ͷ� ����.
		CHOP(id);  //�Է��Ҷ� �߻��ϴ� ����Ű�� �����. 

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {   //������ ������� id, pass Ȯ��

			if (strcmp(id, sql_row[0]) == 0) {     // ���̵� ��ġ�ϸ� ��й�ȣ �Է�.				

				printf("PASSWD : ");
				fgets(passwd, 50, stdin);
				CHOP(passwd);

				if (strcmp(passwd, sql_row[1]) == 0) {  //�α��� ������ sql.c�� ����
					printf("�α��� ����! \n\n");
					
					//mysql�� ��ɾ�� score ���̺��� ������ ������.
					sprintf(buffer, "select * from score where id = '%s'", id);
					mysql_query(&conn, buffer);
					sql_result = mysql_store_result(connection);

					//�Է¹��� id�� �����͸� �ҷ���.
					//atoi�� ���ڿ��� ������ ��ȯ�Ͽ� ���.
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						printf(" ID : %s      %d��, %d��,%d��  \n\n\n", sql_row[0], atoi(sql_row[1]), atoi(sql_row[3]), atoi(sql_row[2]));
					}

					Sleep(1000);
					return 0;
				}

				else if (strcmp(passwd, sql_row[1]) != 0) {   //��й�ȣ�� ��ġ���� ������ ����
					printf("��й�ȣ ����.\n\n");
					goto back;
				}
			}
		}
		// sql���̺� ���� ������ ���� ���� �� �α��� �ʱ�ȭ������ �ǵ���.
		printf("���̵� ����!\n");
		goto back;
	}

	//��ȸ�� �÷���
	else if (choose == 3) {
		system("cls");
		printf("��ȸ������ �÷����մϴ�. \n\n");
		Sleep(500);

		return 0;
	}
	else {
		printf("�ٽ� �Է��ϼ���.\n");
		goto back;
	}
}