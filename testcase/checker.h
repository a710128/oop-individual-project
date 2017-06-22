#pragma once
#include <vector>
bool checkConn(const std::vector<std::vector<int> >&in, const std::vector<std::vector<int> >& ans);

std::vector<std::vector<int> > generator(int row, int col, int num_obs, int num_set);

int random(int hi);
int random(int lo, int hi);