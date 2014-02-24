#include <SFML/Graphics.hpp>
#include <sys/resource.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "tile.h"
#include <cstdlib>

sf::Vector2f findHexTile(float mouseX, float mouseY);
sf::Vector2f findNearestHex(sf::Vector2f coords);

float getXPositionForTiles(int mapPos, int i);
float getYPositionForTiles(bool oddRow, int p, int rhombusOffset);

int size;

double vertOffset;
double horzOffset;
double heightSqrt;
int wrapOffset;

float viewZoomLevel = 1;

int main()
{
    size = 32;
    double zoom = 1;

    int numHorz= 40;
    int numVert = 40;

    heightSqrt = std::sqrt(3.0)/2;
    horzOffset = (3.0/4)*(2.0*size);
    vertOffset = heightSqrt*(2.0*size);

    wrapOffset = numHorz*horzOffset;

    std::cout << heightSqrt << " -\t- " << vertOffset << std::endl;

    sf::RenderWindow window(sf::VideoMode(500,500), "The Hex Selection Is Done...Forever");
    window.clear();
    float viewSizeX = 250.0;
    float viewSizeY = 250.0;
    sf::View view(sf::Vector2f(viewSizeX,viewSizeY), sf::Vector2f(viewSizeX*2,viewSizeY*2));
    window.setView(view);
    //window.setFramerateLimit(300);
    //window.setVerticalSyncEnabled(false);
    window.display();

    std::cout << "\n\n\n" << sizeof(sf::Sprite) << "\n\n" << std::endl;

    sf::Texture oreText;
    if(!oreText.loadFromFile("oreHex.gif"))
    {
        std::cout << "ERROR";
    }
    oreText.setSmooth(false);

    sf::Texture waterText;
    if(!waterText.loadFromFile("waterHex.gif"))
    {
        std::cout << "ERROR";
    }
    waterText.setSmooth(false);


    tile tiles[numHorz*3][numVert];

    int rhombusOffset = 0;
    int vertCount = 1;
    for(int i = 0; i < numHorz; i++)
    {
        rhombusOffset += vertOffset*(vertCount%2);
        for(int p = 0; p < numVert; p++)
        {
            //std::cout << i << " -- " << p << std::endl;
            tiles[i][p] = tile(&waterText);
            tiles[i+numHorz][p] = tile(&oreText);
            tiles[i+numHorz*2][p] = tile(&waterText);

            if(i%2 == 0)
            {
                tiles[i][p].setPosition(getXPositionForTiles(-1, i), getYPositionForTiles(false, p, rhombusOffset));
                tiles[i+numHorz][p].setPosition(getXPositionForTiles(0, i), getYPositionForTiles(false, p, rhombusOffset));
                tiles[i+numHorz*2][p].setPosition(getXPositionForTiles(1, i), getYPositionForTiles(false, p, rhombusOffset));

// 		tiles[i][p].setPosition(i*horzOffset-wrapOffset, p*vertOffset+rhombusOffset);
//                 tiles[i+numHorz][p].setPosition(i*horzOffset, p*vertOffset+rhombusOffset);
//                 tiles[i+numHorz*2][p].setPosition(i*horzOffset+wrapOffset, p*vertOffset+rhombusOffset);
            }
            else
            {
                tiles[i][p].setPosition(getXPositionForTiles(-1, i), getYPositionForTiles(true, p, rhombusOffset));
                tiles[i+numHorz][p].setPosition(getXPositionForTiles(0, i), getYPositionForTiles(true, p, rhombusOffset));
                tiles[i+numHorz*2][p].setPosition(getXPositionForTiles(1, i), getYPositionForTiles(true, p, rhombusOffset));

// 		tiles[i][p].setPosition(i*horzOffset-wrapOffset, p*vertOffset+heightSqrt*size+rhombusOffset);
//                 tiles[i+numHorz][p].setPosition(i*horzOffset, p*vertOffset+heightSqrt*size+rhombusOffset);
//                 tiles[i+numHorz*2][p].setPosition(i*horzOffset+wrapOffset, p*vertOffset+heightSqrt*size+rhombusOffset);

            }
            //rhombusOffset = rhombusOffset + vertOffset*(i%2);
        }
        vertCount++;
    }
    sf::Shader::isAvailable();

    sf::Vector2f viewSize = view.getSize();

    int fpsCounter = 0;
    sf::Clock gameClock;
    sf::Time fpsDelay = sf::milliseconds(1000);
    sf::Time currTime;

    bool mouseMoved = false;
    int mouseDragPositionX = 0;// = view.getCenter().x;
    int mouseDragPositionY = 0;// = view.getCenter().y;


    bool mouseLeftDown = false;

    tile *selectedTile = 0;

    //tiles[2+numHorz][3].selectTile();
    //tiles[0][0].setPosition(0,0);
    while (window.isOpen())
    {
        currTime = gameClock.getElapsedTime();
        if(currTime.asMilliseconds() >= fpsDelay.asMilliseconds())
        {
            //std::cout << "FPS: " << fpsCounter << "\t X- " << mouseDragPositionX << "\t Y- " << mouseDragPositionY << std::endl;
            fpsCounter = 0;
            gameClock.restart();
        }
        ++fpsCounter;
        for(int i = 0; i < numHorz; i++)
        {
            for(int p = 0; p < numVert; p++)
            {
                window.draw(tiles[i][p].getSprite());
                //tiles[i+numHorz][p].updateSprite();
                window.draw(tiles[i+numHorz][p].getSprite());
                window.draw(tiles[i+2*numHorz][p].getSprite());
            }
        }
        window.display();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                int movementOnX = 0;
                int movementOnY = 0;

                bool leftPressed = event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A;
                bool rightPressed = event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D;
                bool upPressed = event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W;
                bool downPressed = event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S;

                if(leftPressed)
                {
                    movementOnX-=10*viewZoomLevel;
                }
                if(downPressed)
                {
                    movementOnY+=10*viewZoomLevel;
                }
                if(rightPressed)
                {
                    movementOnX+=10*viewZoomLevel;
                }
                if(upPressed)
                {
                    movementOnY-=10*viewZoomLevel;
                }
                view.move(movementOnX, movementOnY);
                window.setView(view);
                window.clear();
            }
            if(event.type == sf::Event::MouseWheelMoved)
            {
                if(event.mouseWheel.delta == 1)
                {
		    viewZoomLevel *= .75;
                    zoom = .75;
                }
                if(event.mouseWheel.delta == -1)
                {
		    viewZoomLevel *= 1.5;
                    zoom = 1.5;
                }
                view.zoom(zoom);
                //viewSize = view.getSize();
                window.setView(view);
                window.clear();
            }
            if(event.type == sf::Event::Resized)
            {
                float viewNewX = event.size.width;
                float viewNewY = event.size.height;
                float viewRatio = viewNewX/viewNewY;
                viewSizeX = viewSizeY*viewRatio;
                view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                window.setView(view);
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    mouseLeftDown = true;
                    mouseMoved = false;
                    mouseDragPositionX = sf::Mouse::getPosition(window).x;
                    mouseDragPositionY = sf::Mouse::getPosition(window).y;
                }
            }
            if(event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    mouseLeftDown = false;
                }
                if(!mouseMoved)
                {
                    sf::Vector2i tmp;
                    tmp.x = mouseDragPositionX;
                    tmp.y = mouseDragPositionY;
                    sf::Vector2f mapCoords = window.mapPixelToCoords(tmp, view);
                    sf::Vector2f result = findHexTile(mapCoords.x, mapCoords.y);
                    if(selectedTile != 0)
                        selectedTile->unselectTile();
                    selectedTile = &tiles[(int)result.x+numHorz][(int)result.y];
                    selectedTile->selectTile();
                }
            }
            if(event.type == sf::Event::MouseMoved)
            {
                mouseMoved = true;
                if(mouseLeftDown)
                {
                    float mouseXDiff = mouseDragPositionX - sf::Mouse::getPosition(window).x;
                    float mouseYDiff = mouseDragPositionY - sf::Mouse::getPosition(window).y;
                    view.move(mouseXDiff*viewZoomLevel, mouseYDiff*viewZoomLevel);
                    window.setView(view);
                    window.clear();
                }

                mouseDragPositionX = sf::Mouse::getPosition(window).x;
                mouseDragPositionY = sf::Mouse::getPosition(window).y;
            }
        }
    }
}

