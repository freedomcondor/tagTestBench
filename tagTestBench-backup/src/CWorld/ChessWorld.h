/*---------------------------
	Weixu Zhu 
		zhuweixu_harry@126.com
	Version 1.1 : fixed constructor problem in mac
 *--------------------------- */
#ifndef CHESSWORLD
#define CHESSWORLD

#define SQUARELENGTH 100

class ChessWorld
{
	class ChessWorldSquare
	{
		public:
		int xS, yS, zS;	// start from
		int xL, yL, zL; // length
		double data[SQUARELENGTH][SQUARELENGTH][SQUARELENGTH];

		ChessWorldSquare(int _x, int _y, int _z)
		{
			xS = _x;
			yS = _y;
			zS = _z;
			xL = SQUARELENGTH;
			yL = SQUARELENGTH;
			zL = SQUARELENGTH;
		}

		double get(int x, int y, int z)
		{
			if ((x < xS) || (y < yS) || (z < zS) ||
			    (x >= xS+xL) ||
			    (y >= yS+yL) ||
			    (z >= zS+zL)    )
				return 0;
			return data[x-xS][y-yS][z-zS];
		}

		int set(int x, int y, int z, double value)
		{
			if ((x < xS) || (y < yS) || (z < zS) ||
			    (x >= xS+xL) ||
			    (y >= yS+yL) ||
			    (z >= zS+zL)    )
				return -1;
			data[x-xS][y-yS][z-zS] = value;
			return 0;
		}
	} aworld;

public:
	ChessWorld();
	int set(int x, int y, int z, double value);
	double get(int x, int y, int z);
};

#endif
