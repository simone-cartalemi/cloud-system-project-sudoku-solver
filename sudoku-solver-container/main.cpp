#include "config.h"
#include "matrix.cpp"
#include "functions.cpp"
#include "tabu_solver.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

enum Algorithms {TabuSearch, GeneticSearch};
string algorithms[2] = {"TabuSearch", "GeneticSearch"};

const string folderPath = "../SudokuIstance/SudokuGenerator/";
const string resultsPath = "./results/";
const int maxFiles = 20;
const int totalRounds = 100;


string instances[] = {"easy", "medium", "hard", "extreme"};
int tabMs[] = {250, 500, 1000, 2000};
int genPopulations[] = {50, 800, 4000, 8000};
int maxIterations = 25000;

// Funzione che fa eseguire un comando al sistema operativo
void curlRequest(const string& message, const string& filePaht = "") {
	string botToken = "6917786734:AAFxclFJxmr1n90x7yO6oC-RNyZk-fNSAoY";
	string chatId = "262888284";
	string url = "https://api.telegram.org/bot" + botToken + "/sendMessage";
	string postData = "chat_id=" + chatId + "&text=" + message;

	// Costruisce il comando curl con l'URL specificato
	string command = "curl -s -o nul " + url + " -d \"" + postData + "\"";

	// Esegue il comando curl utilizzando system()
	int result = system(command.c_str());
	
	// Verifica se la chiamata al sistema ha avuto successo
	if (result != 0) {
		cerr << "Errore durante l'invio del messaggio" << endl;
	}

	// Manda file opzionalmente
	if (filePaht != "") {
		url = "https://api.telegram.org/bot" + botToken + "/sendDocument";
		command = "curl -s -o nul -F chat_id=\"" + chatId + "\" -F document=@\"" + filePaht + "\" " + url;

		result = system(command.c_str());
	
		// Verifica se la chiamata al sistema ha avuto successo
		if (result != 0) {
			cerr << "Errore durante l'invio del file" << endl;
		}
	}
}


bool execution(Monitor &M, int j, Algorithms algo, string fileName, string instance, int class_i) {
	// Apri file di output
	string filePath = resultsPath + instance + to_string(j + 1) + "_" + algorithms[algo] + ".json";
	ofstream fileOut(filePath);
	if (!fileOut.is_open()) {
		cerr << "Errore nella creazione del file di output\n";
		return false;
	}
	
	cout << endl << algorithms[algo] << " per l'istanza " << instance << " n " << j + 1 << "/" << maxFiles << endl;
	curlRequest(algorithms[algo] + " per istanza " + instance + " n " + to_string(j + 1) + "/" + to_string(maxFiles));

	// Fa partire il monitor
	M.initialize(fileName, instance, algorithms[algo]);

	for (int k = 0; k < totalRounds; ++k)	{
		// Imposta il round
		M.setRound(k);

		Matrix* out;
		switch (algo) {
		case TabuSearch:
			out = tabuSearch(M, folderPath + fileName, maxIterations, tabMs[class_i]);
			break;
		case GeneticSearch:
			out = geneticSearch(M, folderPath + fileName, 0.9, 0.9, maxIterations, genPopulations[class_i]);
			break;
		default:
			cerr << algo << ": algoritmo sconosciuto\n\n";
			return false;
			break;
		}
		if (!out) {
			cout << "C'è stato un problema con l'output della matrice\n\n";
			return false;
		}
		else delete out;
		cout << "\r" << "ist: " << j + 1 << "/" << maxFiles << ", round: " << k + 1 << "/" << totalRounds;
		curlRequest("istance: " + to_string(j + 1) + "/" + to_string(maxFiles) + ", round: " + to_string(k + 1) + "/" + to_string(totalRounds));
	}
	// Scrivi risultati nel file di output e chiudilo
	fileOut << M.resultsJSON();
	fileOut.close();
	// Spedisci il file
	curlRequest("File di output:", filePath);
	
	cout << "\n------\n";
	return true;
}


int main() {

	Monitor M = Monitor(totalRounds);
	curlRequest("Processo avviato");

	for (int class_i = 0; class_i < 4; ++class_i) {
		cout << "+ Testo la categoria " << instances[class_i] << endl;

		//int parti_da = 1;
		//int fino_a = 10;

		for (int j = 0; j < maxFiles; ++j) {
			//if (j + 1 < parti_da) continue;
			//if (j + 1 > fino_a) break;
			string fileName = instances[class_i] + to_string(j + 1) + ".csv";
			execution(M, j, TabuSearch, fileName, instances[class_i], class_i);
			execution(M, j, GeneticSearch, fileName, instances[class_i], class_i);
		}
	}

	cout << "\n\nProcess ended\n";
	system("PAUSE");
	return 0;
}
