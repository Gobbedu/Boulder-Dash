/* =======================================================  */
/*   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves   */
/*   GRR20203892                                            */
/*   programa BOULDER DASH                                  */
/*  ======================================================= */
#include "structure.h"

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

ALLEGRO_FONT* font;
ALLEGRO_FONT* font_pause;
ALLEGRO_FONT* font_2game;
ALLEGRO_FONT* font_start2help;

unsigned char key[ALLEGRO_KEY_MAX];

struct GAME_DATA GAME_BOARD;
struct GAME_STATES GAME_STATE;
struct PLAYER_T player;
struct SPRITES_T sprites;
struct SOUNDS_T sounds;

time_t thisSecond;
time_t pastSecond;
long frames;

int main()
{
    srand(time(NULL));
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();

    must_init(al_init_image_addon(), "image");
    must_init(al_init_font_addon(), "font addon");
    must_init(al_init_ttf_addon(), "ttf addon");

    sprites_init();
    hud_init();
    audio_init();

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    
    states_init();
    grid_init();
    player_init();


    frames = 0;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE ks;

    al_start_timer(timer);

    while(1)
    {
        al_wait_for_event(queue, &event);

        states_update(event, ks);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                /* update if NOTpause and NOThelp */
                if( GAME_STATE.on_game && 
                (!GAME_STATE.on_help && !GAME_STATE.on_pause))
                {
                    player_update();
                    grid_update();
                    hud_update();
                }

                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;
                
                redraw = true;
                frames++;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        keyboard_update(&event);

        if(redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0,0,0));

            states_draw();

            disp_post_draw();
            redraw = false;
        }
    }

    sprites_deinit();
    grid_deinit();
    hud_deinit();

    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
