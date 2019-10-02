/****************************************************
*@file  IGameScene.h
*@brief シーンの基底クラス IGameScene の宣言
*@date  9.Nov.2018
*@author Y.Kishimoto
***************************************************/
#pragma once
#define __IGAMESCENE_H__

#include "dinput.h"

//  前方宣言
enum GameSceneResultCode;
struct ID2D1RenderTarget;
//
//  class IGameScene
//
class IGameScene
{
public:
	virtual ~IGameScene() = 0;   //  純粋仮想デストラクタ
	virtual GameSceneResultCode    move() = 0;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) = 0;
	//virtual void    reset() = 0;

	//->追加分
	virtual void setJoystic(DIJOYSTATE2 *js);
	//->追加分



};