#ifndef BN_QF20WQ_GRAPH_HPP_
#define BN_QF20WQ_GRAPH_HPP_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph {
	friend class Digraph;

	std::unordered_set<std::string> nodes;
	std::unordered_map<std::string, std::unordered_set<std::string>>
		neighbors_map;

	Graph(std::unordered_set<std::string> nodes,
	      std::unordered_map<std::string, std::unordered_set<std::string>>
	          neighbors_map);

public:
	void add_edge(std::string x, std::string y);
	void remove_edge(std::string x, std::string y);
	bool has_edge(std::string x, std::string y);

	std::vector<std::string> neighbors(std::string node);
};

class Digraph {
	std::unordered_set<std::string> nodes;
	std::unordered_map<std::string, std::unordered_set<std::string>>
		children_map;
	std::unordered_map<std::string, std::unordered_set<std::string>>
		parents_map;

public:
	void add_edge(std::string x, std::string y);
	void remove_edge(std::string x, std::string y);
	bool has_edge(std::string x, std::string y);
	void isolate(std::string x);
	bool connected(std::string x, std::string y);

	std::vector<std::string> children(std::string node);
	std::vector<std::string> parents(std::string node);
	std::unordered_set<std::string> ancestors(std::vector<std::string> node);

	Graph unordered();

	bool d_separated(std::string x, std::string y, std::vector<std::string> zs);

	void moralize();
};

#endif
