#include <stdio.h>
#include <mysql.h>
//#include <exception>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define HOST "46.101.174.185"
#define USER "mandelbrot"
#define PASS "motznehaslo"
#define DB "mandel"
#define SEL "SELECT * FROM tasks_task WHERE status!='done' ORDER BY time LIMIT 1"

int kbhit(void);

typedef struct {
	unsigned x;
	unsigned y;
} Pixel2D;

typedef struct {
	double x;
	double y;
} Coords2D;

typedef struct {
	double duration;
	unsigned framerate;
	Pixel2D frameSize;
	double dotSize;
	Coords2D pathStartPoint;
	Coords2D pathEndPoint;
	double zoomStart;
	double zoomEnd;
	double colorStart;
	double colorEnd;
} Scene;

typedef struct {
	unsigned id;
	const char* datetime;
	unsigned user_id;
	unsigned progress;
	const char* status;
	const char* file_path;
} Task;

class MysqlComm{
public:
	MysqlComm(const char* _host, const char* _user, const char* _pass, const char* _db);
	Scene GetScene();
	void SetScene(Scene _foo);
	void TaskGet();
	void Select(const char* _table, const char* _values);
	void Command(const char* _command);
	void PrintRow();
	void TaskUpdateProgress(unsigned _progress);
	void TaskUpdateStatus(const char* _status);
	void TaskClose();
	void Connect();
	void Disconnect();

	MYSQL_ROW GetRow();
private:
	const char* host;
	const char* user;
	const char* pass;
	const char* db;
	char* command;
	int state;
	int num_fields;
	Task task;
	Scene s_scene;
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;
	void DoCommand(const char* _cmd);
};

MysqlComm::MysqlComm(const char* _host, const char* _user, const char* _pass, const char* _db){
	mysql_init(&mysql);
		num_fields = 0;
		host = _host;
		user = _user;
		pass = _pass;
		db = _db;
}

void MysqlComm::Connect(){
	connection = mysql_real_connect(&mysql,host, user, pass, db, 0,0,0);
}

void MysqlComm::Disconnect(){
	mysql_free_result(result);
	mysql_close(connection);
}

void MysqlComm::DoCommand(const char* _cmd){
	state = mysql_query(connection, _cmd);
	perror("mysql_query:");
	result = mysql_store_result(connection);
	perror("mysql_store_result:");
	num_fields=mysql_num_fields(result);
	perror("mysql_num_fields:");
	row=mysql_fetch_row(result);
	perror("mysql_fetch_row:");
}

void MysqlComm::TaskGet(){
	DoCommand(SEL);
	PrintRow();
	task.id = atoi(row[0]);
	task.datetime = row[1];
	task.user_id = atoi(row[2]);
	task.progress = atoi(row[3]);
	task.status = row[4];
	task.file_path = row[5];
}

void MysqlComm::Select(const char* _table, const char* _values){
	char* cmd = (char*)malloc(strlen("SELECT ")+strlen(_table)+strlen(" FROM ")+strlen(_values) + strlen(" WHERE status!='done' LIMIT 1")+1);
	perror("Malloc:");
	sprintf (cmd, "SELECT %s FROM %s WHERE status!='done' LIMIT 1",_values, _table);
	perror("sprintf SELECT:");
	DoCommand(cmd);
}

void MysqlComm::Command(const char* _cmd){
	DoCommand(_cmd);
}

void MysqlComm::TaskUpdateProgress(unsigned _progress){
	char* cmd = (char*)malloc(strlen("UPDATE tasks_task SET progress= WHERE id=")+21);
	sprintf(cmd, "UPDATE tasks_task SET progress=%d WHERE id=%d", _progress, task.id);
	printf("%s\n", cmd);
	DoCommand(cmd);
}

MYSQL_ROW MysqlComm::GetRow(){
	return row;
}

void MysqlComm::PrintRow(){
	for(int i =0;i<num_fields;i++)
		printf("%s ", row[i]);
	printf("\n");
}

int kbhit(void){
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

	if(ch != EOF){
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}
