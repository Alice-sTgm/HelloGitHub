/****************************************************
*@file  IGameScene.h
*@brief �V�[���̊��N���X IGameScene �̐錾
*@date  9.Nov.2018
*@author Y.Kishimoto
***************************************************/
#pragma once
#define __IGAMESCENE_H__

#include "dinput.h"

//  �O���錾
enum GameSceneResultCode;
struct ID2D1RenderTarget;
//
//  class IGameScene
//
class IGameScene
{
public:
	virtual ~IGameScene() = 0;   //  �������z�f�X�g���N�^
	virtual GameSceneResultCode    move() = 0;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) = 0;
	//virtual void    reset() = 0;

	//->�ǉ���
	virtual void setJoystic(DIJOYSTATE2 *js);
	//->�ǉ���



};