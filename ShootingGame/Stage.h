#pragma once
#include "Selector.h"
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

//->�ǉ���
#include "dinput.h"
//->�ǉ���


class CBG;
class CMapData;
class CPlayer;

// ���g���b�L�[�ȑO���錾
class CEnemySet;
namespace std {
	template<class _Ty> class allocator;
	template <class _Ty, class _Alloc = allocator<_Ty>>class list;
}

class CStage :public IGameScene
{
public:
	CStage(CSelector *pv);
	virtual~CStage(void);
	virtual GameSceneResultCode move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual void reset();
	virtual void AddShot(IGameObject *pObj);
	virtual void AddBullet(IGameObject *pObj);
	virtual CPlayer *GetPlayer();

	//->�ǉ���
	virtual void setJoystic(DIJOYSTATE2 *js);
	//->�ǉ���

	ID2D1RenderTarget *GetRenderTarget();
protected:
	CSelector * m_pSystem;
	BOOL m_bGameOver;
	CBG *m_pBG;
	CPlayer *m_pPlayer;
	CEnemySet *m_pEnemySet;
	std::list<IGameObject*> *m_pEnemies;
	std::list<IGameObject*> *m_pBullets;
	std::list<IGameObject*> *m_pShots;
};