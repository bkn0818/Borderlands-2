#pragma once
class UIObject
	//: public GameObject
{
protected:
	struct VertexRHWC
	{
		D3DXVECTOR4 p;
		D3DCOLOR	c;

		VertexRHWC(D3DXVECTOR4 pp, D3DCOLOR cc) { p = pp; c = cc; };
		enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
	};

	enum UI_TAG
	{
		UI_BUTTON1,
		UI_BUTTON2,
		UI_IMAGEVIEW,
		UI_TEXTVIEW,
		UI_END
	};
public:
	UIObject();
	~UIObject();

	virtual void Update();
	virtual void UpdateWorldTM();
	virtual void UpdateChildren();
	virtual void Render(LPD3DXSPRITE sprite, const char * progressBarName);
	virtual void AddChild(UIObject*);
	virtual void Destroy();
	virtual UIObject* GetChildByTag(int _tag);


protected:
	GETSET(D3DXVECTOR3, localPos, LocalPos);
	GETSET(UIObject*, parent, Parent);
	GETSET(D3DXVECTOR2, size, Size);
	GETSET(int, tag, Tag);

	std::vector<UIObject*> childList;
	D3DXMATRIXA16 worldMatrix;

};

