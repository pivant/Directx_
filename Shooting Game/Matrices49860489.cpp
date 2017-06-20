// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <stdio.h>

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  840
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define RELOAD 30;

#define ENEMY_NUM 10 


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface


LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_score0;
LPDIRECT3DTEXTURE9 sprite_score1;
LPDIRECT3DTEXTURE9 sprite_score2;
LPDIRECT3DTEXTURE9 sprite_score3;
LPDIRECT3DTEXTURE9 sprite_score4;
LPDIRECT3DTEXTURE9 sprite_score5;
LPDIRECT3DTEXTURE9 sprite_hero;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_hero_hit;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_enemy;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet2;

LPDIRECT3DTEXTURE9 back;
LPDIRECT3DTEXTURE9 back2;

LPD3DXFONT font;

struct BulletEX
{
	BOOL Visible;
	LPDIRECT3DTEXTURE9 Texture;
	RECT Source;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Center;
};



									 // function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void init_game(void);
void do_game_logic(void);
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using namespace std;


enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };


//기본 클래스 
class entity {

public:
	float x_pos;
	float y_pos;
	int status;

	int HP;
};


bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{

	if ((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1))
		return true;
	else
		return false;
}

class Score:public entity {
public:
	bool score0_show = true;
	bool score1_show = false;
	bool score2_show = false;
	bool score3_show = false;
	bool score4_show = false;
	bool score5_show = false;

	void init(float x, float y);
};

void Score::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

//주인공 클래스 
class Hero :public entity {

public:
	bool hit_Show;
	bool hit_Move = false;
	bool show();
	void hide();
	void active();

	void fire();
	void super_fire();
	void move(int i);
	void init(float x, float y);
	void hit_init(float x, float y);
	bool check_collision(float x, float y);
};

void Hero::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}

void Hero::hit_init(float x, float y)
{

	x_pos = x-20.0f;
	y_pos = y;

}

void Hero::move(int i)
{
	
	switch (i)
	{
	case MOVE_UP:
		y_pos -= 8.0f;
		hit_Move = true;
		break;

	case MOVE_DOWN:
		y_pos += 8.0f;
		hit_Move = true;
		break;


	case MOVE_LEFT:
		x_pos -= 7.0f;
		hit_Move = true;
		break;


	case MOVE_RIGHT:
		x_pos += 7.0f;
		hit_Move = true;
		break;

	}
}
bool Hero::check_collision(float x, float y)
{

	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32.0f, x, y, 32.0f) == true)
	{
		hit_Show = true;
		return true;
	}
	else {
		return false;
	}
}

bool Hero::show()
{
	return hit_Show;
}

void Hero::hide()
{
	hit_Show = false;
}

void Hero::active()
{
	hit_Show = true;
}



// 적 클래스 
class Enemy :public entity {

public:
	void fire();
	void init(float x, float y);
	void move();

};

void Enemy::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}


void Enemy::move()
{
	x_pos -= 3;

}

class Back :public entity {

public:
	
	void init(float x, float y);
	void move();
	

};

void Back::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}


void Back::move()
{
	x_pos -= 3;

}

class Back2 :public entity {

public:

	void init(float x, float y);
	void move();


};

void Back2::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}


void Back2::move()
{
	x_pos -= 3;

}




typedef struct MISSILE_STRUCT {

	float x, y;
	float vx, vy;
	float oldx, oldy;
	float Angle;
	float OldAngle;
	struct MISSILE_STRUCT * Prec;

	bool IsMissile;


} MISSILE_TYPE ;

//MISSILE_TYPE* NewMissileType();

//void ShootMissile(float x, float y, int length)
//{
//	MISSILE_TYPE * missile;
//	MISSILE_TYPE * prec = NULL;
//
//
//	for (int i = 0; i < length; i++, prec = missile)
//	{
//		missile = NewMissileType();
//		missile->x = missile->oldx = x;
//		missile->y = missile->oldy = y;
//		missile->Angle = missile->OldAngle = 0;
//
//		missile->Prec = prec;
//
//		missile->IsMissile = (i == 0);
//
//	}
//}




