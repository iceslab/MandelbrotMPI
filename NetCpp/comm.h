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

#endif