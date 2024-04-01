#include "config.h"
#include "matrix_buffer.cpp"

#include <iostream>
#include <climits>
#include <string>
#include <ctime>

using namespace std;


// Restituisce m vicini di una matrice
void getNeighbors(const Matrix &original, Matrix* neighbors[], list<int*>** indexes[], int m) {
	for (int i = 0; i < m; ++i) {
		// Copia dell'originale
		neighbors[i]->copyFrom(original);
		// Fai shuffle alla sua lista degli indici per le sottomatrici
		shuffleIndexedElements(indexes[i], N);
	}
}

Matrix* tabuSearch(Monitor &M, const string file_csv, int iterations = 1000, int m = 200, int tabuSize = 500) {
	// Matrice del Sudoku, elementi fissi e soluzione
	Matrix* sudoku = readSudokuFromFile(file_csv);
	if (!sudoku) {
		return NULL;
	}
	Matrix* bestSolution = randomInitialization(sudoku);
	int bestFitness = bestSolution->getFitness();
	Matrix* bestCandidate = new Matrix(*bestSolution);
	
	// Istanzia il pool di m vicini e la lista degli indici
	Matrix* neighbors[m];
	list<int*>** indexes[m];
	for (int i = 0; i < m; ++i) {
		neighbors[i] = new Matrix(*bestSolution);
		indexes[i] = neighbors[i]->getIndexes();
	}
	// Istanzia pool Tabu list
	MatrixBuffer tabuList = MatrixBuffer(tabuSize, *bestSolution);
	tabuList.insert(*bestCandidate);
	
	// Parametri
	srand(time(0));
	
	int itera = 0;
	while(bestFitness > 0 && itera < iterations) {
		auto clock = M.setStart();
		getNeighbors(*bestCandidate, neighbors, indexes, m);
		int bestCandidateFitness = INT_MAX;
		
		for (int i = 0; i < m; ++i) {
			if (tabuList.isInList(*neighbors[i])) {
				continue;
			}
			
			int candidateFitness = neighbors[i]->getFitness();
			if (candidateFitness < bestCandidateFitness) {
				bestCandidateFitness = candidateFitness;
				bestCandidate->copyFrom(*neighbors[i]);
			}
			else {
				continue;
			}
			tabuList.insert(*bestCandidate);
		}
		
		// Fa il controllo sulla soluzione migliore
		if (bestCandidateFitness < bestFitness) {
			bestSolution->copyFrom(*bestCandidate);
			bestFitness = bestCandidateFitness;
			//cout << "Iterazione " << itera + 1 << "\tFitness: " << bestCandidateFitness << "\t---> New best!\n";
		}
		else if ((itera + 1) % 100 == 0) {
			//cout << "Iterazione " << itera + 1 << "\tFitness: " << bestFitness << "\t\n";
		}
		
		itera++;
		M.setStop(bestFitness, clock);
	}

	// Stampa la matrice del Sudoku
	//cout << "\nSoluzione finale del Sudoku:\nFitness = " << bestFitness << " dopo " << itera << " iterazioni\n";
	//bestSolution->print();

	// Dealloca la memoria
	for (int i = 0; i < m; ++i) {
		delete neighbors[i];
		delete indexes[i];
	}
	delete bestCandidate;
	delete sudoku;
	
	return bestSolution;
}
