#pragma once

#ifndef __GRAY_H__
#define __GRAY_H__
#define ZOOMH 32
#define ZOOMW 20

#include "math.h"
#include <vector>
using std::vector;

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//#define RADIAN(angle) ((angle)*PI/180.0) 
static char Fonts[37] ={'0','1','2','3','4','5','6','7','8','9','A','B',
'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','Z'};
static char* HFonts[37] = {"云","京","使","冀","吉","学","宁","川","新","晋","桂","沪","津","浙","渝","港",
"湘","澳","琼","甘","皖","粤","苏","蒙","藏","警","豫","贵","赣","辽","鄂","闽","陕","青","领","鲁","黑"};
CString getFileName(CString filepath);
class CImageRecognize
{
public:

	// 构造函数
	CImageRecognize();
	// Copy构造函数
	CImageRecognize(CImageRecognize& gray);
	// 重载“=”操作符来创建新的对象
	void operator = (CImageRecognize& gray); //图像赋值

	BOOL operator == (CImageRecognize& gray); //判断2幅图像是否相同
	CImageRecognize operator & (CImageRecognize& gray); //图像按位与
	CImageRecognize operator | (CImageRecognize& gray); //图像按位或
	CImageRecognize operator + (CImageRecognize gray); //图像相加
	CImageRecognize operator - (CImageRecognize& gray); //图像减法
	CImageRecognize operator ! (); //图像反色


	// 析构函数
	virtual ~CImageRecognize();

public:
	struct CharacterRect
	{
		int mLeft;
		int mTop;
		int mRight;
		int mBottom;	
		BYTE mColor;
		operator CRect* () const{ return new CRect(mLeft,mTop,mRight,mBottom);}
		CharacterRect():mTop(0),mBottom(0),mLeft(0),mRight(0),mColor(0){};
		CharacterRect(int nTop,int nBottom,int nLeft,int nRight,int nColor)
			:mTop(nTop),mBottom(nBottom),mLeft(nLeft),mRight(nRight),mColor(nColor){};
		CharacterRect(CRect& rc):mTop(rc.top),mBottom(rc.bottom),mLeft(rc.left),mRight(rc.right){};
	};
	struct CharPosition
	{
		int startX;
		int startY;
		int endX;
		int endY;
	}position[7];
public:	
	// 判断位图是否有效
	BOOL IsValidate() { return m_pBMIH != NULL; }

	// 将所有像素的值初始化为color
	void InitPixels(BYTE color);

	// 从文件加载位图
	BOOL AttachFromFile(LPCTSTR lpcPathName);
	BOOL AttachFromFile(CFile &file);
	// 将位图保存到文件
	BOOL SaveToFile(LPCTSTR lpcPathName);
	BOOL SaveToFile(CFile &file);

	// 在DC上绘制位图
	BOOL Draw(CDC* pDC);
	BOOL Draw(CDC* pDC, CRect rt);

	// 设置像素的值
	void SetPixel(int x, int y, COLORREF color);
	// 获取像素的值
	COLORREF GetPixel(int x, int y);
	// 获取灰度值
	BYTE GetGray(int x, int y);


	// 获取一行的字节数
	int GetWidthByte();
	// 获取一行的像素数
	int GetWidthPixel();
	// 获取高度
	int GetHeight();

	//改变位图的尺寸
	void ImResize(int nHeight, int nWidth);


