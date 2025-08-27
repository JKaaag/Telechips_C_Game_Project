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

#define ENEMY_SHOT_W 4
#define ENEMY_SHOT_H 4

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* player_shot[2];
} SPRITES;
SPRITES sprites;

// audio
ALLEGRO_SAMPLE* sample_shot; // �Ѿ� �߻� �Ҹ�

//shot
typedef struct SHOT
{
    int x, y, dx, dy; // �Ѿ� ��ġ, �̵� �ӵ�
    int frame; // �ִϸ��̼� ������
    bool player; // �÷��̾� �Ѿ�(true), �� �Ѿ�(false) ����
    bool used; // ��� ����(Ȱ��ȭ�� �Ѿ�����)
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
    int x, y; // �÷��̾� ��ġ ��ǥ
    int shot_timer; // �Ѿ� �߻� ������ ī����
    int hp; // �÷��̾� HP
    int invincible_timer; // ���� ���� Ÿ�̸�
} PLAYER;
PLAYER player;

void player_init();
void player_update();
void player_draw();

// enemy
typedef enum ENEMY_TYPE
{
    ENEMY_NORMAL = 0, // �Ϲ� ��
    ENEMY_TYPE_N // �� ���� ����
} ENEMY_TYPE;

typedef struct ENEMY
{
    int x, y; // ���� ���� ��ġ
    ENEMY_TYPE type; // ���� ����
    int blink; // �Ѿ� ���� �� ������ ȿ���� Ÿ�̸�
    int hp; // ü��
    bool used; // ��� ������, ȭ�鿡 �����ϴ��� ����
} ENEMY;

#define ENEMIES_N 2
ENEMY enemies[ENEMIES_N];

// hud
ALLEGRO_FONT* font; // HUD�� ��Ʈ
long score_display; // ȭ�鿡 ǥ���� ����

void hud_init();
void hud_deinit();