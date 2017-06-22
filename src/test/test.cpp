#include <MTPFSolver.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "utils/utils.h"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
		return 1;
	}
	std::ifstream fin(argv[1], std::ios::in);
	int row, col;
	fin >> row >> col;

	MTPFSolver solver = MTPFSolver(row, col);

	solver.setTimeLimit(1.0);
	
	const int Obstacle = 1;
	const int Empty = 0;

	for (int i = 0; i < row; ++ i)
		for (int j = 0; j < col; ++ j) {
			int val;
			fin >> val;
			if (val == Obstacle) solver.addObstacle(i, j);
			else if(val == Empty) ;
			else {
				solver.addTerminal(i, j, val);
			}
		}

	if (solver.solve()) {
		std::ofstream fout(argv[2], std::ios::out);
		if (strstr(argv[2], ".html") == NULL) printPlainResult(fout, solver.getResult());
		else printHtmlResult(fout, solver.getItems(), solver.getResult());
		fout.close();
	}
	return 0;
}