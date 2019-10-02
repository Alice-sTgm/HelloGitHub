#pragma once
/************************************************************
*@file   Selector.h
*@brief  クラスCSelector の宣言ファイル
*@date   9.Nov.2018
*@author Y.Kishimoto
***********************************************************/

#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

//->追加分
#include "dinput.h"
//->追加分

enum GamePhase {
	GAMEPHASE_INIT = 0,
	GAMEPHASE_GAME = 0x100,
};

//
//  Scene の終わり方定数
//  IGameScene::move() が返す値
//
enum GameSceneResultCode {
	GAMESCENE_DEFAULT = 0,
	GAMESCENE_END_OK = 1,
	GAMESCENE_END_TIMEOUT = 2,
	GAMESCENE_END_FAILURE = 3,
};

class    IGameScene;    //  シーン管理クラス

struct   ID2D1RenderTarget;
struct   IDWriteTextFormat;
struct   ID2D1SolidColorBrush;

class CSelector
{
public:
	CSelector(ID2D1RenderTarget *pRenderTarget);
	virtual ~CSelector(void);
	void    doAnim();           //  ゲーム全体のアニメート
	void    doDraw(ID2D1RenderTarget *pRenderTarget);    //  ゲーム全体の描画

	//->追加分
	void setJoystic(DIJOYSTATE2 *js);
	//->追加分

	ID2D1RenderTarget   *GetRenderTarget();
	IDWriteTextFormat   *GetSystemTextFormat();

protected:
	ID2D1RenderTarget * m_pRenderTarget;
	IDWriteTextFormat   *m_pTextFormat;
	ID2D1SolidColorBrush *m_pWhiteBrush;

	IGameScene     *m_pScene;       //  シーン
	enum GamePhase m_eGamePhase;    //  状態変数

	INT m_iCount;  //  カウンター（デバッグ用）
};

//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }