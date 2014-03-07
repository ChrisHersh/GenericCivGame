#pragma once
enum tileType {
	CRAY,
	WATER
};

class tile
{
private:
	tileType typeOfThisTile;

public:
	tile(tileType newType);
	~tile(void);
};

