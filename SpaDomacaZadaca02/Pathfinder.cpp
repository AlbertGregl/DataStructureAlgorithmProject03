#include "Pathfinder.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Pathfinder::Pathfinder(sf::RenderWindow* window, int screen_width, int screen_height, 
	char flag, sf::String playerInput)
{
	this->window = window;
	this->screen_width = screen_width;
	this->screen_height = screen_height;
	sf::Clock clock = this->clock;

	switch (flag)
	{
	case '1':
		open_world("world_a.txt");
		break;
	case '2':
		open_world("world_b.txt");
		break;
	}

	set_player_path((screen_width / life_size) * (screen_height / life_size), playerInput);
}

void Pathfinder::draw()
{		
	draw_environment();
	sf::Time elapsed = clock.getElapsedTime();
	float sec = elapsed.asSeconds();
	if (sec >= speed)
	{
		if (!player_location.empty())
		{
			int x = player_location.front().x;
			int y = player_location.front().y;
			world[x][y] = 1;
			player_location.pop();
		}
		clock.restart();		
	}
	else
	{
		draw_environment();

	}
}

void Pathfinder::draw_environment()
{
	// initial drawing coordinates
	sf::Vector2f w;
	w.x = 0.f;
	w.y = 0.f;

	// draw row by row
	for (int i = 0; i < world.size(); i++)
	{
		draw_row(world[i], w);
		w.y += life_size;
	}
}

void Pathfinder::draw_wall(sf::Vector2f w, int color)
{
	int red, green, blue;

	if (color == 999)
	{
		red = 0;
		green = 0;
		blue = 0;
	}
	else
	{
		red = 43 + color / 2;
		green = 102 + color;
		blue = 36 + color / 2;
	}
	
	sf::Vector2f pixel(life_size, life_size);
	sf::RectangleShape life(pixel);
	life.setFillColor(sf::Color(red, green, blue));
	life.setPosition(w.x, w.y);
	window->draw(life);
}

void Pathfinder::draw_floor(sf::Vector2f w)
{
	sf::Vector2f pixel(life_size, life_size);
	sf::RectangleShape life(pixel);
	life.setFillColor(sf::Color(195, 164, 132));
	life.setPosition(w.x, w.y);
	window->draw(life);
}

void Pathfinder::draw_start(sf::Vector2f w)
{
	sf::Vector2f pixel(life_size, life_size);
	sf::RectangleShape life(pixel);
	life.setFillColor(sf::Color(35, 85, 255));
	life.setPosition(w.x, w.y);
	window->draw(life);
}

void Pathfinder::draw_finish(sf::Vector2f w)
{
	sf::Vector2f pixel(life_size, life_size);
	sf::RectangleShape life(pixel);
	life.setFillColor(sf::Color(255, 3, 3));
	life.setPosition(w.x, w.y);
	window->draw(life);
}

void Pathfinder::draw_player(sf::Vector2f w)
{
	sf::Vector2f pixel(life_size, life_size);
	sf::RectangleShape life(pixel);
	life.setFillColor(sf::Color(255, 28, 255));
	life.setPosition(w.x, w.y);
	window->draw(life);	
}

void Pathfinder::draw_row(vector<int> row, sf::Vector2f w)
{
	for (int i = 0; i < row.size(); i++)
	{
		if (row[i] == 0)
		{
			draw_floor(w);
			w.x += life_size;
		}
		else if (row[i] == 1)
		{
			draw_player(w);
			w.x += life_size;
		}
		else if (row[i] == 2)
		{
			draw_start(w);
			w.x += life_size;
		}
		else if (row[i] == 3)
		{
			draw_finish(w);
			w.x += life_size;
		}
		else if (row[i] > 3)
		{
			draw_wall(w, row[i]);
			w.x += life_size;
		}
	}
}

void Pathfinder::open_world(string world_name)
{
	ifstream in(world_name);

	if (!in)
	{
		cout << "Something went wrong while opening the file: \"" << world_name << "\"" << endl;
	}

	vector<int> row(screen_width / life_size);
	string line;
	string temp;

	for (int i = 0; i < screen_height / life_size; i++)
	{
		getline(in, line);
		stringstream ss(line);

		for (int j = 0; j < screen_width / life_size; j++)
		{
			getline(ss, temp, '-');
			row[j] = stoi(temp);
		}
		world.push_back(row);
	}
	in.close();
}

/*method from "http://rosettacode.org/wiki/Dijkstra%27s_algorithm"*/
void Pathfinder::DijkstraComputePaths(vertex_t source, const adjacency_list_t& adjacency_list, 
	std::vector<weight_t>& min_distance, std::vector<vertex_t>& previous)
{
	int n = adjacency_list.size();
	min_distance.clear();
	min_distance.resize(n, max_weight);
	min_distance[source] = 0;
	previous.clear();
	previous.resize(n, -1);
	std::set<std::pair<weight_t, vertex_t> > vertex_queue;
	vertex_queue.insert(std::make_pair(min_distance[source], source));

	while (!vertex_queue.empty())
	{
		weight_t dist = vertex_queue.begin()->first;
		vertex_t u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());

		// Visit each edge exiting u
		const std::vector<neighbor>& neighbors = adjacency_list[u];
		for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
			neighbor_iter != neighbors.end();
			neighbor_iter++)
		{
			vertex_t v = neighbor_iter->target;
			weight_t weight = neighbor_iter->weight;
			weight_t distance_through_u = dist + weight;
			if (distance_through_u < min_distance[v]) {
				vertex_queue.erase(std::make_pair(min_distance[v], v));

				min_distance[v] = distance_through_u;
				previous[v] = u;
				vertex_queue.insert(std::make_pair(min_distance[v], v));
			}

		}
	}
}

