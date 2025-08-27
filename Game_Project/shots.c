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

bool shots_add(bool player, bool straight, int x, int y) {
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
            shots[i].x = x - (PLAYER_SHOT_W / 2);
            shots[i].y = y;
        }

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
            shots[i].y -= 5;

            // ȭ�� ������ ���� �Ѿ��� ����
            if (shots[i].y < -PLAYER_SHOT_H) {
                shots[i].used = false;
                continue;
            }
        }
        shots[i].frame++;
    }
}

bool shots_collide(bool ship, int x, int y, int w, int h) {
    for (int i = 0; i < SHOTS_N; i++) {
        // �Ѿ� ��� ���� Ȯ��
        if (!shots[i].used) continue;

        // �ڱ� �ڽ��� �Ѿ��� ����
        if (shots[i].player == ship) continue;

        // �Ѿ� ũ�� ����
        int sw, sh;
        if (ship) {
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
        if (shots[i].player)
            al_draw_bitmap(sprites.player_shot[frame_display], shots[i].x, shots[i].y, 0);
    }
}
