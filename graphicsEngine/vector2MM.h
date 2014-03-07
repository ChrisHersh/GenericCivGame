#ifndef VECTOR_2_MM
#define VECTOR_2_MM

class Vector2MM
{
public:
    Vector2MM() { Vector2MM(0,0,0,0); }
    Vector2MM(int newXMax, int newXMin, int newYMax, int newYMin)
    {
	xMax = newXMax;
	xMin = newXMin;
	yMax = newYMax;
	yMin = newYMin;
    }

    int xMax;
    int xMin;
    int yMax;
    int yMin;
protected:
};

#endif // TILE_H
