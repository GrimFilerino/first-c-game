#include <stdbool.h>
#include <glad/glad.h>
#include <SDL3/SDL.h>

#define SDL_MAIN_HANDLED

#include "engine/engine.h"

int main(int argc, char *argv[]) {
    engine_init();
    return 0;
}