class Bullet :public entity {

public:

	int hit_count = 0; // 총알이 적과 충돌한 횟수
	bool bShow;
	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);

};

bool Bullet::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32.0f, x, y, 32.0f) == true)
	{
		hit_count = hit_count + 1; // 총알과 적이 충돌할 때마다 충돌횟수를 1씩 증가시킴
		bShow = false;
		return true;
	}
	else {

		return false;
	}
}


void Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;

}

bool Bullet::show()
{
	return bShow;

}


void Bullet::active()
{
	bShow = true;

}

void Bullet::move()
{
	x_pos += 16;
	
}

void Bullet::hide()
{
	bShow = false;

}
	
//	/*float x, float y,*/    // 탄의 좌표
//	float& vx, float& vy,  // 탄의 속도벡터
//	float mx, float my,    // 메인 캐릭터의 좌표
//	float speed,           // 탄의 속도
//	float theta            // 선회 각도의 상한치
//	)
//{
//	// 탄의 원래 속도벡터를 저장
//	float vx0 = vx, vy0 = vy;
//
//	// 메인 캐릭터 방향으로의 속도 벡터(vx1, vy1)를 구하기
//	float vx1, vy1;
//	float d = sqrt((mx - x)*(mx - x) + (my - y)*(my - y));
//	if (d) {
//		vx1 = (mx - x) / d*speed;
//		vy1 = (my - y) / d*speed;
//	}
//	else {
//		vx1 = 0;
//		vy1 = speed;
//	}
//
//	// 시계방향으로 선회할 때의 상한 각도에 해당하는 속도벡터(vx2, vy2)를 구하기:
//	// M_PI는 원주율
//	float rad = 3.14f / 180 * theta;
//	float vx2 = cos(rad)*vx0 - sin(rad)*vy0;
//	float vy2 = sin(rad)*vx0 + cos(rad)*vy0;
//
//	// 메인 캐릭터 방향으로 선회할 지 제한각도만큼만 선회할 지 정하기
//	if (vx0*vx1 + vy0*vy1 >= vx0*vx2 + vy0*vy2) {
//
//		// 메인 캐릭터가 선회 가능한 범위 내에 있을 경우:
//		// 메인 캐릭터 방향으로 선회함
//		vx = vx1;
//		vy = vy1;
//
//	}
//	else {
//
//		// 메인 캐릭터가 선회 가능한 범위 밖에 있을 경우:
//		// 시계 반대방향으로 선회할 때의 상한 각도에 해당하는 속도벡터(vx3, vy3)를 구하기
//		float vx3 = cos(rad)*vx0 + sin(rad)*vy0;
//		float vy3 = -sin(rad)*vx0 + cos(rad)*vy0;
//
//		// 탄에서 메인 캐릭터까지의 상대위치벡터(px, py)를 구하기
//		float px = mx - x, py = my - y;
//
//		// 시계방향으로 선회할 지 시계 반대방향으로 선회할 지 정하기
//		if (px*vx2 + py*vy2 >= px*vx3 + py*vy3) {
//
//			// 시계방향일 경우
//			vx = vx2;
//			vy = vy2;
//
//		}
//		else {
//
//			// 시계 반대방향일 경우
//			vx = vx3;
//			vy = vy3;
//
//		}
//	}
//
//	// 탄의 좌표(x, y)를 갱신하여 탄을 이동시킴
//	x_pos += vx;
//	y_pos += vy;








class Bullet2 :public entity {

public:

	int hit_count = 0; // 총알이 적과 충돌한 횟수
	bool bShow;
	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);

};

bool Bullet2::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32.0f, x, y, 32.0f) == true)
	{
		hit_count = hit_count + 1; // 총알과 적이 충돌할 때마다 충돌횟수를 1씩 증가시킴
		bShow = false;
		return true;
	}
	else {

		return false;
	}
}


void Bullet2::init(float x, float y)
{
	x_pos = x;
	y_pos = y;

}

bool Bullet2::show()
{
	return bShow;

}


void Bullet2::active()
{
	bShow = true;

}

void Bullet2::move()
{
	x_pos += 16;
}

