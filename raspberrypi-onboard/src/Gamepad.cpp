/*
    file: Gamepad.cpp
    written by Elias Geiger
*/

#include "Gamepad.h"

// definitions //

Gamepad::Gamepad()
{
    this->m_gc = nullptr;
    this->m_jid = -1;
}

Gamepad::~Gamepad()
{
    try {
        this->shutdown();
    } catch(...) {}
}

bool Gamepad::init()
{
    // init SDL API
    if(SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_EVENTS) != 0) {
        std::cerr << "[Gamepad] " << "Failed to init SDL API: " << SDL_GetError() << std::endl;
        return false;
    }

    // open first controller available
    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        if(SDL_IsGameController(i)) {
            this->m_gc = SDL_GameControllerOpen(i);
            if(this->m_gc) {
                break;
            }
        }
    }

    if(!this->m_gc) {
        std::cerr << "[Gamepad] " << "No game controller found!" << std::endl;
        SDL_Quit();  
        return false;
    }

    SDL_Joystick* joystick = SDL_GameControllerGetJoystick(this->m_gc);
    this->m_jid = SDL_JoystickInstanceID(joystick);

    std::cout << "[Gamepad] " << "Opened: " << SDL_GameControllerName(this->m_gc) << " | ID: " << this->m_jid << std::endl;

    return true;
}

void Gamepad::shutdown()
{
    if(this->m_gc) {
        SDL_GameControllerClose(this->m_gc);
        this->m_gc = nullptr;
        this->m_jid = -1;
    }
    SDL_Quit();
}

SDL_JoystickID Gamepad::get_instance_id() const
{
    return this->m_jid;
}
