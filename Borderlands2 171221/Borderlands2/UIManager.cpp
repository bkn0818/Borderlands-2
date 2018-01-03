#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager() {}
UIManager::~UIManager() {}

void UIManager::Init()
{
	//Bullet,HP EmptyBar
	emptyBar = new progressBar;
	emptyBar->init("UI/PlayerBasicUI.png", 0 + (Xplus / 2), 0 + (Yplus - 50), "EmptyBar", NULL);
	//emptyBar->init("UI/PlayerBasicUI.png", 0, 0, "EmptyBar", NULL);
	//HP
	HPBar = new progressBar;
	HPBar->init("UI/PlayerHPBar.png", 90 + (Xplus / 2), 630 + (Yplus - 50), "HP", 100);
	//HPBar->init("UI/PlayerHPBar.png", 90, 630, "HP", 100);
	//Bullet
	BulletBar = new progressBar;
	BulletBar->init("UI/PlayerBulletBar.png", 970 - (Xplus / 2), 630 + (Yplus - 50), "Bullet", 100);
	//BulletBar->init("UI/PlayerBulletBar.png", 970, 630, "Bullet", 100);

	//테스트
	/*
	UITextView* text = new UITextView;
	text->SetLocalPos(D3DXVECTOR3(500, 150, 0));
	text->SetSize(D3DXVECTOR2(312, 200));
	text->SetTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	text->SetColor(D3DCOLOR_XRGB(255, 255, 255));
	text->SetTag(5);
	text->SetText("테스트");
	LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->UI);
	text->SetFont(font);
	uiObj.AddChild(text);
	*/

	/*
	UIButton* lButton = new UIButton;
	lButton->SetTag(1);
	lButton->SetLocalPos(D3DXVECTOR3(180, 360, 0));
	//lButton->SetButtonDelegate(this);
	lButton->SetTexture("Images/btn-med-up.png", "Images/btn-med-over.png", "Images/btn-med-down.png");
	uiObj.AddChild(lButton);

	UIButton* rButton = new UIButton;
	rButton->SetTag(2);
	rButton->SetLocalPos(D3DXVECTOR3(180, 430, 0));
	rButton->SetButtonDelegate(this);
	rButton->SetTexture("Images/btn-med-up.png", "Images/btn-med-over.png", "Images/btn-med-down.png");
	uiObj.AddChild(rButton);*/
}

void UIManager::Release() {}

void UIManager::Update()
{
	//uiObj.Update();
	emptyBar->update();
	HPBar->update();
	BulletBar->update();
}

void UIManager::Destroy()
{
	if (sprite) sprite->Release();
}

void UIManager::Render()
{
	//uiObj.Render( sprite, NULL );
	emptyBar->render();	
	HPBar->render();
	BulletBar->render();
}

void UIManager::OnClick(int n)
{
	//char str[256];
	//sprintf(str, "%d번 클릭!", n);
	//UITextView* text = (UITextView*)uiObj.GetChildByTag(5);
	//text->SetText(str);

	BulletBar->isClick(n);
}

void UIManager::OnAttacked(int n)
{
	HPBar->isAttacked(n);
}


/*
void UIManager::OnClick(UIButton * pSender)
{
	char str[256];

	sprintf(str, "%d번 버튼 눌림", (UITextView*)pSender->GetTag());
	UITextView* text = (UITextView*)uiObj.GetChildByTag(5);
	text->SetText(str);
}
*/