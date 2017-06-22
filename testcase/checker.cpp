#include <checker.h>
#include <map>
#include <queue>
#include <set>
#include <cstdlib>
bool checkConn(const std::vector<std::vector<int> >& inp, const std::vector<std::vector<int> >& mp){
	std::map<int, int> count;
	int row = mp.size();
	int col = row ? mp[0].size() : 0;
	for (int i = 0; i < row; ++ i)
		for (int j = 0; j < col; ++ j)
			if (inp[i][j] > 1) {
				if (count.find(inp[i][j]) == count.end()) count[inp[i][j]] = 0;
				count[inp[i][j]] += 1;
			}
	for (auto item : count) {
		int color = item.first;
		int sl = item.second;
		int sx = -1, sy = -1, ctr = 0;
		for (int i = 0; i < row; ++ i)
			for (int j = 0; j < col; ++ j)
				if (mp[i][j] == color) {
					sx = i;
					sy = j;
				}
		if (sx == -1) continue; // removed this terminal
		std::queue<std::pair<int, int> > q;
		q.push(std::make_pair(sx, sy));
		std::set<std::pair<int, int> > vis;
		vis.insert(q.front());
		const int dx[] = {0, 1, 0, -1};
		const int dy[] = {1, 0, -1, 0};
		int size = 0;

		while (!q.empty()) {
			++ size;
			int x = q.front().first;
			int y = q.front().second;
			if (inp[x][y] == color) ++ ctr;
			q.pop();
			for (int dir = 0; dir < 4; ++ dir)
				if (x + dx[dir] >= 0 && x + dx[dir] < row && y + dy[dir] >= 0 && y + dy[dir] < col) {
					if (mp[x + dx[dir]][y + dy[dir]] != color) continue;
					auto nxt = std::make_pair(x + dx[dir], y + dy[dir]);
					if (vis.find(nxt) != vis.end()) continue;
					vis.insert(nxt);
					q.push(nxt);
				}
		}
		if (size > 1 && ctr != sl) { return false; }
	}
	return true;
}

int random(int lo, int hi) {
	return (rand()) % (hi - lo) + lo;
}
int random(int hi) { return random(0, hi); }

std::vector<std::vector<int> > mp;

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

std::vector<std::vector<int> > generator(int n, int m, int num_obs, int num_set) {
	mp.clear();
	mp.resize(n);
	for (auto& col: mp) col.resize(m);
	int total = n * m;
	for (int i = 0; i < num_obs; ++ i) {
		int id = random(total);
		find(id) = 1;
		total -= 1;
	}
	for (int i = 0; i < num_set; ++ i) {
		int num_ter = random(3, std::min(total - (num_set - i - 1) * 3, 10));
		for (int j = 0; j < num_ter; ++ j) {
			int id = random(total);
			find(id) = i + 2;
			total -= 1;
		}
	}
	return mp;
}