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

void player_init()
{
    player.x = (BUFFER_W / 2) - (PLAYER_W / 2);
    player.y = (BUFFER_H / 2) - (PLAYER_H / 2);
    player.shot_timer = 0;
    player.hp = 100;
    player.invincible_timer = 120; // 무적 시간

}

void player_update()
{
    // HP가 0보다 작으면 아무 것도 하지 않음
    if (player.hp < 0)
        return;

    // 키 입력에 따라 이동 처리 및 플레이어 이동 방향 저장
    if (key[ALLEGRO_KEY_LEFT]) {
        player.x -= PLAYER_SPEED;
    }
    if (key[ALLEGRO_KEY_RIGHT]) {
        player.x += PLAYER_SPEED;
    }
    if (key[ALLEGRO_KEY_UP]) {
        player.y -= PLAYER_SPEED;
    }
    if (key[ALLEGRO_KEY_DOWN]) {
        player.y += PLAYER_SPEED;
    }

    // 화면 밖으로 나가지 않도록 제한
    if (player.x < 0)
        player.x = 0;
    if (player.y < 110)
        player.y = 110;

    if (player.x > PLAYER_MAX_X)
        player.x = PLAYER_MAX_X;
    if (player.y > PLAYER_MAX_Y)
        player.y = PLAYER_MAX_Y;

    // 무적 상태 처리
    if (player.invincible_timer > 0) {
        player.invincible_timer--;
    }
    else {
        // 무적이 아닐 때만 충돌 체크
        if (enemies_collide(true, player.x, player.y, PLAYER_W, PLAYER_H)) {
            player.hp -= 10;
            if (player.hp < 0) player.hp = 0;
            player.invincible_timer = 120; // 2초 정도 무적
        }
    }

    // 총알 발사 처리
    // 발사 딜레이 카운터 감소
    if (player.shot_timer)
        player.shot_timer--;
    else
    {
        player.shot_timer = 30; // 재발사 대기 시간
        // Z키: 왼쪽으로 발사
        if (key[ALLEGRO_KEY_Z])
            shots_add(true, true, player.x, player.y, DIR_LEFT);
        // X키: 오른쪽으로 발사
        else if (key[ALLEGRO_KEY_X])
            shots_add(true, true, player.x, player.y, DIR_RIGHT);
    }
}

void player_draw()
{
    if (player.hp <= 0)
        return;

    // 플레이어 HP 표시
    char hp_text[16];
    sprintf(hp_text, "%d", player.hp);
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        player.x + PLAYER_W / 2, // 플레이어 중앙
        player.y - 10,           // 위쪽
        ALLEGRO_ALIGN_CENTER,
        hp_text
    );

    // 무적 깜빡임 효과 처리
    if (player.invincible_timer > 0)
    {
        if (((player.invincible_timer / 2) % 3) == 1)
            return; // 이 프레임에는 그리지 않음
    }


    // 2.5D 효과: y좌표에 따라 스케일 조정
    DEPTH_MIN_SCALE = 1.0f;   // 화면 위쪽 (작게)
    DEPTH_MAX_SCALE = 3.0f;   // 화면 아래쪽 (크게)

    // y=110일 때 min_scale, y=PLAYER_MAX_Y일 때 max_scale
    float t = (float)(player.y - 110) / (PLAYER_MAX_Y - 110);
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    float scale = DEPTH_MIN_SCALE + t * (DEPTH_MAX_SCALE - DEPTH_MIN_SCALE);

    // 스케일된 크기
    float w = PLAYER_W * scale;
    float h = PLAYER_H * scale;

    // 중심을 맞춰서 그리기 (플레이어 좌표 기준으로 보정)
    al_draw_filled_rectangle(
        player.x,
        player.y,
        player.x + w,
        player.y + h,
        al_map_rgb(0, 255, 0)
    );
}