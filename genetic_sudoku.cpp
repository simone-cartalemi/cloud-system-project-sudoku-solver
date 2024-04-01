#include "config.h"
#include "matrix_population.cpp"

#include <iostream>
#include <climits>
#include <string>
#include <ctime>

using namespace std;


Matrix* geneticSearch(Monitor &M, const string file_csv, float crossoverRate, float mutationRate, int iterations = 1000, int maxPopulation = 100) {
	// Matrice del Sudoku, elementi fissi e soluzione
	Matrix* sudoku = readSudokuFromFile(file_csv);
	if (!sudoku) {
		return NULL;
	}

	// Pool della popolazione
	MatrixPopulation population = MatrixPopulation(maxPopulation, sudoku);
	Matrix* bestSolution = new Matrix(*population.getElement(0));
	int bestFitness = bestSolution->getFitness();
	Matrix* bestCandidate = new Matrix(*bestSolution);
	
	// Parametri
	srand(time(0));
	
	int itera = 0;
	while(bestFitness > 0 && itera < iterations) {
		auto clock = M.setStart();
		// Generazione nuova popolazione
		population.newGeneration(crossoverRate, mutationRate);

		// Valutazione della popolazione (prendi il primo perché popolazione ordinata)
		bestCandidate->copyFrom(*population.getElement(0));
		int bestCandidateFitness = bestCandidate->getFitness();

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
	delete bestCandidate;
	delete sudoku;

	return bestSolution;
}

