/* =======================================================  */
/*   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves   */
/*   GRR20203892                                            */
/*   programa BOULDER DASH                                  */
/*  ======================================================= */

#include "structure.h"

//  ======================= GRID =======================
void grid_init(void)
{
    // fprintf(stderr, "iniciando grid... ");

    int i;

    GAME_BOARD.game_score = 0;
    GAME_BOARD.lv = 1;
    GAME_BOARD.last_lv = 5;
    
    // // aloca um vetor de GRID_BLK_H  ponteiros para GRID_BLK_H has
    GAME_BOARD.grid = malloc (GRID_BLK_H  * sizeof (struct GAME_DATA)) ;
    
    // aloca cada uma das GRID_BLK_H has (vetores de GRID_BLK_W  inteiros)
    for (i=0; i < GRID_BLK_H ; i++)
        GAME_BOARD.grid[i] = malloc (GRID_BLK_W  * sizeof (struct GAME_DATA)) ;

    pick_level(GAME_BOARD.lv);

    // fprintf(stderr, "ok\n");
    // return mat;
}

void grid_deinit(void)
{
    // fprintf(stderr, "deletando grid... ");

    int i;

    // libera a memória da matriz
    for (i=0; i < GRID_BLK_H ; i++)
    free (GAME_BOARD.grid[i]);

    // fprintf(stderr, "ok\n");
}

void grid_update(void)
{

    // ========================== GAME TICK ==========================
    thisSecond = time(NULL);
    if( pastSecond != thisSecond )
    {
        pastSecond = thisSecond;
        GAME_BOARD.lv_seconds_left--;

        if(player.invincible_timer < 0)
            player.invincible_timer++;

        else if(player.invincible_timer > 0)
            player.invincible_timer--;

        if(player.respawn_timer > 0)
        {
            player.respawn_timer--;
            if( player.respawn_timer == 0 && player.lives >= 0)
                pick_level(GAME_BOARD.lv);
        }

        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].prevw = GAME_BOARD.playerw;
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].prevh = GAME_BOARD.playerh;

    }
    /*
        printf("player.x = %d    player.y = %d\n", player.x, player.y);
        printf("game playerw = %d    game playerh = %d\n", GAME_BOARD.playerw, GAME_BOARD.playerh);
        printf("PLAYER prevw = %d    prevh = %d\n\n\n", 
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].prevw, 
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].prevh);
    */

    /* score board */
    if(GAME_BOARD.d_value_doubled && GAME_BOARD.lv_seconds_left <= GAME_BOARD.lv_seconds/2 )
    {
        GAME_BOARD.diamonds_value *= 3;
        GAME_BOARD.d_value_doubled = 0;
    }
    if( GAME_BOARD.lv_seconds_left < 0 )
    {
        /* reboot level && kill player EXPLODE AND WAIT A BIT */
        GAME_BOARD.lv_seconds_left = GAME_BOARD.lv_seconds;
        GAME_BOARD.d_value_doubled = 1;
        GAME_BOARD.diamonds_value /= 3;
        player_kill(GAME_BOARD.playerh, GAME_BOARD.playerw);
    }

    /* monster behaviour & boulder falling logic */
    if((frames) % 13 == 1)
    {
        // enemy_update();
        boulder_update();
    }

    /* atualiza direcao sprite entidades */
    sprite_direction();
    
    // fprintf(stderr, "ok\n");
}

