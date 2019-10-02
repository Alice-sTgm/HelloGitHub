#include "stdafx.h"
#include <d2d1.h>
#include "BG.h"
#include "TextureLoader.h"
#include "MapData.h"

//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

CBG::CBG(ID2D1RenderTarget *pRenderTarget)
{
	m_pBitmap = NULL;
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\mapchip.png"), &m_pBitmap);

	//  Mapデータヘッダの読取とデコード
	mCW = CMapData::data[4] & 255;    //  チップ画像の幅
	mCH = CMapData::data[5] & 255;    //    チップ画像の高
	mMW = CMapData::data[2] & 255;    //    マップの幅（列数）
	mMH = CMapData::data[3] & 255;    //    マップの高（行数）
	mPitch = mMW * 2;   //  横一行あたりのバイト数
	mMW *= mCW;         //  マップ幅(pixel)
	mMH *= mCH;         //  マップ高(pixel)

	m_iY = mMH - 300;     //  表示開始Y座標のクリア
}


CBG::~CBG()
{
	SAFE_RELEASE(m_pBitmap);
}
int CBG::setY(int y) {

	//! Y座標が大きすぎるとき
	if (y > mMH)
		y %= mMH;

	//! Y座標が小さすぎるとき
	while (y < 0)
		y += mMH;


	m_iY = y;
	return y;
}

int CBG::getY() {
	return m_iY;
}

/****************************************************
*@method
*  描画プログラム
***************************************************/
void CBG::draw(ID2D1RenderTarget *pRenderTarget) {
	int dx, dy;         //    表示のためのＸ座標およびＹ座標
	int code;           //    キャラコード
	int ix = 0;
	int texCols;
	int tu, tv;
	D2D1_RECT_F rc, src;
	D2D1_SIZE_F size;

	if (m_pBitmap == NULL)
		return;
	size = m_pBitmap->GetSize();
	texCols = ((int)size.width + mCW - 1) / mCW;

	ix = mPitch * (m_iY / mCH);
	dy = -(m_iY % mCH);
	int mapSize = CMapData::length - 6;

	while (dy < 300) {
		for (dx = 0; dx < mMW; dx += mCW) {
			code = CMapData::data[ix + 6] & 255;          //    ヘッダ分をスキップ(+6)
			code += (CMapData::data[ix + 7] & 255) << 8;    //    次のデータ
			ix += 2;
			if (ix >= mapSize)
				ix -= mapSize;
			rc.left = dx;
			rc.top = dy;
			rc.right = rc.left + mCW;
			rc.bottom = rc.top + mCH;
			tu = code % texCols;
			tv = code / texCols;
			src.left = tu * mCW;
			src.top = tv * mCH;
			src.right = src.left + mCW;
			src.bottom = src.top + mCH;
			pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src);
		}
		dy += mCH;
	}
}