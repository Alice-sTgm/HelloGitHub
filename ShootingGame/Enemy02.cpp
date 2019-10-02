#include "stdafx.h"
#include <d2d1.h>
#include "Stage.h"
#include "Enemy02.h"
#include "TextureLoader.h"
#include "Bullet.h"
#include "Player.h"

ID2D1Bitmap *CEnemy02::m_pTexture = NULL;

/****************************************************
*@method
*  コンストラクタ
*@param in x  登場位置のX座標
*@param in y  登場位置のY座標
***************************************************/
CEnemy02::CEnemy02(CStage *pStage, float x, float y)
{
	m_pParent = pStage;    //  後で弾を撃つのに必要
	m_fX = x;
	m_fY = y;
	m_fVX = 0;
	m_fVY = 1.5f;

	m_bBullet = false;
	m_bDamage = false;
	m_iPhase = 0;
}


CEnemy02::~CEnemy02()
{
}

/****************************************************
*@method
*  move
*@return true: 生存 / false: 死亡
***************************************************/
bool CEnemy02::move() {
	m_fX += m_fVX;
	m_fY += m_fVY;
	switch (m_iPhase) {
	case    0:
		if (m_fY >= 175) {
			m_fVY = -m_fVY;
			m_fVY *= 2;
			if (m_fX < 150) {
				m_fVX = 1.5;
			}
			else {
				m_fVX = -1.5;
			}
			//  弾丸発射
			CPlayer *p = m_pParent->GetPlayer();
			CBullet *pObj = new CBullet(m_pParent, m_fX + 32, m_fY + 32, p->getX(), p->getY());
			m_pParent->AddBullet(pObj);
			m_iPhase = 1;
		}
		break;
	case    1:
		if (m_fY < -64)
			return    false;
		break;
	}
	if (m_bDamage)
		return false;
	return    true;
}

void CEnemy02::draw(ID2D1RenderTarget *pRenderTarget) {
	int    ix, iy;
	D2D1_RECT_F rc;
	ix = (int)m_fX;
	iy = (int)m_fY;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + 64;
	rc.bottom = rc.top + 64;
	pRenderTarget->DrawBitmap(m_pTexture, rc);
}

bool CEnemy02::collide(float x, float y, float w, float h) {
	float l = m_fX + 8;
	float r = m_fX + 56;
	float t = m_fY + 8;
	float b = m_fY + 56;
	if (t > (y + h))
		return false;
	if (b < y)
		return false;
	if (l >(x + w))
		return false;
	if (r < x)
		return false;
	return true;
}

bool CEnemy02::collide(IGameObject *pObj) {

	float l = m_fX + 8;
	float t = m_fY + 8;
	return pObj->collide(l, t, 48, 48);
}
void CEnemy02::damage(float amount) {
	m_bDamage = true;
}

/*********************************************************
*@fn
*  共有メディアファイルを読み込む
*  シーン開始時などに呼び出すようにする
*********************************************************/
void CEnemy02::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pTexture);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\smile.png"), &m_pTexture);
}

/*********************************************************
*@fn
*  共有メディアファイルの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CEnemy02::Finalize() {
	SAFE_RELEASE(m_pTexture);
}