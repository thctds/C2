#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

class logger{
	mutex mute;
	string camLog;
public:
	logger(string camArq);
	void escreveLog(string mensagem);
};

#endif /* LOG_H_ */
