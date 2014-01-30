#include "tile.h"

tile::tile()
{

}

tile::tile(sf::Texture* newText)
{
//     currTexture = newText;
// 
//     currTexture.isSmooth();
    currSprite = new sf::Sprite(*newText);
    currSprite->setTexture(*newText);
}

tile::tile(std::string fileName)
{
    if(!currTexture.loadFromFile(fileName))
    {
        std::cout << "ERROR";
    }
    currTexture.isSmooth();
    currSprite = new sf::Sprite(currTexture);
    currSprite->setTexture(currTexture);
}

sf::Texture tile::getTexture()
{
    return currTexture;
}

sf::Sprite tile::getSprite()
{
    return *currSprite;
}

void tile::setPosition(float x, float y)
{
    currSprite->setPosition(x, y);
}

//tile::getUnit()
//{
//
//}

//tile::setUnit()
//{
//    //Need to refactor this
//}
