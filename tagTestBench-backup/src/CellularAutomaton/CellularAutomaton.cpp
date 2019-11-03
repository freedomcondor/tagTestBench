#include "CellularAutomaton.h"

CellularAutomaton::CellularAutomaton()
{
	world = &world1;
	nextworld = &world2;
}

int CellularAutomaton::set(int x, int y, int z, double value)
{
	return world->set(x,y,z,value);
}

double CellularAutomaton::get(int x, int y, int z)
{
	return world->get(x,y,z);
}

int CellularAutomaton::run(int x, int y, int z, int xl, int yl, int zl)
{
	ChessWorld *temp;
	for (int i = x; i < x+xl; i++)
		for (int j = y; j < y+yl; j++)
			for (int k = z; k < z+zl; k++)
			{
				if ( (world->get(i-1,j,k) > 0.5) ||
				     //(world->get(i,j-1,k) > 0.5) ||
				     //(world->get(i+1,j,k) > 0.5) ||
				     (world->get(i,j+1,k) > 0.5))
					nextworld->set(i,j,k,1);
				else 
					nextworld->set(i,j,k,0);
				if ( (world->get(i-1,j-1,k) > 0.5) ||
				     //(world->get(i+1,j-1,k) > 0.5) ||
				     //(world->get(i-1,j+1,k) > 0.5) ||
				     (world->get(i+1,j+1,k) > 0.5) )
					nextworld->set(i,j,k,0);
			}
	temp = world;
	world = nextworld;
	nextworld = temp;
	return 0;
}
