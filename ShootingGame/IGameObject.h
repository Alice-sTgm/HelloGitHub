/***********************************************
*@file IGameObject.h
*  �Q�[����̃I�u�W�F�N�g��\������N���X��
*  �ĉ������C���^�t�F�[�X��p�ӂ��Ă���
*@note ���{�̃Q�[���ƊE���ƁA�^�X�N�ƌĂԏꍇ
*    ����A�i�W�����v�e�[�u���ɒ��ڂ����Ăі��j�A
***********************************************/
#pragma once
#define __IGAMEOBJECT_H__

struct ID2D1RenderTarget;
class IGameObject
{
public:
	virtual ~IGameObject() = 0;
	virtual bool move() = 0;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;

	// �����蔻��֘A
	virtual bool collide(float x, float y, float w, float h) {
		return false;
	}
	virtual bool collide(IGameObject *pObj) {
		return false;
	}
	virtual void damage(float amoount) {};
};

//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }