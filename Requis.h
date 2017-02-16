#ifndef REQUIS_H_
#define REQUIS_H_

#include "Log.h"

class Requis{
private:
	string method;
	string path;
	string version;
	string type;

	
	string getTime();
	string getTipo();
	string getConteudo();
	bool verCmd();
	bool verArq();

public:
	Requis(string requisToda);
	string GetAnswer();
};

#endif /* REQUIS_H_ */