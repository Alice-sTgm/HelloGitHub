/****************************************************
*@file  IGameScene.cpp
*@brief シーンの基底クラス IGameScene の実装
*@date  9.Nov.2018
*@author Y.Kishimoto
***************************************************/
#include "stdafx.h"
#include "IGameScene.h"
#include "Player.h"
//  純粋仮想デストラクタ
IGameScene::~IGameScene()
{
}

//->追加分
void IGameScene::setJoystic(DIJOYSTATE2 * js)
{
}
int IGameScene::getPlayerHp() {
	return 0;
}
//->追加分