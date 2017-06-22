#pragma once
#include <map>
#include <vector>
#include <set>
#include <gurobi_c++.h>

class MTPFSolver {
private:
	GRBEnv* env;
	GRBModel* model;
	int row, col;
	std::vector<std::vector<std::map<int, std::vector<GRBVar> > > > map;
	std::vector<std::vector<int> > items;
	std::set<int> types;
	GRBVar& getVar(int row, int col, int type, int dir);
	std::vector<std::vector<int> > result;
public:
	MTPFSolver(int row, int col);
	~MTPFSolver();
	void setTimeLimit(double timeLimit);
	void setGapLimit(double gap);
	bool addObstacle(int x, int y);
	bool delObstacle(int x, int y);
	bool addTerminal(int x, int y, int type);
	bool delTerminal(int x, int y);
	bool solve();
	void clear();
	const std::vector<std::vector<int> >& getResult();
	const std::vector<std::vector<int> >& getItems();
};
