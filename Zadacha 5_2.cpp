#include <iostream>
#include <fstream>
#include <algorithm> 
#include <vector>
#include <map>
#include <stack>
using namespace std;


void dfs(int& v, vector<vector<int> >& graph, vector<int>& parent, int& timer, vector<bool>& used, vector<int>& tin, vector<int>& tout, stack<int>& nodes_for_search_bridges) {
	//обход вниз без рекурсии, но при этом не обновится tout по цепочке, нужен будет еще обход вверх для обновления tout по цепочке
	stack<pair<int, int> > stack_dfs;
	stack<pair<int, int> > dfs_up;
	stack_dfs.push(make_pair(v, 0));
	used[v] = true;
	while (!stack_dfs.empty()) {
		pair<int, int> edge = stack_dfs.top();
		stack_dfs.pop();
		v = edge.first;
		int i = edge.second;
		if (i == 0) {
			tin[v] = tout[v] = timer++;
			nodes_for_search_bridges.push(v);
		}
		if (i == graph[v].size()) {
			continue;
		}
		int to = graph[v][i];
		stack_dfs.push(make_pair(v, i + 1));
		if (used[to] and (to != parent[v])) {
			tout[v] = min(tout[v], tin[to]);
		}
		else if ((!used[to]) and (to != parent[v])) {
			parent[to] = v;
			dfs_up.push(make_pair(v, to));
			stack_dfs.push(make_pair(to, 0));
			used[to] = true;
		}
	}

	//подъем ввех и обновление tout
	while (!dfs_up.empty()) {
		pair<int, int> zz = dfs_up.top();
		dfs_up.pop();
		v = zz.first;
		int to = zz.second;
		tout[v] = min(tout[v], tout[to]);
	}

}

void search_bridges(stack<int>& nodes_for_search_bridges, vector<pair<int, int> >& bridges, vector<int>& parent, vector<int>& tin, vector<int>& tout) {
	while (nodes_for_search_bridges.size() > 1) {
		int to = nodes_for_search_bridges.top();
		nodes_for_search_bridges.pop();
		int v = parent[to];
		if (tout[to] > tin[v]) {
			if (v < to) {
				bridges.push_back(make_pair(v, to));
			}
			else {
				bridges.push_back(make_pair(to, v));
			}
		}
	}
	nodes_for_search_bridges.pop();
}

int main() {
	ifstream input("bridges.in");
	int n = 0;
	input >> n;
	int m;
	input >> m;
	vector<vector<int> > graph(n, vector<int>());
	map<pair<int, int>, int> edges;
	for (int i = 0; i < m; i++) {
		int a = 0;
		int b = 0;
		input >> a >> b;
		graph[a - 1].push_back(b - 1);
		graph[b - 1].push_back(a - 1);
		if (a < b) {
			edges[make_pair(a - 1, b - 1)] += (i + 1);
			if (edges[make_pair(a - 1, b - 1)] != (i + 1)) {
				edges[make_pair(a - 1, b - 1)] = -i - 1;
			}
		}
		else {
			edges[make_pair(b - 1, a - 1)] += (i + 1);
			if (edges[make_pair(b - 1, a - 1)] != (i + 1)) {
				edges[make_pair(b - 1, a - 1)] = -i - 1;
			}
		}
	}
	input.close();
	int timer = 0;
	vector<int> parent(n, -1);
	vector<int> tin(n);
	vector<int> tout(n);
	vector<pair<int, int> > bridges;
	vector<bool> used(n, false);
	stack<int> nodes_for_search_bridges;
	for (int i = 0; i < n; ++i) {
		if (!used[i])
		{
			dfs(i, graph, parent, timer, used, tin, tout, nodes_for_search_bridges);
			//поиск мостов
			search_bridges(nodes_for_search_bridges, bridges, parent, tin, tout);
		}
	}

	vector<int> rezult;
	for (int i = 0; i < bridges.size(); i++) {
		if (edges[bridges[i]] >= 0) {
			rezult.push_back(edges[bridges[i]]);
		}
	}
	ofstream output("bridges.out");
	output << rezult.size() << endl;
	sort(rezult.begin(), rezult.end());
	for (int i = 0; i < rezult.size(); i++) {
		output << rezult[i] << " ";
	}
	output.close();
	return 0;
}
