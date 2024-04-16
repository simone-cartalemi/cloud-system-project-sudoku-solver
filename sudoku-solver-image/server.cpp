#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 3264;
const int BUFFER_SIZE = 1024;


int createSocket() {
	// Crea socket TCP
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		cerr << "Errore nella creazione del socket" << endl;
		return -1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	// Collega socket
	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
		cerr << "Errore nel collegare socket all'indirizzo e alla porta" << endl;
		close(serverSocket);
		return -1;
	}

	return serverSocket;
}

int listenSocket(const int serverSocket) {
	if (listen(serverSocket, 1) == -1) {
		cerr << "Errore nel mettere il socket in ascolto" << endl;
		return -1;
	}
	cout << "In ascolto su porta " << to_string(PORT) << "..." << endl;

	return 0;
}

int acceptConnection(const int serverSocket) {
	sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
	if (clientSocket == -1) {
		cerr << "Errore nell'accettare la connessione" << endl;
		return -1;
	}
	cout << "Connessione accettata" << endl;

	return clientSocket;
}

int runSolver(char* matrix) {
	string command = "./solver ";
	command += matrix;
	int result = system(command.c_str());
	if (result) {
		cerr << "Errore nell'esecuzione del risolutore" << endl;
		return -1;
	}
	
	return 0;
}

int sendResult(const string filePath, const int clientSocket) {
	FILE *fd = fopen(filePath.c_str(), "rb");
	size_t rret, wret;
	char buffer[1024];
	int bytes_read;
	int bytes_sended = 0;
	while (!feof(fd)) {
		if ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fd)) > 0) {
			send(clientSocket, buffer, bytes_read, 0);
			bytes_sended += bytes_read;
		}
		else {
			break;
		}
	}
	fclose(fd);
	if (!bytes_sended) {
		cerr << "Soluzione non inviata" << endl;
		return -1;
	}
	
	return 0;
}



int main() {
	
	// Crea Socket e mettila in ascolto
	int serverSocket = createSocket();
	if (serverSocket == -1) {
		return 1;
	}
	if(listenSocket(serverSocket)) {
		return 1;
	}
	
	// Accetta una connessione in ingresso
	int clientSocket = acceptConnection(serverSocket);
	if (clientSocket == -1) {
		close(serverSocket);
		return 1;
	}

	// Ricevi la matrice
	char buffer[1024];
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived <= 0) {
		cerr << "Errore nella ricezione della matrice" << endl;
		close(clientSocket);
		close(serverSocket);
		return 1;
	}
	buffer[bytesReceived] = '\0';

	// Esegui risolutore
	if (runSolver(buffer)) {
		close(clientSocket);
		close(serverSocket);
		return 1;
	}
	
	// Invia il risultato
	sendResult("results/solution.txt", clientSocket);

	// Chiudi connessione
	close(clientSocket);
	close(serverSocket);

	return 0;
}
