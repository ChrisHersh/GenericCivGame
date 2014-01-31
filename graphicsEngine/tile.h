#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class tile
{
public:
    tile();
    tile(sf::Texture* newText);
    tile(std::string fileName);
    sf::Texture getTexture();
    sf::Sprite getSprite();
    void setTexture(sf::Texture newText);
    void setPosition(float x, float y);
    //getUnit();
    //setUnit();
    void selectTile();
    void unselectTile();
    


private:
    void updateSprite();
    
    //Unit currUnit;
    sf::Texture *currTexture;
    sf::Sprite *currSprite;
    bool selected;
};

#endif // TILE_H
