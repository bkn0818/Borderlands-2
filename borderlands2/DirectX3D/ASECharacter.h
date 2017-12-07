#pragma once

class ASEFrame;

class ASECharacter : public Object
{
public:
	ASECharacter();
	~ASECharacter();

	void Load(char* filename);
	void Update(D3DXMATRIXA16* parent);
	void Render();

protected:
	ASEScene aseScene;
	ASEFrame* root;
};

