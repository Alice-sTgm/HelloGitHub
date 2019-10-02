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
*  矩形との当たり判定
*@param in x 左上のX座標
*@param in y 左上のY座標
*@param in w 矩形の幅
*@param in h 矩形の高
*@return true : 当たり / false : あたってない
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
*  他オブジェクト当たり判定
*@param in pObj　相手オブジェクト
*@return true : 当たり / false : あたってない
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
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする
*********************************************************/
void CShot::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pBrush);
}
/*********************************************************
*@fn
*  共有メディアデータの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CShot::Finalize() {
	SAFE_RELEASE(m_pBrush);
}