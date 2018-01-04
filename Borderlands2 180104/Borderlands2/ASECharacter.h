

#pragma once

class ASEFrame;

class ASECharacter : public Object
{
public:
	ASECharacter ();
	~ASECharacter ();

	void Load ( char* filename );
	void Update ();
	void Render ();


protected:
	ASEScene aseScene;
	ASEFrame* root;



};

