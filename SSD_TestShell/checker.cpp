#include "checker.h"

bool OutputChecker::outputChecker(const string& data) {
		std::ifstream ssdOutput("ssd_output.txt");
		string outputData;

		if (ssdOutput.is_open()) {
			if (!std::getline(ssdOutput, outputData)) return false;
		}
		ssdOutput.close();

		if (outputData != data) return false;
		return true;
}