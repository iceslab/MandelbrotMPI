#include "comm.h"
int main()
{
	MysqlComm com(HOST, USER, PASS, DB);
	com.Connect();
	//com.Select();
	//com.Select("tasks_task", "id, user_id, cos, status");
	com.TaskGet();

	com.TaskUpdateProgress(10);
	com.PrintRow();
	// skrypt, plil, user, host
	//system("./scp.sh scp.sh mandel 46.101.174.185");
	//printf("Przesyłanie pliku zakończone :D\n");
	com.Disconnect();
	return 0;
}