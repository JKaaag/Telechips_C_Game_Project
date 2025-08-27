#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "common.h"

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch (event->type)
    {
        // 1/60�ʸ��� ȣ��
    case ALLEGRO_EVENT_TIMER:
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= ~KEY_SEEN;
        break;

        // Ű�� ���� ���
    case ALLEGRO_EVENT_KEY_DOWN:
        key[event->keyboard.keycode] = KEY_SEEN | KEY_DOWN;
        break;

        // Ű�� �� ���
    case ALLEGRO_EVENT_KEY_UP:
        key[event->keyboard.keycode] &= ~KEY_DOWN;
        break;
    }
}
/*
����1)
Ű�� ó�� ������ ��:
key[LEFT] = KEY_SEEN | KEY_DOWN = 00000011
���� ���� (KEY_DOWN)
�̹� �����ӿ� ���� (KEY_SEEN)

Ű�� ��� ������ �ִ� ����:
key[LEFT] &= ~KEY_SEEN �� 00000010
KEY_DOWN�� ���� �� ��� ���� �ִ� ���¸� ����

Ű�� ���� ��:
key[LEFT] &= ~KEY_DOWN �� 00000000
���� ���� ������ ����

����2)
������ 1 : Ű ����
key[LEFT] = SEEN | DOWN = 3 (00000011)
�� "����Ű �Է� ����!" �̺�Ʈ ó��

������ 2 : ��� ������ ��
key[LEFT] = DOWN = 2 (00000010)  // SEEN ���ŵ�
�� ��� ���������� �̵� ����

������ 3 : ������ ������ ��
key[LEFT] = DOWN = 2
�� ��� ���������� �̵� ����

������ 4 : Ű�� ��
key[LEFT] = 0
�� �Է� ����
*/