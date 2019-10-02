/******************************************************
*@file  EnemySet.cpp
*@brief �G�Z�b�g�V�X�e���I�u�W�F�N�g
*@date  1.Dec.2018
******************************************************/
#include "stdafx.h"
#include "EnemySet.h"
#include "Enemy01.h"
#include "Enemy02.h"

CEnemySet::CEnemySet(CStage *pStage)
{
	m_iIndex = 0;
	m_pParent = pStage;
}


CEnemySet::~CEnemySet()
{

}

/******************************************************
*@method
*   ���̓G�Z�b�g�I�u�W�F�N�g���Z�b�g����\���
*   �G�L�����I�u�W�F�N�g�p�Ƀf�o�C�X�ˑ��I�u�W�F�N�g
*   ��ǂݍ���ł����B
*   �������邱�Ƃŏo�����ɉ摜��ǂݍ��܂Ȃ��悤�ɂ���
******************************************************/
void CEnemySet::Restore(ID2D1RenderTarget *pRT) {
	CEnemy01::Restore(pRT);
	CEnemy02::Restore(pRT);
}
/******************************************************
*@method
*   ���̓G�Z�b�g�I�u�W�F�N�g���ė��@����
*   �G�L�����I�u�W�F�N�g�p�Ƀf�o�C�X�ˑ��I�u�W�F�N�g
*   ����������B
******************************************************/
void CEnemySet::Finalize() {
	CEnemy01::Finalize();
	CEnemy02::Finalize();
}

/******************************************************
*@method
*   �o�ꂳ����G�L������Ԃ����\�b�h
*@param in y  �G�Z�b�g�p��Y���W
******************************************************/
IGameObject *CEnemySet::GetEnemyToSet(int y) {
	IGameObject *pObj = NULL;
	int    sx, sy;
	if (m_iIndex < m_szSetDataSize) {
		if (y <= m_pSetData[m_iIndex]) {
			sx = m_pSetData[m_iIndex + 1];
			sy = m_pSetData[m_iIndex + 2];
			switch (m_pSetData[m_iIndex + 3]) {
			case    0:
				pObj = new CEnemy01(m_pParent, sx, sy);
				break;

			case    1:
				pObj = new CEnemy02(m_pParent, sx, sy);
				break;


			}
			m_iIndex += 4;
		}
	}
	return    pObj;
}
void CEnemySet::Reset() {
	m_iIndex = 0;
}

/*
*  �G�Z�b�g�f�[�^
*/
SHORT CEnemySet::m_pSetData[] = {
	1360,-64,50,0,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1300,-64,50,0,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1300,300,110,0, //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1240,-64,50,0,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1240,300,110,0, //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1180,-64,50,0,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1180,300,110,0, //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	1120,300,110,0, //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���

	900,200,-64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	860,200,-64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	820,200,-64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	780,200,-64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	740,40, -64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	700,40, -64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	660,40, -64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���
	620,40, -64,1,  //  �G�Z�b�g���s���x���W�A�Z�b�g���W�A���


};

size_t CEnemySet::m_szSetDataSize = _countof(CEnemySet::m_pSetData);