void grid_draw(void)
{
    int x, y, i, j;
    int aux;

    // bordas do jogo
    for(i = 1; i < DISP_BLK_H; i++)
        for(j = 0; j < DISP_BLK_W; j++)
            if( i == 1 || i == DISP_BLK_H - 1 || j == 0 || j == DISP_BLK_W -1 )
            {
                x = i * SPRITE_H;
                y = j * SPRITE_W;
                al_draw_tinted_bitmap(sprites.steelWall, al_map_rgb(188, 255, 255), y, x, 0); 
            }

    // desenha conteudo nivel
    for(x = 0; x < GRID_BLK_H; x++)
        for(y = 0; y < GRID_BLK_W; y++)
        {
            i = (x + 2) * SPRITE_H;
            j = (y + 1) * SPRITE_W;

            al_draw_filled_rectangle(j, i, j+SPRITE_H, i+SPRITE_W, al_map_rgb(10, 15, 20));

            
            switch (GAME_BOARD.grid[x][y].type)
            {
            case DIRT:
                al_draw_tinted_bitmap(sprites.dirt, al_map_rgba_f(255, 255, 255, 0.6), j, i, 0);
                break;
            case BOULDER: 
                al_draw_tinted_scaled_bitmap(sprites.boulder, al_map_rgb(233, 216, 166), 0, 0, 136, 128, j, i, SPRITE_W, SPRITE_H, 0);
                break;
            case WALL:
                al_draw_tinted_bitmap(sprites.wall, al_map_rgb(238, 155, 0), j, i, 0); 
                break;
            case DIAMOND: 
                al_draw_scaled_bitmap(sprites.diamont[GAME_BOARD.grid[x][y].frame], 0, 0, SPRITE_H, SPRITE_W, j-6, i-6, 45, 45, 0);
                break;
            case TNT:
                aux = rand() % 4;
                if( (rand() / 5) % 5 == 0)
                    GAME_BOARD.grid[x][y].frame = rand() % 4;
                if( player.respawn_timer > 1)
                    al_draw_bitmap(sprites.explosion[aux], j, i, 0); 
                else if( player.respawn_timer == 1)
                    al_draw_filled_rectangle(j, i, j+SPRITE_H, i+SPRITE_W, al_map_rgb(10, 15, 20));

                break;
            case EXIT:
                if( GAME_BOARD.diamonds_caught >= GAME_BOARD.pass_level ){
                    if(thisSecond % 2 == 0)
                        al_draw_bitmap(sprites.exit[0], j, i, 0);
                    else
                        al_draw_bitmap(sprites.exit[1], j, i, 0);
                }
                else
                    al_draw_bitmap(sprites.exit[0], j, i, 0);
                break;
            case PLAYER:
                if( GAME_BOARD.lv_seconds_left >= GAME_BOARD.lv_seconds && (frames % 10) <= 5)
                    al_draw_filled_rectangle(j, i, j+SPRITE_H, i+SPRITE_W, al_map_rgb(255, 255, 255));
                else if( GAME_BOARD.grid[x][y].prevh != x || GAME_BOARD.grid[x][y].prevw != y)
                    al_draw_bitmap(sprites.player[GAME_BOARD.grid[x][y].frame + ((frames / 3 ) % 3)], j, i, 0); 
                else 
                    al_draw_bitmap(sprites.player[GAME_BOARD.grid[x][y].frame + 1], j, i, 0); 
                break;
                
            case EMPTY:
                if(GAME_BOARD.diamonds_caught >= GAME_BOARD.pass_level && player.invincible_timer < 0 ){
                    al_draw_filled_rectangle(j, i, j+SPRITE_H, i+SPRITE_W, al_map_rgb(255, 255, 255));
                }
                else
                    al_draw_filled_rectangle(j, i, j+SPRITE_H, i+SPRITE_W, al_map_rgb(10, 15, 20));
                break;
            default:
                al_draw_filled_rectangle(j, i, j+SPRITE_H, i+SPRITE_W, al_map_rgb(255, 0, 0));
                break;
            }
        }
}

