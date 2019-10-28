#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

const int SCREEN_Y = 600;
const int SCREEN_X = 900;
const int FONT_SIZE = 15;
const int STRING_NUMBER = 600 / (15 +2);

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
        for(int i = 0; i < 1000; i++) // finding position of first '\n' to move up text
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

void updateVisibleText(char* visibleText, char* buffer, int* indexCounter, int* stringCounter, int* textSymbCounter)
{
    for(int i = 0; i < 3; i++)
    {   visibleText[*indexCounter + 1] = buffer[*textSymbCounter + 1]; //add 3 symbols from buffer in visible text
        if(buffer[*textSymbCounter + 1] == '\n')
            *stringCounter += 1;
        *textSymbCounter += 1;
        *indexCounter += 1;
        correctText(visibleText, indexCounter, stringCounter);
    }
}


main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "hacker typer");
    
    sf::Text text;

    sf::Font font; 
    font.loadFromFile("arial.ttf");

    text.setFont(font);
    text.setCharacterSize(FONT_SIZE); // in pixeles
    text.setFillColor(sf::Color::Green);
    text.setPosition(10, 0);

    std::ifstream file;     // loading text from file into bufer
    file.open("file.txt");
    int len = fileLen(&file);
    char* buffer = new char[len];
    file.read(buffer, len);
    file.close();

    char *visibleText = new char [SCREEN_X / FONT_SIZE * SCREEN_Y / FONT_SIZE]; 
    for (int i = 0; i < (SCREEN_X / FONT_SIZE * SCREEN_Y / FONT_SIZE); i++)
        visibleText[i] = ' ';

    int indexCounter = -1;
    int stringCounter = 0;
    int textSymbCounter = -1;
  
    sf::Event event;
    
    while (window.isOpen())
    {
        while (window.pollEvent(event))
		{
		    if (event.type == sf::Event::Closed) 
                  window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) // exit by escape
                    window.close();

                updateVisibleText(visibleText, buffer, &indexCounter, &stringCounter, &textSymbCounter);    // by any button
                text.setString(visibleText);     
			}
		}

        window.clear();
        window.draw(text);
        window.display();

    }

    delete[] buffer;
    delete[] visibleText;

}