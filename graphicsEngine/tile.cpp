#include "tile.h"

tile::tile()
{

}

tile::tile(sf::Texture* newText)
{
    selected = false;
    currTexture = newText;
    updateSprite();
}

tile::tile(std::string fileName)
{
    if(!currTexture->loadFromFile(fileName))
    {
        std::cout << "ERROR";
    }
    currTexture->isSmooth();
    tile(currTexture);
}

sf::Texture tile::getTexture()
{
    return *currTexture;
}

sf::Sprite tile::getSprite()
{
    return *currSprite;
}

void tile::setPosition(float x, float y)
{
    xPos = x;
    yPos = y;
    currSprite->setPosition(xPos, yPos);
}

void tile::selectTile()
{
    selected = true;
    updateSprite();
}

void tile::unselectTile()
{
    selected = false;
    updateSprite();
}

void tile::updateSprite()
{
    //std::cout<<"Update" << std::endl;
    currSprite = new sf::Sprite(*currTexture);
    if(selected)
    {
	currSprite->setColor(sf::Color::Red);
    }
    currSprite->setPosition(xPos, yPos);
    currSprite->setRotation(60);
}
//tile::getUnit()
//{
//
//}

//tile::setUnit()
//{
//    //Need to refactor this
//}
