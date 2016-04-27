#include "MysqlComm.h"

MysqlComm::MysqlComm(const char* _host, const char* _user, const char* _pass, const char* _db)
	: host( _host )
	, user( _user )
	, pass( _pass )
	, db( _db )
	, command( nullptr )
	, state( 0 )
	, num_fields( 0 )
	, task()
	, scene()
	, result( nullptr )
	, row( 0 )
	, connection( nullptr )	
	{
	row = MYSQL_ROW();
}
void MysqlComm::Init() {
	mysql_init(&mysql);
	HandleMysqlError();
}
void MysqlComm::Connect() {
	connection = mysql_real_connect(&mysql, host, user, pass, db, 0, NULL, 0);
	HandleMysqlError();
}

void MysqlComm::Disconnect() {
	mysql_free_result(result);
	HandleMysqlError();
	mysql_close(connection);
	HandleMysqlError();
}

void MysqlComm::DoCommand(const char* _cmd, bool _gives_data) {
	mysql_query(connection, _cmd);
	HandleMysqlError();
	if (_gives_data) {
		result = mysql_store_result(connection);
		HandleMysqlError();
		num_fields = mysql_num_fields(result);
		HandleMysqlError();
		row = mysql_fetch_row(result);
		HandleMysqlError();
	}
}

bool MysqlComm::AnythingToDo()
{
	return static_cast<bool>(row != 0);
}

Task MysqlComm::GetTask() {
	task.id = atoi(row[0]);
	task.created_time = row[1];
	task.user_id = atoi(row[2]);
	task.progress = atoi(row[3]);
	task.status = row[4];
	task.file_path = row[9];
	return task;
}

Scene MysqlComm::GetScene(){
	scene.duration=atof(row[8]);
	scene.framerate=strtoul(row[12],NULL,10);
	scene.frameSize=(Pixel2D){static_cast<unsigned int>(strtoul(row[10],NULL,10)),static_cast<unsigned int>(strtoul(row[11],NULL,10))};
	scene.dotSize=atof(row[7]);
	scene.pathStartPoint=(Coords2D){atof(row[15]), atof(row[16])};
	scene.pathEndPoint=(Coords2D){atof(row[13]), atof(row[14])};
	scene.zoomStart=atof(row[18]);
	scene.zoomEnd=atof(row[17]);
	scene.colorStart=atof(row[6]);
	scene.colorEnd=atof(row[5]);
	return scene;
}

void MysqlComm::Select(const char* _table, const char* _values) {
	char* cmd = (char*) malloc(
			strlen("SELECT ") + strlen(_table) + strlen(" FROM ")
					+ strlen(_values) + strlen(" WHERE status!='done' ORDER BY created_time LIMIT 1")
					+ 1);
	if (cmd == NULL)
		throw "Malloc failed.";
	sprintf(cmd, "SELECT %s FROM %s WHERE status!='done' ORDER BY created_time LIMIT 1", _values,
			_table);
	DoCommand(cmd, true);
}

void MysqlComm::TaskUpdateProgress(unsigned _progress) {
	char* cmd = (char*) malloc(
			strlen("UPDATE tasks_task SET progress= WHERE id=") + 23);
	if (cmd == NULL)
		throw "Malloc failed.";
	sprintf(cmd, "UPDATE tasks_task SET progress=%d WHERE id=%d", _progress,
			task.id);
	DoCommand(cmd, false);
}

void MysqlComm::TaskClose(Scene _scene, Task _task, const char* _extension) {
	// skrypt, plik, user, host
	char* cmd = (char*) malloc(
			strlen("./scp.sh mandel 46.101.174.185") + strlen(_extension)
					+ 12);
	if (cmd == NULL)
		throw "Malloc failed.";
	sprintf(cmd, "./scp.sh %d%s mandel 46.101.174.185", task.id, _extension);
	system(cmd);
	// koniec zapisu pliku na serwer
	// aktualizacja bazy
	cmd =
			(char*) malloc(
					strlen(
							"UPDATE tasks_task SET progress=100, status='done', file_path='.' WHERE id=")
							+ strlen(_extension) + 23);
	if (cmd == NULL)
		throw "Malloc failed.";
	sprintf(cmd,
			"UPDATE tasks_task SET progress=100, status='done', file_path='%d.%s' WHERE id=%d",
			task.id, _extension, task.id);
	DoCommand(cmd, false);
}

MYSQL_ROW MysqlComm::GetRow() {
	return row;
}

void MysqlComm::PrintRow() {
	if (row == NULL)
		throw "Row is empty.";
	for (int i = 0; i < num_fields; i++)
		printf("%s ", row[i]);
	printf("\n");
}

void MysqlComm::HandleMysqlError() {
	if ((strcmp(mysql_error(&mysql), "") != 0)) {
		throw mysql_error(&mysql);
	}
}

int kbhit(void) {
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

	if (ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}
void MysqlComm::TaskStart() {
	char* cmd = (char*) malloc(
			strlen("UPDATE tasks_task SET status='working' WHERE id=") + 11);
	if (cmd == NULL)
		throw "Malloc failed.";
	sprintf(cmd, "UPDATE tasks_task SET status='working' WHERE id=%d", task.id);
	DoCommand(cmd, false);
}