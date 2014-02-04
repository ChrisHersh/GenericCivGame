#include <SFML/Graphics.hpp>
#include <sys/resource.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "tile.h"

sf::Vector2f findHexTile(float mouseX, float mouseY);
sf::Vector2f roundToNearestHex(sf::Vector2f coords);

double height; 
double width;

int main()
{
    int size = 32;
    double zoom = 1;

    int numHorz= 20;
    int numVert = 20;

    double heightSqrt = std::sqrt(3.0)/2;
    double horzOffset = height = (3.0/4)*(2.0*size);
    double vertOffset = width = heightSqrt*(2.0*size);

    int wrapOffset = numHorz*horzOffset;

    std::cout << heightSqrt << " -\t- " << vertOffset << std::endl;

    sf::RenderWindow window(sf::VideoMode(500,500), "Coordinates are as evil as printers");
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
            //std::cout << i << " -- " << p << std::endl;
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
    int mouseDragPositionX = 0;// = view.getCenter().x;
    int mouseDragPositionY = 0;// = view.getCenter().y;

    
    bool mouseLeftDown = false;

    tile *selectedTile;
    
    tiles[2+numHorz][3].selectTile();
    
    while (window.isOpen())
    {
//         currTime = gameClock.getElapsedTime();
//         if(currTime.asMilliseconds() >= fpsDelay.asMilliseconds())
//         {
//             std::cout << "FPS: " << fpsCounter << "\t X- " << mouseDragPositionX << "\t Y- " << mouseDragPositionY << std::endl;
//             fpsCounter = 0;
//             gameClock.restart();
//         }
//         ++fpsCounter;
        for(int i = 0; i < numHorz; i++)
        {
            for(int p = 0; p < numVert; p++)
            {
//                 window.draw(tiles[i][p].getSprite());
		//tiles[i+numHorz][p].updateSprite();
                window.draw(tiles[i+numHorz][p].getSprite());
//                 window.draw(tiles[i+2*numHorz][p].getSprite());
            }
        }
        tiles[0][0].setPosition(0,0);
	window.draw(tiles[0][0].getSprite());
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
		    sf::Vector2f pixel = window.mapPixelToCoords(tmp, view);
		    sf::Vector2f result = findHexTile(pixel.x, pixel.y);
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
		    view.move(mouseXDiff*5, mouseYDiff*5);
		    window.setView(view);
		    window.clear();
		}
		
		mouseDragPositionX = sf::Mouse::getPosition(window).x;
		mouseDragPositionY = sf::Mouse::getPosition(window).y;
            }
        }
    }
}

sf::Vector2f findHexTile(float mouseX, float mouseY)
{
    int intHeight = round(height);
    int intWidth =  round(width);
//     std::cout<< "FindHex     X -- " << x << "\tY -- " << y << std::endl;
//     double row = 2.0/3.0 * x / 32.0;
//     double column = (1.0/3*std::sqrt(3.0) * y - 1.0/3.0 * x) / 32.0;
//     sf::Vector2f result;
//     result.x = row;
//     result.y = column;
//     
//     std::cout<<row<<" -- " << column << std::endl;
//     
//     return roundToNearestHex(result);
    
//     int row = (int) mouseY / intHeight;
//     int column = (row % 2 != 0 ? (int) (mouseX / intWidth) : (int) (mouseX + intWidth/2) / intWidth);
//     //std::cout<<"X -- " << column << "\tY -- " << row <<std::endl; 
//     int lineY = mouseY - (row * intHeight);
//     int lineX = (row % 2 != 0 ? mouseX - (column * intWidth) : mouseX - (column * intWidth) + intWidth/2);
// 
//     if (lineY + (lineX / 2) < 10) // LEFT triangle
// 	mouseY -= 20;
//     else if (lineY - (lineX / 2) < -10) // RIGHT triangle
// 	mouseY -= 20;

//     tileY = (int) mouseY / 30;
//     tileX = (tileY % 2 != 0 ? (int) (mouseX / 40) : (int) (mouseX + 20) / 40);
    
    sf::Vector2f result;
    result.y = (int) mouseY / intHeight;//(int) mouseY / intHeight;
    result.x = ((int)result.y % 2 != 0 ? (int) mouseX / intWidth : (int) (mouseX + intWidth/2) / intWidth);//((int)result.y % 2 != 0 ? (int) (mouseX / intWidth) : (int) (mouseX + intWidth/2) / intWidth);
    std::cout<<"X -- " << result.x << "\tY -- " << result.y <<std::endl; 
    return result;
}

sf::Vector2f roundToNearestHex(sf::Vector2f coords)
{
    int roundX = round(coords.x);
    int roundY = round(coords.y);
    
    coords.x = roundX;
    coords.y = roundY;
    
    return coords;
}

