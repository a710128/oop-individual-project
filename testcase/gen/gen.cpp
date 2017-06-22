#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
using namespace std;
int random(int lo, int hi) {
	return (rand()) % (hi - lo) + lo;
}
int random(int hi) { return random(0, hi); }

vector<vector<int> > mp;

int& find(int id) {
	int ctr = 0;
	for (auto& col : mp)
		for (auto& item : col) {
			if (ctr == id) return item;
			if (item == 0) ++ ctr;
		}
	printf("can't find %d\n", id);
	throw "Error";
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <output_file>" << endl;
		return 1;
	}
	srand(time(0));
	int n = random(5, 20);
	int m = random(5, 20);
	int num_obs = random(n * m / 5);
	int num_set = random(2, 6);
	mp.resize(n);
	for (auto& col: mp) col.resize(m);
	int total = n * m;
	for (int i = 0; i < num_obs; ++ i) {
		int id = random(total);
		find(id) = 1;
		total -= 1;
	}
	for (int i = 0; i < num_set; ++ i) {
		int num_ter = random(3, min(total - (num_set - i - 1) * 3, 10));
		for (int j = 0; j < num_ter; ++ j) {
			int id = random(total);
			find(id) = i + 2;
			total -= 1;
		}
	}

	ofstream fout(argv[1], ios::out);
	fout << n << " " << m << endl;
	for (int i = 0; i < n; ++ i) {
		for (int j = 0; j < m; ++ j)
			fout << mp[i][j] << " ";
		fout << endl;
	}
	fout.close();
	return 0;
}