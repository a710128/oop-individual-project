#include <utils/utils.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <set>
const int MaxTestCase = 10;
char buffer[256];
int n, m;
int main() {
	for (int i = 0; i < MaxTestCase; ++ i) {
		sprintf(buffer, "test%d.in", i);
		FILE* fin = fopen(buffer, "r");
		sprintf(buffer, "test%d.out", i);
		FILE* fout = fopen(buffer, "r");
		
		fscanf(fin, "%d%d", &n, &m);

		std::vector<std::vector<int> > inp, opt;
		inp.resize(n);
		opt.resize(n);
		for (int i = 0; i < n; ++ i) {
			inp[i].resize(m);
			opt[i].resize(m);
		}

		for (int i = 0; i < n; ++ i)
			for (int j = 0; j < m; ++ j) {
				fscanf(fin, "%d", &inp[i][j]);
				if (inp[i][j] == 1) inp[i][j] = -1;
			}
		int grids = 0;
		std::set<int> types;
		for (int i = 0; i < n; ++ i)
			for (int j = 0; j < m; ++ j) {
				fscanf(fout, "%d", &opt[i][j]);
				if (opt[i][j] == 1) opt[i][j] = -1;
				else if (opt[i][j] > 1) {
					grids ++;
					types.insert(opt[i][j]);
				}
			}
		fclose(fin);
		fclose(fout);

		sprintf(buffer, "html_result/result%d.html", i);
		std::ofstream fhtml(buffer, std::ios::out);
		printHtmlResult(fhtml, inp, opt);
		printf("test %d sets, %d grids\n", int(types.size()), grids);
		fhtml.close();
	}
	return 0;
}