void Bullet2::hide()
{
	bShow = false;

}





//객체 생성 
Hero hero;
Enemy enemy[ENEMY_NUM];
Bullet bullet;
Bullet2 bullet2;
Bullet bulletex[100];
Score score;
Back backgr;
Back backgr2;
LPD3DXFONT FONT2;




int reload;
int reload_count;
bool reload_flag;




// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, "WindowClass", "Our Direct3D Program",
		WS_EX_DLGMODALFRAME | WS_BORDER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);


	//게임 오브젝트 초기화 
	init_game();

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		do_game_logic();

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		while ((GetTickCount() - starting_point) < 25);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		"Panel3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite);    // load to sprite

	//////////////////////////////////////////////////////////////////////////////
	D3DXCreateTextureFromFileEx(d3ddev,
		"score0.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&sprite_score0);

	D3DXCreateTextureFromFileEx(d3ddev,
		"score1.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&sprite_score1);

	D3DXCreateTextureFromFileEx(d3ddev,
		"score2.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&sprite_score2);

	D3DXCreateTextureFromFileEx(d3ddev,
		"score3.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&sprite_score3);

	D3DXCreateTextureFromFileEx(d3ddev,
		"score4.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&sprite_score4);

	D3DXCreateTextureFromFileEx(d3ddev,
		"score5.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&sprite_score5);
	//////////////////////////////////////////////////////////////////////


	D3DXCreateTextureFromFileEx(d3ddev,   
		"niva_neo_final2.png",  
		D3DX_DEFAULT,   
		D3DX_DEFAULT,    
		D3DX_DEFAULT,    
		NULL,   
		D3DFMT_A8R8G8B8,    
		D3DPOOL_MANAGED,    
		D3DX_DEFAULT,   
		D3DX_DEFAULT,   
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,    
		NULL,    
		&sprite_hero);   

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		"hero_hit.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero_hit);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		"enemy.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		"bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		"bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet2);    // load to sprite

	////////////////////////////////////

	D3DXCreateTextureFromFileEx(d3ddev,
		"back.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&back);

	D3DXCreateTextureFromFileEx(d3ddev,
		"back2.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),
		NULL,
		NULL,
		&back2);

	return;
}


void init_game(void)
{
	//객체 초기화 
	hero.init(150.0f, 300.0f);
	score.init(400.0f, 30.0f);

	backgr.init(0, 0);
	backgr2.init(1904, 0);

	reload_count = 0;
	reload_flag = true;

	/*D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &font);*/

	

	D3DXCreateFont(d3ddev,     //D3D Device

		20,               //Font height

		0,                //Font width

		FW_NORMAL,        //Font Weight

		1,                //MipLevels

		false,            //Italic

		DEFAULT_CHARSET,  //CharSet

		OUT_DEFAULT_PRECIS, //OutputPrecision

		ANTIALIASED_QUALITY, //Quality

		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily

		"System",          //pFacename,

		&FONT2);         //ppFont

	

	//적들 초기화 
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		enemy[i].init((float)(SCREEN_WIDTH + (rand() % 300)), rand() % SCREEN_HEIGHT);
	}
	
	//총알 초기화 
	bullet.init(hero.x_pos, hero.y_pos);
	bullet2.init(hero.x_pos, hero.y_pos);

	for (int i = 0; i < 100; i++)
	{
		bulletex[i].init(hero.x_pos, hero.y_pos);
	}
}


void do_game_logic(void)
{

	//주인공 처리 
	if (KEY_DOWN(VK_UP))
		hero.move(MOVE_UP);

	if (KEY_DOWN(VK_DOWN))
		hero.move(MOVE_DOWN);

	if (KEY_DOWN(VK_LEFT))
		hero.move(MOVE_LEFT);

	if (KEY_DOWN(VK_RIGHT))
		hero.move(MOVE_RIGHT);

	//주인공 충돌 처리
	if (hero.show() == false)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (hero.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
			{
				hero.active();
				hero.hit_init(hero.x_pos, (hero.y_pos) - 5);
			}
		}

	}
	if (hero.show() == true)
	{

		if (bullet.bShow == true && bullet2.bShow == true)
		{
			hero.hit_Show = false;

		}
	}

	//적들 처리 
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].x_pos < 0)
			enemy[i].init((float)(SCREEN_WIDTH + (rand() % 300)), rand() % SCREEN_HEIGHT);
		else
			enemy[i].move();
	}

	backgr.move();
	backgr2.move();

	if (backgr.x_pos + 1904 < 0)
		backgr.init(1904, 0);

	if (backgr2.x_pos + 1904 < 0)
		backgr2.init(1904, 0);



	//총알 처리 

	for (int i = 0; i < 100; i++) {

		if (bullet.show() == false && bullet2.show() == false)
		{
			if (KEY_DOWN(VK_SPACE))
			{
				reload_count = 0;
				reload_flag = true;
			}
		}
	}

	if (GetKeyState(0x5a) & 0x80000000)
	{
		for (INT i = 0; i<100; ++i)
		{
			if (bulletex[i].bShow == FALSE && reload_flag == true)
			{
				bulletex[i].active();
				bulletex[i].init(hero.x_pos, hero.y_pos);
				reload_count++;

				if(reload_count >= 30)
				{
					reload_flag = false;
				}

				//bulletex[i].move();
				break;
			}
		}
	}



	for (int i = 0; i < 100; i++)
		{
			if (bulletex[i].show() == true)
			{
				if (bulletex[i].x_pos > SCREEN_WIDTH)
					bulletex[i].hide();
				else
					bulletex[i].move();	
			}

			
		}


		//충돌 처리
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (bullet.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
			{
				enemy[i].init((float)(SCREEN_WIDTH + (rand() % 300)), rand() % SCREEN_HEIGHT);
			}
		}
		
		

}


