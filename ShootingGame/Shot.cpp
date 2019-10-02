#include "stdafx.h"
#include <d2d1.h>
#include "Shot.h"

ID2D1SolidColorBrush *CShot::m_pBrush = NULL;

CShot::CShot(CStage *pStage,float x,float y)
{
	m_pParent = pStage;
	m_fX = x;
	m_fY = y;
	m_fVX = 0;
	m_fVY = -2.0f;
	m_bDamage = false;
}


CShot::~CShot()
{
}

bool CShot::move() {
	m_fX += m_fVX;
	m_fY += m_fVY;
	if (m_fY < -10)
		return false;
	if (m_bDamage)
		return false;
	return true;
}

void CShot::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = m_fX;
	ellipse.point.y = m_fY;
	ellipse.radiusX = 4.0f;
	ellipse.radiusY = 8.0f;
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
bool CShot::collide(float x, float y, float w, float h) {

	float l = m_fX - 3;
	float r = m_fX + 3;
	float t = m_fY - 5;
	float b = m_fY + 5;
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
/************************************************************
*@method
*  ���I�u�W�F�N�g�����蔻��
*@param in pObj�@����I�u�W�F�N�g
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CShot::collide(IGameObject *pObj) {

	float l = m_fX - 3;
	float t = m_fY - 5;
	return pObj->collide(l, t, 6, 10);
}

void CShot::damage(float amount) {
	m_bDamage = true;
}
/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̐���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CShot::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pBrush);
}
/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CShot::Finalize() {
	SAFE_RELEASE(m_pBrush);
}