#pragma once
#include "Cell.h"
#include <deque>
class Screen {
private:
	const string title;
	const float W;
	const float H;
	sf::Color clearColor;
	sf::RenderWindow window;
	deque<Cell*>cellsNotVisited;
	vector<vector<Cell*>>map;
	vector<Cell*>c;
	friend class Cell;
	const float CELL_SIZE = 25;
	bool flag = 1;
public:
	Screen(string title, float width, float height, sf::Color clearColor = sf::Color::Black);
	~Screen();
	sf::RenderWindow& _window();

	void create_Grid(
		const int numberOfCubesInRow, const int numberOfCubesInColumn, sf::Color colorOfLines = sf::Color(255, 255, 255)
	);

	void create_PartGrid(
		const int numberOfCubesInRow, const int numberOfCubesInColumn, const float sizeX,
		const float sizeY, const float spaceFromLeft = 0.0f, const float spaceFromTop = 0.0f,
		sf::Color fillColor = sf::Color(-1, -1, -1), sf::Color colorOfLines = sf::Color(255, 255, 255)
	);

	void update();

	//	actual code

	void generateMaze();
	void slow();
	void low();
	//void solve();		TODO
};

