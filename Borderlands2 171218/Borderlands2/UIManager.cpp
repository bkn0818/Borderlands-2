#include "stdafx.h"
#include "UIManager.h"


UIManager::UIManager()
{
}


UIManager::~UIManager()
{
}

void UIManager::Init()
{
	D3DXCreateSprite(D3DDEVICE, &sprite);
	D3DXIMAGE_INFO imgInfo;
	LPDIRECT3DTEXTURE9 texture = g_pTextureManager->GetTexture("Images/panel-info.png", &imgInfo);

	UIImageView* panel = new UIImageView;
	panel->SetLocalPos(D3DXVECTOR3(50, 50, 0));
	panel->SetSize(D3DXVECTOR2(imgInfo.Width, imgInfo.Height));
	panel->SetTexture(texture);
	panel->SetDebugRender(true);
	uiObj.AddChild(panel);

	UIButton* lButton = new UIButton;
	lButton->SetTag(1);
	lButton->SetLocalPos(D3DXVECTOR3(180, 360, 0));
	lButton->SetButtonDelegate(this);
	lButton->SetTexture("Images/btn-med-up.png", "Images/btn-med-over.png", "Images/btn-med-down.png");
	lButton->SetDebugRender(true);
	uiObj.AddChild(lButton);

	UIButton* rButton = new UIButton;
	rButton->SetTag(2);
	rButton->SetDebugRender(1);
	rButton->SetLocalPos(D3DXVECTOR3(180, 430, 0));
	rButton->SetButtonDelegate(this);
	rButton->SetTexture("Images/btn-med-up.png", "Images/btn-med-over.png", "Images/btn-med-down.png");
	rButton->SetDebugRender(true);
	uiObj.AddChild(rButton);

	UITextView* text = new UITextView;
	text->SetLocalPos(D3DXVECTOR3(150, 150, 0));
	text->SetSize(D3DXVECTOR2(312, 200));
	text->SetTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	text->SetColor(D3DCOLOR_XRGB(255, 255, 255));
	text->SetDebugRender(true);
	text->SetTag(5);
	text->SetText("웰컴 투더 정글");
	LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->QUEST);
	text->SetFont(font);
	text->SetDebugRender(1);
	uiObj.AddChild(text);
}

void UIManager::Release()
{
}

void UIManager::Update()
{
	uiObj.Update();
}

void UIManager::Destroy()
{
	if (sprite) sprite->Release();
}

void UIManager::Render(RECT drawRect)
{
	uiObj.Render(sprite, drawRect);
}

void UIManager::OnClick(UIButton * pSender)
{
	char str[256];
	sprintf(str, "%d번 버튼 눌림", (UITextView*)pSender->GetTag());
	UITextView* text = (UITextView*)uiObj.GetChildByTag(5);
	text->SetText(str);
}
