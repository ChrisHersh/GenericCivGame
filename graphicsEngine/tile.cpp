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
    currSprite->setPosition(x, y);
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
    currSprite = new sf::Sprite(*currTexture);
    if(selected)
    {
	currSprite->setColor(sf::Color::Red);
    }
}
//tile::getUnit()
//{
//
//}

//tile::setUnit()
//{
//    //Need to refactor this
//}
