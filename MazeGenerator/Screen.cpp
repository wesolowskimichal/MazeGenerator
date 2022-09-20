#include "Screen.h"
#include <random>
#include <set>
Screen::Screen(string title, float width, float height, sf::Color clearColor)
	:title(title), W(width), H(height), clearColor(clearColor)
{
	int Y = (int)this->H / (int)this->CELL_SIZE;
	int X = (int)this->W / (int)this->CELL_SIZE;
	this->window.create(sf::VideoMode(this->W, this->H), title);
	this->map.assign(Y, vector<Cell*>(X, nullptr));
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int y = j * this->CELL_SIZE;
			int x = i * this->CELL_SIZE;
			Cell* temp = new Cell(x, y);
			temp->setBorder("UP", new Border({ x,y }, { x + this->CELL_SIZE, y }));
			temp->setBorder("DOWN", new Border({ x,y + this->CELL_SIZE }, { x + this->CELL_SIZE, y + this->CELL_SIZE }));
			temp->setBorder("LEFT", new Border({ x,y }, { x,y + this->CELL_SIZE }));
			temp->setBorder("RIGHT", new Border({ x + this->CELL_SIZE,y }, { x + this->CELL_SIZE,y + this->CELL_SIZE }));
			this->cellsNotVisited.push_back(temp);
			this->map[j][i]= temp;
		}
	}
}

Screen::~Screen() {
}

sf::RenderWindow& Screen::_window() {
	return this->window;
}

void Screen::update() {
	if (flag) {
		this->generateMaze();
		this->low();
		this->solve();
		flag = 0;
	}
}

void Screen::create_Grid(
	const int numberOfCubesInRow, const int numberOfCubesInColumn, sf::Color colorOfLines
) {
	assert(numberOfCubesInColumn > 0 && "Number of cubes in column must be bigger than 0!");
	assert(numberOfCubesInRow > 0 && "Number of cubes in row must be bigger than 0!");
	try {
		if (numberOfCubesInColumn > this->W / 2 || numberOfCubesInRow > this->H / 2)
			throw(0);
		float cubeSizeX = (float)this->W / (float)numberOfCubesInColumn;
		float cubeSizeY = (float)this->H / (float)numberOfCubesInRow;
		for (int i = 0.0; i <= this->W; i += cubeSizeX) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(i, 0.0f), colorOfLines),
				sf::Vertex(sf::Vector2f(i, this->H), colorOfLines)
			};
			this->window.draw(line, 2, sf::Lines);
		}
		for (int i = 0.0; i <= this->H; i += cubeSizeY) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(0.0f, i), colorOfLines),
				sf::Vertex(sf::Vector2f(this->W, i), colorOfLines)
			};
			this->window.draw(line, 2, sf::Lines);
		}
	}
	catch (int) {
		cerr << "Border of cubes took all the space!\n";
	}
}

void Screen::create_PartGrid(
	const int numberOfCubesInRow, const int numberOfCubesInColumn, const float sizeX,
	const float sizeY, const float spaceFromLeft, const float spaceFromTop, sf::Color fillColor,
	sf::Color colorOfLines
) {
	assert(numberOfCubesInColumn > 0 && "Number of cubes in column must be bigger than 0!");
	assert(numberOfCubesInRow > 0 && "Number of cubes in row must be bigger than 0!");
	assert(sizeX > 0 && "Horizontal size must be bigger than 0!");
	assert(sizeY > 0 && "Vertical size must be bigger than 0!");
	try {
		if (numberOfCubesInColumn > sizeX / 2 || numberOfCubesInRow > sizeY / 2)
			throw(0);
		float cubeSizeX = (float)sizeX / (float)numberOfCubesInColumn;
		float cubeSizeY = (float)sizeY / (float)numberOfCubesInRow;
		int Y = spaceFromTop + sizeY;
		int X = spaceFromLeft + sizeX;
		if (X > this->W || Y > this->H)
			cerr << "Grid out of window!\n";
		if (fillColor != sf::Color(-1, -1, -1)) {
			for (int i = spaceFromTop; i < Y; i += cubeSizeY) {
				for (int j = spaceFromLeft; j < X; j += cubeSizeX) {
					sf::RectangleShape fillBox(sf::Vector2f(cubeSizeX, cubeSizeY));
					fillBox.setFillColor(fillColor);
					fillBox.setPosition(sf::Vector2f(j, i));
					this->window.draw(fillBox);
				}
			}
		}
		for (int i = spaceFromLeft; i <= X; i += cubeSizeX) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(i, spaceFromLeft), colorOfLines),
				sf::Vertex(sf::Vector2f(i, X), colorOfLines)
			};
			this->window.draw(line, 2, sf::Lines);
		}
		for (int i = spaceFromTop; i <= Y; i += cubeSizeY) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(spaceFromTop, i), colorOfLines),
				sf::Vertex(sf::Vector2f(Y, i), colorOfLines)
			};
			this->window.draw(line, 2, sf::Lines);
		}
	}
	catch (int) {
		cerr << "Border of cubes took all the space!\n";
	}
}

