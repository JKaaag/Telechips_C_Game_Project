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
        player.last_dir = DIR_LEFT;
    }
    if (key[ALLEGRO_KEY_RIGHT]) {
        player.x += PLAYER_SPEED;
        player.last_dir = DIR_RIGHT;
    }
    if (key[ALLEGRO_KEY_UP]) {
        player.y -= PLAYER_SPEED;
        player.last_dir = DIR_UP;
    }
    if (key[ALLEGRO_KEY_DOWN]) {
        player.y += PLAYER_SPEED;
        player.last_dir = DIR_DOWN;
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
    else if (key[ALLEGRO_KEY_X])
    {
        // 플레이어 총알 생성
        int x = player.x + (PLAYER_W / 2);
        // 발사 후 재발사 대기
        if (shots_add(true, true, player.x, player.y, player.last_dir))
            player.shot_timer = 5;
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

    // 플레이어 네모 그리기
    al_draw_filled_rectangle(
        player.x,
        player.y,
        player.x + PLAYER_W,
        player.y + PLAYER_H,
        al_map_rgb(0, 255, 0) // 초록색
    );
}