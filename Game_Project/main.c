#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "common.h"

int main() {
    must_init(al_init(), "allegro"); // Allegro ���̺귯�� �ʱ�ȭ
    must_init(al_install_keyboard(), "keyboard"); // Ű���� �Է� �ʱ�ȭ

    // 60fps Ÿ�̸� ����
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    // �̺�Ʈ ť ����
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init(); // ȭ�� �ʱ�ȭ
    audio_init(); // ����� �ʱ�ȭ

    must_init(al_init_image_addon(), "image"); // �̹��� ���� �߰� ��� �ʱ�ȭ
    sprites_init(); // ��������Ʈ(ĳ����, ������ ��) �ʱ�ȭ

    hud_init(); // ����, ��� ǥ�� �� HUD �ʱ�ȭ

    must_init(al_init_primitives_addon(), "primitives"); // ���� �׸��� ��� �ʱ�ȭ
    
    // �̺�Ʈ ť�� �̺�Ʈ �ҽ� ���
    al_register_event_source(queue, al_get_keyboard_event_source()); // Ű����
    al_register_event_source(queue, al_get_display_event_source(disp)); // ���÷���
    al_register_event_source(queue, al_get_timer_event_source(timer)); // Ÿ�̸�
    
    // ���� ��ü �ʱ�ȭ
    keyboard_init(); // Ű���� ���� �ʱ�ȭ
    shots_init(); // �Ѿ� �ʱ�ȭ
    player_init(); // �÷��̾� �ʱ�ȭ
    enemies_init(); // �� �ʱ�ȭ

    // ���� ���� �ʱ�ȭ
    frames = 0; // ������ ī����
    score = 0; // ���� �ʱ�ȭ


    bool done = false; // ���� ���� ����
    bool redraw = true; // ȭ�� �ٽ� �׸��� ����
    ALLEGRO_EVENT event;

    // Ÿ�̸� ���� -> �� 1/60�ʸ��� ALLEGRO_EVENT_TIMER �̺�Ʈ �߻�
    al_start_timer(timer);

    while (1)
    {
        // �̺�Ʈ ���
        // �̺�Ʈ�� ť�� �������� ���̰�, �������� �ϳ��� ó����
        // Ÿ�̸�, Ű����, â �ݱ� �̺�Ʈ�� ������ ������� �����ϰ� ó�� ����
        al_wait_for_event(queue, &event);

        // �̺�Ʈ ó��
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:  // Ÿ�̸� �̺�Ʈ -> ���� ������Ʈ
            shots_update();     // �Ѿ� ������Ʈ
            player_update();      // �÷��̾� ������Ʈ
            enemies_update();    // �� ������Ʈ

            // ESC Ű -> ���� ����
            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            redraw = true; // ȭ�� �ٽ� �׸��� �÷���
            frames++;      // ������ ����;
            break;

            // â �ݱ� �̺�Ʈ
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        // ���� ���� Ȯ��
        if (done)
            break;

        // Ű���� �Է� ������Ʈ
        keyboard_update(&event);

        // ȭ�� �׸���
        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw(); // ���� ���� �غ�
            al_clear_to_color(al_map_rgb(0, 0, 0)); // ȭ�� Ŭ���� (������)

            enemies_draw(); // ��
            shots_draw();   // �Ѿ�
            player_draw(); // �÷��̾�

            disp_post_draw(); // ���� ���� ȭ�鿡 ���
            redraw = false;
        }
    }

    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}