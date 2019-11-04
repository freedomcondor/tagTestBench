/*---------------------------
	Weixu Zhu 
		zhuweixu_harry@126.com
	Version 1.1 : fixed constructor problem in mac
 *--------------------------- */
#include "ChessWorld.h"

ChessWorld::ChessWorld() :
	aworld(-50,-50,-50)
{
}

int ChessWorld::set(int x, int y, int z, double value)
{
	return aworld.set(x,y,z,value);
}

double ChessWorld::get(int x, int y, int z)
{
	return aworld.get(x,y,z);
}
