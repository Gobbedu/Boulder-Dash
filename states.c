/* =======================================================  */
/*   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves   */
/*   GRR20203892                                            */
/*   programa BOULDER DASH                                  */
/*  ======================================================= */

#include "structure.h"

void states_init(void){
    GAME_STATE.on_start = 1;
    GAME_STATE.on_win = 0;
    GAME_STATE.on_lost = 0;
    GAME_STATE.on_help = 0;
    GAME_STATE.on_game = 0;
    GAME_STATE.on_pause = 0;
    GAME_STATE.on_retry = 0;
    GAME_STATE.on_high_scores = 0;
}

void bubble_sort(long a[], int size)
{
    int i, j;
    long aux;

    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            if(a[j] < a[j+1]){
                aux = a[j];
                a[j] = a[j+1];
                a[j+1] = aux;
            }
}

void high_scores_draw(void)
{
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgb(10, 15, 20));
    int i;
    long hs[11];

    FILE* scores;
    scores = fopen("data/scores.txt", "r");

    for(i = 0; i < 10; i++){
        fscanf(scores, "%ld", &hs[i]);
    }

    if( (GAME_STATE.on_lost || GAME_STATE.on_win) && GAME_BOARD.lv > 0)
    {

        hs[10] = GAME_BOARD.game_score;
        bubble_sort(hs, 10);
        fclose(scores);
        scores = fopen("data/scores.txt", "w");

        for( i = 0; i < 10; i++){
            fprintf(scores, "%li\n", hs[i]);
            // printf("hs %d : %ld\n",i,  hs[i]);
        }

        GAME_BOARD.lv = 0;

    }
    


    al_draw_textf(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W , 2*SPRITE_H,
        0,
        "HIGH SCORES:"
    );
            
    al_draw_textf(
    font,
    al_map_rgb(255, 255, 255),
    20*SPRITE_W , 2*SPRITE_H,
    0,
    "YOUR SCORE: %ld", GAME_BOARD.game_score
    );
    for(i = 0; i < 10; i++){

        al_draw_textf(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W , (i)*SPRITE_H + 4*SPRITE_H,
        0,
        "%.2i:   %ld", i + 1, hs[i]
        );
    }

    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        BUFFER_W - 15*SPRITE_W, BUFFER_H - 2*SPRITE_H,
        0,
        "Feito por  Eduardo  Gobbo  W.V.G"
    );

    fclose(scores);
}

void pause_draw(void)
{
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0, 0, 0, 0.8));

    al_draw_text(
        font_pause,
        al_map_rgb(255, 255, 255),
        BUFFER_W/2, BUFFER_H/2,
        ALLEGRO_ALIGN_CENTER,
        "PAUSE"
    );

    
}

void help_draw(void)
{
    al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgb(10, 15, 20));

    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 2*SPRITE_H,
        ALLEGRO_ALIGN_CENTER,
        "HELP ME"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 3*SPRITE_H,
        0,
        "F1 ou H -> mostra essa mensagem"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 5*SPRITE_H,
        0,
        "S -> mostra os HIGH SCORES"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 7*SPRITE_H,
        0,
        "R -> reinicia nivel, perde uma vida"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 9*SPRITE_H,
        0,
        "Space -> Pausa o jogo"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , DISP_H - 2*SPRITE_W,
        0,
        "ESC: sair"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , DISP_H - SPRITE_W,
        0,
        "Voltar: F1 ou H"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        BUFFER_W - 15*SPRITE_W, BUFFER_H - 2*SPRITE_H,
        0,
        "Feito por  Eduardo  Gobbo  W.V.G"
    );
}

void start_draw(void)
{
    al_clear_to_color(al_map_rgb(233,216,166));

    al_draw_bitmap(sprites.logo, 170, 10, 0);
    al_draw_text(
        font_2game,
        al_map_rgb(0, 0, 0),
        BUFFER_W/2 - SPRITE_W, BUFFER_H/2 + 2*SPRITE_H,
        ALLEGRO_ALIGN_CENTER,
        "press space to continue"
    );
    al_draw_text(
        font,
        al_map_rgb(0, 0, 0),
        BUFFER_H/2 - SPRITE_H, BUFFER_W/2 + 2*SPRITE_W,
        ALLEGRO_ALIGN_CENTER,
        "press H or F1 for Help"
    );
    al_draw_text(
        font,
        al_map_rgb(0, 0, 0),
        BUFFER_W - 15*SPRITE_W, BUFFER_H - 1*SPRITE_H,
        0,
        "Feito por  Eduardo  Gobbo  W.V.G"
    );
}

void win_draw(void)
{
    al_draw_text(
        font_pause,
        al_map_rgb(0, 255, 0),
        BUFFER_W/2 - 3*SPRITE_W, BUFFER_H/2,
        0,
        "YOU WON"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , DISP_H - 2*SPRITE_W,
        0,
        "ESC: sair"
    );
    al_draw_text(
        font_start2help,
        al_map_rgb(255, 255, 255),
        500 , 550,
        0,
        "Press SPACE to play again"
    );
}

