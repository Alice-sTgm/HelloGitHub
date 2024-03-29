#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
#include <d2d1.h>
//->追加分
#include "dinput.h"
#include "HP.h"
//->追加分

struct ID2D1RenderTarget;
class CStage;
class CPlayer :public IGameObject {
public:
	CPlayer(CStage *pStage);
	virtual ~CPlayer();
	virtual bool move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget)override;

	virtual bool collide(float x, float y, float w, float h)override;
	virtual bool collide(IGameObject *pObj)override;
	virtual void damage(float amount)override;
	virtual float getX() {
		return m_fX;
	}
	virtual float getY() {
		return m_fY;
	}

	//->追加分
	virtual void setJoystic(DIJOYSTATE2 *js);
	int getHP();
	//->追加分

protected:
	CStage *m_pStage;
	ID2D1SolidColorBrush *m_pRed;
	ID2D1SolidColorBrush *m_pWhite;
	INT m_iDamage;
	FLOAT m_fX, m_fY;
	BOOL m_bShot; // 連射防止フラグ
	CHP m_hp;
	DIJOYSTATE2 m_js;
};
