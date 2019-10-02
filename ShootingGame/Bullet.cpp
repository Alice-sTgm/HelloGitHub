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
*@return true: 生存 / false: 死亡
***************************************************/
bool CBullet::move() {
	m_fX += m_fVX;
	m_fY += m_fVY;

	// 画面買いに出て行ったら消滅
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
*  描画処理
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
*  矩形との当たり判定
*@param in x 左上のX座標
*@param in y 左上のY座標
*@param in w 矩形の幅
*@param in h 矩形の高
*@return true : 当たり / false : あたってない
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
*  他オブジェクト当たり判定
*@param in pObj　相手オブジェクト
*@return true : 当たり / false : あたってない
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
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする
*********************************************************/
void CBullet::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pBrush);
}

/*********************************************************
*@fn
*  共有メディアファイルの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CBullet::Finalize() {
	SAFE_RELEASE(m_pBrush);
}