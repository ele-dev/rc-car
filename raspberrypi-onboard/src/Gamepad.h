#pragma once

// includes
#include <SDL.h>

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