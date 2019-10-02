#pragma once
#include "Selector.h"
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

//->追加分
#include "dinput.h"
//->追加分


class CBG;
class CMapData;
class CPlayer;

// ややトリッキーな前方宣言
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

	//->追加分
	virtual void setJoystic(DIJOYSTATE2 *js);
	//->追加分

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