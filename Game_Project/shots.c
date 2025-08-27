#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "common.h"

void shots_init() {
    for (int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool shots_add(bool player, bool straight, int x, int y, DIRECTION dir) {
    // ���� ��� (�÷��̾�/�� �����ؼ�)
    al_play_sample(
        sample_shot,
        0.3,
        0,
        player ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for (int i = 0; i < SHOTS_N; i++) {
        // �� ���� ã��
        if (shots[i].used) continue;

        // ���� �� �Ѿ����� ����
        shots[i].player = player;

        // �÷��̾� �Ѿ��̸� ��ġ ����
        if (player) {
            shots[i].x = x + (PLAYER_W / 2) - (PLAYER_SHOT_W / 2);
            shots[i].y = y;

            // �÷��̾� ���� ����
            switch (dir) {
                case DIR_UP:    shots[i].dir = SHOT_UP; break;
                case DIR_DOWN:  shots[i].dir = SHOT_DOWN; break;
                case DIR_LEFT:  shots[i].dir = SHOT_LEFT; break;
                case DIR_RIGHT: shots[i].dir = SHOT_RIGHT; break;
            }
        }
        shots[i].dir = dir;

        shots[i].frame = 0;
        shots[i].used = true;
        return true;
    }
    return false;
}

void shots_update() {
    for (int i = 0; i < SHOTS_N; i++) {
        // ��� �� �ϴ� ���� �ǳʶٱ�
        if (!shots[i].used) continue;

        // �÷��̾� �Ѿ� ó��
        
        if (shots[i].player) {
            // ���⿡ ���� �̵�
            switch (shots[i].dir) {
            case SHOT_UP:    shots[i].y -= 5; break;
            case SHOT_DOWN:  shots[i].y += 5; break;
            case SHOT_LEFT:  shots[i].x -= 5; break;
            case SHOT_RIGHT: shots[i].x += 5; break;
            }

            // ȭ�� ���̸� ����
            if (shots[i].x < -PLAYER_SHOT_W || shots[i].x > BUFFER_W ||
                shots[i].y < -PLAYER_SHOT_H || shots[i].y > BUFFER_H) {
                shots[i].used = false;
            }
        }
        shots[i].frame++;
    }
}

bool shots_collide(bool player, int x, int y, int w, int h) {
    for (int i = 0; i < SHOTS_N; i++) {
        // �Ѿ� ��� ���� Ȯ��
        if (!shots[i].used) continue;

        // �ڱ� �ڽ��� �Ѿ��� ����
        if (shots[i].player == player) continue;

        // �Ѿ� ũ�� ����
        int sw, sh;
        if (player) {
            sw = ENEMY_SHOT_W;
            sh = ENEMY_SHOT_H;
        }
        else {
            sw = PLAYER_SHOT_W;
            sh = PLAYER_SHOT_H;
        }

        // �浹 üũ
        if (collide(x, y, x + w, y + h,
            shots[i].x, shots[i].y,
            shots[i].x + sw, shots[i].y + sh)) {
            shots[i].used = false; // �Ѿ� ����
            return true;           // �浹 �߻�
        }
    }
    return false;
}

void shots_draw() {
    for (int i = 0; i < SHOTS_N; i++) {
        // ��� ���� �Ѿ˸� �׸���
        if (!shots[i].used) continue;

        int frame_display = (shots[i].frame / 2) % 2;

        // �÷��̾� �Ѿ�
        if (shots[i].player) {
            ALLEGRO_BITMAP* bmp = sprites.player_shot[frame_display];

            float angle = 0.0f;
            switch (shots[i].dir) {
            case DIR_UP:    angle = 0.0f;               break; // ���� �״��
            case DIR_DOWN:  angle = ALLEGRO_PI;         break; // 180�� ������
            case DIR_LEFT:  angle = -ALLEGRO_PI * 0.5f; break; // -90��
            case DIR_RIGHT: angle = ALLEGRO_PI * 0.5f; break; // +90��
            }

            float t = (float)(shots[i].y - 110) / (PLAYER_MAX_Y - 110);
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            float scale = DEPTH_MIN_SCALE + t * (DEPTH_MAX_SCALE - DEPTH_MIN_SCALE);

            // �߽� ��ǥ
            float cx = al_get_bitmap_width(bmp) / 2.0f;
            float cy = al_get_bitmap_height(bmp) / 2.0f;

            al_draw_scaled_rotated_bitmap(
                bmp,
                cx, cy,                       // ȸ�� ���� (�߽�)
                shots[i].x + cx,              // ȭ��� X ��ġ
                shots[i].y + cy,              // ȭ��� Y ��ġ
                scale, scale,                 // ������ ����
                angle,                        // ȸ��
                0                             // �÷���
            );
        }
    }
}