	// 判断是否是二值图像
	BOOL IsBinaryImg();
	// 判断是否是索引图像
	BOOL IsIndexedImg();
	// 256色索引图像转灰度图像
	bool Index2Gray();
	LPVOID GetColorTable(){return m_lpvColorTable;}
	int GetColorTableEntriesNum(){return m_nColorTableEntries;}
	//////////////////////////////////////////////////////////////////////////
	//处理对比度和亮度的（-1,255）时为反色
	bool LinarTrans(int nFa, int nFb); //**没用到
	//二值化处理 阈值的选择是个关键
	bool ThreshTrans(BYTE byColor);
	bool ThreshTrans(long lWidth, long lHeight);
	//边缘检测，其实还可以做很多，比如柔化等等
	//sobel算子{{-1,-2,-1},{0,0,0},{1,2,1}} {{1,0,-1},{2,0,-2},{1,0,-1}}
	//{{0,-1,0},{-1,5,-1},{0,-1,0}}
	bool Template(const float se[3][3],float sum);
	//两图取最大值（边缘检测时可以用（sobel算子））
	bool MaxTrans(CImageRecognize *img); //**没用到
	//失败的角点检测
	bool Corner();  //**没用到
	//下边三个函数检测连通分量
	bool ConnectionRegin(int nMin);
	bool ConnectionReginSet(CImageRecognize *img,int x,int y,int nConRgn,int &sum);
	//bool ClearRegin(BYTE byColor);
	bool ClearRegin(BYTE byColor,CRect* rc = NULL);
	//**
	int ConnectionRegin(int nMin,int nMinWidth,int nMinHeight,BYTE byColor,vector<CharacterRect>& vcList,BYTE& byColorFlag,CRect* rc=  NULL);
	bool ConnectionReginSet(int x,int y,int& sum,int& nTop,int& nBottom,int& nLeft,int& nRight,BYTE byColor,BYTE nConRgn,CRect* rc = NULL);
	//void GreyPlus();
	void VectRemove(int position,vector<CharacterRect>& vec);
	void VectRemove(int position,vector<float>& vec);
	CString FindCharacter(CImageRecognize& img,int nMin,int nMinWidth,int nMinHeight,BYTE byColor,vector<CharacterRect>& vcList);
	void CharacterRectSort(vector<CharacterRect>& vcList,int sortMode = 0);
	void DeleteFrame(int nMin,int nMinWidth,int nMinHeight,BYTE& byColorFlag,vector<CharacterRect>& vcList,int maxWidth,int iCount = 3);
	void CleanUpRect(vector<CharacterRect>& vcList);
	float AverageWidth(vector<float>& wid);
	float AverageSpace(vector<float>& spc); 
	int InsureCharacter(vector<CharacterRect>& vcListFrom,vector<CharacterRect>& vcListTo,int iStart,bool bToEnd,float averageSpace,float averageWidth,vector<float>& spc,vector<float>& wid);
	void AverageTopAndBottom(vector<CharacterRect>& vcList,float& averageTop,float& averageBottom); 
	void DrawCharRect(vector<CharacterRect>& vcListLast,CImageRecognize& img);
	//去边
	bool ExceptBorder(BYTE bColor = 255,CRect* rc = NULL);
	//**
	//bool ExceptBorder();
	bool DetectLine(int min,int max,CImageRecognize * toImg);
	bool DetectLine_area(int min,int max,int high,int low,CImageRecognize* toImg);
	bool ModelTo(CImageRecognize* img);
	bool DetectLineX(int min,int max,int ax,int ay, int bx, int by);
	bool DetectLineY(int min,int max,int ax,int ay, int bx, int by);
	// 区域跳点检测
	bool AreaDetect(int xmin, int xmax, int ymin, int ymax, CImageRecognize* toImg);
	// 画矩形
	bool DrawRecTangle(int ax, int ay, int bx, int by, COLORREF color, CImageRecognize* toImg);
	void ChangeDIBToGray(void);
	bool GetClosestLine(int &high, int &low);
	bool GetPart(CImageRecognize &cutImg, int ax, int ay, int bx, int by);
	bool Resize(int w, int h, BYTE byColor);
	// 判断是否为标记线
	bool IsMarkLine(int height)
	{
		//if(GetPixel(0,height) == RGB(255,0,0) && GetPixel(GetWidthPixel(),height) == RGB(255,0,0) && GetPixel(GetWidthPixel()/2,height))
		if(GetPixel(0,height) == RGB(255,0,0))
			return true;
		else
			return false;
	}
	bool DetectLine_areaX(int min, int max, int& left, int& right, int high, int low, CImageRecognize* toImg);
	bool JumpPointDetect(CImageRecognize* imgDetected);
	int GetBetterValue(int changeTime,int yPos,int height);
	void GreyPlus();
	bool DetectLine_areaXX(CImageRecognize* toImg, int ax, int ay, int bx, int by);
	static CString RecognizePlate(CString filePath);
	void GetRect(CRect* rect);

	//////////////////////////////////////////////////////////////////////////

private:
	void CleanUp();	
	//////////////////////////////////////////////////////////////////////////
public:
	// 文件数据
	BITMAPINFOHEADER *m_pBMIH;
	LPBYTE  *m_lpData;
protected:
	//颜色表数据
	int m_nColorTableEntries;
	LPVOID m_lpvColorTable;
public:
	int module[ZOOMH][ZOOMW];
	//int sample[ZOOMH][ZOOMW];
public:
	bool GetPlateLocation(int min, int max, CImageRecognize* redImg, CImageRecognize* targetImg, int& high, int& low, int& left, int& right);
public:
	bool DoubleLinarStretch(CRect rectSrc, CRect rectDes);
public:
	CString GetCharFromPic(void);

