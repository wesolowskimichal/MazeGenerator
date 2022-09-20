#pragma once
#include "Border.h"
class Screen;
class Cell{
private:
	const float x;
	const float y;
	unordered_map<string, Border*>borders;
public:
	sf::Color color;
	bool visited;
	Cell(const float x, const float y);
	void setBorder(string border, Border* newBorder);
	void draw(Screen* obj);
	pair<int, int> getCords();
};

