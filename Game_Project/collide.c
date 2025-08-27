#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "common.h"

// 플레이어 <-> 적 충돌 판정
bool enemies_collide()
{
    int px1 = player.x;
    int py1 = player.y;
    int px2 = player.x + PLAYER_W;
    int py2 = player.y + PLAYER_H;

    for (int i = 0; i < ENEMIES_N; i++) {
        if (!enemies[i].used) continue;

        int ex1 = enemies[i].x;
        int ey1 = enemies[i].y;
        int ex2 = enemies[i].x + PLAYER_W;
        int ey2 = enemies[i].y + PLAYER_H;

        if (collide(px1, py1, px2, py2, ex1, ey1, ex2, ey2)) {
            return true; // 하나라도 충돌 시 true
        }
    }

    return false;
}