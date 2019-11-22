#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
#include <d2d1.h>
//->’Ç‰Á•ª
#include "dinput.h"
#include "HP.h"
//->’Ç‰Á•ª

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

	//->’Ç‰Á•ª
	virtual void setJoystic(DIJOYSTATE2 *js);
	int getHP();
	//->’Ç‰Á•ª

protected:
	CStage *m_pStage;
	ID2D1SolidColorBrush *m_pRed;
	ID2D1SolidColorBrush *m_pWhite;
	INT m_iDamage;
	FLOAT m_fX, m_fY;
	BOOL m_bShot; // ˜AŽË–hŽ~ƒtƒ‰ƒO
	CHP m_hp;
	DIJOYSTATE2 m_js;
};
