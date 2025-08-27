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

bool spawn_enabled = true; // 리스폰 여부
void enemies_update()
{
    int new_quota = 5; // 이번 프레임에 새로 출현할 수 있는 몬스터 최대 수
    int start_x = BUFFER_W + 40; // 몬스터가 등장할 x좌표 시작점(화면 오른쪽 밖)
    int gap = 40; // 몬스터 간격

    for (int i = 0; i < ENEMIES_N; i++)
    {
        if (!enemies[i].used)
        {
            if (spawn_enabled && new_quota > 0)
            {
                // x좌표 배치 : 출현 quota 개수에 따라 오른쪽에서 간격을 두고 배치
                enemies[i].x = start_x - (new_quota - 1) * gap;

                // y좌표 배치 : 110 이상, 화면 높이의 75% 지점 이하에서 랜덤
                // (단, ENEMY_H[enemies[i].type] 을 빼서 몬스터 높이를 고려)
                enemies[i].y = between(110, 0.75 * (BUFFER_H - ENEMY_H[enemies[i].type]));
                enemies[i].type = between(0, ENEMY_TYPE_N);
                enemies[i].blink = 0;
                enemies[i].used = true;

                // 타입에 따라 초기 체력/속도 설정
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

        // 이미 사용 중인 몬스터라면 타입에 따라 이동 처리
        switch (enemies[i].type)
        {
        case ENEMY_TYPE_1:
            if (frames % 2) {
                enemies[i].x -= enemies[i].vx;  // 왼쪽으로 이동

                // 화면 왼쪽 경계나 start_x 오른쪽 경계를 벗어나면 방향 반전
                if (enemies[i].x < 0 || enemies[i].x > start_x)
                    enemies[i].vx *= -1;
            }
            break;

        case ENEMY_TYPE_2:
            if (frames % 2) {
                enemies[i].x -= enemies[i].vx;  // x축 이동
                enemies[i].y += enemies[i].vy;  // y축 이동

                // y축이 상단(110)보다 작거나 하단(화면 75%)보다 크면 방향 반전
                if (enemies[i].y < 110 || enemies[i].y > 0.75 * (BUFFER_H - ENEMY_H[enemies[i].type]))
                    enemies[i].vy *= -1;

                // x축이 좌우 경계를 벗어나면 방향 반전
                if (enemies[i].x < 0 || enemies[i].x > start_x)
                    enemies[i].vx *= -1;
            }
            break;
        }

        if (enemies[i].blink)
            enemies[i].blink--;

        // 플레이어 총알 <-> 몹 충돌 체크
        if (shots_collide(false, enemies[i].x, enemies[i].y, ENEMY_W[enemies[i].type], ENEMY_H[enemies[i].type]))
        {
            enemies[i].hp--;
            enemies[i].blink = 4;
        }

        // 외계인 체력 0 이하 시 처리
        if (enemies[i].hp <= 0)
        {
            enemies[i].used = false; // 외계인 비활성화
            continue;
        }
    }
}

void enemies_draw()
{
    for (int i = 0; i < ENEMIES_N; i++) {
        if (!enemies[i].used) continue;

        // 적 HP 표시
        char hp_text[16];
        sprintf(hp_text, "%d", enemies[i].hp);
        al_draw_text(
            font,
            al_map_rgb(255, 255, 255),
            enemies[i].x + PLAYER_W / 2, // 중앙
            enemies[i].y - 10,           // 적 위쪽
            ALLEGRO_ALIGN_CENTER,
            hp_text
        );

        if (!enemies[i].used)
            continue; // 사용 중이 아닌 외계인은 그리지 않음
        if (enemies[i].blink > 2)
            continue; // 총알 맞았을 때 깜박인 동안 잠시 안 보이도록 처리


        // 2.5D 효과: y좌표에 따라 크기 조정
        float t = (float)(enemies[i].y - 110) / (PLAYER_MAX_Y - 110);
        if (t < 0) t = 0;
        if (t > 1) t = 1;
        float scale = DEPTH_MIN_SCALE + t * (DEPTH_MAX_SCALE - DEPTH_MIN_SCALE);

        int w = ENEMY_W[enemies[i].type];
        int h = ENEMY_H[enemies[i].type];

        // 스케일 적용해서 그리기
        al_draw_scaled_bitmap(
            sprites.enemy[enemies[i].type], // 원본 비트맵
            0, 0, w, h,                     // 원본 크기
            enemies[i].x, enemies[i].y,     // 위치
            w * scale, h * scale,           // 크기
            0                               // 플래그
        );
    }
}