// PX2Font.cpp

#include "PX2Font.hpp"
#include "PX2Renderer.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Time.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
#define BLINK_CIRCLE		1200
#define BLINK_HALFCIRCLE	600
#define BLINKMODEL_SMOOTH	1
#define BLINKMODEL_SUDDEN	2
//----------------------------------------------------------------------------
unsigned int Font::msCounts = 0;
//----------------------------------------------------------------------------
Font::Font (FontType type):
mFontPicSize(1024),
mFontType(type),
mIsNeedUpdate(true)
{
	msCounts++;

	mCodingType = CCT_GBK;
	mFontExtStyle = FES_NORMAL;
	mFontWidth = 0;
	mFontHeight = 0;
	mCharWidth = 0.0f;
	mCharHeight = 0.0f;
	mHorInterval = 0.0f;
	mLineInterval = 0.0f;
	mCharNumMaxCache = 0;
	mTexWidth = 0;
	mTexHeight = 0;
	mCharCodingObj = 0;
	mGlyphMap = 0;
	mShowNum = 0;
	mIsNeedUpdate = true;

	mDrawRects.resize(FONT_MAXRECTNUM);
}
//----------------------------------------------------------------------------
Font::Font ()
{
}
//----------------------------------------------------------------------------
Font::~Font ()
{
}
//----------------------------------------------------------------------------
void Font::DumpTex ()
{
	if (mTexture)
	{
		std::string filename = mFontFilename +
			"_" +
			StringHelp::IntToString(mFontWidth) +
			"_" +
			StringHelp::IntToString(mFontHeight) +
			".png";

			//PX2_RM.SaveTex2DPNG(mTexture, filename);
	}
}
//----------------------------------------------------------------------------
void Font::UpdateFontTexs ()
{
	if (!mIsNeedUpdate)
		return;

	if (mTexture)
	{
		Renderer::UpdateAll(mTexture, 0);
	}

	mIsNeedUpdate = false;
}
//----------------------------------------------------------------------------
void Font::SetNeedUpdate()
{
	mIsNeedUpdate = true;
}
//----------------------------------------------------------------------------
bool Font::Initlize (int fontWidth, int fontHeight, 
	const char *fontFilename, CharCodingType codingType,
	unsigned int fontExtStyle)
{
	mFontFilename = fontFilename;
	mCodingType = codingType;
	mFontExtStyle = fontExtStyle;

	mCharWidth = (float)fontWidth;
	mCharHeight = (float)fontHeight;
	mFontWidth = fontWidth;
	mFontHeight = fontHeight;

	return true;
}
//----------------------------------------------------------------------------
void Font::GetFontSize (int &width, int &height)
{
	width = mFontWidth;
	height = mFontHeight;
}
//----------------------------------------------------------------------------
void Font::SetCharSize (int width, int height)
{
	mCharWidth = (float)width;
	mCharHeight = (float)height;
}
//----------------------------------------------------------------------------
void Font::GetCharSize (float &width, float &height)
{
	width = (float)mCharWidth;
	height = (float)mCharHeight;
}
//----------------------------------------------------------------------------
void Font::SetHorInterval (float interval)
{
	mHorInterval = interval;
}
//----------------------------------------------------------------------------
void Font::GetHorInterval (float &interval)
{
	interval = mHorInterval;
}
//----------------------------------------------------------------------------
void Font::SetLineInterval (float interval)
{
	mLineInterval = interval;
}
//----------------------------------------------------------------------------
void Font::GetLineInterval (float &interval)
{
	interval = mLineInterval;
}
//----------------------------------------------------------------------------
void Font::GetCharSize (const unsigned char *character, float &width,
	float &height)
{
	wchar_t wChar = mCharCodingObj->ToUnicode(character);
	mGlyphMap->GetUnicodeCharSize(wChar, width, height);
}
//----------------------------------------------------------------------------
void Font::GetCharExtent (const unsigned char *character, float &width1,
	float &height1, float fontScale)
{
	float width = 0.0f;
	float height = 0.0f;
	GetCharSize(character, width, height);

	width1 = (width / height) * mCharWidth * fontScale;
	height1 = 1.0f * mCharHeight * fontScale;
}
//----------------------------------------------------------------------------
void Font::GetTextExtent (const char *text, float &width, float &height,
	bool doTransfer, float fontScale)
{
	if (!text)
		return;

	width = 0.0f;
	height = 0.0f;

	const char *p = text;
	float allWidth = 0.0f;
	int ctrlCodeSize = 0;
	bool isFirstCharFind = false;
	bool isMeetTransfer = false;
	CharCtrlCode ctrlCode;

	bool doWhile = true;
	while (doWhile)
	{
		ctrlCodeSize = mCharCodingObj->GetControlCode(p, ctrlCode, doTransfer);
		p += ctrlCodeSize;

		if (CCC_ENDOFSTRING == ctrlCode)
		{
			if (allWidth > width)
			{
				width = allWidth;
			}
			break;
		}

		if (CCC_NEWLINE == ctrlCode)
		{
			// 遇到换行，看是否更新最大行宽
			if (allWidth > width)
				width = allWidth;

			// 从新累计行宽
			allWidth = 0;

			// 清除标志，再遇到字符时累计高度
			isFirstCharFind = false;
		}
		else if (CCC_CHARACTER == ctrlCode)
		{
			float charWidth;
			float charHeight;
			unsigned char cc[4];
			int charSize = mCharCodingObj->GetAChar(p, cc);

			GetCharExtent(cc, charWidth, charHeight, fontScale);
			p += charSize;

			if (isMeetTransfer)
			{
				isMeetTransfer = false;
			}
			else
			{
				allWidth += charWidth;

				// 如果是本行的第一个字符，累计高度
				if (!isFirstCharFind)
				{
					height += (mCharHeight + mLineInterval);
					isFirstCharFind = true;
				}
			}
		}
		else if (CCC_TRANSFER == ctrlCode)
		{
			isMeetTransfer = true;
		}
	}
}
//----------------------------------------------------------------------------
void Font::GetTextExtentFitInWidth (const char *text, float fixedWidth, 
	float &width, int &numBytes, bool doTransfer, float fontScale)
{
	if (!text)
		return;

	width = 0.0f;
	numBytes = 0;

	const char *p = text;
	int ctrlCodeSize = 0;
	CharCtrlCode ctrlCode;

	bool doWhile = true;
	while (doWhile)
	{
		ctrlCodeSize = mCharCodingObj->GetControlCode(p, ctrlCode, doTransfer);
		p += ctrlCodeSize;

		if (CCC_ENDOFSTRING == ctrlCode)
			break;

		if (CCC_CHARACTER == ctrlCode)
		{
			float charWidth = 0.0f;
			float charHeight = 0.0f;
			unsigned char cc[4];
			int charSize = mCharCodingObj->GetAChar(p, cc);

			GetCharExtent(cc, charWidth, charHeight, fontScale);
			p += charSize;

			if (width + charWidth > fixedWidth)
				break;

			width += charWidth;
			numBytes += charSize;
		}
	}
}
//----------------------------------------------------------------------------
inline void SetupCharPoly (FontDrawRect *poly, float x, float y, float w,
	float h, const Rectf &rectUV, const Float4 &color)
{
	poly->Rect.Set(x, y, x+w, y+h);
	poly->RectUV = rectUV;
	poly->Color = color;
}
//----------------------------------------------------------------------------
void Font::TextOutM (TriMesh *mesh, const std::string &text, float xPos, float yPos,
	const Float2 &space, const Float4 &color, const Float4 &shadowBorderColor4,
	float shadowBorderSize, unsigned int drawStyle,	bool doTransfer, 
	float fontScale, float depth)
{
	SetHorInterval(space[0]);
	SetLineInterval(space[1]);

	if (text.empty())
	{
		mDrawRects[0] = FontDrawRect();
		mShowNum = 1;
		RenderText(mesh, 0.0f);

		return;
	}

	mShowNum = 0;

	const char *p = text.c_str();
	CharCtrlCode ctrlCode;
	int ctrlCodeSize = 0;
	float curX = xPos;
	float curY = yPos;
	Float4 curColor = color;
	int showCharNum = 0;
	bool blink = false;
	int blinkModel = BLINKMODEL_SMOOTH;

	bool doWhile = true;
	while (doWhile)
	{
		ctrlCodeSize = mCharCodingObj->GetControlCode(p, ctrlCode, doTransfer);
		p += ctrlCodeSize;

		if (CCC_ENDOFSTRING == ctrlCode)
			break;

		switch (ctrlCode)
		{
		case CCC_NEWLINE:
			{
				// 重新计算坐标
				curX = xPos;
				curY -= (mCharHeight + mLineInterval);
			}
			break;
		case CCC_CHARACTER:
			{
				// 取一个字符
				unsigned char cc[4];
				int charSize = mCharCodingObj->GetAChar(p, cc);

				// 获得字符的宽，高
				float charWidth = 0.0f;
				float charHeight = 0.0f;
				GetCharExtent(cc, charWidth, charHeight, fontScale);

				Rectf rectUV;
				Texture2D *tex = TexUVMaping(cc, rectUV);
				mTexture = tex;

				if (mTexture == tex)
				{
					if (drawStyle == FD_SHADOW)
					{
						SetupCharPoly(&mDrawRects[0]+mShowNum,
							curX+shadowBorderSize, curY-shadowBorderSize, charWidth, charHeight,
							rectUV, shadowBorderColor4);
						mShowNum++;
					}
					else if (drawStyle == FD_BORDER)
					{
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX-shadowBorderSize, curY, charWidth, charHeight, 
							rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX+shadowBorderSize, curY, charWidth, charHeight, 
							rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX, curY-shadowBorderSize, charWidth, charHeight, 
							rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX, curY+shadowBorderSize, charWidth, charHeight,
							rectUV, shadowBorderColor4);
						mShowNum++;

						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX-shadowBorderSize, curY-shadowBorderSize, charWidth, charHeight, 
							rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX-shadowBorderSize, curY+shadowBorderSize, charWidth, charHeight, 
							rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX+shadowBorderSize, curY-shadowBorderSize, charWidth, charHeight, 
							rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							curX+shadowBorderSize, curY+shadowBorderSize, charWidth, charHeight,
							rectUV, shadowBorderColor4);
						mShowNum++;
					}

					SetupCharPoly(&mDrawRects[0]+mShowNum,
						curX, curY, charWidth, charHeight, rectUV, curColor);
					mShowNum++;
					showCharNum++;
					p += charSize;
					curX += (charWidth+mHorInterval);
				}

				// 如果贴图缓存满或者多边形缓冲区满或者切换了贴图，则把当前的多边形缓冲区输出
				if (showCharNum>=mCharNumMaxCache || (mShowNum+8)>=FONT_MAXRECTNUM
					|| mTexture!=tex)
				{
					assertion(false, "Fonts, numLarge");
					showCharNum = 0;
				}
			}
			break;
		case CCC_TRANSFER:
			{
				switch(*p)
				{
				case 'R':
					curColor = Float4::RED;
					p += 1;
					break;
				case 'G':
					curColor = Float4::GREEN;
					p += 1;
					break;
				case 'B':
					curColor = Float4::BLUE;
					p += 1;
					break;
				case 'K':
					curColor = Float4::BLACK;
					p += 1;
					break;
				case 'Y':
					curColor = Float4::YELLOW;
					p += 1;
					break;
				case 'W':
					curColor = Float4::WHITE;
					p += 1;
					break;
				case 'n':
					curColor = color;
					p += 1;
					blink = false;
					break;
				case 'b':
					blink = true;
					if (*(p+1) == '1')
					{
						blinkModel = BLINKMODEL_SMOOTH;
						p += 2;
					}
					else if (*(p+1) == '2')
					{
						blinkModel = BLINKMODEL_SUDDEN;
						p += 2;
					}
					else
					{
						blinkModel = BLINKMODEL_SMOOTH;
						p += 1;
					}
					break;
				}

				if (blink)
				{
					if (BLINKMODEL_SMOOTH == blinkModel)
					{
						int microSec = (int)Time::GetTimeInMicroseconds();

						int msNum = microSec % BLINK_CIRCLE;
						if (msNum >= BLINK_HALFCIRCLE)
							msNum = BLINK_CIRCLE - msNum - 1;
						msNum = msNum*320/BLINK_HALFCIRCLE;
						if (msNum > 255)
							msNum = 255;

						float alpha = (float)msNum/255.0f;
						curColor = Float4(curColor[0], curColor[1], curColor[2], alpha);
					}
					else
					{
						int microSec = (int)Time::GetTimeInMicroseconds();
						if ((microSec/BLINK_HALFCIRCLE) % 2)
							curColor = Float4::ZERO;
					}
				}
			}
			break;
		default:
			break;
		} // switch (ctrlCode)
	} // while (true)

	RenderText(mesh, depth);
}
//----------------------------------------------------------------------------
void Font::TextOutRect(TriMesh *mesh, const std::string &text, Rectf &rect, 
	const Float2 &space,
	float offsetX, float offsetY, bool autoWrap, const Float4 &color,
	const Float4 &shadowBorderColor4, float shadowBorderSize, 
	int drawStyle, bool doTransfer, float fontScale, bool isPointAsPunctuation)
{
	PX2_UNUSED(isPointAsPunctuation);

	SetHorInterval(space[0]);
	SetLineInterval(space[1]);

	if (text.empty())
	{
		mDrawRects[0] = FontDrawRect();
		mShowNum = 1;
		RenderText(mesh, 0.0f);

		return;
	}

	mShowNum = 0;

	const char *p = text.c_str();
	CharCtrlCode ctrlCode;
	int ctrlCodeSize = 0;
	
	Rectf rec = rect;
	float curX = rec.Left;
	float curY = rec.Top - mCharHeight*fontScale; // 第一行不需要加上mLineInterval

	Float4 curColor = color;

	int showCharNum = 0;
	bool blink = false;
	int blinkModel = BLINKMODEL_SMOOTH;

	bool doWhile = true;
	while (doWhile)
	{
		ctrlCodeSize = mCharCodingObj->GetControlCode(p, ctrlCode, doTransfer);
		p += ctrlCodeSize;

		if (CCC_ENDOFSTRING == ctrlCode)
			break;

		switch (ctrlCode)
		{
		case CCC_NEWLINE:
			{
				curX = rec.Left;
				curY -= (mCharHeight + mLineInterval) * fontScale;
			}
			break;
		case CCC_CHARACTER:
			{
				float charWidth = 0.0f;
				float charHeight = 0.0f;
				Rectf rectUV;
				float xx1 = 0.0f; // 左
				float xx2 = 0.0f; // 右
				float yy1 = 0.0f; // 下
				float yy2 = 0.0f; // 上
				float uOffset1 = 0.0f;
				float uOffset2 = 0.0f;
				float vOffset1 = 0.0f;
				float vOffset2 = 0.0f;

				//if (autoWrap)
				//{
				//	float dis = GetMinDisOfAWord(p, doTransfer, isPointAsPunctuation, fontScale);
				//	if (curX+dis > rec.Right)
				//	{
				//		if (rec.Left != curX)
				//		{ // 中途遇到一个超长单词,换行
				//			curX = rec.Left;
				//			curY -= (mCharHeight + mLineInterval) * fontScale;

				//			int numSpace = mCharCodingObj->JumpOverSpaces(p);
				//			if (numSpace > 0)
				//			{
				//				p += numSpace;
				//				continue;
				//			}
				//		}
				//	}
				//}

				// 取一个字符
				unsigned char cc[8];
				int charSize = mCharCodingObj->GetAChar(p, cc);

				// 取字符的宽，高
				GetCharExtent(cc, charWidth, charHeight, fontScale);

				if (autoWrap)
				{
					if (curX + charWidth > rect.Right)
					{
						curX = rec.Left;
						curY -= (mCharHeight + mLineInterval) * fontScale;
					}

					xx1 = curX;
					xx2 = curX + charWidth;
				}
				else
				{
					xx1 = curX + offsetX;
					xx2 = curX + charWidth + offsetX;
				}
				yy1 = curY + offsetY;
				yy2 = curY + offsetY + charHeight;

				// 超出矩形区域则不显示，判断下一个字符
				if (yy1>=rec.Top || yy2<=rec.Bottom ||
					(!autoWrap && (xx1>=rec.Right || xx2<=rec.Left)))
				{
					p += charSize;
					curX += charWidth;
					continue;
				}

				// 按矩形区域裁剪字符多边形，同时计算纹理坐标偏移值
				float height = yy2 - yy1;
				if (yy1 < rec.Bottom)
				{
					vOffset1 = (rect.Bottom - yy1)/height;
					yy1 = rec.Bottom;
				}

				if (yy2 > rec.Top)
				{
					vOffset2 = (yy2 - rec.Top)/height;
					yy2 = rec.Top;
				}

				if (!autoWrap)
				{
					float width = xx2 - xx1;

					if (xx1 < rec.Left)
					{
						uOffset1 = (rec.Left - xx1)/width;
						xx1 = rec.Left;
					}
					if (xx2 > rec.Right)
					{
						uOffset2 = (xx2 - rec.Right)/width;
						xx2 = rec.Right;
					}
				}

				// 取字符的贴图数据
				Texture2D *tex = TexUVMaping(cc, rectUV);
				mTexture = tex;

				if (mTexture == tex)
				{
					// 对纹理坐标进行调整
					float uWidth = rectUV.Width();
					float vHeight = rectUV.Height();
					rectUV.Left += uOffset1 * uWidth;
					rectUV.Right -= uOffset2 * uWidth;
					rectUV.Top -= vOffset2 * vHeight;
					rectUV.Bottom += vOffset1 * vHeight;

					if (drawStyle == FD_SHADOW)
					{
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 + shadowBorderSize, yy1 - shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
					}
					if (drawStyle == FD_BORDER)
					{
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 - shadowBorderSize, yy1, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 + shadowBorderSize, yy1, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1, yy1 - shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1, yy1 + shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;

						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 - shadowBorderSize, yy1 - shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 - shadowBorderSize, yy1 + shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 + shadowBorderSize, yy1 - shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
						SetupCharPoly(&mDrawRects[0]+mShowNum, 
							xx1 + shadowBorderSize, yy1 + shadowBorderSize, xx2 - xx1, yy2 - yy1, rectUV, shadowBorderColor4);
						mShowNum++;
					}
					SetupCharPoly(&mDrawRects[0]+mShowNum, 
						xx1, yy1, xx2-xx1, yy2-yy1, rectUV, curColor);
					mShowNum++;
					showCharNum++;
					p += charSize;
					curX += charWidth + mHorInterval*fontScale;
				}

				if (showCharNum>=mCharNumMaxCache || mShowNum>=FONT_MAXRECTNUM 
					|| mTexture!=tex)
				{
					assertion(false, "Fonts, numLarge");
					showCharNum = 0;
				}
			}
			break;
		case CCC_TRANSFER:
			{
				switch (*p)
				{
				case 'R':
					curColor = Float4::RED;
					p += 1;
					break;
				case 'G':
					curColor = Float4::GREEN;
					p += 1;
					break;
				case 'B':
					curColor = Float4::BLUE;
					p += 1;
					break;
				case 'K':
					curColor = Float4::BLACK;
					p += 1;
					break;
				case 'Y':
					curColor = Float4::YELLOW;
					p += 1;
					break;
				case 'W':
					curColor = Float4::WHITE;
					p += 1;
					break;
				case 'n':
					curColor = color;
					p += 1;
					blink = false;
					break;
				case 'b':
					blink = true;
					if (*(p+1) == '1')
					{
						blinkModel = BLINKMODEL_SMOOTH;
						p += 2;
					}
					else if (*(p+1) == '2')
					{
						blinkModel = BLINKMODEL_SUDDEN;
						p += 2;
					}
					else
					{
						blinkModel = BLINKMODEL_SMOOTH;
						p += 1;
					}
					break;
				default:
					break;
				}

				if (blink)
				{
					if (BLINKMODEL_SMOOTH == blinkModel)
					{
						int microSec = (int)Time::GetTimeInMicroseconds();
						int msNum = microSec % BLINK_CIRCLE;
						if (msNum >= BLINK_HALFCIRCLE)
							msNum = BLINK_CIRCLE - msNum - 1;
						msNum = msNum*320/BLINK_HALFCIRCLE;
						if (msNum > 255)
							msNum = 255;
					}
					else
					{
						int microSec = (int)Time::GetTimeInMicroseconds();
						if ((microSec/BLINK_HALFCIRCLE) % 2)
							curColor = Float4::ZERO;
					}
				}
			}
			break;
		default:
			break;
		} // switch (ctrlCodeSize)
	} // whild (true)

	RenderText(mesh);
}
//----------------------------------------------------------------------------
void Font::RenderText (TriMesh *mesh, float depth)
{
	if (mShowNum<=0)
		return;
	
	VertexFormat *vFormat = mesh->GetVertexFormat();
	VertexBuffer *vBuffer = mesh->GetVertexBuffer();
	IndexBuffer *iBuffer = mesh->GetIndexBuffer();
	if (!vBuffer)
	{
		vBuffer = new0 VertexBuffer(4*mShowNum, vFormat->GetStride(), 
			Buffer::BU_DYNAMIC);
		iBuffer = new0 IndexBuffer(6*mShowNum, 2, Buffer::BU_DYNAMIC);
		unsigned short *indices = (unsigned short*)iBuffer->GetData();
		for (int i=0; i<mShowNum; i++)
		{
			unsigned short v0 = (unsigned short)(i*4 + 0);
			unsigned short v1 = (unsigned short)(i*4 + 1);
			unsigned short v2 = (unsigned short)(i*4 + 2);
			unsigned short v3 = (unsigned short)(i*4 + 3);
			*indices++ = v0;
			*indices++ = v1;
			*indices++ = v2;
			*indices++ = v0;
			*indices++ = v2;
			*indices++ = v3;
		}

		mesh->SetVertexBuffer(vBuffer);
		mesh->SetIndexBuffer(iBuffer);
	}
	else
	{
		int vertexNum = vBuffer->GetNumElements();
		if (vertexNum < 4*mShowNum)
		{
			vBuffer = new0 VertexBuffer(4*mShowNum, vFormat->GetStride(), 
				Buffer::BU_DYNAMIC);
			iBuffer = new0 IndexBuffer(6*mShowNum, 2, Buffer::BU_DYNAMIC);
			unsigned short *indices = (unsigned short*)iBuffer->GetData();
			for (int i=0; i<mShowNum; i++)
			{
				unsigned short v0 = (unsigned short)(i*4 + 0);
				unsigned short v1 = (unsigned short)(i*4 + 1);
				unsigned short v2 = (unsigned short)(i*4 + 2);
				unsigned short v3 = (unsigned short)(i*4 + 3);
				*indices++ = v0;
				*indices++ = v1;
				*indices++ = v2;
				*indices++ = v0;
				*indices++ = v2;
				*indices++ = v3;
			}

			mesh->SetVertexBuffer(vBuffer);
			mesh->SetIndexBuffer(iBuffer);
		}
	}

	VertexBufferAccessor vba(vFormat, vBuffer);

	for (int i=0; i<mShowNum; i++)
	{
		FontDrawRect &rect = mDrawRects[i];

		vba.Position<Float3>(4*i)	= Float3(rect.Rect.Left, depth, rect.Rect.Bottom);
		vba.Color<Float4>(0, 4 * i) = rect.Color;
		vba.TCoord<Float2>(0, 4*i)	= Float2(rect.RectUV.Left, rect.RectUV.Bottom);

		vba.Position<Float3>(4*i+1) = Float3(rect.Rect.Right, depth, rect.Rect.Bottom);
		vba.Color<Float4>(0, 4 * i + 1) = rect.Color;
		vba.TCoord<Float2>(0, 4*i+1)= Float2(rect.RectUV.Right, rect.RectUV.Bottom);

		vba.Position<Float3>(4*i+2) = Float3(rect.Rect.Right, depth, rect.Rect.Top);
		vba.Color<Float4>(0, 4 * i + 2) = rect.Color;
		vba.TCoord<Float2>(0, 4*i+2)= Float2(rect.RectUV.Right, rect.RectUV.Top);

		vba.Position<Float3>(4*i+3) = Float3(rect.Rect.Left, depth, rect.Rect.Top);
		vba.Color<Float4>(0, 4 * i + 3) = rect.Color;
		vba.TCoord<Float2>(0, 4*i+3)= Float2(rect.RectUV.Left, rect.RectUV.Top);
	}
	vBuffer->SetNumElements(4*mShowNum);
	iBuffer->SetNumElements(6*mShowNum);
	mesh->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);

	mShowNum = 0;
}
//----------------------------------------------------------------------------
float Font::GetMinDisOfAWord(const char *text, bool doTransfer,
	bool isPointAsPunctuation, float fontScale)
{
	if (!text)
		return 0.0f;

	CharCtrlCode ctrlCode;
	mCharCodingObj->GetControlCode(text, ctrlCode, doTransfer);
	if (CCC_CHARACTER != ctrlCode)
		return 0.0f;	

	const unsigned char *p = (unsigned char *)text;
	float result = 0.0f;
	float charWidth = 0.0f;
	float charHeight = 0.0f;

	unsigned char cc[4];
	int charSize = 0;
	charSize = mCharCodingObj->GetAChar((const char*)p, cc);

	if (mCharCodingObj->IsEnglish(cc)) // 英文，不包括空格
	{
		bool cont = true;
		while (cont)
		{
			charSize = mCharCodingObj->GetAChar((const char*)p, cc);
			if (mCharCodingObj->IsEnglish(cc))
			{
				GetCharExtent(cc, charWidth, charHeight, fontScale);
				result += charWidth;
				p += charSize;

				if (mCharCodingObj->IsPunctuation(cc))
				{
					if (mCharCodingObj->IsPoint(cc) && !isPointAsPunctuation)
					{
						/*_*/
					}
					else
					{
						return result;
					}
				}
			}
			else
			{
				return result;
			}
		}
	}
	else // 中文
	{
		GetCharExtent(cc, charWidth, charHeight, fontScale);
		return charWidth;
	}

	return 0;
}
//----------------------------------------------------------------------------