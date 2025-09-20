#pragma once

// includes
#include <SDL.h>
#include <iostream>

class Gamepad 
{
public:
    Gamepad();
    ~Gamepad();

    bool init();
    void shutdown();

private:
    SDL_GameController* m_gc;
};