void boulder_update(void){
    int i, j;

        for(i = GRID_BLK_H -1 ; i >= 0; i--){
            for(j = 0; j < GRID_BLK_W ; j++)
            { 
                /* falling boulders */
                if( valid_move(i + 1, j) && 
                    (GAME_BOARD.grid[i][j].type == BOULDER || 
                     GAME_BOARD.grid[i][j].type == DIAMOND ))
                {
                    if( GAME_BOARD.grid[i+1][j].type == PLAYER &&
                        ( GAME_BOARD.grid[i][j].prevh != i ||
                        GAME_BOARD.grid[i][j].prevw != j ))
                        {
                            player_kill(i+1, j);
                        }
                    /* fall down INTERCALADO */
                    if( GAME_BOARD.grid[i+1][j].type == EMPTY && GAME_BOARD.grid[i][j].prevw == j)
                    {
                        if( (valid_move(i+2,j) && ( 
                        ((GAME_BOARD.grid[i+2][j].type == BOULDER || 
                        GAME_BOARD.grid[i+2][j].type == DIAMOND) && 
                        GAME_BOARD.grid[i+2][j].prevh != i+1) 
                        || 
                        (GAME_BOARD.grid[i+2][j].type != BOULDER &&
                        GAME_BOARD.grid[i+2][j].type != DIAMOND)) 
                        )
                        ||
                        i >= GRID_BLK_H - 2)
                        {
                            GAME_BOARD.grid[i+1][j].prevh = i;
                            GAME_BOARD.grid[i+1][j].prevw = j;
                            if( GAME_BOARD.grid[i][j].type == BOULDER){
                                GAME_BOARD.grid[i+1][j].type =  BOULDER;
                            }
                            else{
                                GAME_BOARD.grid[i+1][j].type =  DIAMOND;
                                GAME_BOARD.grid[i+1][j].frame = GAME_BOARD.grid[i][j].frame;
                            }
                            GAME_BOARD.grid[i][j].type = EMPTY;
                        }
                    }
                    /* fall to the sides (slides) */
                    else if( GAME_BOARD.grid[i+1][j].type == BOULDER ||
                             GAME_BOARD.grid[i+1][j].type == WALL ||                            
                             GAME_BOARD.grid[i+1][j].type == DIAMOND )
                    {
                        /* add randomness 50% */
                        if( rand()% 100 >= 50){
                            if (!fall_left(i, j) )
                                fall_right(i, j);
                        }
                        else{
                            if( !fall_right(i, j) )
                                fall_left(i, j);
                        }
                        
                    }
                }
                if( GAME_BOARD.grid[i][j].type == BOULDER || GAME_BOARD.grid[i][j].type == DIAMOND)
                {
                    GAME_BOARD.grid[i][j].prevh = i;
                    GAME_BOARD.grid[i][j].prevw = j;
                }            
            }
        }

}

int fall_left(int i, int j)
{
    if(valid_move(i, j - 1) &&
            GAME_BOARD.grid[i+1][j-1].type == EMPTY && 
            GAME_BOARD.grid[i][j-1].type == EMPTY)
    {
        GAME_BOARD.grid[i][j-1].prevw = j-1;
        if( GAME_BOARD.grid[i][j].type == BOULDER){
            GAME_BOARD.grid[i][j-1].type = BOULDER;
        }
        else{
            GAME_BOARD.grid[i][j-1].type = DIAMOND;
            GAME_BOARD.grid[i][j-1].frame  =GAME_BOARD.grid[i][j].frame;
        }
        GAME_BOARD.grid[i][j].type = EMPTY;
        return 1;
    }
    return 0;
}

int fall_right(int i, int j)
{
    if( valid_move(i, j + 1) &&
        GAME_BOARD.grid[i+1][j+1].type == EMPTY &&
        GAME_BOARD.grid[i][j+1].type == EMPTY )
    {
        GAME_BOARD.grid[i][j+1].prevw = j;
        if( GAME_BOARD.grid[i][j].type == BOULDER){
            GAME_BOARD.grid[i][j+1].type = BOULDER;
        }
        else{
            GAME_BOARD.grid[i][j+1].type = DIAMOND;
            GAME_BOARD.grid[i][j+1].frame  =GAME_BOARD.grid[i][j].frame;
        }
        GAME_BOARD.grid[i][j].type = EMPTY;
        return 1;
    }
    return 0;
}

