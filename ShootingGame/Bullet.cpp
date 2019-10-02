#include "stdafx.h"
#include <d2d1.h>
#include "Bullet.h"
#include <math.h>

ID2D1SolidColorBrush *CBullet::m_pBrush = NULL;
CBullet::CBullet(CStage *pStage, float x, float y, float target_x, float target_y)
{
	m_pParent = pStage;
	m_fX = x;
	m_fY = y;

	float vx = target_x - x;
	float vy = target_y - y;
	float l = vx * vx + vy * vy;
	l = 1.0f / sqrtf(l);

	m_fVX = l * vx;
	m_fVY = l * vy;
}

CBullet::~CBullet()
{
}

/****************************************************
*@method
*  move
*@return true: ���� / false: ���S
***************************************************/
bool CBullet::move() {
	m_fX += m_fVX;
	m_fY += m_fVY;

	// ��ʔ����ɏo�čs���������
	if ((m_fX > 320) && (m_fVX > 0))
		return false;
	if ((m_fX < -20) && (m_fVX < 0))
		return false;
	if ((m_fY > 320) && (m_fVY > 0))
		return false;
	if ((m_fY < -20) && (m_fVY < 0))
		return false;

	return true;
}

/****************************************************
*@method
*  �`�揈��
***************************************************/
void CBullet::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = m_fX;
	ellipse.point.y = m_fY;
	ellipse.radiusX = 4.0f;
	ellipse.radiusY = 4.0f;
	pRenderTarget->FillEllipse(ellipse, m_pBrush);
}
/************************************************************
*@method
*  ��`�Ƃ̓����蔻��
*@param in x �����X���W
*@param in y �����Y���W
*@param in w ��`�̕�
*@param in h ��`�̍�
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CBullet::collide(float x, float y, float w, float h) {
	float l = m_fX - 3;
	float r = m_fX + 3;
	float t = m_fY - 3;
	float b = m_fY + 3;
	if (t > (y + h))
		return false;
	if (b < y)
		return false;
	if (r < x)
		return false;
	return true;
}
/************************************************************
*@method
*  ���I�u�W�F�N�g�����蔻��
*@param in pObj�@����I�u�W�F�N�g
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CBullet::collide(IGameObject *pObj) {
	float l = m_fX - 3;
	float t = m_fY - 3;
	return pObj->collide(l, t, 6, 6);
}
void CBullet::damage(float amount) {
	m_bDamage = true;
}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̐���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CBullet::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pBrush);
}

/*********************************************************
*@fn
*  ���L���f�B�A�t�@�C���̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CBullet::Finalize() {
	SAFE_RELEASE(m_pBrush);
}