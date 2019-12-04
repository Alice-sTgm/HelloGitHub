/************************************************************
*@file   Selector.cpp
*@brief  �N���XCSelector �̎����t�@�C��
*@date   9.Nov.2018
*@author Y.Kishimoto
***********************************************************/
#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selector.h"
#include "Stage.h"


//->�ǉ���_

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
//->�ǉ���^

CSelector::CSelector(ID2D1RenderTarget *pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
	m_pRenderTarget->AddRef();

	m_iCount = 0;
	m_eGamePhase = GamePhase::GAMEPHASE_INIT;
	m_pScene = NULL;
	m_pWhiteBrush = NULL;

	//  Direct Write ������
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
	//  �h��Ԃ��p�u���V�̐���
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
	int hit = m_pScene->getPlayerHp();
	

	D2D1_SIZE_F size;
	size = pRenderTarget->GetSize();
	_stprintf_s(str, _countof(str), _T("HP:%d"),hit );//�����ł�����܂����B

	//_stprintf_s(str, _countof(str), _T("%08d"), m_iCount);//8���Ő������o���Ă���o���Ă���
	D2D1_RECT_F    rc;
	rc.left = 0;
	rc.right = size.width;
	rc.top = 0;
	rc.bottom = size.height;
	if (m_pWhiteBrush) {
		pRenderTarget->DrawText(str, _tcslen(str), m_pTextFormat, &rc, m_pWhiteBrush);

	}

	rc.left = 0;
	rc.right = hit/5;
	rc.top = 0;
	rc.bottom = 20;

		//pRenderTarget->DrawText(str, _tcslen(str), m_pTextFormat, &rc, m_pWhiteBrush);
		pRenderTarget->DrawRectangle(rc, m_pWhiteBrush, 1.0f);
		pRenderTarget->FillRectangle(rc, m_pWhiteBrush);


	if (hit <= 0) {
		TCHAR    strGameOver[256] =_T("Game Over!");
		  //������ǉ�����v���O����
		D2D1_RECT_F    rcGameOver;
		rcGameOver.left = 100;
		rcGameOver.right = size.width;
		rcGameOver.top = 100;
		rcGameOver.bottom = size.height;
		if (m_pWhiteBrush) {
			pRenderTarget->DrawText(strGameOver, _tcslen(strGameOver), m_pTextFormat, &rcGameOver, m_pWhiteBrush);
			
		}
	}
	
}


//
//!> Method Name : GetRenderTarget
//  @Desc : �Q�[����ʗp��RenderTarget ��Ԃ�
//  @Note : ���̃��\�b�h���Ԃ��� ID2D1RenderTarget �͕K��Release ���邱��
//
ID2D1RenderTarget *CSelector::GetRenderTarget() {
	m_pRenderTarget->AddRef();
	return m_pRenderTarget;
}

//
//!> Method Name : GetSystemTextFormat
//  @Desc : �f�o�b�O�p��TextFormat ��Ԃ�
//  @Note : ���̃��\�b�h���Ԃ��� IDWriteTextFormat �͕K��Release ���邱��
//
IDWriteTextFormat *CSelector::GetSystemTextFormat() {
	m_pTextFormat->AddRef();
	return m_pTextFormat;
}