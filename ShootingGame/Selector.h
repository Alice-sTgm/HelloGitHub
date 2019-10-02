#pragma once
/************************************************************
*@file   Selector.h
*@brief  �N���XCSelector �̐錾�t�@�C��
*@date   9.Nov.2018
*@author Y.Kishimoto
***********************************************************/

#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

//->�ǉ���
#include "dinput.h"
//->�ǉ���

enum GamePhase {
	GAMEPHASE_INIT = 0,
	GAMEPHASE_GAME = 0x100,
};

//
//  Scene �̏I�����萔
//  IGameScene::move() ���Ԃ��l
//
enum GameSceneResultCode {
	GAMESCENE_DEFAULT = 0,
	GAMESCENE_END_OK = 1,
	GAMESCENE_END_TIMEOUT = 2,
	GAMESCENE_END_FAILURE = 3,
};

class    IGameScene;    //  �V�[���Ǘ��N���X

struct   ID2D1RenderTarget;
struct   IDWriteTextFormat;
struct   ID2D1SolidColorBrush;

class CSelector
{
public:
	CSelector(ID2D1RenderTarget *pRenderTarget);
	virtual ~CSelector(void);
	void    doAnim();           //  �Q�[���S�̂̃A�j���[�g
	void    doDraw(ID2D1RenderTarget *pRenderTarget);    //  �Q�[���S�̂̕`��

	//->�ǉ���
	void setJoystic(DIJOYSTATE2 *js);
	//->�ǉ���

	ID2D1RenderTarget   *GetRenderTarget();
	IDWriteTextFormat   *GetSystemTextFormat();

protected:
	ID2D1RenderTarget * m_pRenderTarget;
	IDWriteTextFormat   *m_pTextFormat;
	ID2D1SolidColorBrush *m_pWhiteBrush;

	IGameScene     *m_pScene;       //  �V�[��
	enum GamePhase m_eGamePhase;    //  ��ԕϐ�

	INT m_iCount;  //  �J�E���^�[�i�f�o�b�O�p�j
};

//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }