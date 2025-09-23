/*
    file: Gamepad.h
    written by Elias Geiger
*/

#pragma once

// includes
#include <SDL.h>
#include <iostream>

class Gamepad 
{
public:
    Gamepad();
    virtual ~Gamepad();

    bool init();
    void shutdown();
    SDL_JoystickID get_instance_id() const;

private:
    SDL_GameController* m_gc;
    SDL_JoystickID m_jid;
};