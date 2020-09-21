#include "Screen.h"
#include <iomanip>

namespace screen{

Screen::Screen():
    m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer(NULL), m_buffer1(NULL){}

bool Screen::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }


    m_buffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
    m_buffer1 = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];

    m_window = SDL_CreateWindow("Particle Fire Explosion",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN
                                          );

    if (m_window == NULL){
        SDL_Quit(); delete m_buffer;
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    if(m_renderer == NULL || m_texture == NULL){
        SDL_DestroyWindow(m_window);
        SDL_Quit(); delete m_buffer;
        return false;
    }

    m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

	memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

    return true;
}

void Screen::update()
{
    SDL_UpdateTexture(m_texture, NULL, m_buffer, SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

bool Screen::processEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT)
            return false;
    }
    return true;
}

//void Screen::clear()
//{
//    memset(m_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));
//    memset(m_buffer1, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));
//}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
{
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

    Uint32 color = 0;

    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<=8;
    color += 0xFF;


    m_buffer[ (y*SCREEN_WIDTH) + x] = color;
}

void Screen::boxBlur()
{
    Uint32* temp = m_buffer;
    m_buffer = m_buffer1;
    m_buffer1 = temp;

    for(int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for(int x = 0; x < SCREEN_WIDTH; x++)
        {

            int redTotal = 0;
            int greenTotal = 0;
            int blueTotal = 0;

            for(short row = -1; row <= 1; row++)
            {
                for(short col = -1; col <=1; col++)
                {
                    int curX = x + col;
                    int curY= y + row;
                    if(curX >= 0 && curX < SCREEN_WIDTH && curY > 0 && curY < SCREEN_HEIGHT)
                    {
                        Uint32 color = m_buffer1[curY * SCREEN_WIDTH + curX];

                        Uint8 red = (color & 0xFF000000)>> 24;
                        Uint8 green = (color & 0x00FF0000) >> 16;
                        Uint8 blue = (color & 0x0000FF00) >> 8;

                        redTotal += red;
                        greenTotal += green;
                        blueTotal += blue;
                    }
                }
            }

            Uint8 red = redTotal/9;
            Uint8 green = greenTotal/9;
            Uint8 blue = blueTotal/9;

            setPixel(x, y, red, green, blue);
        }
    }


}

void Screen::close()
{
    delete [] m_buffer;
    delete [] m_buffer1;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return;
}
}
