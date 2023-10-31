#include "graph.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

Graph::Graph(unordered_set<string> nodes,
             unordered_map<string, unordered_set<string>> neighbors_map)
	: nodes(nodes), neighbors_map(neighbors_map){};

void Graph::add_edge(string x, string y) {
	neighbors_map[x].insert(y);
	neighbors_map[y].insert(x);
}

void Graph::remove_edge(string x, string y) {
	neighbors_map[x].erase(x);
	neighbors_map[y].erase(y);
}

bool Graph::has_edge(string x, string y) {
	return neighbors_map[x].count(y) != 0;
}

vector<string> Graph::neighbors(string node) {
	unordered_set<string> *ns = &neighbors_map[node];
	return vector<string>(ns->begin(), ns->end());
}

void Digraph::add_edge(string x, string y) {
	children_map[x].insert(y);
	parents_map[y].insert(x);
};

void Digraph::remove_edge(string x, string y) {
	children_map[x].insert(y);
	parents_map[y].insert(x);
};

bool Digraph::has_edge(string x, string y) {
	return children_map[x].count(y) != 0;
};

void Digraph::isolate(std::string x) {
	for (const string &parent : parents_map[x]) {
		children_map[parent].erase(x);
	}

	for (const string &child : children_map[x]) { parents_map[child].erase(x); }

	parents_map[x].clear();
	children_map[x].clear();
};

vector<string> Digraph::children(string node) {
	unordered_set<string> *cs = &children_map[node];
	return vector<string>(cs->begin(), cs->end());
};

vector<string> Digraph::parents(string node) {
	unordered_set<string> *ps = &parents_map[node];
	return vector<string>(ps->begin(), ps->end());
};

unordered_set<string> Digraph::ancestors(vector<string> nodes) {
	unordered_set<string> result;
	for (auto &node : nodes) { result.insert(node); }

	vector<string> to_check = move(nodes);

	if (to_check.size() > result.size()) {
		// Duplicates in input vector
		to_check.clear();
		to_check.reserve(result.size());

		for (auto &node : result) { to_check.push_back(node); }
	}

	while (!to_check.empty()) {
		string node = to_check.back();
		to_check.pop_back();

		for (auto &parent : parents_map[node]) {
			bool inserted = result.insert(parent).second;

			if (inserted) { to_check.push_back(parent); }
		}
	}

	return result;
};

Graph Digraph::unordered() {
	auto neighbors_map = children_map;

	for (auto &[node, children] : children_map) {
		for (const string &child : children) {
			neighbors_map[child].insert(node);
		}
	}

	return Graph(nodes, neighbors_map);
}

bool Digraph::d_separated(const string x, const string y,
                          const vector<string> zs) {
	vector<string> nodes = zs;
	nodes.push_back(x);
	nodes.push_back(y);

	auto all_ancestors = ancestors(nodes);

	for (auto &node : nodes) {
		if (all_ancestors.count(node) == 0) { isolate(node); }
	}

	Digraph moralized = *this;
	moralized.moralize();

	for (const string &z : zs) { moralized.isolate(z); }

	return !moralized.connected(x, y);
}

void Digraph::moralize() {
	for (auto &z : nodes) {
		auto z_parents = parents(z);

		for (size_t i = 0; i < z_parents.size(); ++i) {
			auto &x = z_parents[i];

			for (size_t j = i + 1; i < z_parents.size(); ++i) {
				auto &y = z_parents[j];

				if (!has_edge(x, y) || has_edge(y, x)) { add_edge(x, y); }
			}
		}
	}
}
