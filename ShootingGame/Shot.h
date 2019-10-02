/****************************************
*@file Shot.h
*  ÉNÉâÉXCShot ÇÃêÈåæ
*@first edition : 17.Nov.2018
***************************************/
#pragma once
#ifndef __IGAMEOBJECT_H_
#include "IGameObject.h"
#endif
struct ID2D1SolidColorBrush;
class CStage;

class CShot:public IGameObject
{
public:
	CShot(CStage *pStage,float x,float y);
	~CShot();

	virtual bool move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget)override;
	virtual bool collide(float x, float y, float w, float h)override;
	virtual bool collide(IGameObject *pObj)override;
	virtual void damage(float amount)override;
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();

protected:
	CStage * m_pParent;
	BOOL m_bDamage;
	float m_fX, m_fY;
	float m_fVX, m_fVY;
	static ID2D1SolidColorBrush *m_pBrush;
};

