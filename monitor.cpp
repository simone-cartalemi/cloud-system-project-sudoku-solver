#include "config.h"

#include <climits>
#include <vector>
#include <numeric>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

/**
 * Il formato di stampa finale è:
 * {
 * 		"name": "nomeIstanza.csv",
 * 		"algorithm": "tabu"/"gen",
 * 		"class": "easy"/"medium"/"hard"/"extreme",
 * 		"attempts": [
 * 			{
 * 				"total_iterations": # iterazioni totali per trovare la soluzione,
 * 				"total_time": microsecondi di esecuzione totale per trovare la soluzione,
 * 				"final_fitness": valore di fitness dopo l'esecuzione (se diversa da 0 non sono arrivato alla soluzione),
 * 				"fitnesses_for_iteration": [
 * 					valore di fitness all'iterazione 1,
 * 					...
 * 				],
 * 				"times_for_iteration": [
 * 					tempo di esecuzione all'iterazione 1,
 * 					...
 * 				],
 * 			},
 * 		...
 * 		]
 * }
*/
class Monitor {
	private:
		string istanceName;
		string className;
		string algorithm;
		int totalAttempts;
		int totalTime;
		int round;
		vector<int>** timeEachItera;
		vector<int>** fitnessTrend;
	
	public:
		Monitor(const int attempts = 1) {
			totalAttempts = attempts;
			timeEachItera = new vector<int>*[attempts];
			fitnessTrend = new vector<int>*[attempts];
			for (int i = 0; i < attempts; i++) {
				timeEachItera[i] = new vector<int>;
				fitnessTrend[i] = new vector<int>;
			}
		}
		~Monitor() {
			for (int i = 0; i < totalAttempts; i++) {
				delete timeEachItera[i];
				delete fitnessTrend[i];
			}
			delete[] timeEachItera;
			delete[] fitnessTrend;
		}

		/**
		 * Ad ogni istanza inizializza la classe
		*/
		void initialize(string _istanceName = "", string _className = "", string _algorithm = "") {
			istanceName = _istanceName;
			className = _className;
			algorithm = _algorithm;
			totalTime = 0;
			round = 0;
			for (int i = 0; i < totalAttempts; ++i) {
				timeEachItera[i]->clear();
				fitnessTrend[i]->clear();
			}
		}
		void setRound(const int attempt) {
			round = attempt;
		}
		auto setStart() {
			// Fa partire il cronometro all'inizio dell'iterazione
			return high_resolution_clock::now();
		}
		void setStop(const int fitness, const auto start) {
			// Ferma il cronometro alla fine dell'iterazione
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			int totalForIteration = duration.count();

			timeEachItera[round]->push_back(totalForIteration);
			totalTime += totalForIteration;
			fitnessTrend[round]->push_back(fitness);
		}
		string resultsJSON() {
			string out = "{\"name\":\"" + istanceName + "\",";
			out += "\"algorithm\":\"" + algorithm + "\",";
			out += "\"class\":\"" + className + "\",";
			out += "\"attempts\":[";

			for (int i = 0; i < totalAttempts; ++i) {
				int totalIterations = fitnessTrend[i]->size();
				out += "{\"total_iterations\":" + to_string(totalIterations) + ",";
				out += "\"total_time\":" + to_string(accumulate((*timeEachItera[i]).begin(), (*timeEachItera[i]).end(),0)) + ",";
				out += "\"final_fitness\":" + to_string((*fitnessTrend[i])[totalIterations - 1]) + ",";

				string fitnesses = "";
				string times = "";
				for (int j = 0; j < totalIterations; ++j) {
					fitnesses += to_string((*fitnessTrend[i])[j]) + ",";
					times += to_string((*timeEachItera[i])[j]) + ",";
				}
				fitnesses.pop_back();
				times.pop_back();

				out += "\"fitnesses_for_iteration\":[" + fitnesses + "],";
				out += "\"times_for_iteration\":[" + times + "]},";
			}
			out.pop_back();

			return out + "]}";
		}
};
