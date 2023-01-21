/* =======================================================  */
/*   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves   */
/*   GRR20203892                                            */
/*   programa BOULDER DASH                                  */
/*  ======================================================= */
#include "structure.h"

extern ALLEGRO_DISPLAY* disp;
extern ALLEGRO_BITMAP* buffer;

extern ALLEGRO_FONT* font;
extern ALLEGRO_FONT* font_pause;
extern ALLEGRO_FONT* font_2game;
extern ALLEGRO_FONT* font_start2help;

extern unsigned char key[ALLEGRO_KEY_MAX];

extern struct GAME_DATA GAME_BOARD;
extern struct PLAYER_T player;
extern struct SPRITES_T sprites;
extern struct SOUNDS_T sounds;

extern time_t pastSecond;

// ==================== SPRITES ====================
ALLEGRO_BITMAP* sprite_grab(char* local)
{
    char aux[200];
    strcpy(aux, "./resources/sprites/");
    strcat(aux, local);
    ALLEGRO_BITMAP* sprite = al_load_bitmap(aux);
    if (sprite == NULL){ fprintf(stderr, "ponteiro null carrengando %s\n", aux); exit(1);}
    must_init(sprite, aux);

    return sprite;
}

void load_player_sprites(char *source)
{
    int i, j, k;
    sprites.characterSheet = al_load_bitmap(source);
    must_init(sprites.characterSheet, "character sheet");

    k = 0;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 3; j++)
        {
            sprites.player[k] = al_create_sub_bitmap(sprites.characterSheet, j*SPRITE_W, i*SPRITE_H, SPRITE_W, SPRITE_H);
            k++;
        }
}

void sprites_init()
{
    /*
    Attribution Instructions: 
    Please include my name "Clint Bellanger" among your credits. 
    Optionally, also include a link to this page
    or to http://clintbellanger.net or to http://heroinedusk.com
    */

    // carrega sprites player
    load_player_sprites("./resources/characters/santa.png");

    sprites.logo = al_load_bitmap("./resources/sprites/logo.png");
    must_init(sprites.logo, "logo image");
    
    sprites.boulderSheet = al_load_bitmap("./resources/sprites/rock.png");
    must_init(sprites.boulderSheet, "boulder sheet");

    sprites.boulder = al_create_sub_bitmap(sprites.boulderSheet, 272, 0, 136, 128);

    sprites.gemSheet = al_load_bitmap("./resources/sprites/gems_db16.png");
    must_init(sprites.gemSheet, "gems sheet");

    sprites.dirt = sprite_grab("pebble_brown0.png");
    sprites.wall = sprite_grab("brick_brown-vines1.png");
    sprites.steelWall = sprite_grab("mesh3.png");

    sprites.explosion[0] = sprite_grab("lava0.png");
    sprites.explosion[1] = sprite_grab("lava1.png");
    sprites.explosion[2] = sprite_grab("lava2.png");
    sprites.explosion[3] = sprite_grab("lava3.png");

    sprites.diamont[0] = al_create_sub_bitmap(sprites.gemSheet, 0, 32, 32, 32);
    sprites.diamont[1] = al_create_sub_bitmap(sprites.gemSheet, 0, 64, 32, 32);
    sprites.diamont[2] = al_create_sub_bitmap(sprites.gemSheet, 32, 32, 32, 32);
    sprites.diamont[3] = al_create_sub_bitmap(sprites.gemSheet, 32, 64, 32, 32);
    
    sprites.exit[0] = sprite_grab("closed_door.png");
    sprites.exit[1] = sprite_grab("open_door.png");

}

void sprites_deinit()
{
    int i;

    al_destroy_bitmap(sprites.characterSheet);
    al_destroy_bitmap(sprites.gemSheet);
    al_destroy_bitmap(sprites.boulderSheet);

    al_destroy_bitmap(sprites.logo);
    al_destroy_bitmap(sprites.boulder);
    al_destroy_bitmap(sprites.dirt);
    al_destroy_bitmap(sprites.wall);

    for(i = 0; i < 12; i++)
    {
        al_destroy_bitmap(sprites.player[i]);
    }

    for(i = 0; i <= 3; i++){
        al_destroy_bitmap(sprites.explosion[i]);
    }

    al_destroy_bitmap(sprites.diamont[0]);
    al_destroy_bitmap(sprites.diamont[1]);



    al_destroy_bitmap(sprites.exit[0]);
    al_destroy_bitmap(sprites.exit[1]);
    
    al_destroy_bitmap(sprites.steelWall);

}

