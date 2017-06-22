#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <checker.h>
#include <sys/time.h>

const int TestCaseNum = 10;
const int N[TestCaseNum] = {6, 7, 8, 9, 10, 10, 11, 11, 12, 12};
const int M[TestCaseNum] = {6, 7, 8, 9, 10, 10, 11, 11, 12, 12};
const int OBS[TestCaseNum] = {5, 8, 10, 13, 20, 25, 30, 30, 34, 35};
const int SET[TestCaseNum] = {3, 4, 3, 4, 3, 4, 5, 6, 4, 5};
const int SEEDS[TestCaseNum] = {1, -2, 3, -4, 5, -6, 7, -8, 9, 23333};

long getCurrentTime()    
{    
   struct timeval tv;    
   gettimeofday(&tv, NULL);    
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
}    

int main (int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " test_path" << std::endl;
		return 1;
	}
	char *buffer = new char [256];

	for (int i = 0; i < TestCaseNum; ++ i) {
		int n = N[i], m = M[i], num_obs = OBS[i], num_set = SET[i];
		srand(SEEDS[i]);

		auto mp = generator(n, m, num_obs, num_set);

		sprintf(buffer, "test%d.in", i);
		std::ofstream fout(buffer, std::ios::out);

		fout << n << " " << m << std::endl;
		for (int i = 0; i < n; ++ i) {
			for (int j = 0; j < m; ++ j)
				fout << mp[i][j] << " ";
			fout << std::endl;
		}
		fout.close();

		sprintf(buffer, "%s test%d.in test%d.out > /dev/null", argv[1], i, i);
		std::cout << "====== Running Test " << i << " ======" << std::endl;
		auto st = getCurrentTime();
		system(buffer);
		auto tm_cost = double(getCurrentTime() - st) / 1000.;
		sprintf(buffer, "test%d.out", i);
		std::ifstream fin(buffer, std::ios::in);
		std::vector<std::vector<int> > ans;
		ans.resize(n);
		for (int i = 0; i < n; ++ i) {
			ans[i].resize(m);
			for (int j = 0; j < m; ++ j)
				fin >> ans[i][j];
		}
		fin.close();
		if (checkConn(mp, ans)) std::cout << "Ok, time: " <<  tm_cost * 1000 << "ms" << std::endl;
		else std::cout << "Wrong answer!!!!!!! time: " <<  tm_cost * 1000 << "ms" << std::endl;
	}
	delete[] buffer;
	return 0;
}