#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cstring>

#include "GraphicsManeger.h"

const int SCREEN_Y = 600;
const int SCREEN_X = 900;
const int FONT_SIZE = 15;
const int STRING_NUMBER = 600 / (15 + 1);
const int MAX_STRING_LENGHT = 1000;

class Text: public DrawableObject
{
    public:
        sf::Text text;

        void draw(sf::RenderWindow* window)
        {
            window->draw(text);
        }

        void build(sf::Font* font, int fontSize, int x, int y, sf::Color color = sf::Color::Green)
        {
            text.setFont(*font);
            text.setCharacterSize(fontSize); // in pixeles
            text.setFillColor(sf::Color(color));
            text.setPosition(x, y);
        }

        void setString(char* VisibleText)
        {
            text.setString(VisibleText);
        }
};


int fileLen(std::ifstream* file)
{
    file->seekg(0, file->beg);    // lengh of file
    file->seekg(0, file->end);
    int len = file->tellg();
    file->seekg(0, file->beg);

    return len;
}

void correctText(char* visibleText, int* indexCounter, int* stringCounter)
{
    if (*stringCounter == STRING_NUMBER)
    {
        int nPosition = 0;
        for(int i = 0; i < MAX_STRING_LENGHT; i++) // finding position of first '\n' to move up text
            if(visibleText[i] == '\n')
            {
                nPosition = i;
                break;
            }
        
        for(int j = 0; j <= *indexCounter - nPosition - 1; j++) //deleting first string with others
        {
            visibleText[j] = visibleText[j + nPosition + 1];
        }
        for(int j = *indexCounter - nPosition; j <= *indexCounter; j++) //deleting tail
            visibleText[j] = ' ';
        
        *indexCounter = *indexCounter - nPosition - 1 ;
        *stringCounter = *stringCounter - 1;
        
    }
}

void updateVisibleText(char* visibleText, char* buffer, int* indexCounter, int* stringCounter, int* textSymbCounter, int textLen)
{
    for(int i = 0; i < 3; i++)
    {   
        if(*textSymbCounter != textLen - 1) // lemgth limit check
           { 
            visibleText[*indexCounter + 1] = buffer[*textSymbCounter + 1]; //add 3 symbols from buffer in visible text
            if(buffer[*textSymbCounter + 1] == '\n')
                *stringCounter += 1;
            *textSymbCounter += 1;
            *indexCounter += 1;
            correctText(visibleText, indexCounter, stringCounter);
           }
    }
}

 void buildText(sf::Text* text, sf::Font* font, int fontSize, int x, int y, sf::Color color = sf::Color::Green)
 {
    text->setFont(*font);
    text->setCharacterSize(fontSize); // in pixeles
    text->setFillColor(sf::Color(color));
    text->setPosition(x, y);
 }


main()
{
    //CONTEXT:

    sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "hacker typer");
    
    Text text;

    sf::Font font; 
    font.loadFromFile("arial.ttf");

    text.build(&font, FONT_SIZE, 10, 0, sf::Color::Green);                                   

    std::ifstream file;     // loading text from file into bufer
    file.open("file.txt");
    int len = fileLen(&file);
    char* buffer = new char[len];
    file.read(buffer, len);
    file.close();

    char *visibleText = new char [SCREEN_X / FONT_SIZE * SCREEN_Y / FONT_SIZE];
    memset(visibleText, ' ', SCREEN_X / FONT_SIZE * SCREEN_Y / FONT_SIZE ); // set ' ' to all elements

    int indexCounter = -1;
    int stringCounter = 0;
    int textSymbCounter = -1;

    GraphicsManager manager;
    manager.registrate(&text);

    sf::Event event;

    bool isNeedUpdate = false;

    while (window.isOpen())
    {
        //PHISICS: 
        //no phisics in this programm

        //EVEN HANDLER
        while (window.pollEvent(event))
		{
		    if (event.type == sf::Event::Closed) 
                  window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) // exit by escape
                    window.close();

                isNeedUpdate = true; //by any button
			}
		}
        //LOGICS:
        if(isNeedUpdate)
        {
            updateVisibleText(visibleText, buffer, &indexCounter, &stringCounter, &textSymbCounter, len);
            text.setString(visibleText);
            isNeedUpdate = false;
        }

        //GRAPHICS:
        window.clear();
        manager.drawAll(&window);
        window.display();

    }

    delete[] buffer;
    delete[] visibleText;

}