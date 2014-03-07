#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include <SFML/Graphics.hpp>
#include <sys/resource.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "tile.h"
#include <cstdlib>
#include <stdexcept>
#include "vector2MM.h"

class graphicsWindow
{
public:
    int verifyCull(float num, bool isUpperBound, int numTilesInDirection);
    sf::Vector2f findHexTile(float mouseX, float mouseY);
    float getXPositionForTiles(int mapPos, int i);
    float getYPositionForTiles(bool oddRow, int p, int rhombusOffset);
    
//public:
    graphicsWindow(int numTilesHorz, int numTilesVert);
    ~graphicsWindow();
    void run();
    void createTiles();
    
private:
    Vector2MM findMaxRange();
    Vector2MM findMinRange();
    void keyPressedEvents();
    void mouseWheelMovedEvent();
    void mouseButtonReleasedEvent();
    void mouseMovedEvent();
    void resizedEvent();
    void mouseButtonPressedEvent();
    
    sf::Texture oreText;
    sf::Texture waterText;
    int size;
    double vertOffset;
    double horzOffset;
    double heightSqrt;
    int wrapOffset;
    int numVert;
    int numHorz;
    float viewZoomLevel;
    std::vector< std::vector<tile> > tiles;
    sf::RenderWindow *window;
    sf::View view;
    sf::Event event;
    
    bool mouseMoved;
    int mouseDragPositionX;// = view.getCenter().x;
    int mouseDragPositionY;// = view.getCenter().y;
    tile *selectedTile;
    double zoom;

    bool mouseLeftDown;
    
    Vector2MM visibleRange;
};

#endif // TILE_H