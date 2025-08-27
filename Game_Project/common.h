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
#define KEY_SEEN 1 // ���� �����ӿ��� ���ȴ��� Ȯ��, ���� �����ӿ��� ������ ��Ʈ ���� (0b00000001)
#define KEY_DOWN 2 // Ű�� ��� ������ �ִ���, ������ �ִٸ� ���� �����ӱ��� ��Ʈ ���� (0b00000010)

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
ALLEGRO_SAMPLE* sample_shot; // �Ѿ� �߻� �Ҹ�

void audio_init();
void audio_deinit();

//collide
bool enemies_collide();

// player
#define PLAYER_SPEED 3
#define PLAYER_MAX_X (BUFFER_W - PLAYER_W)
#define PLAYER_MAX_Y (BUFFER_H - PLAYER_H)

// �÷��̾� �̵� ����
typedef enum DIRECTION {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} DIRECTION;

typedef struct PLAYER
{
    int x, y; // �÷��̾� ��ġ ��ǥ
    int shot_timer; // �Ѿ� �߻� ������ ī����
    int hp; // �÷��̾� HP
    int invincible_timer; // ���� ���� Ÿ�̸�
    DIRECTION last_dir;     // ������ �̵� ����
} PLAYER;
PLAYER player;

void player_init();
void player_update();
void player_draw();

// enemy
typedef enum ENEMY_TYPE
{
    ENEMY_TYPE_1 = 0, // �Ϲ� ��(1)
    ENEMY_TYPE_2, // �Ϲ� ��(2)
    ENEMY_TYPE_N // �� ���� ����
} ENEMY_TYPE;

typedef struct ENEMY
{
    int x, y; // ���� ���� ��ġ
    int vx, vy; // ���� �ӵ�
    ENEMY_TYPE type; // ���� ����
    int blink; // �Ѿ� ���� �� ������ ȿ���� Ÿ�̸�
    int hp; // ü��
    bool used; // ��� ������, ȭ�鿡 �����ϴ��� ����
} ENEMY;

#define ENEMIES_N 5
ENEMY enemies[ENEMIES_N];

bool spawn_enabled; // ������ ����

void enemies_init();
void enemies_update();
void enemies_draw();

//shot
// �÷��̾� �Ѿ� ����
typedef enum SHOT_DIR {
    SHOT_UP,
    SHOT_DOWN,
    SHOT_LEFT,
    SHOT_RIGHT
} SHOT_DIR;

typedef struct SHOT
{
    int x, y, dx, dy; // �Ѿ� ��ġ, �̵� �ӵ�
    int frame; // �ִϸ��̼� ������
    bool player; // �÷��̾� �Ѿ�(true), �� �Ѿ�(false) ����
    bool used; // ��� ����(Ȱ��ȭ�� �Ѿ�����)
    SHOT_DIR dir; // �Ѿ� ����
} SHOT;

#define SHOTS_N 128
SHOT shots[SHOTS_N];

void shots_init();
bool shots_add(bool player, bool straight, int x, int y, DIRECTION dir);
void shots_update();
bool shots_collide(bool player, int x, int y, int w, int h);
void shots_draw();

// hud
ALLEGRO_FONT* font; // HUD�� ��Ʈ
long score_display; // ȭ�鿡 ǥ���� ����

void hud_init();
void hud_deinit();