#include "config.h"

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;


Matrix* readSudokuFromInput(string matrix) {
	int sudoku[N][N];
	bool places[N][N] = {false};

	stringstream ss(matrix.substr(1, matrix.size() - 2));	// remove '[' and ']' and put it in a stream

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			string number = "0";
			getline(ss, number, ',');
			sudoku[i][j] = stoi(number);
			if (sudoku[i][j] == 0) {
				places[i][j] = true;
			}
		}
	}

	Matrix* out = new Matrix(sudoku, places);
	return out;
}

// Ritorna gli elementi mancanti in una sottomatrice (+ random)
int* getMissingElements(int** matrix, int x_start, int y_start, int x_stop, int y_stop){
	int* missing_element = new int[N];
	int count = 0;

	for (int n = 1; n <= N; ++n) {
		bool found = false;
		for (int x = x_start; x < x_stop; ++x) {
			for (int y = y_start; y < y_stop; ++y) {
				if (matrix[x][y] == n) {
					found = true;
					break;
				}
			}
			if (found) {
				break;
			}
		}
		if (!found) {
			missing_element[count++] = n;
		}
	}
	for (int i = count - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		swap(missing_element[i], missing_element[j]);
	}
	return missing_element;
}

// Fai shuffle di coppie random per alcune liste di puntatori a interi
void shuffleIndexedElements(list<int*>* indexes[], const int len) {
	// Shuffle dei puntatori alle liste
	for (int k = len - 1; k > 0; --k) {
		int j = rand() % (k + 1);
		swap(*indexes[j], *indexes[k]);
	}
	// Sceglie quante liste subiranno uno swap
	int pairs = (rand() % len) + 1;
	// Fai lo swap di una coppia casuale per queste liste
	for (int j = 0; j < pairs; ++j) {
		int size = (*indexes[j]).size();
		if (size < 2) {
			break;
		}
		int a = rand() % size;
		int b = rand() % size;
		while (a == b) {
			b = rand() % size;
		}
		swap(**next(indexes[j]->begin(), a), **next(indexes[j]->begin(), b));
	}
}

// Restituisci una Matrice copia
Matrix* randomInitialization(const Matrix* m) {
	Matrix* new_m = new Matrix(*m);
	int** matrix = new_m->getMatrix();
	bool** places = new_m->getPlaces();
	for (int i = 0; i < N_SUB; ++i) {
		for (int j = 0; j < N_SUB; ++j) {
			int* missingElements = getMissingElements(matrix, i * N_SUB, j * N_SUB, (i + 1) * N_SUB, (j + 1) * N_SUB);
			int count = 0;
			for (int x = i * N_SUB; x < N_SUB * (i + 1); ++x) {
				for (int y = j * N_SUB; y < N_SUB * (j + 1); ++y) {
					if (places[x][y]) {
						matrix[x][y] = missingElements[count++];
					}
				}
			}
			// Dealloca la memoria
			delete[] missingElements;
		}
	}
	
	return new_m;
}
