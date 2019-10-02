#pragma once

struct ID2D1Bitmap;
struct ID2D1RenderTarget;
class CBG
{
public:
	CBG(ID2D1RenderTarget *pRenderTarget);
	virtual ~CBG();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);
	virtual int setY(int y);
	virtual int getY();
protected:
	int mMW,mMH;
	int mCW, mCH;
	int mPitch;
	ID2D1Bitmap *m_pBitmap;
	int m_iY;
};

