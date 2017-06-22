#include "utils.h"
#include <vector>
#include <queue>
#include <map>
void printHtmlHeader(std::ofstream& fout) {
	fout << "<html><head><title>Result</title><link rel='stylesheet' type='text/css' href='default.css'/></head><body>";
}


inline int getId(int x, int y) { return x * 10000 + y + 1; }

void printHtmlResult(std::ofstream& fout, const std::vector<std::vector<int> >& item, const std::vector<std::vector<int> >& res) {
	printHtmlHeader(fout);
	fout << "<table>";
	std::map<int, int> type;

	std::vector<std::vector<int> > fa;
	fa.resize(res.size());
	for (auto& c: fa) c.resize(res[0].size());

	for (int i = 0; i < fa.size(); ++ i)
		for (int j = 0; j < fa[i].size(); ++ j)
			if (res[i][j] > 0 && fa[i][j] == 0) {
				std::queue<std::pair<int, int> > q;
				q.push(std::make_pair(i, j));
				fa[i][j] = -1;
				while (!q.empty()) {
					int x = q.front().first, y = q.front().second;
					q.pop();

					if (x > 0 && res[x - 1][y] == res[x][y] && fa[x - 1][y] == 0) {
						q.push(std::make_pair(x - 1, y));
						fa[x - 1][y] = getId(x, y);
					}
					if (x + 1 < int(res.size()) && res[x + 1][y] == res[x][y] && fa[x + 1][y] == 0) {
						q.push(std::make_pair(x + 1, y));
						fa[x + 1][y] = getId(x, y);
					}
					if (y > 0 && res[x][y - 1] == res[x][y] && fa[x][y - 1] == 0) {
						q.push(std::make_pair(x, y - 1));
						fa[x][y - 1] = getId(x, y);;
					}
					if (y + 1 < int(res[x].size()) && res[x][y + 1] == res[x][y] && fa[x][y + 1] == 0) {
						q.push(std::make_pair(x, y + 1));
						fa[x][y + 1] = getId(x, y);;
					}
				}
			}
	for (int i = 0; i < int(res.size()); ++ i) {
		fout << "<tr>";
		for (int j = 0; j < int(res[i].size()); ++ j) {
			if (item[i][j] != 0) {
				if (item[i][j] == -1) fout << "<td class='block'>";
				else {
					if(type.find(item[i][j]) == type.end()) {
						type[item[i][j]] = type.size() + 1;
					}
					fout << "<td class='type" << type[item[i][j]] << "'>";
				}

			}
			else fout << "<td>";

			if (res[i][j] > 0) {
				fout << "<div class='road_";
				if (i > 0 && (fa[i - 1][j] == getId(i, j) || fa[i][j] == getId(i - 1, j)))
					fout << "u";
				if (i + 1 < int(res.size()) && (fa[i + 1][j] == getId(i, j) || fa[i][j] == getId(i + 1, j)))
					fout << "d";
				if (j > 0 && (fa[i][j - 1] == getId(i, j) || fa[i][j] == getId(i, j - 1)))
					fout << "l";
				if (j + 1 < int(res[i].size()) && (fa[i][j + 1] == getId(i, j) || fa[i][j] == getId(i, j + 1)))
					fout << "r";
				fout << "'></div>";
			}
			fout << "</td>";
		}
		fout << "</tr>";
	}

	fout << "</table></body></html>";
}

void printPlainResult(std::ofstream& fout, const std::vector<std::vector<int> >& res) {
	for (int i = 0; i < int(res.size()); ++ i) {
		for (int j = 0; j < int(res[i].size()); ++ j) {
			if (res[i][j] == -1) fout << 1;
			else fout << res[i][j];
			fout << " ";
		}
		fout << std::endl;
	}
}