// ==================== DISPLAY ====================
void disp_init(void)
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit(void)
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}
// acumula mudancas no buffer
void disp_pre_draw(void)
{
    al_set_target_bitmap(buffer);
}
// desenha todas as mudancas na tela 
void disp_post_draw(void)
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}

// ==================== SOUND ===================
void audio_init()
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    sounds.sample_kaboom = al_load_sample("./resources/sounds/explosion.wav");
    must_init(sounds.sample_kaboom, "kabomm sample");

    sounds.sample_win = al_load_sample("./resources/sounds/winning.wav");
    must_init(sounds.sample_kaboom, "win sample");

    sounds.sample_lose = al_load_sample("./resources/sounds/lose.wav");
    must_init(sounds.sample_kaboom, "lose sample");

    sounds.sample_mine = al_load_sample("./resources/sounds/mine.wav");
    must_init(sounds.sample_kaboom, "mining sample");

    sounds.sample_lvup = al_load_sample("./resources/sounds/next-level.wav");
    must_init(sounds.sample_kaboom, "mining sample");

    sounds.sample_cheers = al_load_sample("./resources/sounds/cheering.wav");
    must_init(sounds.sample_kaboom, "mining sample");
}

void audio_deinit()
{
    al_destroy_sample(sounds.sample_kaboom);
    al_destroy_sample(sounds.sample_win);
    al_destroy_sample(sounds.sample_lose);
    al_destroy_sample(sounds.sample_mine);
}

//  ==================== PLACAR ====================
void hud_init()
{
    font = al_load_ttf_font("./resources/sprites/allegro-cartoon.ttf", 25, 0);
    must_init(font, "font");

    font_pause  = al_load_ttf_font("./resources/sprites/allegro-cartoon.ttf", 100, 0);
    must_init(font_pause, "fonte_pause");

    font_2game = al_load_ttf_font("./resources/sprites/allegro-cartoon.ttf", 50, 0);
    must_init(font_2game, "font -space to continue- at start");

    font_start2help = al_load_ttf_font("./resources/sprites/allegro-cartoon.ttf", 20, 0);
    must_init(font_start2help, "font -H/F1 to help- at start");

    pastSecond = time(NULL);
}

void hud_deinit()
{
    al_destroy_font(font);
    al_destroy_font(font_pause);
    al_destroy_font(font_2game);
    al_destroy_font(font_start2help);
}

/* updates some funtions at each second */
void hud_update()
{

}

void hud_draw()
{
    al_draw_filled_rectangle(0, 0, BUFFER_W, SPRITE_H, al_map_rgb(10, 15, 20));

    /* total session game points */
    al_draw_textf(
        font,
        al_map_rgb(255, 255, 255) ,
        (DISP_BLK_W - 8) * SPRITE_W, 10,
        0,
        "%010ld",
        GAME_BOARD.game_score
    );

    /* time left for level */
    al_draw_textf(
        font,
        al_map_rgb(255, 255, 255),
        (DISP_BLK_W - 15) * SPRITE_W, 10,
        0,
        "%03d",
        GAME_BOARD.lv_seconds_left
    );

    /* number of diamonds caught on level */
    al_draw_textf(
        font,
        al_map_rgb(255, 255, 0),
        13*SPRITE_W, 10,
        0,
        "%d",
        GAME_BOARD.diamonds_caught
    );

    /* number of lives left  */
    al_draw_textf(
        font,
        al_map_rgb(220, 0, 0),
        17*SPRITE_W, 10,
        0,
        "%d",
        player.lives
    );

    /* value per diamond */
    al_draw_textf(
        font,
        al_map_rgb(255, 255, 0),
        3*SPRITE_W, 10,
        0,
        "%d",
        GAME_BOARD.pass_level
    );

    // al_draw_scaled_bitmap(sprites.diamont[3], 0, 0, SPRITE_H, SPRITE_W, SPRITE_W-6, (2*SPRITE_H)-6, 45, 45, 0);
    al_draw_bitmap(sprites.diamont[3], 4.3*SPRITE_W, 0, 1);

    /* total number of diamonds on level */
    al_draw_textf(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 10,
        0,
        "     %d",
        GAME_BOARD.diamonds_value
    );


}

// ==================== KEYBOARD ====================
// headers on game.h
void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch(event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

//  ======================= HELPER =======================
void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}
