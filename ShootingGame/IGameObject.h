/***********************************************
*@file IGameObject.h
*  ゲーム上のオブジェクトを表現するクラスを
*  汎化したインタフェースを用意しておく
*@note 日本のゲーム業界だと、タスクと呼ぶ場合
*    あり、（ジャンプテーブルに着目した呼び名）、
***********************************************/
#pragma once
#define __IGAMEOBJECT_H__

struct ID2D1RenderTarget;
class IGameObject
{
public:
	virtual ~IGameObject() = 0;
	virtual bool move() = 0;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;

	// 当たり判定関連
	virtual bool collide(float x, float y, float w, float h) {
		return false;
	}
	virtual bool collide(IGameObject *pObj) {
		return false;
	}
	virtual void damage(float amoount) {};
};

//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }