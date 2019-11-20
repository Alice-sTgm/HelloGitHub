/************************************************************
*@file   Selector.cpp
*@brief  クラスCSelector の実装ファイル
*@date   9.Nov.2018
*@author Y.Kishimoto
***********************************************************/
#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selector.h"
#include "Stage.h"


//->追加分_

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <basetsd.h>

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>
#include "resource.h"



void CSelector::setJoystic(DIJOYSTATE2 *js) {
	m_pScene->setJoystic(js);
}
//->追加分^

CSelector::CSelector(ID2D1RenderTarget *pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
	m_pRenderTarget->AddRef();

	m_iCount = 0;
	m_eGamePhase = GamePhase::GAMEPHASE_INIT;
	m_pScene = NULL;
	m_pWhiteBrush = NULL;

	//  Direct Write 初期化
	{
		HRESULT hr;
		IDWriteFactory *pFactory;
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pFactory),
			reinterpret_cast<IUnknown **>(&pFactory)
		);

		hr = pFactory->CreateTextFormat(
			_T("consolas"),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"", //locale
			&m_pTextFormat
		);

		SAFE_RELEASE(pFactory);
	}
	//  塗りつぶし用ブラシの生成
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	))) {
		SAFE_RELEASE(m_pWhiteBrush);
	}
}

CSelector::~CSelector(void)
{
	SAFE_DELETE(m_pScene);
	SAFE_RELEASE(m_pWhiteBrush);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pTextFormat);
}

void CSelector::doAnim() {

	switch (m_eGamePhase) {
	case    GAMEPHASE_INIT:
		m_pScene = new CStage(this);
		m_eGamePhase = GAMEPHASE_GAME;
		//++m_iWaitCount;
	case    GAMEPHASE_GAME:
		if (m_pScene != NULL) {
			m_pScene->move();
		}
		break;
	}
	m_iCount++;
}

void CSelector::doDraw(ID2D1RenderTarget *pRenderTarget) {
	TCHAR    str[256];

	if (m_pScene != NULL) {
		m_pScene->draw(pRenderTarget);
	}
	D2D1_SIZE_F size;
	size = pRenderTarget->GetSize();

	_stprintf_s(str, _countof(str), _T("%08d"), m_iCount);
	D2D1_RECT_F    rc;
	rc.left = 0;
	rc.right = size.width;
	rc.top = 0;
	rc.bottom = size.height;
	if (m_pWhiteBrush) {
		pRenderTarget->DrawText(str, _tcslen(str), m_pTextFormat, &rc, m_pWhiteBrush);
	}

	//m_iCount = 200;
	rc.left = 0;
	rc.right = m_iCount;
	rc.top = 0;
	rc.bottom = 20;
	//funcDrawFrame();

	pRenderTarget->DrawRectangle(&rc, m_pWhiteBrush,1.0f);
	pRenderTarget->FillRectangle(&rc, m_pWhiteBrush);
}

BOOL Rectangle(
	HDC hDC,
	int nLeft,
	int nTop,
	int nRight,
	int nBottom
);

//------------------------------------------------
// 外枠と内部を描画
//------------------------------------------------
static VOID funcDrawFrame(HDC hDC)
{
	HPEN   hNewPen = (HPEN)CreatePen(PS_INSIDEFRAME, 4, RGB(0x99, 0x66, 0x00));
	HPEN   hOldPen = (HPEN)SelectObject(hDC, hNewPen);
	HBRUSH hNewBrush = (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x99, 0x00));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	Rectangle(hDC, 16, 32, 176, 128);

	DeleteObject(SelectObject(hDC, hOldBrush));
	DeleteObject(SelectObject(hDC, hOldPen));
}

//------------------------------------------------
// 外枠のみを描画
//------------------------------------------------
static VOID funcDrawEdge(HDC hDC)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
	HPEN   hNewPen = (HPEN)CreatePen(PS_INSIDEFRAME, 4, RGB(0x99, 0x66, 0x00));
	HPEN   hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	Rectangle(hDC, 16, 32, 176, 128);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hNewPen);
}

