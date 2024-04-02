#include "config.h"
#include "matrix.cpp"
#include "functions.cpp"
#include "tabu_solver.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int tabMs = 2000;
int maxIterations = 25000;
string resultsPath = "results/";

int main(int argc, char* argv[]) {
	// Verifica i parametri di input
    if (argc != N * N + 1) {
        cerr << "Usage: " << argv[0] << " <" << N * N << " numeri>" << endl;
        return 1;
    }

	// Matrice del Sudoku in input
	Matrix* sudoku = readSudokuFromInput(argv);

	tabuSearch(sudoku, maxIterations, tabMs);

	// Apri file di output
	ofstream fileOut(resultsPath + "solution.txt");
	if (!fileOut.is_open()) {
		cerr << "Errore nella creazione del file di output\n";
		return false;
	}
	fileOut << sudoku->outMatrix();
	fileOut.close();

	cout << "\n\nProcess ended\n";
	return 0;
}
