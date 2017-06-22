#pragma once
#include <fstream>
#include <vector>

void printPlainResult(std::ofstream& fout, const std::vector<std::vector<int> >& opt);
void printHtmlResult(std::ofstream& fout, const std::vector<std::vector<int> >& inp, const std::vector<std::vector<int> >& opt);