#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "common.h"

void enemies_init()
{
    for (int i = 0; i < ENEMIES_N; i++) {
        enemies[i].x = rand() % (BUFFER_W - PLAYER_W);
        enemies[i].y = rand() % (BUFFER_H - PLAYER_H);
        enemies[i].type = ENEMY_NORMAL;
        enemies[i].hp = 10;
        enemies[i].used = true;
    }
}

void enemies_update()
{
    for (int i = 0; i < ENEMIES_N; i++) {
        if (!enemies[i].used) continue;

        // �÷��̾� �Ѿ˰� �浹 üũ
        if (shots_collide(false, enemies[i].x, enemies[i].y, PLAYER_W, PLAYER_H)) {
            enemies[i].hp--;   // HP ����

            // HP�� 0 �����̸� �� ��Ȱ��ȭ
            if (enemies[i].hp <= 0)
                enemies[i].used = false;
        }
    }
}

void enemies_draw()
{
    for (int i = 0; i < ENEMIES_N; i++) {
        if (!enemies[i].used) continue;

        // �� HP ǥ��
        char hp_text[16];
        sprintf(hp_text, "%d", enemies[i].hp);
        al_draw_text(
            font,
            al_map_rgb(255, 255, 255),
            enemies[i].x + PLAYER_W / 2, // �߾�
            enemies[i].y - 10,           // �� ����
            ALLEGRO_ALIGN_CENTER,
            hp_text
        );

        al_draw_filled_rectangle(
            enemies[i].x,
            enemies[i].y,
            enemies[i].x + PLAYER_W,
            enemies[i].y + PLAYER_H,
            al_map_rgb(255, 0, 0) // ������ �׸�
        );
    }
}