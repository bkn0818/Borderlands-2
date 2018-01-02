#pragma once

class UITextView : public UIObject
{
public:
	UITextView();
	virtual ~UITextView();
	virtual void Render(LPD3DXSPRITE sprite, const char * progressBarName) override;

	GETSET(std::string, text, Text);
	GETSET(DWORD, textFormat, TextFormat);
	GETSET(DWORD, color, Color);
	GETSET(LPD3DXFONT, font, Font);
};

