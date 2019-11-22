class CHP{

public:
    virtual int GetMaxHp();
	int GetCurrentHp();// const { return m_CurrentHp; }
	void AddDamage(int value);// m_CurrentHp -= value; }//NormalizeHp(); }
													   //void RecoverDamage(int value) { m_CurrentHp += value; NormalizeHp(); }
	void SetMaxHp(int value);// { m_MaxHp = value; } //NormalizeHp(); }

protected:
	int m_MaxHp;//ç≈ëÂHP
	int m_CurrentHp;//åªç›ÇÃHP
};
