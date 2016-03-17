#include <cstdio>
#include <cstdlib>
#include <mysql.h>
#include <exception>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define HOST "46.101.174.185"
#define USER "mandelbrot"
#define PASS "motznehaslo"
#define DB "mandel"
#define SEL "SELECT * FROM tasks_task"

 int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

int main(int argc, char **args)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;
	int state;
	mysql_init(&mysql);
	int num_fields=0;
	
	while(!kbhit())
	{
		try{		
			connection = mysql_real_connect(&mysql,HOST,USER,PASS,DB, 0,0,0);
			printf("Connecting\n");
			state = mysql_query(connection, SEL);
			printf("%s\n", SEL);
			result = mysql_store_result(connection);
			printf("Rows:%d\n", (int)mysql_num_rows(result));
			num_fields=mysql_num_fields(result);
			while ( ( row=mysql_fetch_row(result)) != NULL )
			{
				for(int i=0;i<num_fields;i++)
					printf("%s ", row[i]);
				printf("\n");
			}

			mysql_free_result(result);
			mysql_close(connection);
			sleep(1);
			printf("\n");
		}
		catch(std::exception &e)
		{
			printf("%s", e.what());
			return 1;
		}
	}
	return 0;
}