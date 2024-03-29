#pragma once
#ifndef __IGAMEOBJEFCT_H__
#include "IGameObject.h"
#endif
struct ID2D1Bitmap;
class CStage;

class CEnemy01:
	public IGameObject
{
public:
	CEnemy01(CStage *pStage,float x,float y);
	virtual ~CEnemy01();
	virtual bool move();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);
	virtual bool collide(float x, float y, float w, float h)override;
	virtual bool collide(IGameObject *pObj)override;
	virtual void damage(float amount)override;
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();
protected:
	CStage * m_pParent;
	static ID2D1Bitmap *m_pTexture;
	float m_fX;
	float m_fY;
	float m_fVX;
	float m_fVY;
	BOOL m_bBullet;
	BOOL m_bDamage;
};

