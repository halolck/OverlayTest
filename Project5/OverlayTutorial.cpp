//Basic Headerr
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <dwmapi.h>
//test
#include <iostream>



//警告用
#ifdef _MSC_VER
#define DISABLE_C4996   __pragma(warning(push)) __pragma(warning(disable:4996))
#define ENABLE_C4996    __pragma(warning(pop))
#else
#define DISABLE_C4996
#define ENABLE_C4996
#endif

//directX Header
#include <d3d9.h>
#include <d3dx9.h>

//directXlib
#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define WS_EX_LAYERED 0x00080000
#define LWA_ALPHA 0x00000002
#define ARGB_TRANS 0x00000099

LPDIRECT3D9EX d3d; //d3dオブジェクトを使ったウィンドウを作る
LPDIRECT3DDEVICE9EX d3ddev; //d3dオブジェクトを使ったウィンドウ内のレンダー


int w_Width = 800;//初期ウィンドウサイズ(横幅)
int w_Height = 600;//初期ウィンドウサイズ(高さ)

ID3DXFont *font = NULL;//作成ウィンドウ内で使うフォント
HRESULT hr = D3D_OK;
MARGINS windowMargins = { -1,-1,-1,-1 };//ウィンドウのマージン
const char *AppName = " ";

HWND App_hwnd = NULL; //Handle toWindow
DWORD App_pID = 0; //ProcessID Window
HANDLE App_hProc = NULL;

bool init_ok = false;
COORD w_pos = { 50,50 }; //初期ウィンドウ位置
COORD w_res = { 800,600 };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


LPCTSTR WindowName = "無題 - メモ帳";

void DrawStringAt(LPCSTR string, int x, int y, COLORREF color, ID3DXFont* Font)
{
	RECT font_rect;
	SetRect(&font_rect, x, y, w_res.X, w_res.Y);
	Font->DrawText(NULL, string, -1, &font_rect, DT_LEFT | DT_NOCLIP | DT_SINGLELINE,color);
}

HRESULT D3DStartup(HWND hwnd)
{
	BOOL bCompOk = FALSE;
	D3DPRESENT_PARAMETERS pp;
	DWORD msqAAQuality =0;//マスククオリティ
	HRESULT hr;

	//DWM構成が有効になっていることを確認してください
	DwmIsCompositionEnabled(&bCompOk);
	if (!bCompOk) return E_FAIL;

	hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d);
	if (FAILED(hr))
	{
		MessageBox(0, "D3dオブジェクトの作成に失敗しました。","Error", 0);
		return E_FAIL;
	}

	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;

	if (SUCCEEDED(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8B8G8R8, TRUE, D3DMULTISAMPLE_NONMASKABLE, &msqAAQuality)))
	{
		//アンチエイジング設定

		pp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
		pp.MultiSampleQuality = msqAAQuality - 1;
	}
	else
	{
		//アンチエイジング設定なし
		pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	hr = d3d->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, NULL, &d3ddev);

	if (FAILED(hr))
	{
		MessageBox(0, "d3dデバイスオブジェクトの作成に失敗しました。", "Error", 0);
		return E_FAIL;
	}
}
BOOL BUTTONFLUG = FALSE;
VOID Render(VOID)
{
	COLORREF color = 0x20FF00FF; //A R G B :)
	if (d3ddev == NULL) return;

	//バックバッファーのクリアー & バッファーの透明化
	//	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, ARGB_TRANS, 1.0f, 0);
	d3ddev->BeginScene();

	//テキストをウィンドウに描画
	DrawStringAt("DrawTestYeah:)",30,50,color,font);
	if (BUTTONFLUG)
	{
		DrawStringAt(("heyyy"), 30, 150, color, font);
	}
	d3ddev->EndScene();

	//ディスプレイを更新
	d3ddev->PresentEx(NULL, NULL, NULL, NULL, NULL);
}

void D3DShutdown(void)
{
	if (d3ddev != NULL)
		d3ddev->Release();
	if (d3d != NULL)
		d3d->Release();
}

