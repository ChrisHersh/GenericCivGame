#pragma once
#include "tile.h"
#include <stdlib.h>

class map
{
private:
	//Tile stuffs
	tile *grid;	//We'll store things in row major order.  Just because.  I can't speel.
	int oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff;
	int translateArrayPosition(int x, int y);
	
	int mapHeight;
	int mapWidth;

	void generateMap(unsigned int seed);

public:
	map(int mapHieght, int mapWeidth);
	~map(void);

	//Getters
	int get_oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff();
	size_t getMemoryAllocatedFor_grid();
	bool getIsChrisStupid(){return true;};
	int getMapHeight(){ return mapHeight; };
	int getMapWidth(){ return mapWidth; };

	tile *getTile(int x, int y);

	//Setters (Like I would ever give Chris control over this stuff)

};