	//**方浩
	int isbd0q (int sample[ZOOMH][ZOOMW]);
	CString CharRecognize(int sample[ZOOMH][ZOOMW], int num);
	void LoadMod(int count, int num);
	void CharToStandard(int standard[ZOOMH][ZOOMW], LPBYTE* resorce, int nImageHeight, int nImageWidth, int num);
	//int(*CharToStandard(BYTE** Simple,int h,int w, int nSize))[32];
	void CutEdge(int nImageHeight, int nImageWidth, int num, LPBYTE* Simple);
	void CutEdge(int nImageHeight, int nImageWidth, int num);
};



/////////////////////////////////// inline functions ///////////////////////////////
/**************************************************
inline int CImageRecognize::GetWidthByte()

功能：
返回CImageRecognize实例中的图像每行占用的字节数
限制：
无

参数：
无
返回值：
int类型，返回图像每行占用的字节数
***************************************************/
inline int CImageRecognize::GetWidthByte()
{
	return WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
}

/**************************************************
inline int CImageRecognize::GetWidthPixel()

功能：
返回CImageRecognize实例中的图像每行的像素数目，即横向分辨率或宽度
限制：
无

参数：
无
返回值：
int类型，返回图像每行的像素数目
***************************************************/
inline int CImageRecognize::GetWidthPixel()
{
	return m_pBMIH->biWidth;
}

/**************************************************
inline int CImageRecognize::GetHeight()

功能：
返回CImageRecognize实例中的图像每列的像素数目，即纵向分辨率或高度
限制：
无

参数：
无
返回值：
int类型，返回图像每列的像素数目
***************************************************/
inline int CImageRecognize::GetHeight()
{
	return m_pBMIH->biHeight;
}

/**************************************************
inline BYTE CImageRecognize::GetGray(int x, int y)

功能：
返回指定坐标位置像素的灰度值

参数：
int x, int y
指定的像素横、纵坐标值
返回值：
给定像素位置的灰度值
***************************************************/
inline BYTE CImageRecognize::GetGray(int x, int y)
{
	COLORREF ref = GetPixel(x, y);
	BYTE r, g, b, byte;
	r = GetRValue(ref);
	g = GetGValue(ref);
	b = GetBValue(ref);

	if(r == g && r == b)
		return r;

	double dGray = (0.30*r + 0.59*g + 0.11*b);

	// 灰度化
	byte =  (int)dGray;

	return byte;
}

/**************************************************
inline COLORREF CImageRecognize::GetPixel(int x, int y)

功能：
返回指定坐标位置像素的颜色值

参数：
int x, int y
指定像素位置的坐标
返回值：
COLERREF类型，返回用RGB形式表示的指定位置的颜色值
***************************************************/
inline COLORREF CImageRecognize::GetPixel(int x, int y)
{
	if(m_pBMIH->biBitCount == 8)		// 256色图
	{
		BYTE byte = m_lpData[m_pBMIH->biHeight - y - 1][x];
		return RGB(byte, byte, byte);
	}
	else if(m_pBMIH->biBitCount == 1)	// 单色图
	{
		BYTE ret = (1<<(7-x%8) & m_lpData[m_pBMIH->biHeight - y - 1][x/8]);

		// 令0代表黑
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			ret = !ret;

		if(ret)
			return RGB(255, 255, 255);		// 白色
		else 
			return RGB(0, 0, 0);			// 黑色
	}
	else if(m_pBMIH->biBitCount == 24 || m_pBMIH->biBitCount == 32)	// 真彩图
	{
		COLORREF color = RGB(m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 2], 
			m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 1],
			m_lpData[m_pBMIH->biHeight - y - 1][x*3]);
		return color;
	}
	else
	{
		throw "not support now";
		return 0;
	}
}

inline BOOL CImageRecognize::IsBinaryImg()
{
	int i,j;

	for(i = 0; i < m_pBMIH->biHeight; i++)
	{
		for(j = 0; j < m_pBMIH->biWidth; j++)
		{
			if( (GetGray(j, i) != 0) && (GetGray(j, i) != 255) )//存在0和255之外的灰度值
				return FALSE;
		}//for j
	}//for i

	return TRUE;
}

inline BOOL CImageRecognize::IsIndexedImg()
{
	if ((m_lpvColorTable != NULL)&&(m_nColorTableEntries!=0)) {
		return true;
	}
	else {
		return false;
	}
}
#endif // __GRAY_H__

struct DetectArea
{
	int left;
	int right;
	int high;
	int low;
	int changeNum;
	int weigh;
};