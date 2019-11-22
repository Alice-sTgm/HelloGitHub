#include"stdafx.h"
//#include"Player.h"
//#include"stage.h"
#include"HP.h"

class HP { //プレイヤーのHP管理
private:
	//int m_MaxHp;//最大HP
	//int m_CurrentHp;//現在のHP

//public:
	//int GetMaxHp(void)const { return m_MaxHp; }//プレイヤーのHPをつける
	//int GetCurrentHp(void)const { return m_CurrentHp; }
	//void AddDamage(int value) { m_CurrentHp -= value; }//NormalizeHp(); }
	//												   //void RecoverDamage(int value) { m_CurrentHp += value; NormalizeHp(); }
	//void SetMaxHp(int value) { m_MaxHp = value; } //NormalizeHp(); }
												  //void NormalizeHp(void) { if (m_CurrentHp < 0) { m_CurrentHp = 0; } else if (m_CurrentHp > m_MaxHp) { m_CurrentHp - m_MaxHp; } }


};

void CHP::AddDamage(int value) {
	m_CurrentHp -= value;
}

void CHP::SetMaxHp(int value)
{
	m_MaxHp = value;
	m_CurrentHp = value;
}

int CHP::GetMaxHp()
{
	return m_MaxHp;
}

int CHP::GetCurrentHp()
{
	return m_CurrentHp;
}
