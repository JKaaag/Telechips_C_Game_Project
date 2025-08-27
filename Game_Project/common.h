#pragma once
// general
long frames;
long score;

void must_init(bool test, const char* description);
int between(int lo, int hi);
float between_f(float lo, float hi);
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

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

extern const int ENEMY_W[];
extern const int ENEMY_H[];

#define ENEMY_1_W ENEMY_W[0]
#define ENEMY_1_H ENEMY_H[0]
#define ENEMY_2_W ENEMY_W[1]
#define ENEMY_2_H ENEMY_H[1]

#define ENEMY_SHOT_W 4
#define ENEMY_SHOT_H 4

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* player_shot[2];

    ALLEGRO_BITMAP* enemy[2];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h);
void sprites_init();
void sprites_deinit();

// audio
ALLEGRO_SAMPLE* sample_shot; // 총알 발사 소리

void audio_init();
void audio_deinit();

//collide
bool enemies_collide();

// player
#define PLAYER_SPEED 3
#define PLAYER_MAX_X (BUFFER_W - PLAYER_W)
#define PLAYER_MAX_Y (BUFFER_H - PLAYER_H)

// 플레이어 이동 방향
typedef enum DIRECTION {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} DIRECTION;

typedef struct PLAYER
{
    int x, y; // 플레이어 위치 좌표
    int shot_timer; // 총알 발사 딜레이 카운터
    int hp; // 플레이어 HP
    int invincible_timer; // 무적 상태 타이머
    DIRECTION last_dir;     // 마지막 이동 방향
} PLAYER;
PLAYER player;

void player_init();
void player_update();
void player_draw();

// enemy
typedef enum ENEMY_TYPE
{
    ENEMY_TYPE_1 = 0, // 일반 몹(1)
    ENEMY_TYPE_2, // 일반 몹(2)
    ENEMY_TYPE_N // 몹 종류 개수
} ENEMY_TYPE;

typedef struct ENEMY
{
    int x, y; // 몹의 현재 위치
    int vx, vy; // 몹의 속도
    ENEMY_TYPE type; // 몹의 종류
    int blink; // 총알 맞은 후 깜빡인 효과용 타이머
    int hp; // 체력
    bool used; // 사용 중인지, 화면에 존재하는지 여부
} ENEMY;

#define ENEMIES_N 5
ENEMY enemies[ENEMIES_N];

bool spawn_enabled; // 리스폰 여부

void enemies_init();
void enemies_update();
void enemies_draw();

//shot
// 플레이어 총알 방향
typedef enum SHOT_DIR {
    SHOT_UP,
    SHOT_DOWN,
    SHOT_LEFT,
    SHOT_RIGHT
} SHOT_DIR;

typedef struct SHOT
{
    int x, y, dx, dy; // 총알 위치, 이동 속도
    int frame; // 애니메이션 프레임
    bool player; // 플레이어 총알(true), 적 총알(false) 구분
    bool used; // 사용 여부(활성화된 총알인지)
    SHOT_DIR dir; // 총알 방향
} SHOT;

#define SHOTS_N 128
SHOT shots[SHOTS_N];

void shots_init();
bool shots_add(bool player, bool straight, int x, int y, DIRECTION dir);
void shots_update();
bool shots_collide(bool player, int x, int y, int w, int h);
void shots_draw();

// hud
ALLEGRO_FONT* font; // HUD용 폰트
long score_display; // 화면에 표시할 점수

void hud_init();
void hud_deinit();