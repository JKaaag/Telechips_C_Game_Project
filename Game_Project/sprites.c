#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "common.h"

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("spritesheet.png");
    must_init(sprites._sheet, "spritesheet");

    sprites.player_shot[0] = sprite_grab(13, 0, PLAYER_SHOT_W, PLAYER_SHOT_H);
    sprites.player_shot[1] = sprite_grab(16, 0, PLAYER_SHOT_W, PLAYER_SHOT_H);

    sprites.enemy[0] = sprite_grab(19, 0, ENEMY_1_W, ENEMY_1_H);
    sprites.enemy[1] = sprite_grab(19, 10, ENEMY_2_W, ENEMY_2_H);
}

void sprites_deinit()
{
    al_destroy_bitmap(sprites.player_shot[0]);
    al_destroy_bitmap(sprites.player_shot[1]);

    al_destroy_bitmap(sprites.enemy[0]);
    al_destroy_bitmap(sprites.enemy[1]);

    al_destroy_bitmap(sprites._sheet);
}