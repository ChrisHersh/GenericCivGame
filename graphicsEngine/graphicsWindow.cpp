#include "graphicsWindow.h"

int main()
{
    graphicsWindow graphicsWind(50,50);
    graphicsWind.run();
}

//Initialize a bunch of shit
graphicsWindow::graphicsWindow(int numTilesHorz, int numTilesVert)
{
    numHorz = numTilesHorz;
    numVert = numTilesVert;
    viewZoomLevel = 2;
    
    size = 32;

    heightSqrt = std::sqrt(3.0)/2;
    horzOffset = (3.0/4)*(2.0*size);
    vertOffset = heightSqrt*(2.0*size);
    
    wrapOffset = numHorz*horzOffset;
    
    window = new sf::RenderWindow(sf::VideoMode(500,500), "The Hex Selection Is Done...Forever");    
    window->clear();
    float viewSizeX = 250.0;
    float viewSizeY = 250.0;
    view = sf::View(sf::Vector2f(viewSizeX,viewSizeY), sf::Vector2f(viewSizeX*2,viewSizeY*2));
    window->setView(view);
    window->display();
    createTiles();    
    sf::Shader::isAvailable();
    
    zoom = 1;
    mouseMoved = false;
    mouseDragPositionX = 0;// = view.getCenter().x;
    mouseDragPositionY = 0;// = view.getCenter().y;
    mouseLeftDown = false;
    selectedTile = 0;
}

graphicsWindow::~graphicsWindow()
{
    int fjlasdhflashdklfhjalskdhfakjfkjhasd = 0;
    int O000Ooo;
}

void graphicsWindow::run()
{    
    Vector2MM visibleRange(0,0,0,0);
    sf::Vector2f viewSize = view.getSize();
    
    //FPS stuffs
    int fpsCounter = 0;
    sf::Clock gameClock;
    sf::Time fpsDelay = sf::milliseconds(1000);
    sf::Time currTime;
    
    while (window->isOpen())
    {
	visibleRange.xMin = view.getCenter().x - view.getSize().x/2;
	visibleRange.xMax = view.getCenter().x + view.getSize().x/2;
	visibleRange.yMin = view.getCenter().y - view.getSize().y/2;
    	visibleRange.yMax = view.getCenter().y + view.getSize().y/2;
	
        currTime = gameClock.getElapsedTime();
        if(currTime.asMilliseconds() >= fpsDelay.asMilliseconds())
        {
            std::cout << "FPS: " << fpsCounter << std::endl;
            fpsCounter = 0;
            gameClock.restart();
        }
        ++fpsCounter;
	
	Vector2MM possibleRange1(0,0,0,0);
	Vector2MM possibleRange2(0,0,0,0);
	
	possibleRange1 = findMinRange();
	possibleRange2 = findMaxRange();

	int xMin = std::min(possibleRange1.xMin, possibleRange2.xMin);
	int xMax = std::max(possibleRange1.xMax, possibleRange2.xMax);
	int yMin = verifyCull(std::min(possibleRange1.yMin, possibleRange2.yMin)/2, false, numVert);
	int yMax = verifyCull(std::max(possibleRange1.yMax, possibleRange2.yMax)*2, true, numVert);
		
        for(int i = xMin; i < xMax; i++) //Works
        {
            for(int p = yMin; p < yMax; p++) //Doesn't
            {
                window->draw(tiles[i][p].getSprite());
                window->draw(tiles[i+numHorz][p].getSprite());
                window->draw(tiles[i+2*numHorz][p].getSprite());
            }
        }
        window->display();
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if(event.type == sf::Event::KeyPressed)
                keyPressedEvents();
            if(event.type == sf::Event::MouseWheelMoved)
                mouseWheelMovedEvent();
            if(event.type == sf::Event::Resized)
                resizedEvent();
            if(event.type == sf::Event::MouseButtonPressed)
                mouseButtonPressedEvent();
            if(event.type == sf::Event::MouseButtonReleased)
                mouseButtonReleasedEvent();
            if(event.type == sf::Event::MouseMoved)
               mouseMovedEvent();
        }
    }
    
    for(int i = 0; i < numHorz*3; i++)
	for(int x = 0; x < numVert; x++)
	{
	    tiles[i][x].~tile();
	}
}

void graphicsWindow::keyPressedEvents()
{
    int movementOnX = 0;
    int movementOnY = 0;

    bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);

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
    window->setView(view);
    window->clear();
}

void graphicsWindow::mouseButtonPressedEvent()
{
    if(event.mouseButton.button == sf::Mouse::Left)
    {
	mouseLeftDown = true;
	mouseMoved = false;
	mouseDragPositionX = sf::Mouse::getPosition(*window).x;
	mouseDragPositionY = sf::Mouse::getPosition(*window).y;
    }
}

void graphicsWindow::mouseButtonReleasedEvent()
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
	sf::Vector2f mapCoords = window->mapPixelToCoords(tmp, view);
	sf::Vector2f result = findHexTile(mapCoords.x, mapCoords.y);
	if(selectedTile != 0)
	    selectedTile->unselectTile();
	selectedTile = &tiles[(int)result.x+numHorz][(int)result.y];
	selectedTile->selectTile();
    }
}

void graphicsWindow::mouseWheelMovedEvent()
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
    window->setView(view);
    window->clear();
}

