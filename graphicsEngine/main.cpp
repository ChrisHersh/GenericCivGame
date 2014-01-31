#include <SFML/Graphics.hpp>
#include <sys/resource.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "tile.h"

std::vector<int> findHexTile(float x, float y);

int main()
{
    int size = 32;
    double zoom = 1;

    int numHorz= 20;
    int numVert = 20;

    double heightSqrt = std::sqrt(3.0)/2;
    double horzOffset = (3.0/4)*(2*size);
    double vertOffset = heightSqrt*(2*size);

    int wrapOffset = numHorz*horzOffset;

    std::cout << heightSqrt << " -\t- " << vertOffset << std::endl;

    sf::RenderWindow window(sf::VideoMode(500,500), "SFML works!");
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

    for(int i = 0; i < numHorz; i++)
    {
        for(int p = 0; p < numVert; p++)
        {
            std::cout << i << " -- " << p << std::endl;
            tiles[i][p] = tile(&waterText);
            tiles[i+numHorz][p] = tile(&oreText);
            tiles[i+numHorz*2][p] = tile(&waterText);
            if(i%2 == 0)
            {
                tiles[i][p].setPosition(i*horzOffset-wrapOffset, p*vertOffset);
                tiles[i+numHorz][p].setPosition(i*horzOffset, p*vertOffset);
                tiles[i+numHorz*2][p].setPosition(i*horzOffset+wrapOffset, p*vertOffset);
            }
            else
            {
                tiles[i][p].setPosition(i*horzOffset-wrapOffset, p*vertOffset+heightSqrt*size);
                tiles[i+numHorz][p].setPosition(i*horzOffset, p*vertOffset+heightSqrt*size);
                tiles[i+numHorz*2][p].setPosition(i*horzOffset+wrapOffset, p*vertOffset+heightSqrt*size);
            }
        }
    }
    sf::Shader::isAvailable();

    sf::Vector2f viewSize = view.getSize();
    
    int fpsCounter = 0;
    sf::Clock gameClock;
    sf::Time fpsDelay = sf::milliseconds(1000);
    sf::Time currTime;

    bool mouseMoved = false;
    int mouseDragPositionX;// = view.getCenter().x;
    int mouseDragPositionY;// = view.getCenter().y;

    
    bool mouseLeftDown = false;

    tile *selectedTile;
    while (window.isOpen())
    {
        currTime = gameClock.getElapsedTime();
        if(currTime.asMilliseconds() >= fpsDelay.asMilliseconds())
        {
            std::cout << "FPS: " << fpsCounter << "\t X- " << mouseDragPositionX << "\t Y- " << mouseDragPositionY << std::endl;
            fpsCounter = 0;
            gameClock.restart();
        }
        ++fpsCounter;
        for(int i = 0; i < numHorz; i++)
        {
            for(int p = 0; p < numVert; p++)
            {
//                 window.draw(tiles[i][p].getSprite());
                window.draw(tiles[i+numHorz][p].getSprite());
//                 window.draw(tiles[i+2*numHorz][p].getSprite());
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
                    movementOnX-=10;
                }
                if(downPressed)
                {
                    movementOnY+=10;
                }
                if(rightPressed)
                {
                    movementOnX+=10;
                }
                if(upPressed)
                {
                    movementOnY-=10;
                }
                view.move(movementOnX, movementOnY);
                window.setView(view);
                window.clear();
            }
            if(event.type == sf::Event::MouseWheelMoved)
            {
                if(event.mouseWheel.delta == 1)
                {
                    zoom = .75;
                }
                if(event.mouseWheel.delta == -1)
                {
                    zoom = 1.5;
                }
                //std::cout << zoom << std::endl;
                view.zoom(zoom);
		viewSize = view.getSize();
                window.setView(view);
                window.clear();
            }
            if(event.type == sf::Event::Resized)
            {
                view = sf::View(sf::FloatRect(viewSizeX, viewSizeY, event.size.width, event.size.height));
		view.setSize(viewSize);
                window.setView(view);
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
		//std::cout << "Mouse 1 Down" << std::endl;
                if(event.mouseButton.button == sf::Mouse::Left)
                {
		    mouseLeftDown = true;
		    mouseMoved = false;
                    //mouseDragTime = gameClock.getElapsedTime();
		    mouseDragPositionX = event.mouseMove.x;
		    mouseDragPositionY = event.mouseMove.y;
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
		    //std::vector<int> result = 
		    sf::Vector2f tmp;
		    tmp.x = mouseDragPositionX;
		    tmp.y = mouseDragPositionY;
		    sf::Vector2i pixel = window.mapCoordsToPixel(tmp);
		    findHexTile(pixel.x, pixel.y);
		    //selectedTile = &tiles[result[0]][result[1]];
		    //selectedTile->selectTile();
		}
	    }
            if(event.type == sf::Event::MouseMoved)
            {
		mouseMoved = true;
		if(mouseLeftDown)
		{
		    float mouseXDiff = event.mouseMove.x - mouseDragPositionX;
		    float mouseYDiff = event.mouseMove.y - mouseDragPositionY;		    
		    view.move(mouseXDiff*2, mouseYDiff*2);
		    window.setView(view);
		    window.clear();
		}
		mouseDragPositionX = event.mouseMove.x;
		mouseDragPositionY = event.mouseMove.y;
		//std::cout << mouseDragPositionY << std::endl;
            }
        }
    }
}

std::vector<int> findHexTile(float x, float y)
{
    double row = 2.0/3.0 * x / 32.0;
    double column = (1.0/3*std::sqrt(3.0) * y - 1.0/3.0 * x) / 32.0;
    std::vector<int> result;
    //result.push_back(row);
    //result.push_back(column);
    
    std::cout<<row<<" -- " << column << std::endl;
    
    return result;
}

