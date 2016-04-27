#include "comm.hpp"

int main()
{
	MysqlComm com(HOST, USER, PASS, DB);
	try{
		com.Init();
		com.Connect();
		com.Select("tasks_task", "*");
		com.PrintRow();
		Scene scene = com.GetScene();
		Task task = com.GetTask();
		com.TaskUpdateProgress(0);
		com.TaskClose(scene, task, ".txt");
		com.Disconnect();
	} catch(const char* e){
		printf("%s\n", e);
		fflush(stdout);
	}
	return 0;
}
