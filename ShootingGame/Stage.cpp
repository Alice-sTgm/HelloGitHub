#include "stdafx.h"
#include <d2d1.h>
#include <list>
#include "Stage.h"
#include "TextureLoader.h"
#include "BG.h"
#include "Player.h"
#include "EnemySet.h"
#include "Bullet.h"
#include "Shot.h"

//->�ǉ���
void CStage::setJoystic(DIJOYSTATE2 *js) {
	m_pPlayer->setJoystic(js);
}
//->�ǉ���

/***********************************
* ctor
***********************************/
CStage::CStage(CSelector *pSystem)
{
	ID2D1RenderTarget *pRenderTarget;
	m_pSystem = pSystem;
	pRenderTarget = m_pSystem->GetRenderTarget();
	m_pBG = new CBG(pRenderTarget);
	m_pPlayer = new CPlayer(this);

	m_pEnemySet = new CEnemySet(this);
	m_pEnemySet->Restore(pRenderTarget);
	m_pEnemies = new std::list<IGameObject*>();

	CBullet::Restore(pRenderTarget);
	m_pBullets = new std::list<IGameObject*>();

	CShot::Restore(pRenderTarget);
	m_pShots = new std::list<IGameObject*>();

	SAFE_RELEASE(pRenderTarget);
}

CStage::~CStage()
{
	// �S�G�L�����̋����폜
	if (m_pEnemies) {
		std::list<IGameObject*>::iterator it = m_pEnemies->begin();
		while (it != m_pEnemies->end()) {
			SAFE_DELETE(*it);
			it = m_pEnemies->erase(it);
		}
		delete m_pEnemies;
		m_pEnemies = NULL;
	}

	// �S�o���b�g�̋�������
	if (m_pBullets) {
		std::list<IGameObject*>::iterator it = m_pBullets->begin();
		while (it != m_pBullets->end()) {
			SAFE_DELETE(*it);
			it = m_pBullets->erase(it);
		}
		delete m_pBullets;
		m_pBullets = NULL;
	}
	CBullet::Finalize();

	// �S�V���b�g�̋����폜
	if (m_pShots) {
		std::list<IGameObject*>::iterator it = m_pShots->begin();
		while(it != m_pShots->end()) {
			SAFE_DELETE(*it);
			it = m_pShots->erase(it);
		}
		delete m_pShots;
		m_pShots = NULL;
	}
	CShot::Finalize();

	SAFE_DELETE(m_pEnemySet); //!< �G�Z�b�g�V�X�e���̍폜

	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pBG);
	CTextureLoader::Destroy();    //  TextureLoader �I�u�W�F�N�g�̉��
}

/***********************************************
*@method
*    ���Z�b�g
**********************************************/
void CStage::reset() {

}

