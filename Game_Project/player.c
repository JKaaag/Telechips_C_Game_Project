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
    player.invincible_timer = 120; // ���� �ð�

}

void player_update()
{
    // HP�� 0���� ������ �ƹ� �͵� ���� ����
    if (player.hp < 0)
        return;

    // Ű �Է¿� ���� �̵� ó�� �� �÷��̾� �̵� ���� ����
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

    // ȭ�� ������ ������ �ʵ��� ����
    if (player.x < 0)
        player.x = 0;
    if (player.y < 110)
        player.y = 110;

    if (player.x > PLAYER_MAX_X)
        player.x = PLAYER_MAX_X;
    if (player.y > PLAYER_MAX_Y)
        player.y = PLAYER_MAX_Y;

    // ���� ���� ó��
    if (player.invincible_timer > 0) {
        player.invincible_timer--;
    }
    else {
        // ������ �ƴ� ���� �浹 üũ
        if (enemies_collide(true, player.x, player.y, PLAYER_W, PLAYER_H)) {
            player.hp -= 10;
            if (player.hp < 0) player.hp = 0;
            player.invincible_timer = 120; // 2�� ���� ����
        }
    }

    // �Ѿ� �߻� ó��
    // �߻� ������ ī���� ����
    if (player.shot_timer)
        player.shot_timer--;
    else if (key[ALLEGRO_KEY_X])
    {
        // �÷��̾� �Ѿ� ����
        int x = player.x + (PLAYER_W / 2);
        // �߻� �� ��߻� ���
        if (shots_add(true, true, player.x, player.y, player.last_dir))
            player.shot_timer = 5;
    }
}

void player_draw()
{
    if (player.hp <= 0)
        return;

    // �÷��̾� HP ǥ��
    char hp_text[16];
    sprintf(hp_text, "%d", player.hp);
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        player.x + PLAYER_W / 2, // �÷��̾� �߾�
        player.y - 10,           // ����
        ALLEGRO_ALIGN_CENTER,
        hp_text
    );

    // ���� ������ ȿ�� ó��
    if (player.invincible_timer > 0)
    {
        if (((player.invincible_timer / 2) % 3) == 1)
            return; // �� �����ӿ��� �׸��� ����
    }

    // �÷��̾� �׸� �׸���
    al_draw_filled_rectangle(
        player.x,
        player.y,
        player.x + PLAYER_W,
        player.y + PLAYER_H,
        al_map_rgb(0, 255, 0) // �ʷϻ�
    );
}