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

const int ENEMY_W[] = { 14, 13 };
const int ENEMY_H[] = { 9, 10 };

void enemies_init()
{
    for (int i = 0; i < ENEMIES_N; i++) {
        enemies[i].used = false;
    }
}

bool spawn_enabled = true; // ������ ����
void enemies_update()
{
    int new_quota = 5; // �̹� �����ӿ� ���� ������ �� �ִ� ���� �ִ� ��
    int start_x = BUFFER_W + 40; // ���Ͱ� ������ x��ǥ ������(ȭ�� ������ ��)
    int gap = 40; // ���� ����

    for (int i = 0; i < ENEMIES_N; i++)
    {
        if (!enemies[i].used)
        {
            if (spawn_enabled && new_quota > 0)
            {
                // x��ǥ ��ġ : ���� quota ������ ���� �����ʿ��� ������ �ΰ� ��ġ
                enemies[i].x = start_x - (new_quota - 1) * gap;

                // y��ǥ ��ġ : 110 �̻�, ȭ�� ������ 75% ���� ���Ͽ��� ����
                // (��, ENEMY_H[enemies[i].type] �� ���� ���� ���̸� ���)
                enemies[i].y = between(110, 0.75 * (BUFFER_H - ENEMY_H[enemies[i].type]));
                enemies[i].type = between(0, ENEMY_TYPE_N);
                enemies[i].blink = 0;
                enemies[i].used = true;

                // Ÿ�Կ� ���� �ʱ� ü��/�ӵ� ����
                switch (enemies[i].type)
                {
                case ENEMY_TYPE_1:
                    enemies[i].hp = 2;
                    enemies[i].vx = 1;
                    break;
                case ENEMY_TYPE_2:
                    enemies[i].hp = 4;
                    enemies[i].vx = 1;
                    enemies[i].vy = 2;
                    break;
                }

                new_quota--;
            }
            continue;
        }

        // �̹� ��� ���� ���Ͷ�� Ÿ�Կ� ���� �̵� ó��
        switch (enemies[i].type)
        {
        case ENEMY_TYPE_1:
            if (frames % 2) {
                enemies[i].x -= enemies[i].vx;  // �������� �̵�

                // ȭ�� ���� ��質 start_x ������ ��踦 ����� ���� ����
                if (enemies[i].x < 0 || enemies[i].x > start_x)
                    enemies[i].vx *= -1;
            }
            break;

        case ENEMY_TYPE_2:
            if (frames % 2) {
                enemies[i].x -= enemies[i].vx;  // x�� �̵�
                enemies[i].y += enemies[i].vy;  // y�� �̵�

                // y���� ���(110)���� �۰ų� �ϴ�(ȭ�� 75%)���� ũ�� ���� ����
                if (enemies[i].y < 110 || enemies[i].y > 0.75 * (BUFFER_H - ENEMY_H[enemies[i].type]))
                    enemies[i].vy *= -1;

                // x���� �¿� ��踦 ����� ���� ����
                if (enemies[i].x < 0 || enemies[i].x > start_x)
                    enemies[i].vx *= -1;
            }
            break;
        }

        if (enemies[i].blink)
            enemies[i].blink--;

        // �÷��̾� �Ѿ� <-> �� �浹 üũ
        if (shots_collide(false, enemies[i].x, enemies[i].y, ENEMY_W[enemies[i].type], ENEMY_H[enemies[i].type]))
        {
            enemies[i].hp--;
            enemies[i].blink = 4;
        }

        // �ܰ��� ü�� 0 ���� �� ó��
        if (enemies[i].hp <= 0)
        {
            enemies[i].used = false; // �ܰ��� ��Ȱ��ȭ
            continue;
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

        if (!enemies[i].used)
            continue; // ��� ���� �ƴ� �ܰ����� �׸��� ����
        if (enemies[i].blink > 2)
            continue; // �Ѿ� �¾��� �� ������ ���� ��� �� ���̵��� ó��


        // 2.5D ȿ��: y��ǥ�� ���� ũ�� ����
        float t = (float)(enemies[i].y - 110) / (PLAYER_MAX_Y - 110);
        if (t < 0) t = 0;
        if (t > 1) t = 1;
        float scale = DEPTH_MIN_SCALE + t * (DEPTH_MAX_SCALE - DEPTH_MIN_SCALE);

        int w = ENEMY_W[enemies[i].type];
        int h = ENEMY_H[enemies[i].type];

        // ������ �����ؼ� �׸���
        al_draw_scaled_bitmap(
            sprites.enemy[enemies[i].type], // ���� ��Ʈ��
            0, 0, w, h,                     // ���� ũ��
            enemies[i].x, enemies[i].y,     // ��ġ
            w * scale, h * scale,           // ũ��
            0                               // �÷���
        );
    }
}