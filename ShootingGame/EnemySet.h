#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
class CStage;

class CEnemySet
{
public:
	CEnemySet(CStage *pStage);
	virtual ~CEnemySet();
	virtual void Restore(ID2D1RenderTarget *pRT);
	virtual void Finalize();
	virtual IGameObject *GetEnemyToSet(int y);
	virtual void Reset();
protected:
	CStage * m_pParent;
	int m_iIndex;
	static SHORT m_pSetData[];
	static size_t m_szSetDataSize;
};

