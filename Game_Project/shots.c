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
            shots[i].x = x + (PLAYER_W / 2) - (PLAYER_SHOT_W / 2);
            shots[i].y = y;

            // 플레이어 방향 저장
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
        // 사용 안 하는 슬롯 건너뛰기
        if (!shots[i].used) continue;

        // 플레이어 총알 처리
        
        if (shots[i].player) {
            // 방향에 따라 이동
            switch (shots[i].dir) {
            case SHOT_UP:    shots[i].y -= 5; break;
            case SHOT_DOWN:  shots[i].y += 5; break;
            case SHOT_LEFT:  shots[i].x -= 5; break;
            case SHOT_RIGHT: shots[i].x += 5; break;
            }

            // 화면 밖이면 제거
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
        // 총알 사용 여부 확인
        if (!shots[i].used) continue;

        // 자기 자신의 총알은 제외
        if (shots[i].player == player) continue;

        // 총알 크기 설정
        int sw, sh;
        if (player) {
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
        if (shots[i].player) {
            ALLEGRO_BITMAP* bmp = sprites.player_shot[frame_display];

            float angle = 0.0f;
            switch (shots[i].dir) {
            case DIR_UP:    angle = 0.0f;               break; // 원본 그대로
            case DIR_DOWN:  angle = ALLEGRO_PI;         break; // 180도 뒤집기
            case DIR_LEFT:  angle = -ALLEGRO_PI * 0.5f; break; // -90도
            case DIR_RIGHT: angle = ALLEGRO_PI * 0.5f; break; // +90도
            }

            float t = (float)(shots[i].y - 110) / (PLAYER_MAX_Y - 110);
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            float scale = DEPTH_MIN_SCALE + t * (DEPTH_MAX_SCALE - DEPTH_MIN_SCALE);

            // 중심 좌표
            float cx = al_get_bitmap_width(bmp) / 2.0f;
            float cy = al_get_bitmap_height(bmp) / 2.0f;

            al_draw_scaled_rotated_bitmap(
                bmp,
                cx, cy,                       // 회전 기준 (중심)
                shots[i].x + cx,              // 화면상 X 위치
                shots[i].y + cy,              // 화면상 Y 위치
                scale, scale,                 // 스케일 적용
                angle,                        // 회전
                0                             // 플래그
            );
        }
    }
}