/*
void enemy_update(void)
{
    int i, j;

        for(i = GRID_BLK_H - 1; i >= 0 ; i--){
            for(j = 0; j < GRID_BLK_W ; j++){ 
                // falling boulders
                if((GAME_BOARD.grid[i][j].type == FIREFLY || 
                    GAME_BOARD.grid[i][j].type == BUTTERFLY ))
                {
                    // kill on horizontal or vertical 
                    if( ( valid_move(i+1, j) && GAME_BOARD.grid[i+1][j].type == PLAYER) ||
                        ( valid_move(i-1, j) && GAME_BOARD.grid[i-1][j].type == PLAYER) ||
                        ( valid_move(i, j+1) && GAME_BOARD.grid[i][j+1].type == PLAYER) ||
                        ( valid_move(i, j-1) && GAME_BOARD.grid[i][j-1].type == PLAYER))
                    {
                        player_kill(i, j);
                        // player_kill(GAME_BOARD.playerh, GAME_BOARD.playerw);
                    }
                    // move clock-wise on borders
                    // todo 
                }
            }
        }
}
*/
void player_kill(int h, int w)
{
    int i, j;

    if( player.respawn_timer == 0)
    {
        al_play_sample(sounds.sample_kaboom, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

        player.lives--;
        player.respawn_timer = 3;

        for(i = -1; i <= 1; i++)
            for(j = -1; j <= 1; j++)
                if(valid_move(h + i, w + j))
                {
                    GAME_BOARD.grid[h+i][w+j].type = TNT;
                }
    }
}

void pick_level(int lv)
{
    int i, j;
    int value;
    
    if(lv > GAME_BOARD.last_lv)
    {
        printf("play win song\n");
        al_play_sample(sounds.sample_cheers, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_play_sample(sounds.sample_win, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

        GAME_STATE.on_win = 1;
        GAME_STATE.on_game = 0;
        return;
    }

    else if( GAME_BOARD.lv != 1 && GAME_STATE.on_game)
        al_play_sample(sounds.sample_lvup, 0.25, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

    char aux[100];
    sprintf(aux, "data/level%i.txt", lv);

    FILE *level;
    level = fopen(aux, "r");

    /* read these values from level.txt */
    fscanf(level, "%d", &GAME_BOARD.pass_level);
    fscanf(level, "%d", &GAME_BOARD.diamonds_value);
    fscanf(level, "%d", &GAME_BOARD.lv_seconds);

    GAME_BOARD.lv_seconds_left = GAME_BOARD.lv_seconds;
    GAME_BOARD.diamonds_caught = 0;

    GAME_BOARD.d_value_doubled = 1;
    

    for(i = 0; i < GRID_BLK_H; i++){
            for(j = 0; j < GRID_BLK_W; j++){ 
                fscanf(level, "%i", &value);

                GAME_BOARD.grid[i][j].type = value;

                GAME_BOARD.grid[i][j].prevh = i;
                GAME_BOARD.grid[i][j].prevw = j;

                GAME_BOARD.grid[i][j].frame = 0;

                if( value == DIAMOND )
                    GAME_BOARD.grid[i][j].frame = rand() % 4;

                if( value == PLAYER )
                {
                    player.x = player.absx = (j + 1) * SPRITE_W;
                    player.y = player.absy = (i + 2) * SPRITE_H;
                    GAME_BOARD.playerw = j;
                    GAME_BOARD.playerh = i;
                }
            }
        }
    
    fclose(level);
}

void sprite_direction(void)
{
    int dw, dh;

    dw = GAME_BOARD.playerw - GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].prevw;
    dh = GAME_BOARD.playerh - GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].prevh;

    // pra direita 
    if( dw >= 1 && dh == 0)
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].frame = 6;
    
    // pra esquerda 
    else if( dw <= -1 && dh == 0)
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].frame = 3;

    // pra baixo ou diagonal baixo
    else if( (dw == 0 && dh >= 1) || dh >= 1)
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].frame = 0;
    
    // pra cima ou diagonal cima
    else if( (dw == 0 && dh <= -1) || dh <= -1)
        GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].frame = 9;
 
}
