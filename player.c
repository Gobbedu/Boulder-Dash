/* =======================================================  */
/*   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves   */
/*   GRR20203892                                            */
/*   programa BOULDER DASH                                  */
/*  ======================================================= */
#include "structure.h"

// ==================== player ====================
void player_init()
{
    int i, j;


    for (i = 0; i < GRID_BLK_H; i++) { 
        for (j = 0; j < GRID_BLK_W; j++) {
            if( GAME_BOARD.grid[i][j].type == PLAYER ) {
                // 2 & 1 CONTABILIZA DESVIO DAS BORDAS FIXAS E PLACAR
                player.x = player.absx = (j + 1) * SPRITE_W;
                player.y = player.absy = (i + 2) * SPRITE_H;
                GAME_BOARD.playerw = j;
                GAME_BOARD.playerh = i;
            }
        }   
    }
    
    player.lives = 5;
    player.respawn_timer = 0;
    player.invincible_timer = 120;
}

void player_update()
{
    if(player.lives < 0 && player.respawn_timer == 0){
        al_play_sample(sounds.sample_lose, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

        GAME_STATE.on_lost = 1;
        GAME_STATE.on_game = 0;
        return;
    }

    if(player.respawn_timer > 0)
    {
        // player.respawn_timer--;
        return;
    }

    if(key[ALLEGRO_KEY_LEFT])
    {
        player.absx -= player_SPEED;
    }
    if(key[ALLEGRO_KEY_RIGHT])
    {
        player.absx += player_SPEED;
    }
    if(key[ALLEGRO_KEY_UP])
    {
        player.absy -= player_SPEED;
    }
    if(key[ALLEGRO_KEY_DOWN])
    {
        player.absy += player_SPEED;
    }

    // mantem player dentro da tela 
    if(player.absx < SPRITE_W)      player.absx = SPRITE_W;
    if(player.absy < 2 * SPRITE_H)  player.absy = 2 * SPRITE_H;

    if(player.absx > player_MAX_X) player.absx = player_MAX_X;
    if(player.absy > player_MAX_Y) player.absy = player_MAX_Y;

    // move player em blocos
    // processa absx relativo ao deslocamento de blocos absolutos
    int aux;
    if(player.absx > player.x*SPRITE_W || player.absx < player.x*SPRITE_W)
    {
        aux = (player.absx/SPRITE_W) - 1;
        move_player(GAME_BOARD.playerh, aux);
    }


    if(player.absy > player.y*SPRITE_H || player.absx < player.y*SPRITE_H)
    {
        aux = (player.absy/SPRITE_H) - 2;
        move_player(aux, GAME_BOARD.playerw);
    }

}

void move_player(int playerh, int playerw)
{
    // check if position is changed
    if( GAME_BOARD.playerw != playerw )
    {
        if( player_can_move(playerh, playerw) ){
            player.x = (playerw + 1) * SPRITE_W;                                                    // calcula nova posicao do pixel
            GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].type = EMPTY;                   // valor tile passado
            GAME_BOARD.grid[GAME_BOARD.playerh][playerw].prevw = GAME_BOARD.playerw;                // atualiza passado player
            GAME_BOARD.playerw = playerw;                                                           // atualiza pixel player
            GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].type = PLAYER;                  // atualiza tile player
        }
        else{
            player.absx = player.x;
        }
    }
    else if(GAME_BOARD.playerh != playerh )
    {
        if( player_can_move(playerh, playerw) ){
            player.y = (playerh + 2) * SPRITE_H;
            GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].type = EMPTY;
            GAME_BOARD.grid[playerh][GAME_BOARD.playerw].prevh = GAME_BOARD.playerh;          
            GAME_BOARD.playerh = playerh; 
            GAME_BOARD.grid[GAME_BOARD.playerh][GAME_BOARD.playerw].type = PLAYER;          
        }
        else{
            player.absy = player.y;
        }
    }
}

int player_can_move(int i, int j)
{
    if( GAME_BOARD.grid[i][j].type == DIAMOND )
    {
        al_play_sample(sounds.sample_mine, 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

        GAME_BOARD.game_score += GAME_BOARD.diamonds_value;
        GAME_BOARD.diamonds_caught ++;

        /* ao atingir a cota da fase ganha imunidade, utilizado para backlight branco */
        if (GAME_BOARD.diamonds_caught == GAME_BOARD.pass_level)
            player.invincible_timer = -1;
    }

    // if(GAME_BOARD.grid[i][j].type == FIREFLY)
    //     player_kill(i, j);

    if( GAME_BOARD.grid[i][j].type == EXIT )
    {
        /* proximo nivel */
        if( GAME_BOARD.diamonds_caught >= GAME_BOARD.pass_level ){
            GAME_BOARD.lv++;
            GAME_BOARD.game_score += GAME_BOARD.lv_seconds_left;
            player.respawn_timer = 1;
            player.invincible_timer = 2;
        }
        /* porta fechada */
        else
            return 0;
    }

    /* empurra pedras */
    int pushx;
    pushx = j - GAME_BOARD.playerw;
    if( valid_move(i, j + pushx) && GAME_BOARD.grid[i][j].type == BOULDER && GAME_BOARD.grid[i][j + pushx].type == EMPTY){
        GAME_BOARD.grid[i][j + pushx].type = BOULDER;
        GAME_BOARD.grid[i][j].type = EMPTY;
    }

    return  GAME_BOARD.grid[i][j].type != WALL   &&
            GAME_BOARD.grid[i][j].type != BOULDER;
        
}

int valid_move(int i, int j){
    return  i >= 0 && 
            j >= 0 &&
            i < GRID_BLK_H &&
            j < GRID_BLK_W;
}