void InputPerText(std::string str,int SleepTimer,int FinishSleep)
{
	for (int i = 0; i < (int)str.size(); ++i) {
		char ch = str[i];
		std::cout << ch;
		Sleep(SleepTimer);
	}
	Sleep(FinishSleep);
}
#include <random>
#include <string>
std::string random_string(std::string::size_type length)
{
	static auto& chrs = "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	thread_local static std::mt19937 rg{ std::random_device{}() };
	thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

	std::string s;

	s.reserve(length);

	while (length--)
		s += chrs[pick(rg)];

	return s;
}

INT main(int argc, char* argv[],HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{

	int sleeptimer = 10;
	std::cout << R"(  _____ _____ ____ _____ _     ___    _    ____  _____ ____  )"<<std::endl;
	Sleep(sleeptimer);
	std::cout << R"( |_   _| ____/ ___|_   _| |   / _ \  / \  |  _ \| ____|  _ \)" << std::endl;
	Sleep(sleeptimer);
	std::cout << R"(   | | |  _| \___ \ | | | |  | | | |/ _ \ | | | |  _| | |_) |)" << std::endl;
	Sleep(sleeptimer);
	std::cout << R"(   | | | |___ ___) || | | |__| |_| / ___ \| |_| | |___|  _ <)" << std::endl;
	Sleep(sleeptimer);
	std::cout << R"(   |_| |_____|____/ |_| |_____\___/_/   \_\____/|_____|_| \_\)" <<"\n\n";
	Sleep(sleeptimer);
		

	if (argc == 1)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE | NULL);
		InputPerText("[?]Looking for applications...",20,500);
		Sleep(500);


		DISABLE_C4996
		char szPath[_MAX_PATH];
		char szDrive[_MAX_DRIVE];
		char szDir[_MAX_DIR];
		char szFileName[_MAX_FNAME];
		char szExt[_MAX_EXT];
		DWORD dwRet;

		memset(szPath, 0x00, sizeof(szPath));
		memset(szDrive, 0x00, sizeof(szDrive));
		memset(szDir, 0x00, sizeof(szDir));
		memset(szExt, 0x00, sizeof(szExt));
		dwRet = 0;
		dwRet = GetModuleFileName(NULL, szPath, sizeof(szPath));
		_splitpath(szPath, szDrive, szDir, szFileName, szExt);

		ENABLE_C4996
		SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | NULL);
		InputPerText("\n[-]Found Application ", 20,0);
		InputPerText("(" + std::string(szPath) + ")", 2, 1000);

		SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE | NULL);
		InputPerText("\n[?]Rename Application... ", 20,0);
		InputPerText("Target:" + std::string(szFileName)+ std::string(szExt), 2, 500);

		std::string randapp = random_string(rand() % 20 + 14);
		std::string newRename = std::string(szDrive) + std::string(szDir) + randapp + std::string(szExt);
		rename(szPath, newRename.c_str());
		SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | NULL);
		InputPerText("\n[-]Rename Success! ", 20, 0);
		InputPerText(randapp+ std::string(szExt), 2, 500);
		SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE | NULL);
		InputPerText("\n[?]ReBoot App...", 20, 800);
		ShellExecute(NULL, "open", newRename.c_str(), "True", NULL, SW_SHOWNORMAL);//Trueを後で変更する(licensekeyに)

		std::exit(1);

	}
	else
	{
		InputPerText("Lisence:Free!!\n", 20, 800);
		Sleep(1000);
		std::cout << "waiting'" << WindowName << "'..." << std::endl;
		while (App_hwnd == NULL)
		{
			App_hwnd = FindWindow(NULL, WindowName);
			Sleep(500);
		}
		InputPerText("\n[-]Found App!", 20, 1000);

		//プログラム起動まで待機
	}

	char* a = argv[0];

	//名前を次に備えてランダムに変える(作成中は隠さないと増えて大変なことになるかもそれか引用起動で通常起動FalseとするとTrueの時だけ普通に起動、そうじゃなければ自身の名前を変えた後Trueで再度起動とか良いかも)
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	HWND hWnd = NULL;
	MSG uMsg;
	HRESULT hr;
	long Loops = 0;
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),NULL,WindowProc,NULL,NULL,hInstance,LoadIcon(NULL,MAKEINTRESOURCE(32512)),
		LoadCursor(NULL,IDC_CROSS),NULL,NULL,AppName,LoadIcon(NULL,IDI_APPLICATION) };

	RegisterClassEx(&wc);

	//オーバーレイを介してその背後のウィンドウに入力を渡す透明なウィンドウ (オーバーレイ) を作成する 
	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_COMPOSITED | WS_EX_TRANSPARENT | WS_EX_LAYERED, AppName, AppName,
		WS_POPUP, w_pos.X, w_pos.Y, w_res.X, w_res.Y, NULL, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), (BYTE)255, LWA_ALPHA);
	hr = DwmExtendFrameIntoClientArea(hWnd, &windowMargins);

	AppName = random_string(rand() % 20 + 14).c_str();

	//D3D初期化
	if (SUCCEEDED(D3DStartup(hWnd)))
	{
		D3DXCreateFont(d3ddev, 20, 10, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Meryo"), &font);



		//ウィンドウを開く
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);


		while (TRUE)
		{
			//メッセージをチェック
			if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
			{	
				//メッセージが'WM_QUIT'だったら
				if (uMsg.message == WM_QUIT)
				{
					//ループを終了する(break)
					break;
				}

				//メッセージを送る
				TranslateMessage(&uMsg);
				DispatchMessage(&uMsg);
			}

			if (App_hwnd == NULL)
				App_hwnd = FindWindow(NULL, WindowName);
			if ((App_hwnd != NULL) && (App_pID == 0))
			{
				GetWindowThreadProcessId(App_hwnd, &App_pID);
			}
			if ((App_hProc == NULL) && (App_pID != 0))
			{
				App_hProc = OpenProcess(PROCESS_VM_READ, false, App_pID);
			}
			if (App_hProc != NULL)
			{
				if ((!init_ok) || ((Loops % 1) == 0)) //%の後ろの値について・1000回ループ割る後ろの数字分処理 5の場合1000につき200 これをimgui当たりでごにょごにょしてあげれば良い感じになりそう
				{
					RECT client_rect;
					GetClientRect(App_hwnd, &client_rect); //res取得
					w_res.X = client_rect.right;
					w_res.Y = client_rect.bottom;

					RECT bounding_rect;
					GetWindowRect(App_hwnd, &bounding_rect); //pos取得
					if (!init_ok)
					{
						if ((w_pos.X != bounding_rect.left) || (w_pos.Y != bounding_rect.top))
						{
							MoveWindow(hWnd, bounding_rect.left, bounding_rect.top, client_rect.right, client_rect.bottom, false);
							w_pos.X = bounding_rect.left;
							w_pos.Y = bounding_rect.top;
						}
					}
					else
					{
						if ((bounding_rect.left == 0) && (bounding_rect.top == 0))
						{
							//1ピクセルの移動を強制するdxのバグがある可能性があるので、ウィンドウのサイズが変更されないようこうする
							MoveWindow(hWnd, bounding_rect.left - 1, bounding_rect.top - 1, client_rect.right, client_rect.bottom, false);
						}
						MoveWindow(hWnd, bounding_rect.left, bounding_rect.top, client_rect.right, client_rect.bottom, false);
					}
					init_ok = true;		//最後に初期化
				}
			}

			if (GetAsyncKeyState(VK_F3) & 0x01)
			{
				BUTTONFLUG = TRUE;
			}
			else
			{
				BUTTONFLUG = FALSE;
			}

			if (GetAsyncKeyState(VK_END) & 0x01)
			{
				break;
			}

			if (Loops % 10 == 0)
			{
				if (FindWindow(NULL, WindowName) == NULL)
				{
					SendMessage(hWnd, WM_CLOSE, NULL, NULL);//ゲームが終了したときプログラムも閉じるようにする
				}
			}
			Loops++;
			if (Loops > 1000) Loops = 0;


			//RENDERRRRRRRRRRRRRRRRRRRRRRRR IYAAAAAAAAAAAAAAAAAAAAAAA
			Render();
		}
	}
	std::system("cls");
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cout << "ShutDown Application.\nThis console will exit after 1 second.";
	Sleep(1000);
	//D3Dシャットダウン
	D3DShutdown();

	//終了
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return 0;
	}break;
	}

	return DefWindowProc( hWnd, message, wParam, lparam);
}