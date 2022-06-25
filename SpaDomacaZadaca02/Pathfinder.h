#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <queue>

//for Dijkstra's Algorithm
#include <iostream>
#include <list>
#include <limits>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>

using namespace std;

typedef int vertex_t;
typedef double weight_t;

const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor {
	vertex_t target;
	weight_t weight;
	neighbor(vertex_t arg_target, weight_t arg_weight)
		: target(arg_target), weight(arg_weight) { }
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;

//player


class Pathfinder
{
public:
	//
	Pathfinder(sf::RenderWindow* window, int screen_width, int screen_height, 
		char flag, sf::String playerInput);
	void draw();
	//
private:
	// Game
	void draw_environment();
	void draw_wall(sf::Vector2f w, int color);
	void draw_floor(sf::Vector2f w);
	void draw_start(sf::Vector2f w);
	void draw_finish(sf::Vector2f w);
	void draw_player(sf::Vector2f w);
	void draw_row(vector<int> row, sf::Vector2f w);
	void open_world(string world_name);
	sf::RenderWindow* window;
	sf::Clock clock;
	float speed = 0.1f;
	int screen_width;
	int screen_height;
	const float life_size = 20.f;
	vector<vector<int>> world;

	//player
	struct player
	{
		int x, y;
	};
	queue<player> player_location;

	// for Dijkstra's Algorithm
	void DijkstraComputePaths(vertex_t source, const adjacency_list_t& adjacency_list,
		std::vector<weight_t>& min_distance, std::vector<vertex_t>& previous);
	list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const vector<vertex_t>& previous);
	void set_player_path(int length, sf::String playerInput);
	int set_neighbor(vector<vector<int>>& vertex, int i, int j, char flag);
	int set_edge(vector<vector<int>>& world, int i, int j, char flag);
};