float getXPositionForTiles(int mapPos, int i)
{
    return i*horzOffset+(wrapOffset*mapPos);
}

float getYPositionForTiles(bool oddRow, int p, int rhombusOffset)
{
    if(oddRow)
        return p*vertOffset+heightSqrt*size+rhombusOffset;
    else
        return p*vertOffset+rhombusOffset;
}

sf::Vector2f findHexTile(float x, float y)
{
    y = y - heightSqrt*size;
    //x = x + size;

    sf::Vector2f result;

    result.x = ((2.0/3)*x / size);
    result.y = (((1.0/3) * std::sqrt(3) * y - (1.0/3) * x) / size);


    //return findNearestHex(result);
    return result;
}

sf::Vector2f findNearestHex(sf::Vector2f coords)
{
    float x = coords.x;
    float y = coords.y;
    float z = -coords.x-coords.y;

    int rx = round(x);
    int ry = round(y);
    int rz = round(z);

    float xDiff = abs(rx-x);
    float yDiff = abs(ry-y);
    float zDiff = abs(rz - z);

    if(xDiff > yDiff && xDiff > zDiff)
    {
        rx = -ry-rz;
    }
    else if(yDiff > zDiff)
    {
        ry = -rx-rz;
    }
    else
    {
        rz = -rx-ry;
    }



    float q = rx;
    float r = ry + (rx-(rx&1))/2;

    coords.x = q;
    coords.y = r;
    std::cout<<"X -- " << coords.x << "\tY -- " << coords.y <<std::endl;

    coords.x = coords.x - abs(x-coords.x);
    coords.y = coords.y - abs(y-coords.y);

    return coords;
}
