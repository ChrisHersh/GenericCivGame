#include "StdAfx.h"
#include "map.h"

map::map(int mapHieght, int mapWeidth)
{
	oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff = mapHieght * mapWeidth;
	grid = new tile[oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff];

	mapHeight = mapHieght;
	mapWidth = mapWeidth;
}

map::~map(void)
{
}

//Map generation
void map::generateMap(unsigned int seed)
{
	srand(seed);

	for(int i = 0; i < oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff; i++)
	{

	}
}

//Array arithmetics
int map::translateArrayPosition(int x, int y)
{
	if(x > mapWidth || x < 0)
		return -1;

	if(y > mapHeight || y < 0)
		return -1;

	return (y * mapWidth) + x;
}

//Getters
int map::get_oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff()
{
	return oneDimensionalArraySizeJustToAnnoyChrisAlsoILikeReallyLongVariableNamesAndStuff;
}

size_t map::getMemoryAllocatedFor_grid()
{
	return sizeof(*grid);
}

tile *map::getTile(int x, int y)
{
	int translatedPosition;
	if((translatedPosition = translateArrayPosition(x, y)) == -1)
		return NULL;

	return (grid + translatedPosition);
}