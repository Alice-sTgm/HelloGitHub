#include "stdafx.h"
#include <d2d1.h>
#include "Player.h"
#include "Stage.h"
#include "Shot.h"

//->追加分_
void CPlayer::setJoystic(DIJOYSTATE2 *js) {
	m_js = *js;
}
/*
struct XINPUT_DEVICE_NODE 
{
	DWORD dwVidPid;
	XINPUT_DEVICE_NODE* pNext;
};

XINPUT_DEVICE_NODE* g_pXInputDeviceList = nullptr;
*/
//->追加分^


CPlayer::CPlayer(CStage *pStage)
{
	ID2D1RenderTarget *pRenderTarget;
	m_pStage = pStage;
	m_pRed = NULL;
	m_pWhite = NULL;
	m_iDamage = 0;
	m_bShot = true;
	//    CSelector が所有しているID2D1RenderTarget を、
	//  CStage からまた借りする

	//->追加分_
	/*
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while (pNode) {
	*/

		pRenderTarget = pStage->GetRenderTarget();
		if (pRenderTarget) {
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRed);
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhite);
			pRenderTarget->Release();
		}


//	}
		m_fX = 150.0f;
		m_fY = 240.0f;
}


CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pWhite);
	SAFE_RELEASE(m_pRed);
}

bool CPlayer::move() {

	if (GetAsyncKeyState(VK_LEFT))
		m_fX -= 3.0f;
	if (GetAsyncKeyState(VK_RIGHT))
		m_fX += 3.0f;
	if (GetAsyncKeyState(VK_UP))
		m_fY -= 3.0f;
	if (GetAsyncKeyState(VK_DOWN))
		m_fY += 3.0f;
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_SPACE))
		m_fX -= 6.0f;
	if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_SPACE))
		m_fX += 6.0f;
	if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_SPACE))
		m_fY -= 6.0f;
	if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_SPACE))
		m_fY += 6.0f;

	if (&m_js != nullptr ) {
		m_fX += m_js.lX/500;
		m_fY += m_js.lY/500;
	}

	if (m_js.rgbButtons[3]) {
		if (!m_bShot) {
			IGameObject *pObj = new CShot(m_pStage, m_fX, m_fY);
			m_pStage->AddShot(pObj);
		}
		m_bShot = true;
	}
	else
		m_bShot = false;

	if (m_iDamage > 0)
		--m_iDamage;

	return true;
}
/********************************************************
*@method
*  描画処理
*******************************************************/
void CPlayer::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = m_fX;
	ellipse.point.y = m_fY;
	ellipse.radiusX = 8.0f;
	ellipse.radiusY = 8.0f;
	pRenderTarget->FillEllipse(ellipse, m_pRed);

	if (m_iDamage > 0) {
		m_pWhite->SetOpacity((FLOAT)m_iDamage*(1.0f / 30.0f));
		pRenderTarget->FillEllipse(ellipse, m_pWhite);
	}
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
bool CPlayer::collide(float x, float y, float w, float h) {
	float l = m_fX - 5;
	float r = m_fX + 5;
	float t = m_fY - 5;
	float b = m_fY + 5;
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
/************************************************************
*@method
*  他オブジェクト当たり判定
*@param in pObj　相手オブジェクト
*@return true : 当たり / false : あたってない
************************************************************/
bool CPlayer::collide(IGameObject *pObj) {
	float l = m_fX - 5;
	float t = m_fY - 5;
	return pObj->collide(l, t, 10, 10);
}


void CPlayer::damage(float amount) {
	m_iDamage = 30;
}