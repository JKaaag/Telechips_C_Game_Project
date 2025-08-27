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
    // 사운드 재생 (플레이어/적 구분해서)
    al_play_sample(
        sample_shot,
        0.3,
        0,
        player ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for (int i = 0; i < SHOTS_N; i++) {
        // 빈 슬롯 찾기
        if (shots[i].used) continue;

        // 누가 쏜 총알인지 저장
        shots[i].player = player;

        // 플레이어 총알이면 위치 설정
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
        // 사용 안 하는 슬롯 건너뛰기
        if (!shots[i].used) continue;

        // 플레이어 총알 처리
        if (shots[i].player) {
            shots[i].y -= 5;

            // 화면 밖으로 나간 총알은 제거
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
        // 총알 사용 여부 확인
        if (!shots[i].used) continue;

        // 자기 자신의 총알은 제외
        if (shots[i].player == ship) continue;

        // 총알 크기 설정
        int sw, sh;
        if (ship) {
            sw = ENEMY_SHOT_W;
            sh = ENEMY_SHOT_H;
        }
        else {
            sw = PLAYER_SHOT_W;
            sh = PLAYER_SHOT_H;
        }

        // 충돌 체크
        if (collide(x, y, x + w, y + h,
            shots[i].x, shots[i].y,
            shots[i].x + sw, shots[i].y + sh)) {
            shots[i].used = false; // 총알 제거
            return true;           // 충돌 발생
        }
    }
    return false;
}

void shots_draw() {
    for (int i = 0; i < SHOTS_N; i++) {
        // 사용 중인 총알만 그리기
        if (!shots[i].used) continue;

        int frame_display = (shots[i].frame / 2) % 2;

        // 플레이어 총알
        if (shots[i].player)
            al_draw_bitmap(sprites.player_shot[frame_display], shots[i].x, shots[i].y, 0);
    }
}
