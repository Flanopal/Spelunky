#pragma once

#include <functional>

using namespace std;

struct Coordinates
{
public:
	Coordinates() {}
	Coordinates(int X, int Y, int ObjectType) :x(X), y(Y), objectType(ObjectType) {}
	int x;
	int y;
	int objectType;
};

class Command
{
public:
	Command(int id, function<void()> func) :identifier(id), callback(func) {}
	int identifier;
	function<void()> callback;
};