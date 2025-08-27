#pragma once
// general
long frames;
long score;

void must_init(bool test, const char* description);
float between_f(float lo, float hi);

// display
#define BUFFER_W 320
#define BUFFER_H 240

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init();
void disp_deinit();
void disp_pre_draw();
void disp_post_draw();

// keyboard
#define KEY_SEEN 1 // 현재 프레임에서 눌렸는지 확인, 다음 프레임에선 무조건 비트 해제 (0b00000001)
#define KEY_DOWN 2 // 키를 계속 누르고 있는지, 누르고 있다면 다음 프레임까지 비트 유지 (0b00000010)

unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init();
void keyboard_update(ALLEGRO_EVENT* event);

//sprite
#define PLAYER_W 10
#define PLAYER_H 10

#define PLAYER_SHOT_W 2
#define PLAYER_SHOT_H 9

#define ENEMY_SHOT_W 4
#define ENEMY_SHOT_H 4

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* player_shot[2];
} SPRITES;
SPRITES sprites;

// audio
ALLEGRO_SAMPLE* sample_shot; // 총알 발사 소리

//shot
typedef struct SHOT
{
    int x, y, dx, dy; // 총알 위치, 이동 속도
    int frame; // 애니메이션 프레임
    bool player; // 플레이어 총알(true), 적 총알(false) 구분
    bool used; // 사용 여부(활성화된 총알인지)
} SHOT;

void shots_init();
bool shots_add(bool player, bool straight, int x, int y);
void shots_update();
bool shots_collide(bool ship, int x, int y, int w, int h);
void shots_draw();

#define SHOTS_N 128
SHOT shots[SHOTS_N];

// player
#define PLAYER_SPEED 3
#define PLAYER_MAX_X (BUFFER_W - PLAYER_W)
#define PLAYER_MAX_Y (BUFFER_H - PLAYER_H)

typedef struct PLAYER
{
    int x, y; // 플레이어 위치 좌표
    int shot_timer; // 총알 발사 딜레이 카운터
    int hp; // 플레이어 HP
    int invincible_timer; // 무적 상태 타이머
} PLAYER;
PLAYER player;

void player_init();
void player_update();
void player_draw();

// enemy
typedef enum ENEMY_TYPE
{
    ENEMY_NORMAL = 0, // 일반 몹
    ENEMY_TYPE_N // 몹 종류 개수
} ENEMY_TYPE;

typedef struct ENEMY
{
    int x, y; // 몹의 현재 위치
    ENEMY_TYPE type; // 몹의 종류
    int blink; // 총알 맞은 후 깜빡인 효과용 타이머
    int hp; // 체력
    bool used; // 사용 중인지, 화면에 존재하는지 여부
} ENEMY;

#define ENEMIES_N 2
ENEMY enemies[ENEMIES_N];

// hud
ALLEGRO_FONT* font; // HUD용 폰트
long score_display; // 화면에 표시할 점수

void hud_init();
void hud_deinit();