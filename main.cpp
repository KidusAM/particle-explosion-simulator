#include <iostream>
#include <SDL.h>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <Swarm.h>

#include "Screen.h"

using namespace std;
using namespace screen;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    Screen screen;

    if(screen.init() == false){
        cout << "Error initializing screen" << endl;
    }

    Swarm swarm;

    int counter;
    while(true){
         //Update particles, draw them etc...
        if (!screen.processEvents()) break;

        int elapsed = SDL_GetTicks();

//        screen.clear();
        swarm.update(elapsed);

        const Particle * const pParticles = swarm.getParticles();

        unsigned char red = (1 + sin(counter)) * 128;
        unsigned char green = (1 + sin(elapsed * 0.0007)) * 128;
        unsigned char blue = (1 + sin(elapsed * 0.0009)) * 128;


        for(int i = 0; i < Swarm::NPARTICLES; i++ )
        {
            Particle particle = pParticles[i];

            int x = (particle.m_x + 1) * 0.5 * Screen::SCREEN_WIDTH;
            int y = particle.m_y * 0.5 * Screen::SCREEN_WIDTH + (Screen::SCREEN_HEIGHT * 0.5);

            screen.setPixel(x, y, red, green, blue);

        }

        screen.boxBlur();
//        screen.setPixel(400, 300, 0, 0, 0);
        screen.update();
    }

    screen.close();


    return 0;
}
