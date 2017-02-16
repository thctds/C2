#include "Log.h" 
#include "Requis.h"

using namespace std;

#define PORT 31415 

void processoCliente(int cliente, string endIp, logger* textoLog); 

int main(int argc, char *argv[]){ 

    logger *textoLog = new logger("log.txt"); 
	int FDSocket; 
    struct sockaddr_in serv_addr, cli_addr; 
    cout << "[SERVER] Using port " << PORT << ".\n"; 


    // Criacao da Server Socket:
    FDSocket = socket(AF_INET, SOCK_STREAM, 0); 

   if (FDSocket < 0){ 
        cerr << "[ERROR] Could not create server socket.\n"; 
        textoLog->escreveLog("[ERROR] Could not create server socket: Server was not initialized!\n"); 
        exit(1); 
    }

    // Configurando TCP Socket
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    int trueInt = 1;
    if (setsockopt(FDSocket, SOL_SOCKET, SO_REUSEADDR, &trueInt, sizeof(int)) < 0) {
        perror("Failed to setsockopt(SO_REUSEADDR) failed");
        abort();
    }

    //
    if(bind(FDSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){  
        cerr << "[ERROR] Could not configure bind.\n"; 

        textoLog->escreveLog("[ERROR] Could not configure bind: Server was not initialized!\n"); 
        exit(1); 
    }

    listen(FDSocket, 0); 
    for(;;){ 
        cout << "[SERVER] Waiting for clients.\n"; 
        socklen_t clilen = sizeof(cli_addr); 
        int cliente = accept(FDSocket, (struct sockaddr *) &cli_addr, &clilen); 

        char str[1048];
        inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN); 

        string endIp = str; 

        textoLog->escreveLog("New client connected. [IP: " + endIp + "]\n"); 

        cout << "[SERVER] Client " << endIp << " connected!\n"; 
        thread threadCliente(processoCliente, cliente, endIp, textoLog); 
        threadCliente.detach(); 
    }

    delete textoLog;
    return 0;
}



void processoCliente(int cliente, string endIp, logger* textoLog){ 
    char bufferR[2048]; 
	string buffer;

    while (true) {
        if (recv(cliente, bufferR, sizeof(bufferR), 0) <= 0) {
            printf("[CLIENT %s] Falha lendo buffer:\n", endIp.c_str());
            close(cliente);
            return; 
        }

        buffer += bufferR;
        if (buffer.find("\r\n\r\n") != string::npos)
            break;
    }

    try {
        Requis novaReq(buffer);
        
        printf("[CLIENT %s] Requesting:\n%s\n", endIp.c_str(), buffer.c_str()); 
        textoLog->escreveLog("[CLIENT " + endIp + "] Requesting:\n" + buffer + "\n"); 

        string resp = novaReq.GetAnswer(); 

        const char *saida = resp.c_str(); 
        send(cliente, saida, resp.size(), 0); 

        printf("[CLIENT %s] Received answer:\n%s\n", endIp.c_str(), resp.c_str());
        textoLog->escreveLog("Client " + endIp + " received:\n" + resp + "\n");

        printf("[CLIENT %s] Disconnecting.\n", endIp.c_str());
        textoLog->escreveLog("Client " + endIp + " disconnected.\n");

        close(cliente);
    } catch (...) {
        printf("[CLIENT %s] Request inválido:\n", buffer.c_str()); 
        textoLog->escreveLog("[CLIENT " + endIp + "] Request inválido:\n" + buffer + "\n"); 
        close(cliente);
        return;
    }
}
