#pragma once
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <exception>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

class Border
{
public:
	float startX;
	float startY;
	float endX;
	float endY;

	Border(pair<float, int>startCords, pair<int,int>finishCords) :
		startX(startCords.first), startY(startCords.second),
		endX(finishCords.first), endY(finishCords.second) {};

	Border operator = (Border* border) {
		this->startX = border->startX;
		this->startY = border->startY;
		this->endX = border->endX;
		this->endY = border->endY;
	}
};

