#include "config.h"

using namespace std;


/**
 * Questa classe è una coda di supporto che tiene traccia delle matrici
*/
class MatrixBuffer {
	private:
		int start;
		int end;
		int size;
		int pushedElements;
		Matrix** list;
		
	public:
		MatrixBuffer(const int tabuSize, const Matrix &shape) {
			start = 0;
			end = 0;
			pushedElements = 0;
			size = tabuSize;
			list = new Matrix*[size];
			for (int i = 0; i < size; ++i) {
				list[i] = new Matrix(shape);
			}
		}
		~MatrixBuffer() {
			for (int i = 0; i < size; ++i) {
				delete list[i];
			}
			delete[] list;
		}
		/**
		 * Inserimento fittizio: i valori della matrice vengono copiati su quella del buffer
		*/
		void insert(Matrix &m) {
			if (pushedElements == size) {
				--pushedElements;
				start = (start + 1) % size;
			}
			if (!isInList(m)) {
				list[end]->copyFrom(m);
				++pushedElements;
				end = (end + 1) % size;
			}
		}
		bool isInList(Matrix &target) {
			for (int i = start; i != end; i = (i + 1) % size) {
				if (list[i]->isEqual(target)) {
					return true;
				}
			}
			return false;
		}
		
};