bool check(int i, int j, int n, int m) {
	return !(i < 0 || j < 0 || i >= n || j >= m);
}

void Screen::generateMaze() {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, 3); // define the range
	vector<pair<int, int>>dir = { {0,1}, {1,0}, {0,-1}, {-1,0} };
	int END = this->map.size() * this->map[0].size();
	while (cellsNotVisited.size() >= END) {
		Cell* curr = cellsNotVisited.front();
		int currX = (int)curr->getCords().first / this->CELL_SIZE;
		int currY = (int)curr->getCords().second / this->CELL_SIZE;
		this->map[currY][currX]->visited = 1;
		this->c.push_back(curr);
		this->slow();
		int nextX = -1, nextY = -1;
		set<pair<int, int>>seet;
		bool found = 1;
		while (1) {
			int rX = distr(gen);
			nextX = dir[rX].first;
			nextY = dir[rX].second;
			seet.insert({ currX + nextX,  currY + nextY });
			if (check(currX + nextX, currY + nextY, this->map[0].size(), this->map.size())) {
				if (!this->map[currY + nextY][currX + nextX]->visited) {
					this->cellsNotVisited.push_front(this->map[currY + nextY][currX + nextX]);
					break;
				}
			}
			if (seet.size() == 4) {
				found = 0;
				break;
			}
		}
		if (!found && !cellsNotVisited.empty()) {
			cellsNotVisited.pop_front();
			//delete curr;
			continue;
		}
		Cell* next = this->map[currY + nextY][currX + nextX];
		if (nextX == 0 && nextY == 1) {
			curr->setBorder("DOWN", nullptr);
			next->setBorder("UP", nullptr);
		}
		else if (nextX == 0 && nextY == -1) {
			curr->setBorder("UP", nullptr);
			next->setBorder("DOWN", nullptr);

		}
		else if (nextX == 1 && nextY == 0) {
			curr->setBorder("RIGHT", nullptr);
			next->setBorder("LEFT", nullptr);

		}
		else if (nextX == -1 && nextY == 0) {
			curr->setBorder("LEFT", nullptr);
			next->setBorder("RIGHT", nullptr);
		}
		//delete curr;
		//delete next;
		/*this->c.push_back(curr);
		this->slow();*/

	}
}

void Screen::slow() {
	this->window.clear();
	for (int i{}; i < this->c.size() - 1; i++) {
		c[i]->color = sf::Color::Black;
		c[i]->draw(this);
	}
	this->c[c.size() - 1]->color = sf::Color::Green;
	this->c[c.size() - 1]->draw(this);

	this->window.display();
}

void Screen::low() {
	this->window.clear();
	for (int i{}; i < this->c.size(); i++) {
		c[i]->color = sf::Color::Black;
		c[i]->draw(this);
	}
	this->window.display();
}

//TODO:
//void Screen::solve() {
//	/*int Y = (int)this->H / (int)this->CELL_SIZE;
//	int X = (int)this->W / (int)this->CELL_SIZE;*/
//	int Y = this->H;
//	int X = this->W;
//	vector<vector<char>>m(Y, vector<char>(X, '#'));
//	sf::Vector2u windowSize = window.getSize();
//	sf::Texture texture;
//	texture.create(windowSize.x, windowSize.y);
//	texture.update(window);
//	sf::Image img = texture.copyToImage();
//	for (int i{}; i < Y; i++) {
//		for (int j{}; j < X; j++) {
//			//auto color = img.getPixel(j*this->CELL_SIZE, i*this->CELL_SIZE);
//			auto color = img.getPixel(j, i);
//			if (color != sf::Color(255, 255, 255)) 
//				m[i][j] = '-';
//		}
//	}
//	for (auto r : m) {
//		for (auto c : r)
//			cout << c << ' ';
//		cout << endl;
//	}
//}