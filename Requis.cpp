#include "Requis.h"
#include <sstream>

Requis::Requis(string requisToda)
{
	stringstream ss { requisToda };
	ss >> method;
	ss >> path;
	ss >> version;

	// Pega o tipo do arquivo
	size_t p = path.rfind('.');
	if (p != string::npos)
		type = path.substr(p + 1);
}

bool Requis::verCmd() // Returns true if the command is valid
{
	return method == "GET";
}


bool Requis::verArq()  // Returns false if the file does not exist
{
	ifstream input ("files/" + path);
	return input.good();
}

string Requis::getTime()
{
	const string nomeMes[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	const string diaSem[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	time_t Tempo = time(NULL);
	struct tm *aTempo = localtime(&Tempo);

	int seg = aTempo->tm_sec;
	int hora = aTempo->tm_hour;
	int min = aTempo->tm_min;
	int dia = aTempo->tm_mday;
	int diasem = aTempo->tm_wday;
	int mes = aTempo->tm_mon;
	int ano = aTempo->tm_year + 1900;	
	
	return diaSem[diasem] + ", " + to_string(dia) + " " + nomeMes[mes] + " " + to_string(ano) + " " + to_string(hora) + ":" + to_string(min) + ":" + to_string(seg) + " BRT";
}

string Requis::getTipo()
{
	if (type == "html")
		return "text/html; charset=utf-8";
	if (type == "jpg")
		return "image/jpeg";
	if (type == "mp3")
		return "audio/mp3";
	return "text/plain";
}

string Requis::getConteudo()
{
	string linha, buffer;

	ifstream input ("files/" + path);
	while(getline(input, linha))
		buffer += linha + "\n";

	return buffer;
}

string Requis::GetAnswer()
{
    string ret, conteudo;

	if(verCmd()){
    	if (verArq()){
    		conteudo = getConteudo();

    		ret = "HTTP/1.1 200 OK\n"
    			  "Date: " + getTime() + "\n"
    			  "Server: DeepShadow/1.0.2\n"
				  "Content-Length: " + to_string(conteudo.size()) + "\n"
				  "Content-Type: " + getTipo() + "\n"
				  "Connection: Closed\n\n" + conteudo;

    	}
    	else{
    		conteudo = "<!DOCTYPE html>\n"
  					  "<html>\n"
 					  "<head>\n"
 					  "<title>Not Found</title>\n"
 					  "</head>\n"
 					  "<body>\n"
 					  "<H1><b>ERROR 404: NOT FOUND</b></H1>\n"
					  "</body>\n"
					  "</html>\n";

    		ret = "HTTP/1.1 404 Not Found\n"
      			  "Date: " + getTime() + "\n"
      			  "Server: DeepShadow/1.0.2\n"
				  "Content-Length: " + to_string(conteudo.size()) + "\n"
  				  "Content-Type: text/html; charset=iso-8859-1\n"
				  "Connection: Closed\n\n" + conteudo;
    	}
    }
	else{
		conteudo = "<!DOCTYPE html>\n"
				  "<html>\n"
				  "<head>\n"
				  "<title>Bad Request</title>\n"
				  "</head>\n"
				  "<body>\n"
				  "<H1><b>ERROR 400: BAD REQUEST</b></H1>\n"
				  "</body>\n"
				  "</html>\n";

		ret = "HTTP/1.1 400 Bad Request\n"
    	      "Date: " + getTime() + "\n"
    	      "Server: DeepShadow/1.0.2\n"
			  "Content-Length: " + to_string(conteudo.size()) + "\n"
    	  	  "Content-Type: text/html; charset=iso-8859-1\n"
			  "Connection: Closed\n\n" + conteudo;
	}

	return ret;
}
