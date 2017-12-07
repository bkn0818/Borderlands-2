// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <d3d9.h>
#include <d3dx9math.h>
#include <utility>
#include <assert.h>
#include <time.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define MAX_LOADSTRING 100

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p) = NULL; } } 
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p) = NULL; } } 
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p) = NULL; } } 

#define SINGLETON( className ) private: \
className (); \
~className (); \
public: \
	static className* GetInstance () \
	{ \
		static className instance; \
		return &instance; \
	}

extern HINSTANCE	g_hInst;
extern HWND			g_HWND_Window;
extern POINT		g_ptMouse;


#include "Asciitok.h"
#include "VideoDevice.h"
#include "CameraCtrl.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "KeyManager.h"
#include "TextureManager.h"
#include "Object.h"
#include "ObjectManager.h"
#include "FontManager.h"
#include "Utility.h"
#include "CommonMacroFunction.h"
#include "MaptoolDevice.h"
#include "UIObject.h"
#include "UIButton.h"
#include "UIImageView.h"
#include "UITextView.h"
#include "TimeManager.h"
#include "FrustumCulling.h"
#include "SceneNode.h"
#include "Particle_Snow.h"
#include "ParticleSystem.h"

#include "AllocateHierarchy.h"
#include "SkinnedMesh.h"
#include "RayCtrl.h"
#include "AutoReleasePool.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionMove.h"
#include "ActionRepeat.h"
#include "ActionSequence.h"
#include "OBJLoader.h"
#include "MaterialTexture.h"
#include "iMap.h"
#include "OBJMap.h"
#include "HeightMap.h"
#include "Group.h"
#include "ASECharacter.h"
#include "ASEFrame.h"
#include "ASELoader.h"
#include "SkyBox.h"
#include "Environment.h"
#include "UIManager.h"

#include "CubeCtrl.h"
#include "SkinnedMeshManager.h"
#include "GridCtrl.h"
#include "CharacterCtrl.h"
#include "CharacterManager.h"
#include "LinearInterpolation.h"
#include "MapTool.h"
#include "MainGame.h"
#include "MainGameManager.h"





