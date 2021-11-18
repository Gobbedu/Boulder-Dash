#ifndef __STRUCTURES__
#define __STRUCTURES__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>


ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

// =============== DISPLAY STUFF ===============
#define DISP_BLK_W 40
#define DISP_BLK_H 23
#define GRID_BLK_W 38
#define GRID_BLK_H 20

#define BUFFER_W SPRITE_W * DISP_BLK_W
#define BUFFER_H SPRITE_H * DISP_BLK_H

#define DISP_SCALE 0.9
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

void disp_init(void);
void disp_deinit(void);
void disp_pre_draw(void);
void disp_post_draw(void);
// ======================= PLAYER STUFF =======================
/* speed tem que ser multiplo de sprite_w && sprite_h */
#define player_SPEED 5
#define player_MAX_X (BUFFER_W - 2 * SPRITE_W)
#define player_MAX_Y (BUFFER_H - 2 * SPRITE_H)

struct PLAYER_T
{
    int x, y;
    int absx, absy;
    int lives;
    int respawn_timer;
    int invincible_timer;
};


void player_init(void);
void player_deinit(void);
void player_update(void);
void player_kill(int i, int j);
int valid_move(int i, int j);
void move_player(int i, int j);
int player_can_move(int i, int j);


// ==================== GRID STUFF ====================
struct ENTITY {
    int type;
    int prevh;
    int prevw;
    int frame;
};

struct GAME_DATA {
    struct ENTITY **grid; /* entity grid */
    int playerw;
    int playerh;
    
    int lv; 
    int last_lv;

    int lv_seconds_left;
    int lv_seconds;

    int pass_level;
    int diamonds_caught;
    int diamonds_value;
    int d_value_doubled;
    int diamonds_total;
    long game_score;

};

void grid_init(void );
void grid_deinit(void );
void grid_update(void );
void grid_update_objs(void );
void boulder_update(void); /* falling boulder && diamonds */
int fall_left(int i, int j);
int fall_right(int i, int j);
void enemy_update(void);
void firefly_update(void);
void grid_draw(void );

/* ================== STATES STUFF ================== */
struct GAME_STATES{
    int on_pause;
    int on_help;
    int on_game;
    int on_start;
    int on_lost;
    int on_win;
    int on_retry;
    int on_high_scores;
};

void states_update(ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE ks);
void states_draw(void);
void states_init(void);
void start_draw(void);
void help_draw(void);
void pause_draw(void);
void lost_draw(void);
void win_draw(void);
void high_scores_draw(void);
void bubble_sort(long a[], int size);


// ==================== SPRITES STUFF ==================== 
//32x32px
#define SPRITE_W 32
#define SPRITE_H 32
enum TILE_TYPE {
    EMPTY = 0,
    PLAYER,
    DIRT,
    BOULDER,
    WALL,
    DIAMOND,
    EXIT,
    TNT,
    NUM_TILES 
};

struct SPRITES_T
{
    ALLEGRO_BITMAP* characterSheet;
    ALLEGRO_BITMAP* boulderSheet;
    ALLEGRO_BITMAP* gemSheet;

    ALLEGRO_BITMAP* logo;
    ALLEGRO_BITMAP* boulder;
    ALLEGRO_BITMAP* steelWall;  
    
    ALLEGRO_BITMAP* player[12];

    ALLEGRO_BITMAP* wall[4];        
    ALLEGRO_BITMAP* dirt[8];        
    ALLEGRO_BITMAP* exit[2];        
    ALLEGRO_BITMAP* explosion[4];
    ALLEGRO_BITMAP* diamont[4];     
};

ALLEGRO_BITMAP* sprite_grab(char* local);
void sprites_init(void);
void sprites_deinit(void);
void sprite_direction(void);
void load_player_sprites(char *source);

//  ----------------- font -----------------
ALLEGRO_FONT* font;
ALLEGRO_FONT* font_pause;
ALLEGRO_FONT* font_2game;
ALLEGRO_FONT* font_start2help;

void hud_init();
void hud_deinit();
void hud_update();
void hud_draw();

// ----- keyboard ------
#define KEY_SEEN     1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

// ---------- audio -------------
void audio_init(void);
void audio_deinit(void);

struct SOUNDS_T{
    ALLEGRO_SAMPLE* sample_kaboom;
    ALLEGRO_SAMPLE* sample_win;
    ALLEGRO_SAMPLE* sample_lose;
    ALLEGRO_SAMPLE* sample_mine;
    ALLEGRO_SAMPLE* sample_lvup;
    ALLEGRO_SAMPLE* sample_cheers;
};

void keyboard_init(void);
void keyboard_update(ALLEGRO_EVENT* event);

// ------- helper functions -------
void must_init(bool test, const char* msg);
void pick_level(int lv);


// ===================== GLOBAL VARIABLES =====================
struct GAME_DATA GAME_BOARD;
struct GAME_STATES GAME_STATE;
struct PLAYER_T player;
struct SPRITES_T sprites;
struct SOUNDS_T sounds;


long frames;
time_t thisSecond;
time_t pastSecond;


#endif