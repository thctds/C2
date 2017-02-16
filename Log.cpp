#include "Log.h"

logger::logger(string camArq)
{
	this->camLog = camArq;
}

void logger::escreveLog(string mensagem)
{
	std::ofstream saida;

	this->mute.lock();  // Entering critic session

	saida.open(this->camLog.c_str(), std::ios::out|std::ios::app);
	saida << mensagem;
	saida.close();

	this->mute.unlock();  // Exit critic session
}