// this is the function used to render a single frame
void render_frame(void)
{

	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);  

	char string[100];
	RECT TextRt;
	SetRect(&TextRt, 0, 0, 800, 600);
	sprintf(string, " 장탄수   %d / 30  ", 30 - reload_count);
	FONT2->DrawText(NULL, string, -1, &TextRt, DT_LEFT|DT_NOCLIP, 0xFFFFFFFF);
	

	RECT part3;
	SetRect(&part3, 0, 0, 1904, 480);
	D3DXVECTOR3 center1(0, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position3(backgr.x_pos, 0, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(back, &part3, &center1, &position3, D3DCOLOR_ARGB(255, 255, 255, 255));


	RECT part4;
	SetRect(&part4, 0, 0, 1904, 480);
	D3DXVECTOR3 center5(0, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position4(backgr2.x_pos, 0, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(back2, &part4, &center1, &position4, D3DCOLOR_ARGB(255, 255, 255, 255));

	

	if (hero.hit_Show == false)
	{											 //주인공 
		RECT part;
		SetRect(&part, 0, 0, 200, 150);
		D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (hero.hit_Show == true)
	{
		RECT part_1;
		SetRect(&part_1, 0, 0, 64, 64);
		D3DXVECTOR3 center_1(0.0f, 0.0f, 0.0f);    
		D3DXVECTOR3 position_1(hero.x_pos, hero.y_pos, 0.0f);    
		d3dspt->Draw(sprite_hero_hit, &part_1, &center_1, &position_1, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	////총알 
	
	for (int i = 0; i < 100; i++)
	{
			if (bulletex[i].bShow == true)
			{
				RECT part6;
				SetRect(&part6, 0, 0, 120, 70);
				D3DXVECTOR3 center6(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
				D3DXVECTOR3 position6(bulletex[i].x_pos, bulletex[i].y_pos, 0.0f);    // position at 50, 50 with no depth
				d3dspt->Draw(sprite_bullet, &part6, &center6, &position6, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
	}
	

	////에네미 
	RECT part2;
	SetRect(&part2, 0, 0, 64, 64);
	D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

	for (int i = 0; i<ENEMY_NUM; i++)
	{
		D3DXVECTOR3 position2(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//

	
	


	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_hero->Release();
	sprite_hero_hit->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();

	back->Release();
	back->Release();

	FONT2->Release();

	return;
}
