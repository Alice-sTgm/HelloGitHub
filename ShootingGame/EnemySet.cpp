/******************************************************
*@file  EnemySet.cpp
*@brief 敵セットシステムオブジェクト
*@date  1.Dec.2018
******************************************************/
#include "stdafx.h"
#include "EnemySet.h"
#include "Enemy01.h"
#include "Enemy02.h"

CEnemySet::CEnemySet(CStage *pStage)
{
	m_iIndex = 0;
	m_pParent = pStage;
}


CEnemySet::~CEnemySet()
{

}

/******************************************************
*@method
*   この敵セットオブジェクトがセットする予定の
*   敵キャラオブジェクト用にデバイス依存オブジェクト
*   を読み込んでおく。
*   こうすることで出現毎に画像を読み込まないようにする
******************************************************/
void CEnemySet::Restore(ID2D1RenderTarget *pRT) {
	CEnemy01::Restore(pRT);
	CEnemy02::Restore(pRT);
}
/******************************************************
*@method
*   この敵セットオブジェクトが監理　する
*   敵キャラオブジェクト用にデバイス依存オブジェクト
*   を消去する。
******************************************************/
void CEnemySet::Finalize() {
	CEnemy01::Finalize();
	CEnemy02::Finalize();
}

/******************************************************
*@method
*   登場させる敵キャラを返すメソッド
*@param in y  敵セット用のY座標
******************************************************/
IGameObject *CEnemySet::GetEnemyToSet(int y) {
	IGameObject *pObj = NULL;
	int    sx, sy;
	if (m_iIndex < m_szSetDataSize) {
		if (y <= m_pSetData[m_iIndex]) {
			sx = m_pSetData[m_iIndex + 1];
			sy = m_pSetData[m_iIndex + 2];
			switch (m_pSetData[m_iIndex + 3]) {
			case    0:
				pObj = new CEnemy01(m_pParent, sx, sy);
				break;

			case    1:
				pObj = new CEnemy02(m_pParent, sx, sy);
				break;


			}
			m_iIndex += 4;
		}
	}
	return    pObj;
}
void CEnemySet::Reset() {
	m_iIndex = 0;
}

/*
*  敵セットデータ
*/
SHORT CEnemySet::m_pSetData[] = {
	1360,-64,50,0,  //  敵セットを行うＹ座標、セット座標、種類
	1300,-64,50,0,  //  敵セットを行うＹ座標、セット座標、種類
	1300,300,110,0, //  敵セットを行うＹ座標、セット座標、種類
	1240,-64,50,0,  //  敵セットを行うＹ座標、セット座標、種類
	1240,300,110,0, //  敵セットを行うＹ座標、セット座標、種類
	1180,-64,50,0,  //  敵セットを行うＹ座標、セット座標、種類
	1180,300,110,0, //  敵セットを行うＹ座標、セット座標、種類
	1120,300,110,0, //  敵セットを行うＹ座標、セット座標、種類

	900,200,-64,1,  //  敵セットを行うＹ座標、セット座標、種類
	860,200,-64,1,  //  敵セットを行うＹ座標、セット座標、種類
	820,200,-64,1,  //  敵セットを行うＹ座標、セット座標、種類
	780,200,-64,1,  //  敵セットを行うＹ座標、セット座標、種類
	740,40, -64,1,  //  敵セットを行うＹ座標、セット座標、種類
	700,40, -64,1,  //  敵セットを行うＹ座標、セット座標、種類
	660,40, -64,1,  //  敵セットを行うＹ座標、セット座標、種類
	620,40, -64,1,  //  敵セットを行うＹ座標、セット座標、種類


};

size_t CEnemySet::m_szSetDataSize = _countof(CEnemySet::m_pSetData);