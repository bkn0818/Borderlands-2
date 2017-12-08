#pragma once

class UITextView : public UIObject
{
public:
	UITextView();
	virtual ~UITextView();
	virtual void Render(LPD3DXSPRITE sprite, RECT drawRect) override;

	GETSET(std::string, text, Text);
	GETSET(DWORD, textFormat, TextFormat);
	GETSET(DWORD, color, Color);
	//GETSET_ADD_REF( LPD3DXFONT, font, Font );
	GETSET(LPD3DXFONT, font, Font);
};

