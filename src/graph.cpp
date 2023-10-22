#include "graph.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;


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
  auto ns = &neighbors_map[node];
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

vector<string> Digraph::children(string node) {
  auto cs = &children_map[node];
  return vector<string>(cs->begin(), cs->end());
};

vector<string> Digraph::parents(string node) {
  auto ps = &parents_map[node];
  return vector<string>(ps->begin(), ps->end());
};
