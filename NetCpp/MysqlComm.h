#ifndef _MYSQL_COMM_H_
#define _MYSQL_COMM_H_

#include <stdio.h>
#include <mysql.h>
//#include <exception>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define HOST "46.101.174.185"
#define HOST2 "156.17.41.50"
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
	const char* created_time;
	unsigned user_id;
	unsigned progress;
	const char* status;
	const char* file_path;
} Task;

class MysqlComm {
public:
	MysqlComm(const char* _host, const char* _user, const char* _pass,
			const char* _db);
	void Init();
	Scene GetScene();
	MYSQL_ROW GetRow();
	Task GetTask();
	void Select(const char* _table, const char* _values);
	void Command(const char* _command);
	bool AnythingToDo();
	void TaskUpdateProgress(unsigned _progress);
	void TaskUpdateStatus(const char* _status);
	void TaskStart();
	void TaskClose(Scene _scene, Task _task, const char* _extension);
	void Connect();
	void Disconnect();
	void PrintRow();
private:
	const char* host;
	const char* user;
	const char* pass;
	const char* db;
	char* command;
	int state;
	int num_fields;
	Task task;
	Scene scene;
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;
	void HandleMysqlError();
	void DoCommand(const char* _cmd, bool _gives_data);
};

#endif