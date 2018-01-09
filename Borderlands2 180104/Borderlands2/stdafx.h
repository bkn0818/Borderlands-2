// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <assert.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <CommCtrl.h>

#include <InitGuid.h>
#include <dinput.h>
#include <time.h>


#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib" )
#pragma comment(lib, "imm32.lib")


extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern POINT		g_ptMouse;

#include "Asciitok.h"
#include "Utility.h"
#include "VideoDevice.h"
#include "CommonMacroFunction.h"

#include "Menu.h"
#include "Object.h"
#include "MatrialTexture.h"
#include "Group.h"
#include "OBJLoader.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "AutoReleasePool.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "SoundManager.h"
#include "SkyBox.h"
#include "RayCtrl.h"
#include "iMap.h"
#include "OBJMap.h"
#include "HeightMap.h"
#include "CharacterCtrl.h"
#include "Player.h"
#include "EnemyManager.h"

#include "CubeCtrl.h"
#include "CameraCtrl.h"
#include "GridCtrl.h"
#include "LinearInterpolation.h"
#include "UIObject.h"
#include "UIImageView.h"
#include "UITextView.h"
#include "UIButton.h"
#include "UIManager.h"
#include "VideoDevice.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "InputDef.h"
#include "InputManager.h"
#include "SkinnedMesh.h"
#include "Environment.h"
#include "SkinnedMeshManager.h"
#include "ParticleSystem.h"
#include "Particle_Firework.h"
#include "Particle_GunFire.h"
#include "Particle_Snow.h"
#include "ParticleManager.h"
#include "FrustomCulling.h"
#include "SceneNode.h"
#include "SceneManager.h"
#include "MaptoolScene.h"
#include "GameScene.h"
#include "MainScene.h"
#include "LoadingScene.h"



#include "MainGameManager.h"