void lost_draw(void)
{
    al_draw_text(
        font_pause,
        al_map_rgb(255, 0, 0),
        BUFFER_W/2 - 3*SPRITE_W, BUFFER_H/2,
        0,
        "YOU LOST"
    );
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , DISP_H - 2*SPRITE_W,
        0,
        "ESC: sair"
    );
    al_draw_text(
        font_start2help,
        al_map_rgb(255, 255, 255),
        500 , 550,
        0,
        "Press SPACE to play again"
    );

}

void states_update(ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE ks)
{
    if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        al_get_keyboard_state(&ks);

        /* restart level */        
        if( event.keyboard.keycode == ALLEGRO_KEY_R && GAME_STATE.on_game )
            player_kill(GAME_BOARD.playerh, GAME_BOARD.playerw);

        /* GAME <------> PAUSE */
        if(event.keyboard.keycode == ALLEGRO_KEY_SPACE && (GAME_STATE.on_game) ){
            GAME_STATE.on_pause = !GAME_STATE.on_pause;
        }

        /* COVER -------> GAME */
        if( GAME_STATE.on_start &&  event.keyboard.keycode == ALLEGRO_KEY_SPACE ){
            GAME_STATE.on_start = 0;
            GAME_STATE.on_game = 1;
        }

        /* ???? <-------> help */
        if( event.keyboard.keycode == ALLEGRO_KEY_F1 || event.keyboard.keycode == ALLEGRO_KEY_H ){
            /* if on start maintain on start */
            /* if on game stay on game */
            /* if on HS stay on HS */
            GAME_STATE.on_help = !GAME_STATE.on_help;
        }

        /*  START/help <-------> HIGH SCORES */
        if( (GAME_STATE.on_start || GAME_STATE.on_help || GAME_STATE.on_high_scores) &&
             event.keyboard.keycode == ALLEGRO_KEY_S ) 
        {
            GAME_STATE.on_start = !GAME_STATE.on_start;
            GAME_STATE.on_high_scores = !GAME_STATE.on_high_scores;
        }

        if( (GAME_STATE.on_win || GAME_STATE.on_lost) && event.keyboard.keycode == ALLEGRO_KEY_SPACE)
        {
            grid_deinit();
            grid_init();
            player_init();
            if( GAME_STATE.on_win) GAME_STATE.on_win = 0;
            if( GAME_STATE.on_lost) GAME_STATE.on_lost = 0;
            GAME_STATE.on_game = 1;
        }

        /* thou shall not find my egg */
        if( event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE )
        {
            if( GAME_STATE.on_game && GAME_STATE.on_pause && GAME_STATE.on_help && GAME_STATE.on_high_scores)
            {
                int i, j;
                for(i = 0; i < GRID_BLK_H; i++)
                    for(j = 0; j < GRID_BLK_W; j++)
                        if( GAME_BOARD.grid[i][j].type == DIRT ){
                            GAME_BOARD.grid[i][j].type = DIAMOND;
                            GAME_BOARD.grid[i][j].prevh = i;
                            GAME_BOARD.grid[i][j].prevw = j;
                            GAME_BOARD.grid[i][j].frame = rand() % 4;
                        }
                GAME_STATE.on_help = 0;
                GAME_STATE.on_high_scores = 0;
            }
        }
        /* change characters */
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_1:
            load_player_sprites("./resources/characters/cat.png");
            break;
                
        case ALLEGRO_KEY_2:
            load_player_sprites("./resources/characters/bicho.png");
            break;
                
        case ALLEGRO_KEY_3:
            load_player_sprites("./resources/characters/dog.png");
            break;
                
        case ALLEGRO_KEY_4:
            load_player_sprites("./resources/characters/zombie.png");
            break;
                
        case ALLEGRO_KEY_5:
            load_player_sprites("./resources/characters/magic.png");
            break;
                
        case ALLEGRO_KEY_6:
            load_player_sprites("./resources/characters/minato.png");
            break;
                
        case ALLEGRO_KEY_7:
            load_player_sprites("./resources/characters/pumpkin.png");
            break;
                
        case ALLEGRO_KEY_8:
            load_player_sprites("./resources/characters/santa.png");
            break;
                
        case ALLEGRO_KEY_9:
            load_player_sprites("./resources/characters/snow.png");
            break;
                
        case ALLEGRO_KEY_0:
            load_player_sprites("./resources/characters/teacher.png");
            break;
                
        }
    }
}

void states_draw(void)
{
        
    if( GAME_STATE.on_high_scores )
    {
        high_scores_draw();
    }

    else if ( GAME_STATE.on_help )
    {
        help_draw();
    }
    
    else if( GAME_STATE.on_pause )
    {
        grid_draw();
        // fx_draw();
        hud_draw();
        pause_draw();
    }

    else if( GAME_STATE.on_game )
    {
        grid_draw();
        // fx_draw();
        hud_draw();
    }


    else if( GAME_STATE.on_start )
    {
        start_draw();
    }

    else if( GAME_STATE.on_lost )
    {
        high_scores_draw();
        lost_draw();
    }

    else if ( GAME_STATE.on_win)
    { 
        high_scores_draw();
        win_draw();
    }

    else {
        al_draw_textf(
        font,
        al_map_rgb(255, 255, 255),
        4*SPRITE_W + 10 , 10,
        0,
        "ERROR: DOES NOT BELONG TO ANY STATE"
    );
    }

}