//------------------------------------------------
// 内部のみを描画
//------------------------------------------------
static VOID funcDrawFill(HDC hDC)
{
	HPEN   hOldPen = (HPEN)SelectObject(hDC, GetStockObject(NULL_PEN));
	HBRUSH hNewBrush = (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x99, 0x00));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);

	Rectangle(hDC, 16, 32, 176, 128);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hNewBrush);
}

//------------------------------------------------
// 長方形の座標マクロ
//------------------------------------------------
#define SIZE16(x,y,w,l)     ((x) * 16), ((y) * 16), (((x)+(w)) * 16), (((y)+(l)) * 16)

//------------------------------------------------
// 長方形の描画
//------------------------------------------------
static VOID funcColorRectangle(HDC hDC)
{
	LONG cx = 1;
	LONG cy = 1;

	// 橙色
	SetDCPenColor(hDC, RGB(0x99, 0x66, 0x00));
	SetDCBrushColor(hDC, RGB(0xFF, 0xCC, 0x00));
	Rectangle(hDC, SIZE16(cx, cy, 9, 6));
	cx += 4;
	cy += 3;
	// 黄緑色
	SetDCPenColor(hDC, RGB(0x66, 0x99, 0x00));
	SetDCBrushColor(hDC, RGB(0xCC, 0xFF, 0x00));
	Rectangle(hDC, SIZE16(cx, cy, 9, 6));
	cx += 4;
	cy += 3;
	// 桃色
	SetDCPenColor(hDC, RGB(0x99, 0x00, 0x66));
	SetDCBrushColor(hDC, RGB(0xFF, 0x00, 0xCC));
	Rectangle(hDC, SIZE16(cx, cy, 9, 6));
	cx += 4;
	cy += 3;
	// 紫色
	SetDCPenColor(hDC, RGB(0x66, 0x00, 0x99));
	SetDCBrushColor(hDC, RGB(0xCC, 0x00, 0xFF));
	Rectangle(hDC, SIZE16(cx, cy, 9, 6));
	cx += 4;
	cy += 3;
	// 青緑色
	SetDCPenColor(hDC, RGB(0x00, 0x99, 0x66));
	SetDCBrushColor(hDC, RGB(0x00, 0xFF, 0xCC));
	Rectangle(hDC, SIZE16(cx, cy, 9, 6));
	cx += 4;
	cy += 3;
	// 青色
	SetDCPenColor(hDC, RGB(0x00, 0x66, 0x99));
	SetDCBrushColor(hDC, RGB(0x00, 0xCC, 0xFF));
	Rectangle(hDC, SIZE16(cx, cy, 9, 6));
}

//------------------------------------------------
// WM_PAINTメッセージの処理
//------------------------------------------------
static VOID OnPaint(HWND hWnd)
{
	PAINTSTRUCT     ps;
	HDC             hDC;

	hDC = BeginPaint(hWnd, &ps);
	SelectObject(hDC, GetStockObject(DC_PEN));
	SelectObject(hDC, GetStockObject(DC_BRUSH));
	funcColorRectangle(hDC);
	EndPaint(hWnd, &ps);
}


//
//!> Method Name : GetRenderTarget
//  @Desc : ゲーム画面用のRenderTarget を返す
//  @Note : このメソッドが返した ID2D1RenderTarget は必ずRelease すること
//
ID2D1RenderTarget *CSelector::GetRenderTarget() {
	m_pRenderTarget->AddRef();
	return m_pRenderTarget;
}

//
//!> Method Name : GetSystemTextFormat
//  @Desc : デバッグ用のTextFormat を返す
//  @Note : このメソッドが返した IDWriteTextFormat は必ずRelease すること
//
IDWriteTextFormat *CSelector::GetSystemTextFormat() {
	m_pTextFormat->AddRef();
	return m_pTextFormat;
}