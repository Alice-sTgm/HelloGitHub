#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

struct ID2D1SolidColorBrush;
class CStage;

class CBullet:public IGameObject
{
public:
	CBullet(CStage *pStage,float x,float y,float target_x,float target_y);
	virtual ~CBullet();
	virtual bool move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget)override;
	virtual bool collide(float x, float y, float w, float h)override;
	virtual bool collide(IGameObject *pObj)override;
	virtual void damage(float amount)override;
	static void Restore(ID2D1RenderTarget *RT);
	static void Finalize();
protected:
	CStage * m_pParent;
	BOOL m_bDamage;
	float m_fX, m_fY;
	float m_fVX, m_fVY;
	static ID2D1SolidColorBrush *m_pBrush;
};

