#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>               // Allegro 기본
#include <allegro5/allegro_image.h>         // 이미지
#include <allegro5/allegro_font.h>          // 폰트
#include <allegro5/allegro_ttf.h>           // TTF 폰트
#include <allegro5/allegro_primitives.h>    // 사각형, 원 등 도형
#include <allegro5/allegro_audio.h>         // 오디오
#include <allegro5/allegro_acodec.h>        // 오디오 코덱
#include <allegro5/allegro_native_dialog.h> // 메시지박스 등

int main(void) {
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro!\n");
        return -1;
    }

    // 키보드, 마우스, 이미지, 폰트, 오디오 초기화
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);

    // 화면 생성
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Failed to create display!\n");
        return -1;
    }

    // 게임 루프 등 구현...

    // 화면 해제
    al_destroy_display(display);
    return 0;
}