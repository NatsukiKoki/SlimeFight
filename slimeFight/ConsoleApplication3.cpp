// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*

#include <iostream>

#include<SFML/Graphics.hpp>

#include<box2d/box2d.h>

#include"map.h"

using namespace sf;


int main()
{
    VideoMode vm(1920, 1000);
    RenderWindow window(vm, "testmap", Style::Default);
    Image red;
    red.create(10, 10, Color::Red);
    Image blue;
    blue.create(10, 10, Color::Blue);

    Texture styleA;
    Texture styleB;
    styleA.loadFromImage(red);
    styleB.loadFromImage(blue);

    map kk(220, 120);

    //kk.testShow(&styleA,&styleB);
    //kk.test_setTexture(styleA, styleB);

    while (window.isOpen())
    {

        
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            kk.setRandomRoom(500);
            kk.setRandomRoom(500);
            kk.beforeCreateMaze();
            kk.makeRoute();
            kk.degenerate();
            kk.setbrick();
            kk.testShow(&styleA, &styleB);
        }
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            //kk.beforeCreateMaze();
            //kk.makeRoute();
            //kk.testShow(&styleA, &styleB);
        }
        if (Keyboard::isKeyPressed(Keyboard::LAlt))
        {
            //kk.makeRoute();
            //kk.testShow(&styleA, &styleB);
        }
        if (Keyboard::isKeyPressed(Keyboard::RAlt))
        {
            //kk.degenerate();
            //kk.testShow(&styleA, &styleB);
        }
        window.clear();
        for (int i = 0; i < kk.visableBrick.size(); i++)
        {
            window.draw(kk.visableBrick[i]);
        }
        window.display();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
*/