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
static char* HFonts[37] = {"��","��","ʹ","��","��","ѧ","��","��","��","��","��","��","��","��","��","��",
"��","��","��","��","��","��","��","��","��","��","ԥ","��","��","��","��","��","��","��","��","³","��"};
CString getFileName(CString filepath);
class CImageRecognize
{
public:

	// ���캯��
	CImageRecognize();
	// Copy���캯��
	CImageRecognize(CImageRecognize& gray);
	// ���ء�=���������������µĶ���
	void operator = (CImageRecognize& gray); //ͼ��ֵ

	BOOL operator == (CImageRecognize& gray); //�ж�2��ͼ���Ƿ���ͬ
	CImageRecognize operator & (CImageRecognize& gray); //ͼ��λ��
	CImageRecognize operator | (CImageRecognize& gray); //ͼ��λ��
	CImageRecognize operator + (CImageRecognize gray); //ͼ�����
	CImageRecognize operator - (CImageRecognize& gray); //ͼ�����
	CImageRecognize operator ! (); //ͼ��ɫ


	// ��������
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
	// �ж�λͼ�Ƿ���Ч
	BOOL IsValidate() { return m_pBMIH != NULL; }

	// ���������ص�ֵ��ʼ��Ϊcolor
	void InitPixels(BYTE color);

	// ���ļ�����λͼ
	BOOL AttachFromFile(LPCTSTR lpcPathName);
	BOOL AttachFromFile(CFile &file);
	// ��λͼ���浽�ļ�
	BOOL SaveToFile(LPCTSTR lpcPathName);
	BOOL SaveToFile(CFile &file);

	// ��DC�ϻ���λͼ
	BOOL Draw(CDC* pDC);
	BOOL Draw(CDC* pDC, CRect rt);

	// �������ص�ֵ
	void SetPixel(int x, int y, COLORREF color);
	// ��ȡ���ص�ֵ
	COLORREF GetPixel(int x, int y);
	// ��ȡ�Ҷ�ֵ
	BYTE GetGray(int x, int y);


	// ��ȡһ�е��ֽ���
	int GetWidthByte();
	// ��ȡһ�е�������
	int GetWidthPixel();
	// ��ȡ�߶�
	int GetHeight();

	//�ı�λͼ�ĳߴ�
	void ImResize(int nHeight, int nWidth);


	// �ж��Ƿ��Ƕ�ֵͼ��
	BOOL IsBinaryImg();
	// �ж��Ƿ�������ͼ��
	BOOL IsIndexedImg();
	// 256ɫ����ͼ��ת�Ҷ�ͼ��
	bool Index2Gray();
	LPVOID GetColorTable(){return m_lpvColorTable;}
	int GetColorTableEntriesNum(){return m_nColorTableEntries;}
	//////////////////////////////////////////////////////////////////////////
	//����ԱȶȺ����ȵģ�-1,255��ʱΪ��ɫ
	bool LinarTrans(int nFa, int nFb); //**û�õ�
	//��ֵ������ ��ֵ��ѡ���Ǹ��ؼ�
	bool ThreshTrans(BYTE byColor);
	bool ThreshTrans(long lWidth, long lHeight);
	//��Ե��⣬��ʵ���������ܶ࣬�����ữ�ȵ�
	//sobel����{{-1,-2,-1},{0,0,0},{1,2,1}} {{1,0,-1},{2,0,-2},{1,0,-1}}
	//{{0,-1,0},{-1,5,-1},{0,-1,0}}
	bool Template(const float se[3][3],float sum);
	//��ͼȡ���ֵ����Ե���ʱ�����ã�sobel���ӣ���
	bool MaxTrans(CImageRecognize *img); //**û�õ�
	//ʧ�ܵĽǵ���
	bool Corner();  //**û�õ�
	//�±��������������ͨ����
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
	//ȥ��
	bool ExceptBorder(BYTE bColor = 255,CRect* rc = NULL);
	//**
	//bool ExceptBorder();
	bool DetectLine(int min,int max,CImageRecognize * toImg);
	bool DetectLine_area(int min,int max,int high,int low,CImageRecognize* toImg);
	bool ModelTo(CImageRecognize* img);
	bool DetectLineX(int min,int max,int ax,int ay, int bx, int by);
	bool DetectLineY(int min,int max,int ax,int ay, int bx, int by);
	// ����������
	bool AreaDetect(int xmin, int xmax, int ymin, int ymax, CImageRecognize* toImg);
	// ������
	bool DrawRecTangle(int ax, int ay, int bx, int by, COLORREF color, CImageRecognize* toImg);
	void ChangeDIBToGray(void);
	bool GetClosestLine(int &high, int &low);
	bool GetPart(CImageRecognize &cutImg, int ax, int ay, int bx, int by);
	bool Resize(int w, int h, BYTE byColor);
	// �ж��Ƿ�Ϊ�����
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
	// �ļ�����
	BITMAPINFOHEADER *m_pBMIH;
	LPBYTE  *m_lpData;
protected:
	//��ɫ������
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

