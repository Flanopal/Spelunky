#pragma once
struct Coordinates
{
public:
	Coordinates() {}
	Coordinates(int X, int Y, int ObjectType) :x(X), y(Y), objectType(ObjectType) {}
	int x;
	int y;
	int objectType;
};