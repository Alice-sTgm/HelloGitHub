#include "stdafx.h"
#include <d2d1.h>
#include "Stage.h"
#include "Enemy01.h"
#include "TextureLoader.h"
#include "Bullet.h"
#include "Player.h"

ID2D1Bitmap *CEnemy01::m_pTexture = NULL;

/****************************************************
*@method
*  �R���X�g���N�^
*@param in x  �o��ʒu��X���W
*@param in y  �o��ʒu��Y���W
***************************************************/
CEnemy01::CEnemy01(CStage *pStage, float x, float y)
{
	m_pParent = pStage;    //  ��Œe�����̂ɕK�v
	m_fX = x;
	m_fY = y;
	m_fVX = -1;
	if (m_fX < 150) {
		m_fVX = 1;
	}

	m_bBullet = false;
	m_bDamage = false;
}


CEnemy01::~CEnemy01()
{
}

/****************************************************
*@method
*  move
*@return true: ���� / false: ���S
***************************************************/
bool CEnemy01::move() {
	m_fX += m_fVX;
	if (m_fVX < 0) {

		if (m_fX < 40 && !m_bBullet) {
			CPlayer *pPlayer = m_pParent->GetPlayer();
			if (pPlayer) {
				IGameObject *pObj = new CBullet(m_pParent, m_fX + 32, m_fY + 32,
					pPlayer->getX(), pPlayer->getY());
			}
			//m_bBullet = true;
		}

		if (m_fX < -64)
			return  false;    //    ��ʊO�ɏo����I��
	}
	else {

		if (m_fX > 196 && !m_bBullet) {
			CPlayer *pPlayer = m_pParent->GetPlayer();
			if (pPlayer) {
				IGameObject *pObj = new CBullet(m_pParent, m_fX + 32, m_fY + 32,
					pPlayer->getX(), pPlayer->getY());
				m_pParent->AddBullet(pObj);
			}
			//m_bBullet = true;

		}

		if (m_fX > 300)
			return  false;    //    ��ʊO�ɏo����I��
	}
	if (m_bDamage)
		return false;
	return    true;
}

void CEnemy01::draw(ID2D1RenderTarget *pRenderTarget) {
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

bool CEnemy01::collide(float x, float y, float w, float h) {

	float l = m_fX + 8;
	float r = m_fX + 56;
	float t = m_fY + 8;
	float b = m_fY + 56;
	if (t > (y + h))
		return false;
	if (b < y)
		return false;
	if (l > (x + w))
		return false;
	if (r < x)
		return false;

	return true;
}

bool CEnemy01::collide(IGameObject *pObj) {

	float l = m_fX + 8;
	float t = m_fY + 8;
	return pObj->collide(l, t, 48, 48);

}
void CEnemy01::damage(float amount) {
	m_bDamage = true;
}

/*********************************************************
*@fn
*  ���L���f�B�A�t�@�C����ǂݍ���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CEnemy01::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pTexture);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\smile.png"), &m_pTexture);
}

/*********************************************************
*@fn
*  ���L���f�B�A�t�@�C���̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CEnemy01::Finalize() {
	SAFE_RELEASE(m_pTexture);
}