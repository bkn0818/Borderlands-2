#pragma once

#define g_pFontManager FontManager::GetInstance()

class FontManager
{
	SINGLETON( FontManager )
public:
	enum FontType {
		NORMAL,
		CHAT,
		QUEST,
	};

private:
	std::map<FontType, LPD3DXFONT> fontMap;

public:
	LPD3DXFONT GetFont( FontType fontType );
	void Destroy();

};

