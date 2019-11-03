#ifndef CELLULARAUTOMATON
#define CELLULARAUTOMATON

#include "ChessWorld.h"

class CellularAutomaton
{
private:
	ChessWorld world1, world2;
	ChessWorld *world, *nextworld;

public:
	CellularAutomaton();
	int set(int x, int y, int z, double value);
	double get(int x, int y, int z);
	int run(int x, int y, int z, int xl, int yl, int zl);
	int draw(int x, int y, int z, int xl, int yl, int zl);
};

#endif
