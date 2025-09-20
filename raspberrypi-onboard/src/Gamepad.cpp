#include "Gamepad.h"

// definitions //

Gamepad::Gamepad()
{
    this->m_gc = nullptr;
}

Gamepad::~Gamepad()
{
    this->m_gc = nullptr;
}

bool Gamepad::init()
{
    // init SDL API
    if(SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to init SDL API: " << SDL_GetError() << std::endl;
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
        std::cerr << "No game controller found!" << std::endl;
        SDL_Quit();  
        return false;
    }
    std::cout << "Opend: " << SDL_GameControllerName(this->m_gc) << std::endl;

    return true;
}

void Gamepad::shutdown()
{
    if(this->m_gc) {
        SDL_GameControllerClose(this->m_gc);
        this->m_gc = nullptr;
    }
    SDL_Quit();
}