void graphicsWindow::mouseMovedEvent()
{
    mouseMoved = true;
    if(mouseLeftDown)
    {
    float mouseXDiff = mouseDragPositionX - sf::Mouse::getPosition(*window).x;
    float mouseYDiff = mouseDragPositionY - sf::Mouse::getPosition(*window).y;
    view.move(mouseXDiff*viewZoomLevel*2, mouseYDiff*viewZoomLevel*2);
    window->setView(view);
    window->clear();
    }

    mouseDragPositionX = sf::Mouse::getPosition(*window).x;
    mouseDragPositionY = sf::Mouse::getPosition(*window).y;
}

void graphicsWindow::resizedEvent()
{
    float viewNewX = event.size.width;
    float viewNewY = event.size.height;
    float viewRatio = viewNewX/viewNewY;
    //viewSizeX = viewSizeY*viewRatio;
    view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
    window->setView(view);
}

int graphicsWindow::verifyCull(float num, bool isUpperBound, int numTilesInDirection)
{
    if(isUpperBound)
    {
	num = ceil(num);
	if(num < 0)
	    num = 10; //Set to some default higher than 0
	num += 3; //Increase by 3 to make sure the boarders are rendered properally
	if(num > numTilesInDirection)
	    num = numTilesInDirection;
    }
    else
    {
	num = floor(num) - 3;
	if(num < 0)
	    num = 0; //Set to minimum if less than zero
    }
    return num;
}

float graphicsWindow::getXPositionForTiles(int mapPos, int i)
{
    return i*horzOffset+(wrapOffset*mapPos);
}

float graphicsWindow::getYPositionForTiles(bool oddRow, int p, int rhombusOffset)
{
    if(oddRow)
        return p*vertOffset+heightSqrt*size+rhombusOffset;
    else
        return p*vertOffset+rhombusOffset;
}

sf::Vector2f graphicsWindow::findHexTile(float x, float y)
{
    //Lowers the y so the Calculations are more accurate
    //Not always needed, more testing needs to be done
    //y = y - heightSqrt*size;

    sf::Vector2f result;
    //Pointy-top -> Flat-top swap x and y in these 2 equations
    //If res.x = (...)*y/(...) then Pointy-top
    result.x = (2.0/3)*y/size;
    result.y = ((1.0/3)*sqrt(3)*x-(1.0/3)*y)/size;
    return result;
}


Vector2MM graphicsWindow::findMinRange()
{
    Vector2MM range(0,0,0,0);
    range.xMin = verifyCull(findHexTile(visibleRange.xMin, visibleRange.yMin).x, false, numHorz);
    range.xMax = verifyCull(findHexTile(visibleRange.xMax, visibleRange.yMin).x, true, numHorz);
    range.yMin = verifyCull(findHexTile(visibleRange.xMin, visibleRange.yMin).y, false, numVert);
    range.yMax = verifyCull(findHexTile(visibleRange.xMin, visibleRange.yMax).y, true, numVert);
    
    return range;
}

Vector2MM graphicsWindow::findMaxRange()
{
    Vector2MM range(0,0,0,0);
    range.xMin = verifyCull(findHexTile(visibleRange.xMin, visibleRange.yMax).x, false, numHorz);
    range.xMax = verifyCull(findHexTile(visibleRange.xMax, visibleRange.yMax).x, true, numHorz);
    range.yMin = verifyCull(findHexTile(visibleRange.xMax, visibleRange.yMin).y, false, numVert);
    range.yMax = verifyCull(findHexTile(visibleRange.xMax, visibleRange.yMax).y, true, numVert);
    
    return range;
}

//Refuses to collapse on KDevelop
void graphicsWindow::createTiles()
{
    if(!oreText.loadFromFile("oreHex.gif"))
    {
        std::cout << "ERROR" << std::endl;
    }
    oreText.setSmooth(false);

    if(!waterText.loadFromFile("waterHex.gif"))
    {
        std::cout << "ERROR" << std::endl;;
    }
    waterText.setSmooth(false);
    
    int rhombusOffset = 0;
    int vertCount = 1;
    for(int i = 0; i < numHorz*3; i++)
    {
	std::vector<tile> tempTiles;
        for(int p = 0; p < numVert; p++)
        {
            //std::cout << i << " -- " << p << std::endl;
	    if(i < numHorz || i > numHorz*2-1)
		tempTiles.push_back(tile(&waterText));
	    else	
		tempTiles.push_back(tile(&oreText));
            //tiles[i+numHorz][p] = tile(&oreText);
           // tiles[i+numHorz*2][p] = tile(&waterText);
	}
	tiles.push_back(tempTiles);
    }
    for(int i = 0; i < numHorz; i++)
    {
        rhombusOffset += vertOffset*(vertCount%2);
	std::vector<tile> tempTiles;
        for(int p = 0; p < numVert; p++)
	{
            if(i%2 == 0)
            {
		tiles[i][p].setPosition(getYPositionForTiles(false, p, rhombusOffset), getXPositionForTiles(-1, i));
                tiles[i+numHorz][p].setPosition(getYPositionForTiles(false, p, rhombusOffset), getXPositionForTiles(0, i));
		tiles[i+numHorz*2][p].setPosition(getYPositionForTiles(false, p, rhombusOffset), getXPositionForTiles(1, i));
            }
            else
            {
                tiles[i][p].setPosition(getYPositionForTiles(true, p, rhombusOffset), getXPositionForTiles(-1, i));
		tiles[i+numHorz][p].setPosition(getYPositionForTiles(true, p, rhombusOffset), getXPositionForTiles(0, i));
		tiles[i+numHorz*2][p].setPosition(getYPositionForTiles(true, p, rhombusOffset), getXPositionForTiles(1, i));
            }
        }
        tiles.push_back(tempTiles);
        vertCount++;
    }
}