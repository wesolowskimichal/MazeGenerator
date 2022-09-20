#include "Cell.h"
#include "Screen.h"

Cell::Cell(const float x, const float y) :x(x), y(y) {
	this->borders["UP"] = nullptr;
	this->borders["DOWN"] = nullptr;
	this->borders["LEFT"] = nullptr;
	this->borders["RIGHT"] = nullptr;
	this->color = sf::Color::Black;
	this->visited = 0;
}

void Cell::setBorder(string border, Border* newBorder) {
	try {
		if (!this->borders[border] && newBorder == nullptr)
			return;
		if (!this->borders[border])
			this->borders[border] = new Border({ NULL, NULL }, {NULL, NULL});
		this->borders[border] = newBorder;
	}
	catch(int) {
		cerr << "Border is already null!\n";
	}
}

void Cell::draw(Screen* obj) {
	sf::RectangleShape me(sf::Vector2f(obj->CELL_SIZE, obj->CELL_SIZE));
	me.setFillColor(this->color);
	me.setPosition(sf::Vector2f(this->x, this->y));
	obj->window.draw(me);
	//cout << this->x << ' ' << this->y << endl;
	for (auto it = this->borders.begin(); it != this->borders.end(); it++) {
		if (it->second == nullptr)
			continue;
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(it->second->startX, it->second->startY)),
			sf::Vertex(sf::Vector2f(it->second->endX, it->second->endY))
		};
		obj->window.draw(line, 2, sf::Lines);
	}
	//obj->window.display();
	//obj->window.display();

}

pair<int, int> Cell::getCords() {
	return { this->x, this->y };
}