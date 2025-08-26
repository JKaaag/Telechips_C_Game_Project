#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>               // Allegro �⺻
#include <allegro5/allegro_image.h>         // �̹���
#include <allegro5/allegro_font.h>          // ��Ʈ
#include <allegro5/allegro_ttf.h>           // TTF ��Ʈ
#include <allegro5/allegro_primitives.h>    // �簢��, �� �� ����
#include <allegro5/allegro_audio.h>         // �����
#include <allegro5/allegro_acodec.h>        // ����� �ڵ�
#include <allegro5/allegro_native_dialog.h> // �޽����ڽ� ��

int main(void) {
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro!\n");
        return -1;
    }

    // Ű����, ���콺, �̹���, ��Ʈ, ����� �ʱ�ȭ
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);

    // ȭ�� ����
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Failed to create display!\n");
        return -1;
    }

    // ���� ���� �� ����...

    // ȭ�� ����
    al_destroy_display(display);
    return 0;
}