	//**����
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

���ܣ�
����CImageRecognizeʵ���е�ͼ��ÿ��ռ�õ��ֽ���
���ƣ�
��

������
��
����ֵ��
int���ͣ�����ͼ��ÿ��ռ�õ��ֽ���
***************************************************/
inline int CImageRecognize::GetWidthByte()
{
	return WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
}

/**************************************************
inline int CImageRecognize::GetWidthPixel()

���ܣ�
����CImageRecognizeʵ���е�ͼ��ÿ�е�������Ŀ��������ֱ��ʻ���
���ƣ�
��

������
��
����ֵ��
int���ͣ�����ͼ��ÿ�е�������Ŀ
***************************************************/
inline int CImageRecognize::GetWidthPixel()
{
	return m_pBMIH->biWidth;
}

/**************************************************
inline int CImageRecognize::GetHeight()

���ܣ�
����CImageRecognizeʵ���е�ͼ��ÿ�е�������Ŀ��������ֱ��ʻ�߶�
���ƣ�
��

������
��
����ֵ��
int���ͣ�����ͼ��ÿ�е�������Ŀ
***************************************************/
inline int CImageRecognize::GetHeight()
{
	return m_pBMIH->biHeight;
}

/**************************************************
inline BYTE CImageRecognize::GetGray(int x, int y)

���ܣ�
����ָ������λ�����صĻҶ�ֵ

������
int x, int y
ָ�������غᡢ������ֵ
����ֵ��
��������λ�õĻҶ�ֵ
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

	// �ҶȻ�
	byte =  (int)dGray;

	return byte;
}

/**************************************************
inline COLORREF CImageRecognize::GetPixel(int x, int y)

���ܣ�
����ָ������λ�����ص���ɫֵ

������
int x, int y
ָ������λ�õ�����
����ֵ��
COLERREF���ͣ�������RGB��ʽ��ʾ��ָ��λ�õ���ɫֵ
***************************************************/
inline COLORREF CImageRecognize::GetPixel(int x, int y)
{
	if(m_pBMIH->biBitCount == 8)		// 256ɫͼ
	{
		BYTE byte = m_lpData[m_pBMIH->biHeight - y - 1][x];
		return RGB(byte, byte, byte);
	}
	else if(m_pBMIH->biBitCount == 1)	// ��ɫͼ
	{
		BYTE ret = (1<<(7-x%8) & m_lpData[m_pBMIH->biHeight - y - 1][x/8]);

		// ��0�����
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			ret = !ret;

		if(ret)
			return RGB(255, 255, 255);		// ��ɫ
		else 
			return RGB(0, 0, 0);			// ��ɫ
	}
	else if(m_pBMIH->biBitCount == 24 || m_pBMIH->biBitCount == 32)	// ���ͼ
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
			if( (GetGray(j, i) != 0) && (GetGray(j, i) != 255) )//����0��255֮��ĻҶ�ֵ
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