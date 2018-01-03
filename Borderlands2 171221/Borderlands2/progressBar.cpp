#include "stdafx.h"
#include "progressBar.h"

progressBar::progressBar() {}
progressBar::~progressBar() {}

void progressBar::init(const char * fullPath, int x, int y, const char * bar, int value)
{
	LPDIRECT3DTEXTURE9 texture = g_pTextureManager->GetTexture(fullPath, &progressBarimgInfo);
	UIImageView* Bar = new UIImageView;
	Bar->SetTexture(texture);
	Bar->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
	Bar->SetLocalPos(D3DXVECTOR3(x, y, 0));
	progressBarRoot.AddChild(Bar);
	progressBarName = bar;

	if (bar == "emptyBar")
	{
		Bar->SetSize(D3DXVECTOR2(progressBarimgInfo.Width - 17, progressBarimgInfo.Height - 39));
		Bar->SetTag(1);
	}
	if (bar == "HP")
	{
		Bar->SetTag(1);
		progressBarCount = value;
		MaxCount = value;
		CurrentHP = progressBarimgInfo.Width;

		//자릿수
		digit = 0;
		if (0 <= value && value < 10) digit = 1;
		if (10 <= value && value < 100) digit = 2;
		if (100 <= value && value < 1000) digit = 3;
		if (1000 <= value && value < 10000) digit = 4;

		//출력할 번호 이미지
		//printNum = 0;
		digit1000 = digit100 = digit10 = digit1 = 0;

		if (digit == 4)
			digit1000 = value / 1000;
		if (digit == 4 || digit == 3)
			digit100 = (value - (digit1000 * 1000)) / 100;
		if (digit == 4 || digit == 3 || digit == 2)
			digit10 = (value - ((digit1000 * 1000) + (digit100 * 100))) / 10;
		if (digit == 4 || digit == 3 || digit == 2 || digit == 1)
			digit1 = (value - ((digit1000 * 1000) + (digit100 * 100) + (digit10 * 10))) / 1;

		//printNum = (digit1000 * 1000) + (digit100 * 100) + (digit10 * 10) + (digit1 * 1);

		//image == digitNum
		digitFullPath1000 = nullptr;
		digitFullPath100 = nullptr;
		digitFullPath10 = nullptr;
		digitFullPath1 = nullptr;

		//digit1000
		if (digit1000 == 0) digitFullPath1000 = "UI/HP0.png";
		if (digit1000 == 1) digitFullPath1000 = "UI/HP1.png";
		if (digit1000 == 2) digitFullPath1000 = "UI/HP2.png";
		if (digit1000 == 3) digitFullPath1000 = "UI/HP3.png";
		if (digit1000 == 4) digitFullPath1000 = "UI/HP4.png";
		if (digit1000 == 5) digitFullPath1000 = "UI/HP5.png";
		if (digit1000 == 6) digitFullPath1000 = "UI/HP6.png";
		if (digit1000 == 7) digitFullPath1000 = "UI/HP7.png";
		if (digit1000 == 8) digitFullPath1000 = "UI/HP8.png";
		if (digit1000 == 9) digitFullPath1000 = "UI/HP9.png";

		//digit100
		if (digit100 == 0) digitFullPath100 = "UI/HP0.png";
		if (digit100 == 1) digitFullPath100 = "UI/HP1.png";
		if (digit100 == 2) digitFullPath100 = "UI/HP2.png";
		if (digit100 == 3) digitFullPath100 = "UI/HP3.png";
		if (digit100 == 4) digitFullPath100 = "UI/HP4.png";
		if (digit100 == 5) digitFullPath100 = "UI/HP5.png";
		if (digit100 == 6) digitFullPath100 = "UI/HP6.png";
		if (digit100 == 7) digitFullPath100 = "UI/HP7.png";
		if (digit100 == 8) digitFullPath100 = "UI/HP8.png";
		if (digit100 == 9) digitFullPath100 = "UI/HP9.png";

		//digit10
		if (digit10 == 0) digitFullPath10 = "UI/HP0.png";
		if (digit10 == 1) digitFullPath10 = "UI/HP1.png";
		if (digit10 == 2) digitFullPath10 = "UI/HP2.png";
		if (digit10 == 3) digitFullPath10 = "UI/HP3.png";
		if (digit10 == 4) digitFullPath10 = "UI/HP4.png";
		if (digit10 == 5) digitFullPath10 = "UI/HP5.png";
		if (digit10 == 6) digitFullPath10 = "UI/HP6.png";
		if (digit10 == 7) digitFullPath10 = "UI/HP7.png";
		if (digit10 == 8) digitFullPath10 = "UI/HP8.png";
		if (digit10 == 9) digitFullPath10 = "UI/HP9.png";

		//digit1
		if (digit1 == 0) digitFullPath1 = "UI/HP0.png";
		if (digit1 == 1) digitFullPath1 = "UI/HP1.png";
		if (digit1 == 2) digitFullPath1 = "UI/HP2.png";
		if (digit1 == 3) digitFullPath1 = "UI/HP3.png";
		if (digit1 == 4) digitFullPath1 = "UI/HP4.png";
		if (digit1 == 5) digitFullPath1 = "UI/HP5.png";
		if (digit1 == 6) digitFullPath1 = "UI/HP6.png";
		if (digit1 == 7) digitFullPath1 = "UI/HP7.png";
		if (digit1 == 8) digitFullPath1 = "UI/HP8.png";
		if (digit1 == 9) digitFullPath1 = "UI/HP9.png";

		//HPCurrent
		D3DXIMAGE_INFO digitimgInfo;
		//digit1
		if ((digit == 4 || digit == 3 || digit == 2 || digit == 1) && digit1 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1texture = g_pTextureManager->GetTexture(digitFullPath1, &digitimgInfo);
			UIImageView* digit1CurNum = new UIImageView;
			digit1CurNum->SetTexture(digit1texture);
			digit1CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			if (digit == 4) digit1CurNum->SetLocalPos(D3DXVECTOR3(146 + (Xplus / 2), 651 + (Yplus - 50), 0));
			if (digit == 3) digit1CurNum->SetLocalPos(D3DXVECTOR3(131 + (Xplus / 2), 653 + (Yplus - 50), 0));
			if (digit == 2) digit1CurNum->SetLocalPos(D3DXVECTOR3(116 + (Xplus / 2), 655 + (Yplus - 50), 0));
			if (digit == 1) digit1CurNum->SetLocalPos(D3DXVECTOR3(101 + (Xplus / 2), 657 + (Yplus - 50), 0));
			digit1CurNum->SetTag(4);
			progressBarRoot.AddChild(digit1CurNum);
		}
		//digit10
		if ((digit == 4 || digit == 3 || digit == 2) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit10texture = g_pTextureManager->GetTexture(digitFullPath10, &digitimgInfo);
			UIImageView* digit10CurNum = new UIImageView;
			digit10CurNum->SetTexture(digit10texture);
			digit10CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			if (digit == 4) digit10CurNum->SetLocalPos(D3DXVECTOR3(131 + (Xplus / 2), 653 + (Yplus - 50), 0));
			if (digit == 3) digit10CurNum->SetLocalPos(D3DXVECTOR3(116 + (Xplus / 2), 655 + (Yplus - 50), 0));
			if (digit == 2) digit10CurNum->SetLocalPos(D3DXVECTOR3(101 + (Xplus / 2), 657 + (Yplus - 50), 0));
			digit10CurNum->SetTag(5);
			progressBarRoot.AddChild(digit10CurNum);
		}
		//digit100
		if ((digit == 4 || digit == 3) && digit100 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit100texture = g_pTextureManager->GetTexture(digitFullPath100, &digitimgInfo);
			UIImageView* digit100CurNum = new UIImageView;
			digit100CurNum->SetTexture(digit100texture);
			digit100CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			if (digit == 4) digit100CurNum->SetLocalPos(D3DXVECTOR3(116 + (Xplus / 2), 655 + (Yplus - 50), 0));
			if (digit == 3) digit100CurNum->SetLocalPos(D3DXVECTOR3(101 + (Xplus / 2), 657 + (Yplus - 50), 0));
			digit100CurNum->SetTag(6);
			progressBarRoot.AddChild(digit100CurNum);
		}
		//digit1000
		if (digit == 4 && digit1000 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1000texture = g_pTextureManager->GetTexture(digitFullPath1000, &digitimgInfo);
			UIImageView* digit1000CurNum = new UIImageView;
			digit1000CurNum->SetTexture(digit1000texture);
			digit1000CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			digit1000CurNum->SetLocalPos(D3DXVECTOR3(101 + (Xplus / 2), 657 + (Yplus - 50), 0));
			digit1000CurNum->SetTag(7);
			progressBarRoot.AddChild(digit1000CurNum);
		}

		//text
		/*
		//current HP
		UITextView* BarText = new UITextView;
		BarText->SetLocalPos(D3DXVECTOR3(x, y - 53, 0));
		BarText->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
		string val = "HP : " + to_string(progressBarCount);
		BarText->SetText(val);
		BarText->SetTag(2);
		BarText->SetColor(D3DCOLOR_XRGB(255, 0, 0));
		BarText->SetTextFormat(DT_NOCLIP | DT_VCENTER | DT_CENTER);
		LPD3DXFONT BarFont = g_pFontManager->GetFont(g_pFontManager->UI);
		BarText->SetFont(BarFont);
		progressBarRoot.AddChild(BarText);
		//HP Digit
		UITextView* PrintHP = new UITextView;
		PrintHP->SetLocalPos(D3DXVECTOR3(x, y - 106, 0));
		PrintHP->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
		progressBarCount = value;
		string val2 = "자릿수 : " + to_string(digit) + ", 출력 이미지 : " + to_string(printNum);
		PrintHP->SetText(val2);
		PrintHP->SetTag(3);
		PrintHP->SetColor(D3DCOLOR_XRGB(0, 0, 0));
		PrintHP->SetTextFormat(DT_NOCLIP | DT_VCENTER | DT_CENTER);
		LPD3DXFONT PrintHPFont = g_pFontManager->GetFont(g_pFontManager->UI);
		PrintHP->SetFont(PrintHPFont);
		progressBarRoot.AddChild(PrintHP);
		//HP minusCount
		UITextView* CurHP = new UITextView;
		CurHP->SetLocalPos(D3DXVECTOR3(x, y - 159, 0));
		CurHP->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
		progressBarCount = value;
		string val3 = to_string(CurrentHP);
		CurHP->SetText(val3);
		CurHP->SetTag(8);
		CurHP->SetColor(D3DCOLOR_XRGB(0, 0, 0));
		CurHP->SetTextFormat(DT_NOCLIP | DT_VCENTER | DT_CENTER);
		LPD3DXFONT PrintcurHPFont = g_pFontManager->GetFont(g_pFontManager->UI);
		CurHP->SetFont(PrintcurHPFont);
		progressBarRoot.AddChild(CurHP);
		*/
	}
	if (bar == "Bullet")
	{
		Bar->SetTag(9);
		progressBarCount = value;
		MaxCount = value;
		CurrentBullet = progressBarimgInfo.Width;

		//자릿수
		digit = 0;
		if (0 <= value && value < 10) digit = 1;
		if (10 <= value && value < 100) digit = 2;
		if (100 <= value && value < 1000) digit = 3;
		if (1000 <= value && value < 10000) digit = 4;

		//출력할 번호 이미지
		//printNum = 0;
		digit1000 = digit100 = digit10 = digit1 = 0;

		if (digit == 4)
			digit1000 = value / 1000;
		if (digit == 4 || digit == 3)
			digit100 = (value - (digit1000 * 1000)) / 100;
		if (digit == 4 || digit == 3 || digit == 2)
			digit10 = (value - ((digit1000 * 1000) + (digit100 * 100))) / 10;
		if (digit == 4 || digit == 3 || digit == 2 || digit == 1)
			digit1 = (value - ((digit1000 * 1000) + (digit100 * 100) + (digit10 * 10))) / 1;

		//printNum = (digit1000 * 1000) + (digit100 * 100) + (digit10 * 10) + (digit1 * 1);

		//image == digitNum
		digitFullPath1000 = nullptr;
		digitFullPath100 = nullptr;
		digitFullPath10 = nullptr;
		digitFullPath1 = nullptr;

		//digit1000
		if (digit1000 == 0) digitFullPath1000 = "UI/Bullet0.png";
		if (digit1000 == 1) digitFullPath1000 = "UI/Bullet1.png";
		if (digit1000 == 2) digitFullPath1000 = "UI/Bullet2.png";
		if (digit1000 == 3) digitFullPath1000 = "UI/Bullet3.png";
		if (digit1000 == 4) digitFullPath1000 = "UI/Bullet4.png";
		if (digit1000 == 5) digitFullPath1000 = "UI/Bullet5.png";
		if (digit1000 == 6) digitFullPath1000 = "UI/Bullet6.png";
		if (digit1000 == 7) digitFullPath1000 = "UI/Bullet7.png";
		if (digit1000 == 8) digitFullPath1000 = "UI/Bullet8.png";
		if (digit1000 == 9) digitFullPath1000 = "UI/Bullet9.png";

		//digit100
		if (digit100 == 0) digitFullPath100 = "UI/Bullet0.png";
		if (digit100 == 1) digitFullPath100 = "UI/Bullet1.png";
		if (digit100 == 2) digitFullPath100 = "UI/Bullet2.png";
		if (digit100 == 3) digitFullPath100 = "UI/Bullet3.png";
		if (digit100 == 4) digitFullPath100 = "UI/Bullet4.png";
		if (digit100 == 5) digitFullPath100 = "UI/Bullet5.png";
		if (digit100 == 6) digitFullPath100 = "UI/Bullet6.png";
		if (digit100 == 7) digitFullPath100 = "UI/Bullet7.png";
		if (digit100 == 8) digitFullPath100 = "UI/Bullet8.png";
		if (digit100 == 9) digitFullPath100 = "UI/Bullet9.png";

		//digit10
		if (digit10 == 0) digitFullPath10 = "UI/Bullet0.png";
		if (digit10 == 1) digitFullPath10 = "UI/Bullet1.png";
		if (digit10 == 2) digitFullPath10 = "UI/Bullet2.png";
		if (digit10 == 3) digitFullPath10 = "UI/Bullet3.png";
		if (digit10 == 4) digitFullPath10 = "UI/Bullet4.png";
		if (digit10 == 5) digitFullPath10 = "UI/Bullet5.png";
		if (digit10 == 6) digitFullPath10 = "UI/Bullet6.png";
		if (digit10 == 7) digitFullPath10 = "UI/Bullet7.png";
		if (digit10 == 8) digitFullPath10 = "UI/Bullet8.png";
		if (digit10 == 9) digitFullPath10 = "UI/Bullet9.png";

		//digit1
		if (digit1 == 0) digitFullPath1 = "UI/Bullet0.png";
		if (digit1 == 1) digitFullPath1 = "UI/Bullet1.png";
		if (digit1 == 2) digitFullPath1 = "UI/Bullet2.png";
		if (digit1 == 3) digitFullPath1 = "UI/Bullet3.png";
		if (digit1 == 4) digitFullPath1 = "UI/Bullet4.png";
		if (digit1 == 5) digitFullPath1 = "UI/Bullet5.png";
		if (digit1 == 6) digitFullPath1 = "UI/Bullet6.png";
		if (digit1 == 7) digitFullPath1 = "UI/Bullet7.png";
		if (digit1 == 8) digitFullPath1 = "UI/Bullet8.png";
		if (digit1 == 9) digitFullPath1 = "UI/Bullet9.png";

		D3DXIMAGE_INFO digitimgInfo;
		//BulletS
		LPDIRECT3DTEXTURE9 digitSlashTexture = g_pTextureManager->GetTexture("UI/bulletSlash.png", &digitimgInfo);
		UIImageView* digitSlash = new UIImageView;
		digitSlash->SetTexture(digitSlashTexture);
		digitSlash->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
		if (digit == 4) digitSlash->SetLocalPos(D3DXVECTOR3(1042 - (Xplus / 2), 649 + (Yplus - 50), 0));
		if (digit == 3) digitSlash->SetLocalPos(D3DXVECTOR3(1027 - (Xplus / 2), 651 + (Yplus - 50), 0));
		if (digit == 2) digitSlash->SetLocalPos(D3DXVECTOR3(1012 - (Xplus / 2), 653 + (Yplus - 50), 0));
		if (digit == 1) digitSlash->SetLocalPos(D3DXVECTOR3(997 - (Xplus / 2), 655 + (Yplus - 50), 0));
		digitSlash->SetTag(13);
		progressBarRoot.AddChild(digitSlash);

		//BulletMax
		//digit1
		if ((digit == 4 || digit == 3 || digit == 2 || digit == 1) && digit1 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1texture = g_pTextureManager->GetTexture(digitFullPath1, &digitimgInfo);
			UIImageView* digit1CurNum = new UIImageView;
			digit1CurNum->SetTexture(digit1texture);
			digit1CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			digit1CurNum->SetLocalPos(D3DXVECTOR3(977 - (Xplus / 2), 657 + (Yplus - 50), 0));
			progressBarRoot.AddChild(digit1CurNum);
		}
		//digit10
		if ((digit == 4 || digit == 3 || digit == 2) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit10texture = g_pTextureManager->GetTexture(digitFullPath10, &digitimgInfo);
			UIImageView* digit10CurNum = new UIImageView;
			digit10CurNum->SetTexture(digit10texture);
			digit10CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			digit10CurNum->SetLocalPos(D3DXVECTOR3(992 - (Xplus / 2), 655 + (Yplus - 50), 0));
			progressBarRoot.AddChild(digit10CurNum);
		}
		//digit100
		if ((digit == 4 || digit == 3) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit100texture = g_pTextureManager->GetTexture(digitFullPath100, &digitimgInfo);
			UIImageView* digit100CurNum = new UIImageView;
			digit100CurNum->SetTexture(digit100texture);
			digit100CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			digit100CurNum->SetLocalPos(D3DXVECTOR3(1007 - (Xplus / 2), 653 + (Yplus - 50), 0));
			progressBarRoot.AddChild(digit100CurNum);
		}
		//digit1000
		if (digit == 4 && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1000texture = g_pTextureManager->GetTexture(digitFullPath1000, &digitimgInfo);
			UIImageView* digit1000CurNum = new UIImageView;
			digit1000CurNum->SetTexture(digit1000texture);
			digit1000CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			digit1000CurNum->SetLocalPos(D3DXVECTOR3(1022 - (Xplus / 2), 651 + (Yplus - 50), 0));
			progressBarRoot.AddChild(digit1000CurNum);
		}

		//BulletCurrent
		//digit1
		if ((digit == 4 || digit == 3 || digit == 2 || digit == 1) && digit1 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1texture = g_pTextureManager->GetTexture(digitFullPath1, &digitimgInfo);
			UIImageView* digit1CurNum = new UIImageView;
			digit1CurNum->SetTexture(digit1texture);
			digit1CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			if (digit == 4) digit1CurNum->SetLocalPos(D3DXVECTOR3(1057 - (Xplus / 2), 647 + (Yplus - 50), 0));
			if (digit == 3) digit1CurNum->SetLocalPos(D3DXVECTOR3(1042 - (Xplus / 2), 649 + (Yplus - 50), 0));
			if (digit == 2) digit1CurNum->SetLocalPos(D3DXVECTOR3(1027 - (Xplus / 2), 651 + (Yplus - 50), 0));
			if (digit == 1) digit1CurNum->SetLocalPos(D3DXVECTOR3(1012 - (Xplus / 2), 653 + (Yplus - 50), 0));
			digit1CurNum->SetTag(14);
			progressBarRoot.AddChild(digit1CurNum);
		}
		//digit10
		if ((digit == 4 || digit == 3 || digit == 2) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit10texture = g_pTextureManager->GetTexture(digitFullPath10, &digitimgInfo);
			UIImageView* digit10CurNum = new UIImageView;
			digit10CurNum->SetTexture(digit10texture);
			digit10CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			if (digit == 4) digit10CurNum->SetLocalPos(D3DXVECTOR3(1072 - (Xplus / 2), 645 + (Yplus - 50), 0));
			if (digit == 3) digit10CurNum->SetLocalPos(D3DXVECTOR3(1057 - (Xplus / 2), 647 + (Yplus - 50), 0));
			if (digit == 2) digit10CurNum->SetLocalPos(D3DXVECTOR3(1042 - (Xplus / 2), 649 + (Yplus - 50), 0));
			digit10CurNum->SetTag(15);
			progressBarRoot.AddChild(digit10CurNum);
		}
		//digit100
		if ((digit == 4 || digit == 3) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit100texture = g_pTextureManager->GetTexture(digitFullPath100, &digitimgInfo);
			UIImageView* digit100CurNum = new UIImageView;
			digit100CurNum->SetTexture(digit100texture);
			digit100CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			if (digit == 4) digit100CurNum->SetLocalPos(D3DXVECTOR3(1087 - (Xplus / 2), 643 + (Yplus - 50), 0));
			if (digit == 3) digit100CurNum->SetLocalPos(D3DXVECTOR3(1072 - (Xplus / 2), 645 + (Yplus - 50), 0));
			digit100CurNum->SetTag(16);
			progressBarRoot.AddChild(digit100CurNum);
		}
		//digit1000
		if (digit == 4 && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1000texture = g_pTextureManager->GetTexture(digitFullPath1000, &digitimgInfo);
			UIImageView* digit1000CurNum = new UIImageView;
			digit1000CurNum->SetTexture(digit1000texture);
			digit1000CurNum->SetSize(D3DXVECTOR2(digitimgInfo.Width, digitimgInfo.Height));
			digit1000CurNum->SetLocalPos(D3DXVECTOR3(1102 - (Xplus / 2), 641 + (Yplus - 50), 0));
			digit1000CurNum->SetTag(17);
			progressBarRoot.AddChild(digit1000CurNum);
		}


		//text
		/*
		//current Bullet
		UITextView* BarText = new UITextView;
		BarText->SetLocalPos(D3DXVECTOR3(x, y - 53, 0));
		BarText->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
		string val = "Bullet : " + to_string(progressBarCount);
		BarText->SetText(val);
		BarText->SetTag(10);
		BarText->SetColor(D3DCOLOR_XRGB(0, 0, 255));
		BarText->SetTextFormat(DT_NOCLIP | DT_VCENTER | DT_CENTER);
		LPD3DXFONT BarFont = g_pFontManager->GetFont(g_pFontManager->UI);
		BarText->SetFont(BarFont);
		progressBarRoot.AddChild(BarText);
		//Bullet Digit
		UITextView* PrintBullet = new UITextView;
		PrintBullet->SetLocalPos(D3DXVECTOR3(x, y - 106, 0));
		PrintBullet->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
		progressBarCount = value;
		string val2 = "자릿수 : " + to_string(digit) + ", 출력 이미지 : " + to_string(printNum);
		PrintBullet->SetText(val2);
		PrintBullet->SetTag(11);
		PrintBullet->SetColor(D3DCOLOR_XRGB(0, 0, 0));
		PrintBullet->SetTextFormat(DT_NOCLIP | DT_VCENTER | DT_CENTER);
		LPD3DXFONT PrintBulletFont = g_pFontManager->GetFont(g_pFontManager->UI);
		PrintBullet->SetFont(PrintBulletFont);
		progressBarRoot.AddChild(PrintBullet);
		//Bullet minusCount
		UITextView* CurBullet = new UITextView;
		CurBullet->SetLocalPos(D3DXVECTOR3(x, y - 159, 0));
		CurBullet->SetSize(D3DXVECTOR2(progressBarimgInfo.Width, progressBarimgInfo.Height));
		progressBarCount = value;
		string val3 = to_string(CurrentBullet);
		CurBullet->SetText(val3);
		CurBullet->SetTag(12);
		CurBullet->SetColor(D3DCOLOR_XRGB(0, 0, 0));
		CurBullet->SetTextFormat(DT_NOCLIP | DT_VCENTER | DT_CENTER);
		LPD3DXFONT PrintcurBulletFont = g_pFontManager->GetFont(g_pFontManager->UI);
		CurBullet->SetFont(PrintcurBulletFont);
		progressBarRoot.AddChild(CurBullet);
		*/
	}

	D3DXCreateSprite(D3DDEVICE, &progressBarsprite);
}

