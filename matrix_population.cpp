#include "config.h"

#include <list>

using namespace std;


/**
 * Questa classe è una coda di supporto che tiene traccia delle matrici
*/
class MatrixPopulation {
	private:
		int size;
		Matrix** population;
        int* fitnesses;

		/**
		 * Implementazione a mano dell'ordinamento
		*/
		int partition(int low, int high) {
			int pivot = fitnesses[high];
			int i = low - 1;
			for (int j = low; j <= high - 1; j++) {
				if (fitnesses[j] < pivot) {
					++i;
					swap(fitnesses[i], fitnesses[j]);
					swap(population[i], population[j]);
				}
			}
			swap(fitnesses[i + 1], fitnesses[high]);
			swap(population[i + 1], population[high]);
			return (i + 1);
		}
		void quickSort(int low, int high) {
			if (low < high) {
				int pi = partition(low, high);
				quickSort(low, pi - 1);
				quickSort(pi + 1, high);
			}
		}
		/**
		 * Funzione di scelta dei due genitori
		 * Scegli i due genitori in base alla loro fitness: più è bassa, maggiori sono le probabilità di essere scelti
		*/
		void setIndexParents(int &indexA, int &indexB) {
			float totalFitnesses = 0;
			float fitnessScores[size];
			for (int i = 0; i < size; ++i) {
				int fitness = fitnesses[i];
				fitnessScores[i] = 1 / (float) fitness;
				totalFitnesses += fitnessScores[i];
			}
			// Ricerca gli indici casuali
			float a = (((float) rand()) / (float) RAND_MAX) * totalFitnesses;
			float b = (((float) rand()) / (float) RAND_MAX) * totalFitnesses;
			float cumulate = 0;
			bool foundA = false;
			bool foundB = false;
			int i = 0;
			while (!(foundA && foundB) && (i < size)) {
				if (!foundA && cumulate + fitnessScores[i] >= a) {
					indexA = i;
					foundA = true;
				}
				if (!foundB && cumulate + fitnessScores[i] >= b) {
					indexB = i;
					foundB = true;
				}
				cumulate += fitnessScores[i++];
			}
		}
		void crossover(Matrix &parentA, Matrix &parentB, Matrix &childA, Matrix &childB) {
			int** cildAMatrix = childA.getMatrix();
			int** cildBMatrix = childB.getMatrix();
			int** parentAMatrix = parentA.getMatrix();
			int** parentBMatrix = parentB.getMatrix();
			
			for (int i = 0; i < N_SUB; ++i) {
				for (int j = 0; j < N_SUB; ++j) {
					int crossing = rand() % 2;
					for (int r = i * N_SUB; r < (i + 1) * N_SUB; ++r) {
						for (int c = j * N_SUB; c < (j + 1) * N_SUB; ++c) {
							if (crossing) {
								cildAMatrix[r][c] = parentAMatrix[r][c];
								cildBMatrix[r][c] = parentBMatrix[r][c];
							}
							else {
								cildAMatrix[r][c] = parentBMatrix[r][c];
								cildBMatrix[r][c] = parentAMatrix[r][c];
							}
						}
					}
				}
			}
		}
		void applyMutation(Matrix &matrix, const float mutationRate) {
			if ( (rand() % 100) <= (int)(mutationRate * 100) ) {
				list<int*>** indexes = matrix.getIndexes();
				shuffleIndexedElements(indexes, N);
				delete[] indexes;
			}
		}

	public:
		MatrixPopulation(const int maxPopulation, const Matrix* shape) {
			size = maxPopulation;
			population = new Matrix*[size * 2];
			fitnesses = new int[size * 2];
			for (int i = 0; i < size * 2; ++i) {
				population[i] = randomInitialization(shape);
				fitnesses[i] = population[i]->getFitness();
			}
			sortPopulation();
		}
		~MatrixPopulation() {
			for (int i = 0; i < size * 2; ++i) {
				delete population[i];
			}
			delete[] population;
            delete[] fitnesses;
		}
		Matrix* getElement(const int i) {
			return population[i];
		}
		
		void sortPopulation() {
			quickSort(0, size * 2 - 1);
		}
		void newGeneration(const float crossoverRate, const float mutationRate) {
			for (int i = size; i < size * 2; i += 2) {
				int indexParentA, indexParentB;
				setIndexParents(indexParentA, indexParentB);
				population[i]->copyFrom(*population[indexParentA]);
				population[i + 1]->copyFrom(*population[indexParentB]);
				
				if ( (rand() % 100) <= (int)(crossoverRate * 100) ) {
					crossover(*population[indexParentA], *population[indexParentB], *population[i], *population[i + 1]);
				}
				// Applica la mutazione
				applyMutation(*population[i], mutationRate);
				applyMutation(*population[i + 1], mutationRate);
				// Calcola nuove fitness
				fitnesses[i] = population[i]->getFitness();
				fitnesses[i + 1] = population[i + 1]->getFitness();
			}
			sortPopulation();
		}
		
};