/***********************************************
*@method
*   �A�j���[�V�����i�P�t���[���j�̎��s
*@return GAMESCENE_DEFAULT: �p��
*        GAMESCENE_END_FAILURE: �Q�[���I�[�o�[
**********************************************/
GameSceneResultCode CStage::move() {
	int old_y = m_pBG->getY();
	int y = m_pBG->setY(old_y - 1);

	if (m_pEnemies&&m_pEnemySet) {
		IGameObject *pObj;
		if (y > old_y) {
			m_pEnemySet->Reset();
		}
		do {
			pObj = m_pEnemySet->GetEnemyToSet(y);
			if (pObj != NULL)
				m_pEnemies->push_back(pObj);
		} while (pObj);
		std::list<IGameObject*>::iterator it = m_pEnemies->begin();
		while (it != m_pEnemies->end()) {
			if ((*it)->move()) {
				++it;
			}
			else {
				SAFE_DELETE(*it);
				it = m_pEnemies->erase(it);
			}
		}
	}

	// �e�̏���
	if (m_pBullets) {
		std::list<IGameObject*>::iterator it = m_pBullets->begin();
		while (it != m_pBullets->end()) {
			if ((*it)->move()) {
				++it;
			}
			else {
				SAFE_DELETE(*it);
				it = m_pBullets->erase(it);
			}
		}
	}

	// �V���b�g�̏���
	if (m_pShots) {
		std::list<IGameObject*>::iterator it = m_pShots->begin();
		while (it != m_pShots->end()) {
			if ((*it)->move()) {
				++it;
			}
			else {
				SAFE_DELETE(*it);
				it = m_pShots->erase(it);
			}
		}
	}


	if (m_pPlayer)
		m_pPlayer->move();


	// �e�ƃv���C���[�̓����蔻��
	if (m_pBullets) {
		std::list<IGameObject*>::iterator it = m_pBullets->begin();
		while (it != m_pBullets->end()) {
			if ((*it)->collide(m_pPlayer)) {
				(*it)->damage(1.0f);
				m_pPlayer->damage(1.0f);
			}
			++it;
		}
	}

	// �V���b�g�ƓG�̓����蔻��
	if (m_pShots&&m_pEnemies) {
		std::list<IGameObject*>::iterator it1 = m_pShots->begin();
		std::list<IGameObject*>::iterator it2;
		while (it1 != m_pShots->end()) {
			it2 = m_pEnemies->begin();
			while (it2 != m_pEnemies->end()) {
				if ((*it2)->collide(*it1)) {
					(*it1)->damage(1.0f);
					(*it2)->damage(1.0f);
				}
				it2++;
			}
			++it1;
		}
	}

	return GameSceneResultCode::GAMESCENE_DEFAULT;
}

/***********************************************
*@method
*    �`�揈��
*@param in pRenderTarget  �`��^�[�Q�b�g
**********************************************/
void CStage::draw(ID2D1RenderTarget *pRenderTarget) {
	if (m_pBG)
		m_pBG->draw(pRenderTarget);

	if (m_pPlayer)
		m_pPlayer->draw(pRenderTarget);

	if (m_pEnemies) {
		std::list<IGameObject*>::iterator it = m_pEnemies->begin();
		while (it != m_pEnemies->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}

	// �e�̏���
	if (m_pBullets) {
		std::list<IGameObject*>::iterator it = m_pBullets->begin();
		while (it != m_pBullets->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}

	// �V���b�g�̏���
	if (m_pShots) {
		std::list<IGameObject*>::iterator it = m_pShots->begin();
		while (it != m_pShots->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
}

/***********************************************
*@method
*   ID2D1RenderTarget ���擾���ĕԂ��A
*    ���̃��\�b�h��RenderTarget ���󂯎������
*   �g�p�I���ォ�Ȃ炸Release ���邱��
*@return ID2D1RenderTarget �I�u�W�F�N�g
**********************************************/
ID2D1RenderTarget *CStage::GetRenderTarget() {
	ID2D1RenderTarget *result = NULL;
	if (m_pSystem) {
		result = m_pSystem->GetRenderTarget();
	}
	return result;
}

/***********************************************
*@method
*    �������ꂽ�V���b�g�������N���X�g�ɓo�^����
*@param in pObj  �V�����e�̃I�u�W�F�N�g
************************************************/
void CStage::AddShot(IGameObject *pObj) {
	if (m_pShots) {
		m_pShots->push_back(pObj);
	}
}
/***********************************************
*@method
*    �������ꂽ�e�������N���X�g�ɓo�^����
*@param in pObj  �V�����e�̃I�u�W�F�N�g
************************************************/
void CStage::AddBullet(IGameObject *pObj) {
	if (m_pBullets) {
		m_pBullets->push_back(pObj);
	}
}

/***********************************************
*@method
*    �v���C���[�̃C���X�^���X��Ԃ�
*@return  CPlayer �I�u�W�F�N�g
************************************************/
CPlayer *CStage::GetPlayer() {
	return m_pPlayer;
}