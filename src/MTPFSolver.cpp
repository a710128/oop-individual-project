#include <MTPFSolver.h>
#include <iostream>
#include <cstdio>
#include <string>

GRBVar& MTPFSolver::getVar(int row, int col, int type, int dir) {
	if (map[row][col].find(type) == map[row][col].end()) {
		char* buffer = new char [256];
		sprintf(buffer, "a%d,%d,%d", type, row, col);
		std::string name = std::string(buffer);
		delete[] buffer;
		std::vector<GRBVar>tmp = std::vector<GRBVar>();
		tmp.push_back(model->addVar(0.0, 1e3, 0.0, GRB_INTEGER, name + "_right_out"));
		tmp.push_back(model->addVar(0.0, 1e3, 0.0, GRB_INTEGER, name + "_down_out"));
		tmp.push_back(model->addVar(0.0, 1e3, 0.0, GRB_INTEGER, name + "_left_out"));
		tmp.push_back(model->addVar(0.0, 1e3, 0.0, GRB_INTEGER, name + "_up_out"));
		tmp.push_back(model->addVar(0.0, 1.0, 0.0, GRB_BINARY, name + "_use"));
		map[row][col][type] = tmp;
	}
	return map[row][col][type][dir];
}


MTPFSolver::MTPFSolver(int row, int col) {
	env = new GRBEnv();
	model = new GRBModel(*env);

	this->row = row;
	this->col = col;

	map.resize(row);
	items.resize(row);
	for (auto& c : map) c.resize(col);
	for (auto& c : items) c.resize(col);
	result.clear();
}

void MTPFSolver::setTimeLimit(double timeLimit) {
	model->set(GRB_DoubleParam_TimeLimit, timeLimit);
}

void MTPFSolver::setGapLimit(double gap) {
	model->set(GRB_DoubleParam_MIPGap, gap);
}

bool MTPFSolver::addObstacle(int x, int y) {
	if (items[x][y] == 0) {
		items[x][y] = -1;
		return true;
	}
	return false;
}

bool MTPFSolver::delObstacle(int x, int y) {
	if (items[x][y] == -1) {
		items[x][y] = 0;
		return true;
	}
	return false;
}

bool MTPFSolver::addTerminal(int x, int y, int type) {
	if (items[x][y] == 0 && type > 0) {
		if (types.find(type) == types.end()) types.insert(type);
		items[x][y] = type;
		return true;
	}
	return false;
}

bool MTPFSolver::delTerminal(int x, int y) {
	if (items[x][y] > 0) {
		items[x][y] = 0;
		return true;
	}
	return false;
}

bool MTPFSolver::solve() {
	if (result.size() != 0) return true;
	try {
		std::vector<std::vector<int> > ret;
		ret.resize(row);
		for (auto& c : ret) { c.resize(col); }

		// add constr part1
		const int dx[] = {0, 1, 0, -1};
		const int dy[] = {1, 0, -1, 0};


		GRBLinExpr ok_terms = GRBLinExpr(0);
		for (auto type : types) {
			int counter = 0;
			auto use_term = model->addVar(0.0, 1.0, 0.0, GRB_BINARY);
			ok_terms += use_term;
			for (int i = 0; i < row; ++ i)
				for (int j = 0; j < col; ++ j)
					if (items[i][j] == type)
						counter ++;
			bool first = true;;
			for (int i = 0; i < row; ++ i) {
				for (int j = 0; j < col; ++ j) {
						GRBLinExpr sum_in = GRBLinExpr(0); 
						for (int dir = 0; dir < 4; ++ dir)
							if (i + dx[dir] >= 0 && j + dy[dir] >= 0 && i + dx[dir] < row && j + dy[dir] < col)
								sum_in += getVar(i + dx[dir], j + dy[dir], type, (dir + 2) % 4);
							else
								model->addConstr(getVar(i, j, type, dir) == 0);

						GRBLinExpr sum_out = GRBLinExpr(0);
						for (int dir = 0; dir < 4; ++ dir)
							sum_out += getVar(i, j, type, dir);
						if (items[i][j] == type) {
							if (first) {
								model->addConstr(sum_in + counter * use_term == sum_out + 1 * use_term);
								first = false;
							}
							else model->addConstr(sum_in == sum_out + 1 * use_term);
						}
						else if (items[i][j] == 0) {
							model->addConstr(sum_in == sum_out);
						}
						else if(items[i][j] != -1) {
							//model->addConstr(getVar(i, j, type, 4) == 0);
							model->addConstr(sum_in == sum_out);
						}
						else {
							model->addConstr(getVar(i, j, type, 4) == 0);
						}

						model->addConstr(sum_in <= getVar(i, j, type, 4) * 1000);
						model->addConstr(sum_out <= getVar(i, j, type, 4) * 1000);
					}
			}
		}

		// add routing grid constr
		for (int i = 0; i < row; ++ i)
			for (int j = 0; j < col; ++ j) {
				GRBLinExpr sum = GRBLinExpr(0);
				for (auto type : types)
					sum += getVar(i, j, type, 4);
				model->addConstr(sum <= 1);
			}

		// set objective
		{
			GRBLinExpr sum = GRBLinExpr(0);
			for (int i = 0; i < row; ++ i)
				for (int j = 0; j < col; ++ j)
					for (auto type : types)
						sum += getVar(i, j, type, 4);
			model->setObjective(sum - ok_terms * (row * col * 10), GRB_MINIMIZE);
		}

		model->optimize();

		for (int i = 0; i < row; ++ i)
			for (int j = 0; j < col; ++ j) {
				if (items[i][j] < 0) ret[i][j] = -1;
				else {
					for (auto type : types) {
						if (getVar(i, j, type, 4).get(GRB_DoubleAttr_X) > 0.5) {
							ret[i][j] = type;
							break;
						}
					}
				}
			}
		result = ret;
		return true;
	} catch(GRBException e) {
		std::cout << "Error code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
		return false;
	}
}

void MTPFSolver::clear() {
	result.clear();
}

const std::vector<std::vector<int> >& MTPFSolver::getResult() {
	if (result.size()) return result;
	if (solve()) return result;
	return result;
}

const std::vector<std::vector<int> >& MTPFSolver::getItems() {
	return items;
}

MTPFSolver::~MTPFSolver() {
	delete model;
	delete env;
}