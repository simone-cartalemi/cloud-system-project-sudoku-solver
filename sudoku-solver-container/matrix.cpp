#include "config.h"

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;


class Matrix{
	private:
		int** matrix;
		bool** places;
		list<int*>* indexes[N];
		
		void initializeIndexesList() {
			for (int i = 0; i < N; ++i) {
				indexes[i] = new list<int*>;
			}
		}
		template <typename T, typename S>
		void fillMatrix(T _matr, S _plac) {
			for (int i = 0; i < N; ++i) {
				matrix[i] = new int[N];
				places[i] = new bool[N];
				for (int j = 0; j < N; ++j) {
					matrix[i][j] = _matr[i][j];
					places[i][j] = _plac[i][j];
					if (places[i][j]) {
						indexes[(i / N_SUB) * N_SUB + (j / N_SUB)]->push_back(&matrix[i][j]);
					}
				}
			}
		}

	public:
		Matrix(const int _matr[][N], const bool _plac[][N]) {
			matrix = new int*[N];
			places = new bool*[N];
			initializeIndexesList();
			fillMatrix(_matr, _plac);
		}
		Matrix(const Matrix& father) {	// Costruttore di copia
			matrix = new int*[N];
			places = new bool*[N];
			initializeIndexesList();
			fillMatrix(father.matrix, father.places);
		}
		~Matrix() {
			for (int i = 0; i < N; ++i) {
				delete[] matrix[i];
				delete[] places[i];
				delete indexes[i];
			}
			delete[] matrix;
			delete[] places;
		}
		int** getMatrix() {
			return matrix;
		}
		bool** getPlaces() {
			return places;
		}
		list<int*>** getIndexes() {
			list<int*>** listCopy = new list<int*>*[N];
			for (int i = 0; i < N; ++i) {
				listCopy[i] = indexes[i];
			}
			return listCopy;
		}
		int getFitness() {
			int fitness_amount = 0;
			// Per ogni numero da 1 a N controlla
			for (int n = 1; n <= N; ++n) {
				// Le righe e le colonne
				for (int i = 0; i < N; ++i) {
					int nInRow = count(matrix[i], matrix[i] + N, n);
					if (nInRow > 1) {
						fitness_amount++;
					}
					int nInColumn = 0;
					for (int j = 0; j < N; ++j) {
						if (matrix[j][i] == n) {
							nInColumn += 1;
						}
					}
					if (nInColumn > 1) {
						fitness_amount++;
					}
				}
				// Le N sottomatrici
				for (int i = 0; i < N_SUB; ++i) {
					for (int j = 0; j < N_SUB; ++j) {
						int nInSubmatrix = 0;
						for (int x = i * N_SUB; x < N_SUB * (i + 1); ++x) {
							for (int y = j * N_SUB; y < N_SUB * (j + 1); ++y) {
								if (matrix[x][y] == n) {
									nInSubmatrix++;
								}
							}
						}
						if (nInSubmatrix > 1) {
							fitness_amount++;
						}
					}
				}
			}
			return fitness_amount;
		}
		bool isEqual(const Matrix& other) const {
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					if (matrix[i][j] != other.matrix[i][j]) {
						return false;
					}
				}
			}
			return true;
		}
		void copyFrom(const Matrix& other) {
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					matrix[i][j] = other.matrix[i][j];
				}
			}
		}
		
		void print() {
			cout << "Matrice del Sudoku:\n";
			for (int i = 0; i < N; ++i) {
				cout << " ";
				for (int j = 0; j < N; ++j) {
					cout << matrix[i][j] << ' ';
					if (j == 2 || j == 5) {
						cout << "| ";
					}
				}
				if (i == 2 || i == 5) {
					cout << "\n ------+-------+------";
				}
				cout << endl;
			}
		}
};