void progressBar::update()
{
	progressBarRoot.Update();
}

void progressBar::render()
{
	progressBarRoot.Render(progressBarsprite, progressBarName);
}

void progressBar::destroy()
{
	if (progressBarsprite) progressBarsprite->Release();
}

//BulletBar
void progressBar::isClick(int n)
{
	int minusCount = (progressBarimgInfo.Width / MaxCount) * n;
	if (progressBarName == "Bullet" && n > 0 && progressBarimgInfo.Width >= minusCount && progressBarCount > 0)
	{
		UIImageView* BulletBar = (UIImageView*)progressBarRoot.GetChildByTag(9);
		BulletBar->SetSize(D3DXVECTOR2(progressBarimgInfo.Width - minusCount, progressBarimgInfo.Height));

		//count
		--digit1;
		if (digit1 < 0)
		{
			digit1 = 9;
			--digit10;
		}
		if (digit10 < 0)
		{
			--digit100;
			digit10 = 9;
		}
		if (digit100 < 0)
		{
			--digit1000;
			digit100 = 9;
		}

		//digit1000
		if (digit1000 == 0) digitFullPath1000 = "UI/Bullet0.png";
		if (digit1000 == 1) digitFullPath1000 = "UI/Bullet1.png";
		if (digit1000 == 2) digitFullPath1000 = "UI/Bullet2.png";
		if (digit1000 == 3) digitFullPath1000 = "UI/Bullet3.png";
		if (digit1000 == 4) digitFullPath1000 = "UI/Bullet4.png";
		if (digit1000 == 5) digitFullPath1000 = "UI/Bullet5.png";
		if (digit1000 == 6) digitFullPath1000 = "UI/Bullet6.png";
		if (digit1000 == 7) digitFullPath1000 = "UI/Bullet7.png";
		if (digit1000 == 8) digitFullPath1000 = "UI/Bullet8.png";
		if (digit1000 == 9) digitFullPath1000 = "UI/Bullet9.png";

		//digit100
		if (digit100 == 0) digitFullPath100 = "UI/Bullet0.png";
		if (digit100 == 1) digitFullPath100 = "UI/Bullet1.png";
		if (digit100 == 2) digitFullPath100 = "UI/Bullet2.png";
		if (digit100 == 3) digitFullPath100 = "UI/Bullet3.png";
		if (digit100 == 4) digitFullPath100 = "UI/Bullet4.png";
		if (digit100 == 5) digitFullPath100 = "UI/Bullet5.png";
		if (digit100 == 6) digitFullPath100 = "UI/Bullet6.png";
		if (digit100 == 7) digitFullPath100 = "UI/Bullet7.png";
		if (digit100 == 8) digitFullPath100 = "UI/Bullet8.png";
		if (digit100 == 9) digitFullPath100 = "UI/Bullet9.png";

		//digit10
		if (digit10 == 0) digitFullPath10 = "UI/Bullet0.png";
		if (digit10 == 1) digitFullPath10 = "UI/Bullet1.png";
		if (digit10 == 2) digitFullPath10 = "UI/Bullet2.png";
		if (digit10 == 3) digitFullPath10 = "UI/Bullet3.png";
		if (digit10 == 4) digitFullPath10 = "UI/Bullet4.png";
		if (digit10 == 5) digitFullPath10 = "UI/Bullet5.png";
		if (digit10 == 6) digitFullPath10 = "UI/Bullet6.png";
		if (digit10 == 7) digitFullPath10 = "UI/Bullet7.png";
		if (digit10 == 8) digitFullPath10 = "UI/Bullet8.png";
		if (digit10 == 9) digitFullPath10 = "UI/Bullet9.png";

		//digit1
		if (digit1 == 0) digitFullPath1 = "UI/Bullet0.png";
		if (digit1 == 1) digitFullPath1 = "UI/Bullet1.png";
		if (digit1 == 2) digitFullPath1 = "UI/Bullet2.png";
		if (digit1 == 3) digitFullPath1 = "UI/Bullet3.png";
		if (digit1 == 4) digitFullPath1 = "UI/Bullet4.png";
		if (digit1 == 5) digitFullPath1 = "UI/Bullet5.png";
		if (digit1 == 6) digitFullPath1 = "UI/Bullet6.png";
		if (digit1 == 7) digitFullPath1 = "UI/Bullet7.png";
		if (digit1 == 8) digitFullPath1 = "UI/Bullet8.png";
		if (digit1 == 9) digitFullPath1 = "UI/Bullet9.png";

		D3DXIMAGE_INFO digitimgInfo;
		//digit1
		if ((digit == 4 || digit == 3 || digit == 2 || digit == 1) && digit1 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1texture = g_pTextureManager->GetTexture(digitFullPath1, &digitimgInfo);
			UIImageView* Clickdigit1CurNum = (UIImageView*)progressBarRoot.GetChildByTag(14);
			Clickdigit1CurNum->SetTexture(digit1texture);
		}
		//digit10
		if ((digit == 4 || digit == 3 || digit == 2) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit10texture = g_pTextureManager->GetTexture(digitFullPath10, &digitimgInfo);
			UIImageView* Clickdigit10CurNum = (UIImageView*)progressBarRoot.GetChildByTag(15);
			Clickdigit10CurNum->SetTexture(digit10texture);
		}
		//digit100
		if ((digit == 4 || digit == 3) && digit100 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit100texture = g_pTextureManager->GetTexture(digitFullPath100, &digitimgInfo);
			UIImageView* Clickdigit100CurNum = (UIImageView*)progressBarRoot.GetChildByTag(16);
			Clickdigit100CurNum->SetTexture(digit100texture);
		}
		//digit1000
		if (digit == 4 && digit1000 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1000texture = g_pTextureManager->GetTexture(digitFullPath1000, &digitimgInfo);
			UIImageView* Clickdigit1000CurNum = (UIImageView*)progressBarRoot.GetChildByTag(17);
			Clickdigit1000CurNum->SetTexture(digit1000texture);
		}


		//text
		/*
		//current Bullet
		--progressBarCount;
		string val = "HP : " + to_string(progressBarCount);
		UITextView* BarText = (UITextView*)progressBarRoot.GetChildByTag(10);
		BarText->SetText(val);
		//Bullet Digit
		if (0 <= progressBarCount && progressBarCount < 10) digit = 1;
		if (10 <= progressBarCount && progressBarCount < 100) digit = 2;
		if (100 <= progressBarCount && progressBarCount < 1000) digit = 3;
		if (1000 <= progressBarCount && progressBarCount < 10000) digit = 4;
		--printNum;
		string val2 = "자릿수 : " + to_string(digit) + ", 출력 이미지 : " + to_string(printNum);
		UITextView* PrintBullet = (UITextView*)progressBarRoot.GetChildByTag(11);
		PrintBullet->SetText(val2);
		//Bullet minusCount
		CurrentBullet = progressBarimgInfo.Width - (progressBarimgInfo.Width / MaxCount) * n;
		string val3 = "남은 Bullet : " + to_string(CurrentBullet);
		UITextView* CurBullet = (UITextView*)progressBarRoot.GetChildByTag(12);
		CurBullet->SetText(val3);
		*/
	}
}

