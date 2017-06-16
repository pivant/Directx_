// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  840
#define SCREEN_HEIGHT 480
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

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






// 총알 클래스 
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
		hit_count = hit_count+1; // 총알과 적이 충돌할 때마다 충돌횟수를 1씩 증가시킴
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
	y_pos -= 10;
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
Score score;


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
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
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
		L"Panel3.png",    // the file name
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
		L"score0.png",
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
		L"score1.png",
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
		L"score2.png",
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
		L"score3.png",
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
		L"score4.png",
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
		L"score5.png",
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
		L"hero.png",  
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
		L"hero_hit.png",    // the file name
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
		L"enemy.png",    // the file name
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
		L"bullet.png",    // the file name
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
		L"bullet.png",    // the file name
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

	return;
}


void init_game(void)
{
	//객체 초기화 
	hero.init(150.0f, 300.0f);
	score.init(400.0f, 30.0f);
	//적들 초기화 
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		enemy[i].init((float)(SCREEN_WIDTH + (rand() % 300)), rand() % SCREEN_HEIGHT);
	}
	
	//총알 초기화 
	bullet.init(hero.x_pos, hero.y_pos);
	bullet2.init(hero.x_pos, hero.y_pos);
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



	//총알 처리 
	if (bullet.show() == false && bullet2.show() == false)
	{
		if (KEY_DOWN(VK_SPACE))
		{
			bullet.active();
			bullet2.active();
			bullet.init(hero.x_pos, hero.y_pos);
			bullet2.init(hero.x_pos, hero.y_pos+20);
		}
	}


	if (bullet.show() == true)
	{
		if (bullet.x_pos > SCREEN_WIDTH)
			bullet.hide();
		else
			bullet.move();


		//충돌 처리(충돌한 횟수 체크해서 스코어 계산)
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (bullet.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
			{
				enemy[i].init((float)(SCREEN_WIDTH + (rand() % 300)), rand() % SCREEN_HEIGHT);
				if (bullet.hit_count == 1) // 충돌 횟수 1회시
				{
					score.score0_show = false;
					score.score1_show = true;
				}
				if (bullet.hit_count == 2) // 충돌 횟수 2회시
				{
					score.score1_show = false;
					score.score2_show = true;
				}
				if (bullet.hit_count == 3) 
				{
					score.score2_show = false;
					score.score3_show = true;
				}
				if (bullet.hit_count == 4)
				{
					score.score3_show = false;
					score.score4_show = true;
				}
				if (bullet.hit_count == 5) // 충
				{
					score.score4_show = false;
					score.score5_show = true;
				}
			}
		}
	}
	if (bullet2.show() == true)
	{
		if (bullet2.x_pos > SCREEN_WIDTH)
			bullet2.hide();
		else
			bullet2.move();


		//충돌 처리(충돌한 횟수 체크해서 스코어 계산)
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (bullet2.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
			{
				enemy[i].init((float)(SCREEN_WIDTH + (rand() % 300)), rand() % SCREEN_HEIGHT);
				if (bullet2.hit_count == 1) // 충돌 횟수 1회시
				{
					score.score0_show = false;
					score.score1_show = true;
				}
				if (bullet2.hit_count == 2) // 충돌 횟수 2회시
				{
					score.score1_show = false;
					score.score2_show = true;
				}
				if (bullet2.hit_count == 3)
				{
					score.score2_show = false;
					score.score3_show = true;
				}
				if (bullet2.hit_count == 4)
				{
					score.score3_show = false;
					score.score4_show = true;
				}
				if (bullet2.hit_count == 5) // 충
				{
					score.score4_show = false;
					score.score5_show = true;
				}
			}
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

	/////////////////////////////////////스코어//////////////////////////////////////////

	if (score.score0_show == true) // score 0점을 불러온다
	{
		RECT Spart;
		SetRect(&Spart, 0, 0, 176, 54);
		D3DXVECTOR3 Scenter(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Sposition(score.x_pos, score.y_pos, 0.0f);
		d3dspt->Draw(sprite_score0, &Spart, &Scenter, &Sposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (score.score1_show == true) // score 1점을 불러온다
	{
		RECT Spart;
		SetRect(&Spart, 0, 0, 176, 54);
		D3DXVECTOR3 Scenter(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Sposition(score.x_pos, score.y_pos, 0.0f);
		d3dspt->Draw(sprite_score1, &Spart, &Scenter, &Sposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (score.score2_show == true)
	{
		RECT Spart;
		SetRect(&Spart, 0, 0, 176, 54);
		D3DXVECTOR3 Scenter(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Sposition(score.x_pos, score.y_pos, 0.0f);
		d3dspt->Draw(sprite_score2, &Spart, &Scenter, &Sposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (score.score3_show == true)
	{
		RECT Spart;
		SetRect(&Spart, 0, 0, 176, 54);
		D3DXVECTOR3 Scenter(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Sposition(score.x_pos, score.y_pos, 0.0f);
		d3dspt->Draw(sprite_score3, &Spart, &Scenter, &Sposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (score.score4_show == true)
	{
		RECT Spart;
		SetRect(&Spart, 0, 0, 176, 54);
		D3DXVECTOR3 Scenter(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Sposition(score.x_pos, score.y_pos, 0.0f);
		d3dspt->Draw(sprite_score4, &Spart, &Scenter, &Sposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
		
	if (score.score5_show == true)
	{
		RECT Spart;
		SetRect(&Spart, 0, 0, 176, 54);
		D3DXVECTOR3 Scenter(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Sposition(score.x_pos, score.y_pos, 0.0f);
		d3dspt->Draw(sprite_score5, &Spart, &Scenter, &Sposition, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	/////////////////////////////////////스코어끝/////////////////////////////////////////
	if (hero.hit_Show == false)
	{											 //주인공 
		RECT part;
		SetRect(&part, 0, 0, 64, 64);
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
	if (bullet.bShow == true)
	{
		RECT part1;
		SetRect(&part1, 0, 0, 64, 64);
		D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position1(bullet.x_pos, bullet.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (bullet2.bShow == true)
	{
		RECT part1;
		SetRect(&part1, 0, 0, 64, 64);
		D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position1(bullet2.x_pos, bullet2.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_bullet2, &part1, &center1, &position1, D3DCOLOR_ARGB(255, 255, 255, 255));
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

	return;
}