/*method from "http://rosettacode.org/wiki/Dijkstra%27s_algorithm"*/
list<vertex_t> Pathfinder::DijkstraGetShortestPathTo(vertex_t vertex, const vector<vertex_t>& previous)
{
	std::list<vertex_t> path;
	for (; vertex != -1; vertex = previous[vertex])
		path.push_front(vertex);
	return path;
}

 void Pathfinder::set_player_path(int length, sf::String playerInput)
{
	// starting and final position of player
	stringstream ss(playerInput);
	string temp;
	int start_x = 0;
	int start_y = 0;
	int final_x = 29;
	int final_y = 39;
	try
	{
		getline(ss, temp, ',');
		start_x = stoi(temp);
		getline(ss, temp, '-');
		start_y = stoi(temp);
		getline(ss, temp, ',');
		final_x = stoi(temp);
		getline(ss, temp);
		final_y = stoi(temp);
	}
	catch (const std::exception&)
	{
		cout << "Error => wrong input, enter format: (xx,xx-xx,xx), where x is a integer value." << endl;
	}
	// initialize start and final position if "playerInput" was wrong
	int start = 0;
	int finish = 1199;
	 
	adjacency_list_t adjacency_list(length);

	int height = screen_height / life_size;
	int width = screen_width / life_size;

	vector<vector<int>> vertex;
	vector<int> v;

	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			v.push_back(index);
			if (i == start_x && j == start_y)
			{
				start = index;
				world[i][j] = 2;
			}
			if (i == final_x && j == final_y)
			{
				finish = index;
				world[i][j] = 3;
			}
			index++;
		}
		vertex.push_back(v);
		v.clear();
	}

	index = 0;

	for (int i = 0; i < height; i++) //rows [i]
	{
		for (int j = 0; j < width; j++) //columns [j]
		{			
			// world edge top-bottom and corners
			if (i == 0)
			{
				if (j == 0)
				{	//corner top left
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'e'), set_edge(world, i, j, 'e')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'g'), set_edge(world, i, j, 'g')));
				}
				else if (j == width - 1)
				{  //corner top right
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'd'), set_edge(world, i, j, 'd')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'g'), set_edge(world, i, j, 'g')));
				}
				else
				{
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'd'), set_edge(world, i, j, 'd')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'e'), set_edge(world, i, j, 'e')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'g'), set_edge(world, i, j, 'g')));
				}
			}
			else if (i == height - 1)
			{
				if (j == 0)
				{	//corner bot left
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'b'), set_edge(world, i, j, 'b')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'e'), set_edge(world, i, j, 'e')));
				}
				else if (j == width - 1)
				{  //corner bot right
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'b'), set_edge(world, i, j, 'b')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'd'), set_edge(world, i, j, 'd')));
				}
				else
				{
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'b'), set_edge(world, i, j, 'b')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'd'), set_edge(world, i, j, 'd')));
					adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'e'), set_edge(world, i, j, 'e')));
				}
			}
			//world edge left-right
			else if (j == 0)
			{
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'b'), set_edge(world, i, j, 'b')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'e'), set_edge(world, i, j, 'e')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'g'), set_edge(world, i, j, 'g')));
			}
			else if (j == width - 1)
			{
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'b'), set_edge(world, i, j, 'b')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'd'), set_edge(world, i, j, 'd')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'g'), set_edge(world, i, j, 'g')));
			}
			else
			{
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'b'), set_edge(world, i, j, 'b')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'd'), set_edge(world, i, j, 'd')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'e'), set_edge(world, i, j, 'e')));
				adjacency_list[index].push_back(neighbor(set_neighbor(vertex, i, j, 'g'), set_edge(world, i, j, 'g')));
			}
			index++;
		}
	}
	
	std::vector<weight_t> min_distance;
	std::vector<vertex_t> previous;

	DijkstraComputePaths(start, adjacency_list, min_distance, previous);	

	list<vertex_t> path = DijkstraGetShortestPathTo(finish, previous);

	// vertex nodes to index, for drawing on world map.
	player p;
	for (auto it = path.begin(); it != path.end(); ++it)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (*it == vertex[i][j])
				{
					p.x = i;
					p.y = j;
					player_location.push(p);
				}
			}
		}
	}
}

int Pathfinder::set_neighbor(vector<vector<int>>& vertex, int i, int j, char flag)
{
	switch (flag)
	{
	case 'a':
		return vertex[--i][--j];
		break;
	case 'b':
		return vertex[--i][j];
		break;
	case 'c':
		return vertex[--i][++j];
		break;
	case 'd':
		return vertex[i][--j];
		break;
	case 'e':
		return vertex[i][++j];
		break;
	case 'f':
		return vertex[++i][--j];
		break;
	case 'g':
		return vertex[++i][j];
		break;
	case 'h':
		return vertex[++i][++j];
		break;
	}
}

int Pathfinder::set_edge(vector<vector<int>>& world, int i, int j, char flag)
{
	switch (flag)
	{
	case 'a':
		return world[--i][--j];
		break;
	case 'b':
		return world[--i][j];
		break;
	case 'c':
		return world[--i][++j];
		break;
	case 'd':
		return world[i][--j];
		break;
	case 'e':
		return world[i][++j];
		break;
	case 'f':
		return world[++i][--j];
		break;
	case 'g':
		return world[++i][j];
		break;
	case 'h':
		return world[++i][++j];
		break;
	}
}