//HPBar
void progressBar::isAttacked(int n)
{
	int minusCount = (progressBarimgInfo.Width / MaxCount) * n;
	if (progressBarName == "HP" && n > 0 && progressBarimgInfo.Width >= minusCount && progressBarCount > 0)
	{
		//img
		//Bar
		UIImageView* HPBar = (UIImageView*)progressBarRoot.GetChildByTag(1);
		HPBar->SetSize(D3DXVECTOR2(progressBarimgInfo.Width - minusCount, progressBarimgInfo.Height));

		//count
		--digit1;
		if (digit1 < 0)
		{
			digit1 = 9;
			--digit10;
		}
		if (digit10 < 0)
		{
			--digit100;
			digit10 = 9;
		}
		if (digit100 < 0)
		{
			--digit1000;
			digit100 = 9;
		}

		//digit1000
		if (digit1000 == 0) digitFullPath1000 = "UI/HP0.png";
		if (digit1000 == 1) digitFullPath1000 = "UI/HP1.png";
		if (digit1000 == 2) digitFullPath1000 = "UI/HP2.png";
		if (digit1000 == 3) digitFullPath1000 = "UI/HP3.png";
		if (digit1000 == 4) digitFullPath1000 = "UI/HP4.png";
		if (digit1000 == 5) digitFullPath1000 = "UI/HP5.png";
		if (digit1000 == 6) digitFullPath1000 = "UI/HP6.png";
		if (digit1000 == 7) digitFullPath1000 = "UI/HP7.png";
		if (digit1000 == 8) digitFullPath1000 = "UI/HP8.png";
		if (digit1000 == 9) digitFullPath1000 = "UI/HP9.png";

		//digit100
		if (digit100 == 0) digitFullPath100 = "UI/HP0.png";
		if (digit100 == 1) digitFullPath100 = "UI/HP1.png";
		if (digit100 == 2) digitFullPath100 = "UI/HP2.png";
		if (digit100 == 3) digitFullPath100 = "UI/HP3.png";
		if (digit100 == 4) digitFullPath100 = "UI/HP4.png";
		if (digit100 == 5) digitFullPath100 = "UI/HP5.png";
		if (digit100 == 6) digitFullPath100 = "UI/HP6.png";
		if (digit100 == 7) digitFullPath100 = "UI/HP7.png";
		if (digit100 == 8) digitFullPath100 = "UI/HP8.png";
		if (digit100 == 9) digitFullPath100 = "UI/HP9.png";

		//digit10
		if (digit10 == 0) digitFullPath10 = "UI/HP0.png";
		if (digit10 == 1) digitFullPath10 = "UI/HP1.png";
		if (digit10 == 2) digitFullPath10 = "UI/HP2.png";
		if (digit10 == 3) digitFullPath10 = "UI/HP3.png";
		if (digit10 == 4) digitFullPath10 = "UI/HP4.png";
		if (digit10 == 5) digitFullPath10 = "UI/HP5.png";
		if (digit10 == 6) digitFullPath10 = "UI/HP6.png";
		if (digit10 == 7) digitFullPath10 = "UI/HP7.png";
		if (digit10 == 8) digitFullPath10 = "UI/HP8.png";
		if (digit10 == 9) digitFullPath10 = "UI/HP9.png";

		//digit1
		if (digit1 == 0) digitFullPath1 = "UI/HP0.png";
		if (digit1 == 1) digitFullPath1 = "UI/HP1.png";
		if (digit1 == 2) digitFullPath1 = "UI/HP2.png";
		if (digit1 == 3) digitFullPath1 = "UI/HP3.png";
		if (digit1 == 4) digitFullPath1 = "UI/HP4.png";
		if (digit1 == 5) digitFullPath1 = "UI/HP5.png";
		if (digit1 == 6) digitFullPath1 = "UI/HP6.png";
		if (digit1 == 7) digitFullPath1 = "UI/HP7.png";
		if (digit1 == 8) digitFullPath1 = "UI/HP8.png";
		if (digit1 == 9) digitFullPath1 = "UI/HP9.png";

		D3DXIMAGE_INFO digitimgInfo;
		//digit1
		if ((digit == 4 || digit == 3 || digit == 2 || digit == 1) && digit1 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1texture = g_pTextureManager->GetTexture(digitFullPath1, &digitimgInfo);
			UIImageView* Clickdigit1CurNum = (UIImageView*)progressBarRoot.GetChildByTag(4);
			Clickdigit1CurNum->SetTexture(digit1texture);
		}
		//digit10
		if ((digit == 4 || digit == 3 || digit == 2) && digit10 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit10texture = g_pTextureManager->GetTexture(digitFullPath10, &digitimgInfo);
			UIImageView* Clickdigit10CurNum = (UIImageView*)progressBarRoot.GetChildByTag(5);
			Clickdigit10CurNum->SetTexture(digit10texture);
		}
		//digit100
		if ((digit == 4 || digit == 3) && digit100 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit100texture = g_pTextureManager->GetTexture(digitFullPath100, &digitimgInfo);
			UIImageView* Clickdigit100CurNum = (UIImageView*)progressBarRoot.GetChildByTag(6);
			Clickdigit100CurNum->SetTexture(digit100texture);
		}
		//digit1000
		if (digit == 4 && digit1000 >= 0)
		{
			LPDIRECT3DTEXTURE9 digit1000texture = g_pTextureManager->GetTexture(digitFullPath1000, &digitimgInfo);
			UIImageView* Clickdigit1000CurNum = (UIImageView*)progressBarRoot.GetChildByTag(7);
			Clickdigit1000CurNum->SetTexture(digit1000texture);
		}


		//text
		/*
		//current HP
		--progressBarCount;
		string val = "HP : " + to_string(progressBarCount);
		UITextView* BarText = (UITextView*) progressBarRoot.GetChildByTag(2);
		BarText->SetText(val);
		//HP Digit
		if (0 <= progressBarCount && progressBarCount < 10) digit = 1;
		if (10 <= progressBarCount && progressBarCount < 100) digit = 2;
		if (100 <= progressBarCount && progressBarCount < 1000) digit = 3;
		if (1000 <= progressBarCount && progressBarCount < 10000) digit = 4;
		--printNum;
		string val2 = "자릿수 : " + to_string(digit) + ", 출력 이미지 : " + to_string(printNum);
		UITextView* PrintHP = (UITextView*) progressBarRoot.GetChildByTag(3);
		PrintHP->SetText(val2);
		//HP minusCount
		CurrentHP = progressBarimgInfo.Width - (progressBarimgInfo.Width / MaxCount) * n;
		string val3 = "남은 HP : " + to_string(CurrentHP);
		UITextView* CurHP = (UITextView*)progressBarRoot.GetChildByTag(8);
		CurHP->SetText(val3);
		*/
	}
}