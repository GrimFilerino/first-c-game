#include "engine.h"
#include "engine/config/config.h"
#include "engine/entities/entity_manager.h"
#include "engine/render/render.h"
#include "engine/scenes/scene.h"
#include "engine/utils/time/time.h"

Engine engine = {0};

static void engine_run(void) {
    
    time_init(60); //fps 60
    config_init();
    SDL_Window* window = render_init();

    entity_manager_init();

    engine.is_running = true;

    memset(engine.texture_slots, 0, sizeof(engine.texture_slots));

    engine.current_scene = "menu";

    scene_init(engine.current_scene);

    while(engine.is_running) {
        time_update();
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_EVENT_QUIT:
                    engine.is_running = false;
                    break;
                default:
                    break;
            }
        }
        input_update();

        render_begin();

        scene_update(engine.current_scene);

        render_end(window, engine.texture_slots);

        time_update_late();
    }
}

void engine_init(void) {
    engine_run();
}


