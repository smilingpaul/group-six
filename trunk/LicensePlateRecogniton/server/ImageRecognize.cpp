#include "StdAfx.h"
#include "ImageRecognize.h"


//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageRecognize.h"
#include <vector>
#include <math.h>
#include <CString>
#include <fstream>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CImageRecognize::CImageRecognize()
{
	m_pBMIH = NULL;
	m_lpvColorTable = NULL;

	m_lpData = NULL;
}

BOOL CImageRecognize::operator == (CImageRecognize& gray)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	if(nHeight != gray.GetHeight())
		return false;

	if(nWidth != gray.GetWidthPixel())
		return false;


	for(int i=0; i<nHeight; i++)
	{
		for(int j=0; j<nWidth; j++)
		{
			if( GetGray(j, i) != gray.GetGray(j, i) )
				return false;
		}
	}

	return true;
}

CImageRecognize CImageRecognize::operator & (CImageRecognize& gray)
{
	CImageRecognize grayRet = *this;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	if(nHeight != gray.GetHeight())
	{
		AfxMessageBox(L"计算或运算的两幅图像必须具有相同的大小！运算失败，返回原图像。");
		return grayRet;
	}

	if(nWidth != gray.GetWidthPixel())
	{
		AfxMessageBox(L"计算或运算的两幅图像必须具有相同的大小！运算失败，返回原图像。");
		return grayRet;
	}


	// 两幅图像的与
	for(int i=0; i<nHeight; i++)
	{
		for(int j=0; j<nWidth; j++)
		{
			if(gray.GetGray(j, i) == 255)
				grayRet.SetPixel(j, i, RGB(255, 255, 255));
		}
	}

	return grayRet;	
}

CImageRecognize CImageRecognize::operator | (CImageRecognize& gray)
{
	CImageRecognize grayRet = *this;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	if(nHeight != gray.GetHeight())
	{
		AfxMessageBox(L"计算或运算的两幅图像必须具有相同的大小！运算失败，返回原图像。");
		return grayRet;
	}

	if(nWidth != gray.GetWidthPixel())
	{
		AfxMessageBox(L"计算或运算的两幅图像必须具有相同的大小！运算失败，返回原图像。");
		return grayRet;
	}


	// 两幅图像的或
	for(int i=0; i<nHeight; i++)
	{
		for(int j=0; j<nWidth; j++)
		{
			if(gray.GetGray(j, i) == 0)
				grayRet.SetPixel(j, i, RGB(0, 0, 0));
		}
	}

	return grayRet;
}

CImageRecognize CImageRecognize::operator ! ()
{
	CImageRecognize grayRet = *this;
	grayRet.InitPixels(255); //结果图像置白

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i,j;

	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			int pixel = 255 - GetGray(j, i);
			grayRet.SetPixel(j, i, RGB(pixel, pixel, pixel));
		}
	}

	return grayRet;
}

/*******************
CImageRecognize CImageRecognize::operator + (CImageRecognize gray)

功能：图像按位加

参数：
CImageRecognize 对象

返回值:
CImageRecognize 相加后的 CImageRecognize 类图像对象
*******************/
CImageRecognize CImageRecognize::operator + (CImageRecognize gray)
{
	CImageRecognize grayRet; //返回图像
	grayRet = *this;

	//取得图像的高和宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//循环变量

	//不能在CImageRecognize类对象中直接进行像素相加，因为相加的结果可能超过255
	vector< vector<int> > GrayMat;//求和后暂存图像的灰度点阵
	vector<int> vecRow(nWidth, 0); //GrayMat中的一行（初始化为0）
	for(i=0; i<nHeight; i++)
	{
		GrayMat.push_back(vecRow);
	}

	//最大、最小灰度和值
	int nMax = 0;
	int nMin = 255*2; 

	//逐行扫描图像
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			//按位相加
			GrayMat[i][j] = GetGray(j, i) + gray.GetGray(j, i);

			//统计最大、最小值
			if( GrayMat[i][j] > nMax)
				nMax = GrayMat[i][j];
			if( GrayMat[i][j] < nMin)
				nMin = GrayMat[i][j];
		}// j
	}// i


	//将GrayMat的取值范围重新归一化到[0, 255]
	int nSpan = nMax - nMin;

	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			BYTE bt;
			if(nSpan > 0)
				bt = (GrayMat[i][j] - nMin)*255/nSpan;
			else if(GrayMat[i][j] <= 255)
				bt = GrayMat[i][j] ;
			else
				bt = 255;

			grayRet.SetPixel(j, i, RGB(bt, bt, bt));			


		}// for j
	}// for i



	return grayRet;
}

void CImageRecognize::operator = (CImageRecognize& gray)
{
	CleanUp();

	m_nColorTableEntries = gray.m_nColorTableEntries;


	if(gray.m_pBMIH != NULL)
	{
		m_pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4];
		memcpy(m_pBMIH, gray.m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

		if(m_nColorTableEntries != 0)
		{
			m_lpvColorTable = m_pBMIH + 1;
		}
	}

	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	m_lpData = new LPBYTE[m_pBMIH->biHeight];
	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memcpy(m_lpData[i], gray.m_lpData[i], nWidthBytes);
	}
}

CImageRecognize::CImageRecognize(CImageRecognize& gray)
{	
	m_pBMIH = NULL;
	m_lpvColorTable = NULL;

	m_nColorTableEntries = gray.m_nColorTableEntries;

	if(gray.m_pBMIH != NULL)
	{
		m_pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4];
		memcpy(m_pBMIH, gray.m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

		if(m_nColorTableEntries != 0)
		{
			m_lpvColorTable = m_pBMIH + 1;
		}
	}

	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	m_lpData = new LPBYTE[m_pBMIH->biHeight];
	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memcpy(m_lpData[i], gray.m_lpData[i], nWidthBytes);
	}
}

CImageRecognize::~CImageRecognize()
{
	CleanUp();
}

void CImageRecognize::CleanUp()
{	
	if(m_lpData != NULL)
	{
		int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
		for(int i=0; i<m_pBMIH->biHeight; i++)
		{
			delete[] m_lpData[i];
		}
		delete[] m_lpData;
	}

	if(m_pBMIH != NULL)
	{
		delete[] m_pBMIH;
		m_pBMIH = NULL;
	}
}

/**************************************************
void CImageRecognize::ImResize(int nHeight, int nWidth)

功能：
用给定的大小重新初始化CImageRecognize对象

限制：
CImageRecognize对象必须已经包含有效的图像数据，否则将出错

参数：
int nHeight
重新初始化成的宽度
int nWidth
重新初始化成的高度
返回值：
无
***************************************************/
void CImageRecognize::ImResize(int nHeight, int nWidth)
{
	int i; //循环变量
	//释放图像数据空间
	for(i=0; i<m_pBMIH->biHeight; i++)
	{
		delete[] m_lpData[i];
	}
	delete[] m_lpData;

	//更新信息头中的相应内容
	m_pBMIH->biHeight = nHeight; //更新高度
	m_pBMIH->biWidth = nWidth; //更新宽度

	//重新分配数据空间
	m_lpData = new LPBYTE [nHeight];
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	for(i=0; i<nHeight; i++)
	{
		m_lpData[i] = new BYTE [nWidthBytes];
	}
}

/**************************************************
void CImageRecognize::InitPixels(BYTE color)
功能：
用给定的颜色值初始化图像的所有像素

限制：
只能使用灰度值提供颜色值，即只能初始化为某种灰色
参数：
BYTE color：指定的用来初始化图像的灰度值
返回值：
无
***************************************************/
void CImageRecognize::InitPixels(BYTE color)
{
	//获得图像高、宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//行、列循环变量

	//逐行扫描图像，依次对每个像素设置color灰度
	if(m_lpData != NULL)
	{
		for(i=0; i<GetHeight(); i++)
		{
			for(j=0; j<GetWidthPixel(); j++)	
			{
				SetPixel(j, i, RGB(color, color, color));
			}//for j
		}//for i
	}
}


/**************************************************
BOOL CImageRecognize::AttachFromFile(LPCTSTR lpcPathName)

功能：
打开指定的图像文件并附加到CImageRecognize对象上
限制：
只能处理位图图像

参数：LPCTSTR lpcPathName
欲打开文件的完整路径
返回值：
BYTE类型，TRUE为成功，FALSE为失败
***************************************************/
BOOL CImageRecognize::AttachFromFile(LPCTSTR lpcPathName)
{
	// 使用CFile对象简化操作
	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead|CFile::shareDenyWrite))
		return FALSE;

	BOOL bSuc = AttachFromFile(file);

	file.Close();
	return bSuc;
}

/**************************************************
BOOL CImageRecognize::AttachFromFile(CFile &file)

功能：
打开指定的图像文件并附加到CImageRecognize对象上
限制：
只能处理位图图像

参数：
CFile &file
欲打开的CFile对象
返回值：
BYTE类型，TRUE为成功，FALSE为失败
***************************************************/

BOOL CImageRecognize::AttachFromFile(CFile &file)
{
	// 文件数据
	LPBYTE  *lpData;
	// 位图信息头
	BITMAPINFOHEADER *pBMIH;
	// 颜色表指针
	LPVOID lpvColorTable = NULL;
	// 颜色表颜色数目
	int nColorTableEntries;

	BITMAPFILEHEADER bmfHeader;

	// 读取文件头
	if(!file.Read(&bmfHeader, sizeof(bmfHeader)))
		return FALSE;


	// 检查开头两字节是否为BM
	if(bmfHeader.bfType != MAKEWORD('B', 'M'))
	{
		return FALSE;
	}

	// 读取信息头
	pBMIH = (BITMAPINFOHEADER*)new BYTE[bmfHeader.bfOffBits - sizeof(bmfHeader)];
	if(!file.Read(pBMIH, bmfHeader.bfOffBits - sizeof(bmfHeader)))
	{
		delete pBMIH;
		return FALSE;
	}

	// 定位到颜色表
	nColorTableEntries = 
		(bmfHeader.bfOffBits - sizeof(bmfHeader) - sizeof(BITMAPINFOHEADER))/sizeof(RGBQUAD);
	if(nColorTableEntries > 0)
	{
		lpvColorTable = pBMIH + 1;
	}

	pBMIH->biHeight = abs(pBMIH->biHeight);

	// 读取图像数据,WIDTHBYTES宏用于生成每行字节数
	int nWidthBytes = WIDTHBYTES((pBMIH->biWidth)*pBMIH->biBitCount);

	// 申请biHeight个长度为biWidthBytes的数组,用他们来保存位图数据
	lpData = new LPBYTE[(pBMIH->biHeight)];
	for(int i=0; i<(pBMIH->biHeight); i++)
	{
		lpData[i] = new BYTE[nWidthBytes];
		file.Read(lpData[i], nWidthBytes);

	}

	// 更新数据
	CleanUp();

	m_lpData = lpData;
	m_pBMIH = pBMIH;
	m_lpvColorTable = lpvColorTable;
	m_nColorTableEntries = nColorTableEntries;


	return TRUE;
}

BOOL CImageRecognize::SaveToFile(LPCTSTR lpcPathName)
{
	if(!IsValidate())
		return FALSE;

	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead|CFile::modeWrite|CFile::modeCreate))
	{
		return FALSE;
	}

	BOOL bSuc = SaveToFile(file);
	file.Close();

	return bSuc;
}

/**************************************************
BOOL CImageRecognize::SaveToFile(CFile &file)

功能：
把CImageRecognize实例中的图像数据保存到指定的图像文件
限制：
只能处理位图图像

参数：
CFile &file
欲保存到的CFile对象
返回值：
BYTE类型，TRUE为成功，FALSE为失败
***************************************************/
BOOL CImageRecognize::SaveToFile(CFile &file)
{	
	// 判断是否有效
	if(!IsValidate())
		return FALSE;	


	// 构建BITMAPFILEHEADER结构
	BITMAPFILEHEADER bmfHeader = { 0 };
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);


	bmfHeader.bfType = MAKEWORD('B', 'M');
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) 
		+ sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4;

	bmfHeader.bfSize = bmfHeader.bfOffBits + m_pBMIH->biHeight * nWidthBytes;

	// 向文件中写入数据
	file.Write(&bmfHeader, sizeof(bmfHeader));
	file.Write(m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		file.Write(m_lpData[i], nWidthBytes);
	}


	return TRUE;
}


/**************************************************
BOOL CImageRecognize::Draw(CDC* pDC)

功能：
在给定的设备上下文环境中将CImageRecognize对象中存储的图像绘制到屏幕上

限制：
无

参数：
CDC * pDC
指定的设备上下文环境的指针
返回值：
布尔类型，TRUE为成功，FALSE为失败
***************************************************/
BOOL CImageRecognize::Draw(CDC* pDC)
{
	if(m_pBMIH == NULL)
		return FALSE;

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{

		::SetDIBitsToDevice(*pDC, 0, 0, m_pBMIH->biWidth, 
			m_pBMIH->biHeight, 0, 0, i, 1, m_lpData[i], (BITMAPINFO*)m_pBMIH, DIB_RGB_COLORS);
	}

	return TRUE;
}


BOOL CImageRecognize::Draw(CDC* pDC, CRect rt)
{
	if(m_pBMIH == NULL)
		return FALSE;

	if(rt.IsRectNull())
	{
		rt.right = m_pBMIH->biWidth;
		rt.bottom = m_pBMIH->biHeight;
	}

	if(rt.Width() > m_pBMIH->biWidth)
	{
		rt.right = rt.left + m_pBMIH->biWidth;
	}

	if(rt.Height() > m_pBMIH->biHeight)
	{
		rt.bottom = rt.top + m_pBMIH->biHeight;
	}

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{

		::SetDIBitsToDevice(*pDC, rt.left, rt.top, rt.Width(), 
			rt.Height(), 0, 0, i, 1, m_lpData[i], (BITMAPINFO*)m_pBMIH, DIB_RGB_COLORS);

	}

	return TRUE;
}

/**************************************************
void CImageRecognize::SetPixel(int x, int y, COLORREF color)

功能：
设定指定坐标位置像素的颜色值

限制：
无

参数：
int x, int y
指定的像素横、纵坐标值
COLORREF
欲设定的指定位置的颜色值，RGB形式给出
返回值：
无
***************************************************/
void CImageRecognize::SetPixel(int x, int y, COLORREF color)
{
	if(m_pBMIH->biBitCount == 8)			// 256色图
	{
		m_lpData[m_pBMIH->biHeight - y - 1][x] = GetRValue(color);
	}
	else if(m_pBMIH->biBitCount == 1)		// 单色图
	{
		BYTE Color = GetRValue(color);


		// 令0代表黑
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			Color = !Color;

		y = m_pBMIH->biHeight - y - 1;

		// 黑色点
		if (Color == 0)  // 设置对应位为0
		{
			if(x % 8==0)
			{
				m_lpData[y][x/8] &= 0x7F;  
			}
			else if(x % 8==1)
			{
				m_lpData[y][x/8] &= 0xBF;
			}
			else if(x % 8==2)
			{
				m_lpData[y][x/8] &= 0xDF;
			}
			else if(x % 8==3)
			{
				m_lpData[y][x/8] &= 0xEF;
			}
			else if(x % 8==4)
			{
				m_lpData[y][x/8] &= 0xF7;
			}
			else if(x % 8==5)
			{
				m_lpData[y][x/8] &= 0xFB;
			}
			else if(x % 8==6)
			{
				m_lpData[y][x/8] &= 0xFD;
			}
			else if(x % 8==7)
			{
				m_lpData[y][x/8] &= 0xFE;
			}
		}
		else // 白色点
		{
			if(x % 8==0)
			{
				m_lpData[y][x/8] |= 0x80;
			}
			else if(x % 8==1)
			{
				m_lpData[y][x/8] |= 0x40;
			}
			else if(x % 8==2)
			{
				m_lpData[y][x/8] |= 0x20;
			}
			else if(x % 8==3)
			{
				m_lpData[y][x/8] |= 0x10;
			}
			else if(x % 8==4)
			{
				m_lpData[y][x/8] |= 0x08;
			}
			else if(x % 8==5)
			{
				m_lpData[y][x/8] |= 0x04;
			}
			else if(x % 8==6)
			{
				m_lpData[y][x/8] |= 0x02;
			}
			else if(x % 8==7)
			{
				m_lpData[y][x/8] |= 0x01;
			}
		}
	}
	else if(m_pBMIH->biBitCount == 24)
	{
		m_lpData[m_pBMIH->biHeight - y - 1][x*3] = GetBValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 1] = GetGValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 2] = GetRValue(color);
	}
}




// 索引图像转灰度图像
bool CImageRecognize::Index2Gray()
{
	int i;

	if (!IsIndexedImg()) return false;
	RGBQUAD *table = (RGBQUAD*)m_lpvColorTable;

	m_pBMIH->biBitCount = 8;

	// 更新颜色数据
	for (i=0; i<GetHeight(); i++)
	{
		for (int j=0; j<GetWidthPixel(); j++)
		{
			RGBQUAD rgb = *(table+GetGray(j, i));
			BYTE gray = rgb.rgbBlue * 0.114 + rgb.rgbGreen * 0.587 + rgb.rgbRed * 0.299 + 0.5;
			SetPixel(j, i, RGB(gray, gray, gray));
		}
	}

	// 更新颜色表
	for (i=0; i<256; i++)
	{
		(table + i)->rgbBlue = i;
		(table + i)->rgbGreen = i;
		(table + i)->rgbRed = i;
		(table + i)->rgbReserved = 0;
	}

	m_nColorTableEntries = 256;
	return true;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************
CImageRecognize CImageRecognize::operator - (CImageRecognize gray)

功能：图像按位减

参数：
CImageRecognize 对象

返回值:
CImageRecognize： 相减后的 CImageRecognize 类图像对象
*******************/
CImageRecognize CImageRecognize::operator - (CImageRecognize &gray)
{
	CImageRecognize grayRet = *this; //返回图像

	//取得图像的高和宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//循环变量

	//不能在CImageRecognize类对象中直接进行像素相减，因为相减的结果可能小于0
	vector< vector<int> > GrayMat;//相减后暂存图像的灰度点阵
	vector<int> vecRow(nWidth, 0); //GrayMat中的一行（初始化为0）
	for(i=0; i<nHeight; i++)
	{
		GrayMat.push_back(vecRow);
	}

	//最大、最小灰度和值
	int nMax = -255;
	int nMin = 255; 

	//逐行扫描图像
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			//按位相加
			GrayMat[i][j] = GetGray(j, i) - gray.GetGray(j, i);

			//统计最大、最小值
			if( GrayMat[i][j] > nMax)
				nMax = GrayMat[i][j];
			if( GrayMat[i][j] < nMin)
				nMin = GrayMat[i][j];
		}// j
	}// i


	//将GrayMat的取值范围重新归一化到[0, 255]
	int nSpan = nMax - nMin;

	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			BYTE bt;
			if(nSpan > 0)
				bt = (GrayMat[i][j] - nMin)*255/nSpan;
			else if(GrayMat[i][j] <= 255)
				bt = GrayMat[i][j] ;
			else
				bt = 255;

			grayRet.SetPixel(j, i, RGB(bt, bt, bt));			


		}// for j
	}// for i



	return grayRet;
}




bool CImageRecognize::LinarTrans(int nFa, int nFb)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetGray(i,j);

			int reby = by * nFa + nFb;

			if (reby < 0)
				reby = 0;

			if (reby >=256)
				reby = 255;

			SetPixel(i,j,RGB(reby,reby,reby));

		}
	}

	return true;
}

bool CImageRecognize::ThreshTrans(BYTE byColor)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetPixel(i,j);

			if (by > byColor)
				SetPixel(i,j,RGB(255,255,255));
			else
				SetPixel(i,j,RGB(0,0,0));
		}
	}

	return true;
}

bool CImageRecognize::Template(const float se[3][3],float sum)
{
	CImageRecognize img = *this;

	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight() -1;j++)
		{
			float re = 0;
			for(int k=i-1;k<=i+1;k++)
			{
				for(int l=j-1;l<=j+1;l++)
				{
					BYTE by = img.GetPixel(k,l);
					re += by*se[k-i+1][l-j+1];
				}
			}

			re = re/sum;
			re = fabs(re);

			if (re  < 0)
				re = 0;

			if (re >= 256)
				re = 255;

			SetPixel(i,j,RGB(int(re),int(re),int(re)));
		}
	}

	return true;
}

bool CImageRecognize::MaxTrans(CImageRecognize *img)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE byi = img->GetGray(i,j);
			BYTE by = GetGray(i,j);

			if (byi > by)
				SetPixel(i,j,RGB(byi,byi,byi));
		}
	}
	return true;
}

bool CImageRecognize::Corner()
{
	CImageRecognize imgOrg = *this;

	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight()-1;j++)
		{
			byte by11 = imgOrg.GetGray(i-1,j-1);
			byte by12 =  imgOrg.GetGray(i,j-1);
			byte by13 =  imgOrg.GetGray(i+1,j-1);
			byte by21 =  imgOrg.GetGray(i-1,j);
			byte by22 =  imgOrg.GetGray(i,j);
			byte by23 =  imgOrg.GetGray(i+1,j);
			byte by31 =  imgOrg.GetGray(i-1,j+1);
			byte by32 =  imgOrg.GetGray(i,j+1);
			byte by33 = imgOrg.GetGray(i+1,j+1);

			if (by11 == 255 && by12 == 255 && by13 == 255 && by21 == 255 && by22 == 0 && by23 == 0 && by31 == 255 && by32 == 0 && by33 ==0)
			{
				SetPixel(i,j,RGB(1,1,1));
			}
			else
			{
				SetPixel(i,j,RGB(255,255,255));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight()-1;j++)
		{
			byte by11 = imgOrg.GetGray(i-1,j-1);
			byte by12 =  imgOrg.GetGray(i,j-1);
			byte by13 =  imgOrg.GetGray(i+1,j-1);
			byte by21 =  imgOrg.GetGray(i-1,j);
			byte by22 =  imgOrg.GetGray(i,j);
			byte by23 =  imgOrg.GetGray(i+1,j);
			byte by31 =  imgOrg.GetGray(i-1,j+1);
			byte by32 =  imgOrg.GetGray(i,j+1);
			byte by33 = imgOrg.GetGray(i+1,j+1);

			if (by11 == 255 && by12 == 255 && by13 == 255 && by21 == 0 && by22 == 0 && by23 == 255 && by31 == 0 && by32 == 0 && by33 == 255)
			{
				SetPixel(i,j,RGB(2,2,2));
			}
			else
			{
				SetPixel(i,j,RGB(255,255,255));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight()-1;j++)
		{
			byte by11 = imgOrg.GetGray(i-1,j-1);
			byte by12 =  imgOrg.GetGray(i,j-1);
			byte by13 =  imgOrg.GetGray(i+1,j-1);
			byte by21 =  imgOrg.GetGray(i-1,j);
			byte by22 =  imgOrg.GetGray(i,j);
			byte by23 =  imgOrg.GetGray(i+1,j);
			byte by31 =  imgOrg.GetGray(i-1,j+1);
			byte by32 =  imgOrg.GetGray(i,j+1);
			byte by33 = imgOrg.GetGray(i+1,j+1);

			if (by11 == 255 && by12 == 0 && by13 == 0 && by21 == 255 && by22 == 0 && by23 == 0 && by31 == 255 && by32 == 255 && by33 == 255)
			{
				SetPixel(i,j,RGB(3,3,3));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////	for(int i=1;i<GetWidthPixel()-1;i++)
	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight()-1;j++)
		{
			byte by11 = imgOrg.GetGray(i-1,j-1);
			byte by12 =  imgOrg.GetGray(i,j-1);
			byte by13 =  imgOrg.GetGray(i+1,j-1);
			byte by21 =  imgOrg.GetGray(i-1,j);
			byte by22 =  imgOrg.GetGray(i,j);
			byte by23 =  imgOrg.GetGray(i+1,j);
			byte by31 =  imgOrg.GetGray(i-1,j+1);
			byte by32 =  imgOrg.GetGray(i,j+1);
			byte by33 = imgOrg.GetGray(i+1,j+1);

			if (by11 == 0 && by12 == 0 && by13 == 255 && by21 == 0 && by22 == 0 && by23 == 255 && by31 == 255 && by32 == 255 && by33 ==255)
			{
				SetPixel(i,j,RGB(4,4,4));
			}
		}
	}

	return true;
}

bool CImageRecognize::ConnectionRegin(int nMin)
{
	int nConRgn = 1;

	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight()-1;j++)
		{
			if (GetGray(i,j) == 0)
			{
				int sum = 0;
				ConnectionReginSet(this,i,j,nConRgn,sum);

				if (sum <= nMin)
				{
					ClearRegin(nConRgn);
					continue;
				}

				nConRgn++;
			}
		}
	}

	return true;
}

//bool CImageRecognize::ClearRegin(BYTE byColor)
//{
//	for(int i=0;i<GetWidthPixel();i++)
//	{
//		for(int j=0;j<GetHeight();j++)
//		{
//			if (GetGray(i,j) == byColor)
//				SetPixel(i,j,RGB(255,255,255));
//		}
//	}
//
//	return true;
//}

bool CImageRecognize::ConnectionReginSet(CImageRecognize *img,int x,int y,int nConRgn,int &sum)
{
	SetPixel(x,y,RGB(nConRgn,nConRgn,nConRgn));
	//SetPixel(x,y,RGB(0,0,nConRgn));
	sum ++;

	if (sum > 4000)
		return true;

	if (GetGray(x-1,y) == 0)
		ConnectionReginSet(NULL,x-1,y,nConRgn,sum);

	if (GetGray(x,y-1) == 0)
		ConnectionReginSet(NULL,x,y-1,nConRgn,sum);

	if (GetGray(x,y+1) == 0)
		ConnectionReginSet(NULL,x,y+1,nConRgn,sum);

	if (GetGray(x+1,y) == 0)
		ConnectionReginSet(NULL,x+1,y,nConRgn,sum);


	return true;
}



//bool CImageRecognize::ExceptBorder()
//{
//	for(int i=0;i<GetWidthPixel();i++)
//	{
//		SetPixel(i,0,RGB(255,255,255));
//	}
//
//	for(int i=0;i<GetWidthPixel();i++)
//	{
//		SetPixel(i,GetHeight()-1,RGB(255,255,255));
//	}
//
//	for(int i=0;i<GetHeight();i++)
//	{
//		SetPixel(0,i,RGB(255,255,255));
//	}
//
//	for(int i=0;i<GetHeight();i++)
//	{
//		SetPixel(GetWidthPixel()-1,i,RGB(255,255,255));
//	}
//
//	return true;
//
//}
//

void CImageRecognize::VectRemove(int position,vector<CharacterRect>& vec)
{
	for (int i=position; i<vec.size()-1; i++)
	{
		vec[i] = vec[i+1];
	}
	vec.pop_back();
}

void CImageRecognize::VectRemove(int position,vector<float>& vec)
{
	for (int i=position; i<vec.size()-1; i++)
	{
		vec[i] = vec[i+1];
	}
	vec.pop_back();
}


int CImageRecognize::ConnectionRegin(int nMin,int nMinWidth,int nMinHeight,BYTE byColor,vector<CharacterRect>& vcList,BYTE& byColorFlag,CRect* rc)
{
	int flag = 0;
	if (rc == NULL)
	{
		rc = new CRect(1,1,GetWidthPixel()-1,GetHeight()-1);
		flag = 1;
	}

	CharacterRect cr;

	for(int i=rc->left;i<=rc->right;i++)
	{
		for(int j=rc->top;j<=rc->bottom;j++)
		{
			if (GetGray(i,j) == byColor)
			{
				int sum = 0;
				int nTop = j;
				int nBottom = j;
				int nLeft = i;
				int nRight = i;

				ConnectionReginSet(i,j,sum,nTop,nBottom,nLeft,nRight,byColor,byColorFlag,rc);


				if (sum <= nMin)
				{
					ClearRegin(byColorFlag);
					continue;
				}
				if (nBottom-nTop<nMinHeight)
				{
					ClearRegin(byColorFlag);
					continue;
				}
				/*if (nRight-nLeft<nMinWidth)
				{
				ClearRegin(byColorFlag);
				continue;
				}*/
				float white = 0;
				float black = 0;
				for (int x=nLeft; x<=nRight; x++)
				{
					for (int y=nTop; y<=nBottom; y++)
					{
						if (GetGray(x,y) == 0)
						{
							black++;
						}
						else
						{
							white++;
						}
					}
				}
				float p = white/(black+white);
				if ((p<0.1) || (p>0.8 && nRight-nLeft>10))
				{
					ClearRegin(byColorFlag);
					continue;
				}

				cr.mLeft = nLeft;
				cr.mRight = nRight;
				cr.mBottom = nBottom;
				cr.mTop = nTop;
				cr.mColor = byColorFlag;
				vcList.push_back(cr);
				byColorFlag++;
			}
		}
	}
	if (flag) delete rc;
	return vcList.size();
}



void CImageRecognize::CharacterRectSort(vector<CharacterRect>& vcList, int sortMode)
{
	int nSize = vcList.size();
	if (sortMode == 0)
	{
		for (int i=0; i<nSize-1; i++)
		{
			for (int j=i; j<nSize; j++)
			{ 
				if (vcList[i].mLeft>vcList[j].mLeft)
				{
					CharacterRect temp = vcList[i];
					vcList[i] = vcList[j];
					vcList[j] = temp;
				}
			}
		}

	}
	else
	{
		//
	}
}



void CImageRecognize::DeleteFrame(int nMin,int nMinWidth,int nMinHeight,BYTE& byColorFlag,vector<CharacterRect>& vcList,int maxWidth,int iCount)
{
	int nSize = vcList.size();
	for (int i=0; i<nSize; i++)
	{
		if (vcList[i].mRight-vcList[i].mLeft > maxWidth)
		{
			ExceptBorder(0,vcList[i]);
			int k = ConnectionRegin(nMin,nMinWidth,nMinHeight,vcList[i].mColor,vcList,byColorFlag,vcList[i]);
			int j=1;
			for (j=1; j<iCount && (k-nSize==1 && vcList[nSize].mRight-vcList[nSize].mLeft>maxWidth*2/3); ++j)
			{
				ExceptBorder(0,vcList[k-1]);
				k = ConnectionRegin(nMin,nMinWidth,nMinHeight,vcList[k-1].mColor,vcList,byColorFlag,vcList[k-1]);
				VectRemove(nSize,vcList);
				k--;
			}
			if (k-nSize != 1 || vcList[nSize].mRight-vcList[nSize].mLeft<=maxWidth*2/3)
			{
				VectRemove(i,vcList);
			}
			else
			{
				vcList.pop_back();
			}
			nSize = vcList.size();
		}	
	}
}

void CImageRecognize::CleanUpRect(vector<CharacterRect>& vcList)
{
	int nSzie = vcList.size();
	for (int i=1; i<nSzie; i++)
	{
		if (vcList[i].mLeft<=vcList[i-1].mRight)
		{
			if (vcList[i].mRight > vcList[i-1].mRight || vcList[i-1].mRight-vcList[i-1].mLeft-vcList[i].mRight+vcList[i-1].mLeft<5)
			{
				vcList[i-1].mRight = vcList[i-1].mRight>vcList[i].mRight?vcList[i-1].mRight:vcList[i].mRight;
				vcList[i-1].mTop = vcList[i-1].mTop<vcList[i].mTop?vcList[i-1].mTop:vcList[i].mTop;
				vcList[i-1].mBottom = vcList[i-1].mBottom>vcList[i].mBottom?vcList[i-1].mBottom:vcList[i].mBottom;
				VectRemove(i,vcList);
				nSzie--;
				continue;
			}
		}	
		if (abs(vcList[i].mLeft-vcList[i-1].mLeft)<1 && (vcList[i].mTop>=vcList[i-1].mBottom||vcList[i-1].mTop>=vcList[i].mBottom))
		{
			vcList[i-1].mLeft = vcList[i-1].mLeft<vcList[i].mLeft?vcList[i-1].mLeft:vcList[i].mLeft;
			vcList[i-1].mRight = vcList[i-1].mRight>vcList[i].mRight?vcList[i-1].mRight:vcList[i].mRight;
			vcList[i-1].mTop = vcList[i-1].mTop<vcList[i].mTop?vcList[i-1].mTop:vcList[i].mTop;
			vcList[i-1].mBottom = vcList[i-1].mBottom>vcList[i].mBottom?vcList[i-1].mBottom:vcList[i].mBottom;
			VectRemove(i,vcList);
			nSzie--;
		}
	}
}



float CImageRecognize::AverageWidth(vector<float>& wid)
{
	int nSize = wid.size();
	float minWidth = 1000,maxWidth = 0,sumWidth = 0,averageWidth;

	for (int i=0; i<nSize; i++)
	{
		if(wid[i]<minWidth)
		{
			minWidth = wid[i];
		}
		if(wid[i]>maxWidth)
		{
			maxWidth = wid[i];
		}
	}

	int wmax = 0,wmin = 0;
	for (int i=0; i<nSize; i++)
	{
		if (wid[i] == minWidth) 
		{
			wmin = i;
		}
		if (wid[i] == maxWidth)
		{
			wmax = i;
		}
	}
	for (int i=0; i<nSize; i++)
	{
		if (nSize>3 && (i == wmax || i == wmin))
		{
			continue;
		}
		sumWidth += wid[i];
	}

	if (nSize > 3)
	{
		averageWidth = sumWidth/(nSize-2);
	}
	else
	{
		averageWidth = sumWidth/nSize;
	}
	return averageWidth;
}

float CImageRecognize::AverageSpace(vector<float>& spc) 
{
	int spcSize = spc.size();
	float minSpace = 1000,maxSpace = 0,sumSpace = 0,averageSpace;
	int smax = 0,smin = 0;

	for (int i=0; i<spcSize; i++)
	{
		if(spc[i]<minSpace)
		{
			minSpace = spc[i];
		}
		if(spc[i]>maxSpace)
		{
			maxSpace = spc[i];
		}
	}
	for (int i=0; i<spcSize; i++)
	{
		if (spc[i] == minSpace)
		{
			smin = i;
		}
		if (spc[i] == maxSpace)
		{
			smax = i;
		}
	}
	for (int i=0; i<spcSize; i++)
	{
		if (spcSize > 3 && (i == smin || i == smax))
		{
			continue;
		}		
		sumSpace += spc[i];
	}
	if (spcSize > 3)
	{
		averageSpace = sumSpace/(spcSize-2);
	}
	else
	{
		averageSpace = sumSpace/spcSize;
	}

	return averageSpace;
}

void CImageRecognize::AverageTopAndBottom(vector<CharacterRect>& vcList,float& averageTop,float& averageBottom) 
{
	int iSize = vcList.size();
	int minTop = 1000,maxTop = 0,sumTop = 0;
	int minBottom = 1000,maxBottom = 0,sumBottom = 0; 

	int topMax = 0,topMin = 0;
	int bottomMax = 0,bottomMin = 0;

	for (int i=0; i<iSize; i++)
	{
		if(vcList[i].mTop < minTop)
		{
			minTop = vcList[i].mTop;
		}
		if(vcList[i].mTop > maxTop)
		{
			maxTop = vcList[i].mTop;
		}
		if(vcList[i].mBottom < minBottom)
		{
			minBottom = vcList[i].mBottom;
		}
		if(vcList[i].mBottom > maxBottom)
		{
			maxBottom = vcList[i].mBottom;
		}

	}
	for (int i=0; i<iSize; i++)
	{
		if (vcList[i].mTop == minTop)
		{
			topMin = i;
		}
		if (vcList[i].mTop == maxTop)
		{
			topMax = i;
		}
		if (vcList[i].mBottom == minBottom)
		{
			bottomMin = i;
		}
		if (vcList[i].mBottom == maxBottom)
		{
			bottomMax = i;
		}
	}
	for (int i=0; i<iSize; i++)
	{
		if (iSize <= 3 || (i != topMin && i != topMax))
		{
			sumTop += vcList[i].mTop;
		}		
		if (iSize <= 3 || (i != bottomMin && i != bottomMax))
		{
			sumBottom += vcList[i].mBottom;
		}	
	}
	if (iSize > 3)
	{
		if (topMax == topMin)
		{
			averageTop = sumTop/(iSize-1);
		}
		else
		{
			averageTop = sumTop/(iSize-2);
		}
		if (bottomMax == bottomMin)
		{
			averageBottom = sumBottom/(iSize-1);
		}
		else
		{
			averageBottom = sumBottom/(iSize-2);
		}

	}
	else
	{
		averageTop = sumTop/iSize;
		averageBottom = sumBottom/iSize;
	}

}

int CImageRecognize::InsureCharacter(vector<CharacterRect>& vcListFrom,vector<CharacterRect>& vcListTo,int iStart,bool bToEnd,float averageSpace,float averageWidth,vector<float>& spc,vector<float>& wid)
{
	int nSize = vcListFrom.size();

	if (bToEnd)
	{
		for (int i=iStart; i<nSize-1; i++)
		{
			if (fabs(spc[i]-averageSpace) < averageSpace/2+1/*  || wid[i+1]<averageWidth/2 */)
			{
				if (fabs(wid[i+1]-averageWidth)<3 || wid[i+1]<averageWidth)
				{
					vcListTo.push_back(vcListFrom[i+1]);
				}
				else
				{
					vcListTo.push_back(CRect(vcListFrom[i+1].mLeft,vcListFrom[i+1].mTop,averageWidth+vcListFrom[i+1].mLeft+averageSpace/2,vcListFrom[i+1].mBottom));
					vcListTo.push_back(CRect(averageWidth+vcListFrom[i+1].mLeft+averageSpace,vcListFrom[i+1].mTop,vcListFrom[i+1].mRight,vcListFrom[i+1].mBottom));
				}
			}
			else
			{
				return i+1;
			}
		}

	}
	else
	{
		for (int i=iStart; i>0; i--)
		{
			if (fabs(spc[i-1]-averageSpace) < averageSpace/2+1/* || wid[i-1]<averageWidth-2*/)
			{
				if (fabs(wid[i-1]-averageWidth)<3 || wid[i-1]<averageWidth)
				{
					vcListTo.push_back(vcListFrom[i-1]);
				}
				else
				{
					vcListTo.push_back(CRect(vcListFrom[i-1].mLeft,vcListFrom[i-1].mTop,averageWidth+vcListFrom[i-1].mLeft+averageSpace/2,vcListFrom[i-1].mBottom));
					vcListTo.push_back(CRect(averageWidth+vcListFrom[i-1].mLeft+averageSpace,vcListFrom[i-1].mTop,vcListFrom[i-1].mRight,vcListFrom[i-1].mBottom));
				}
			}
			else
			{
				return i-1;
			}
		}
	}
}

bool CImageRecognize::ThreshTrans(long lWidth, long lHeight)
{

	//循环变量
	long i;
	long j;

	//像素值
	BYTE byPixel;

	//直方图数组
	long lHistogram[256];

	//阈值，最大灰度值与最小灰度值，两个区域的平均灰度值
	BYTE byThreshold,byNewThreshold,byMaxGrayValue,byMinGrayValue,byMean1GrayValue,byMean2GrayValue;

	//用于计算区域灰度平均值的中间变量
	long lP1,lP2,lS1,lS2;

	//迭代次数
	int iIterationTimes;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0; i < 256;i++)
	{
		lHistogram[i]=0;
	}

	//获得直方图
	byMaxGrayValue = 0;
	byMinGrayValue = 255;

	for (i = 0;i < lWidth ;i++)
	{
		for(j = 0;j < lHeight ;j++)
		{
			byPixel = (BYTE)GetPixel(i,j);

			lHistogram[byPixel]++;
			//修改最大，最小灰度值
			if(byMinGrayValue > byPixel)
			{
				byMinGrayValue = byPixel;
			}
			if(byMaxGrayValue < byPixel)
			{
				byMaxGrayValue = byPixel;
			}
		}
	}

	//迭代求最佳阈值
	byNewThreshold = (byMinGrayValue + byMaxGrayValue)/2;
	byThreshold = 0;

	for(iIterationTimes = 0; byThreshold != byNewThreshold && iIterationTimes < 100;iIterationTimes ++)
	{
		byThreshold = byNewThreshold;
		lP1 =0;
		lP2 =0;
		lS1 = 0;
		lS2 = 0;
		//求两个区域的灰度平均值
		for (i = byMinGrayValue;i < byThreshold;i++)
		{
			lP1 += lHistogram[i]*i;
			lS1 += lHistogram[i];
		}
		byMean1GrayValue = (unsigned char)(lP1 / lS1);
		for (i = byThreshold+1;i < byMaxGrayValue;i++)
		{
			lP2 += lHistogram[i]*i;
			lS2 += lHistogram[i];
		}
		byMean2GrayValue = (unsigned char)(lP2 / lS2);
		byNewThreshold =    (byMean1GrayValue + byMean2GrayValue)/2;
	}

	//根据阈值将图像二值化
	for (i = 0;i < lWidth ;i++)
	{
		for(j = 0;j < lHeight ;j++)
		{
			byPixel = GetPixel(i,j);

			if(byPixel <= byThreshold)
			{
				SetPixel(i,j,RGB(0,0,0));
			}
			else
			{
				SetPixel(i,j,RGB(255,255,255));
			}
		}
	}

	return TRUE;
}

CString CImageRecognize::FindCharacter(CImageRecognize& img,int nMin,int nMinWidth,int nMinHeight,BYTE byColor,vector<CharacterRect>& vcListLast)
{
	CString result = NULL;
	vector<CharacterRect> vcList;
	byte byColorFlag = 1;
	ConnectionRegin(nMin,nMinWidth,nMinHeight,byColor,vcList,byColorFlag);
	//DrawCharRect(vcList,img);
	DeleteFrame(nMin,nMinWidth,nMinHeight,byColorFlag,vcList,GetWidthPixel()/7,5);	
	CharacterRectSort(vcList);
	CleanUpRect(vcList);

	int nSize = vcList.size();


	float averageSpace,averageWidth,averageTop,averageBottom;
	int maxBottom = 0, minTop = 1000; 
	vector<float> spc;
	vector<float> wid;
	for (int i=0; i<nSize-1; i++)
	{		
		spc.push_back(vcList[i+1].mLeft-vcList[i].mRight);	
	}
	for(int i=0; i<nSize; i++)
	{
		wid.push_back(vcList[i].mRight-vcList[i].mLeft);
	}
	averageSpace = AverageSpace(spc);
	averageWidth = AverageWidth(wid);

	for (int i=0; i<nSize-1; i++)
	{
		//***
		if (spc[i]<averageSpace/2 && (averageWidth/2+2>wid[i] || averageWidth/2+2>wid[i+1]))
		{
			vcList[i].mRight = vcList[i+1].mRight;
			vcList[i].mTop = vcList[i].mTop<vcList[i+1].mTop?vcList[i].mTop:vcList[i+1].mTop;
			vcList[i].mBottom = vcList[i].mBottom>vcList[i+1].mBottom?vcList[i].mBottom:vcList[i+1].mBottom;
			wid[i] = vcList[i].mRight-vcList[i].mLeft;
			VectRemove(i,spc);
			VectRemove(i+1,wid);
			averageWidth = AverageSpace(wid);
			averageSpace = AverageWidth(spc);		
			VectRemove(i+1,vcList);
			nSize--;
			i--;
		}
	}

	DeleteFrame(nMin,nMinWidth,nMinHeight,byColorFlag,vcList,2*averageWidth,4);
	nSize = vcList.size();
	spc.clear();
	wid.clear();
	for (int i=0; i<nSize-1; i++)
	{		
		spc.push_back(vcList[i+1].mLeft-vcList[i].mRight);	
	}
	for(int i=0; i<nSize; i++)
	{
		wid.push_back(vcList[i].mRight-vcList[i].mLeft);
	}
	averageWidth = AverageSpace(wid);
	averageSpace = AverageWidth(spc);	


	if (nSize < 1) return false;
	int start = -1;
	for (int i=(nSize+1)/2; i<nSize; i++)
	{
		if (fabs(wid[i]-averageWidth)<2)
		{
			start = i;
			break;
		}
	}
	if (start == -1)
	{
		for (int i=(nSize-1)/2; i>=0; i--)
		{
			if (fabs(wid[i]-averageWidth)<2)
			{
				start = i;
				break;
			}
		}
	}

	if (start == -1)
	{
	//	AfxMessageBox(L"error!!");
		return false;
	}

	//vector<CharacterRect> vcListLast;
	vcListLast.push_back(vcList[start]);

	int endStart = start;
	while (endStart <nSize-1)
	{
		endStart = InsureCharacter(vcList,vcListLast,endStart,true,averageSpace,averageWidth,spc,wid);
		if (endStart < nSize-1)
		{
			vcListLast.push_back(vcList[endStart]);
		}
	}
	int firstStart = start;
	while (firstStart > 0)
	{
		firstStart = InsureCharacter(vcList,vcListLast,firstStart,false,averageSpace,averageWidth,spc,wid);
		if (firstStart > 0)
		{
			vcListLast.push_back(vcList[firstStart]);
		}
	}

	AverageTopAndBottom(vcListLast,averageTop,averageBottom); 
	nSize = vcListLast.size();

	CharacterRectSort(vcListLast);

	for (int i=0; i<nSize && nSize>7; i++)
	{
		if (fabs(vcListLast[nSize-i-1].mBottom-averageBottom)>3 ||
			fabs(vcListLast[nSize-i-1].mTop-averageTop)>3 ||
			vcListLast[nSize-i-1].mRight-vcListLast[nSize-i-1].mLeft<averageWidth/2 ||
			vcListLast[nSize-i-1].mRight-vcListLast[nSize-i-1].mLeft>3*averageWidth/2)
		{
			VectRemove(nSize-i-1,vcListLast);
			--nSize;
		}
		//if (nSize == 7) break;
		if (fabs(vcListLast[i].mBottom-averageBottom)>3 ||
			fabs(vcListLast[i].mTop-averageTop)>3 ||
			vcListLast[i].mRight-vcListLast[i].mLeft<averageWidth/2 ||
			vcListLast[i].mRight-vcListLast[i].mLeft>3*averageWidth/2)
		{
			VectRemove(i,vcListLast);
			--nSize;
		}
	}

	if (vcListLast[0].mRight-vcListLast[0].mLeft<3)
	{
		VectRemove(0,vcListLast);
		--nSize;
	}

	while (nSize > 7)
	{
		vcListLast.pop_back();
		--nSize;
	}
	while (nSize>0 && nSize<7)
	{
		if (vcListLast[nSize-1].mRight+averageWidth+averageSpace+1<GetWidthPixel())
		{
			vcListLast.push_back(CRect(vcListLast[nSize-1].mRight+averageSpace-1,averageTop-1,vcListLast[nSize-1].mRight+averageSpace+averageWidth+1,averageBottom+1));
			nSize++;
		}
		else
		{
			break;
		}
	}

	while(nSize>0 && nSize<7)
	{
		if (vcListLast[0].mLeft-averageWidth-averageSpace-1>0)
		{
			vcListLast.push_back(CRect(vcListLast[0].mLeft-averageWidth-averageSpace-1,averageTop-1,vcListLast[0].mLeft-averageSpace+1,averageBottom+1));
			CharacterRectSort(vcListLast);
			nSize++;
		}
		else
		{
			break;
		}
	}

	for (int i=0; i<nSize; i++)
	{
		if (fabs(vcListLast[i].mBottom-averageBottom)>1) 			
		{
			vcListLast[i].mBottom = averageBottom>vcListLast[i].mBottom?averageBottom:vcListLast[i].mBottom;
		}
		if (fabs(vcListLast[i].mTop-averageTop)>1)
		{
			vcListLast[i].mTop = averageTop<vcListLast[i].mTop?averageTop:vcListLast[i].mTop;
		}
	}

	//DrawCharRect(vcListLast,img);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CImageRecognize cimg;
	cimg = img;
	for (int i=0; i<nSize; i++)
	{
		//////////////////////////////////////////////////////////////////////////
		/*float fRotateAngle;
		float k1 = fabs((float)(vcListLast[nSize].mTop - vcListLast[0].mTop)/(vcListLast[nSize].mLeft - vcListLast[0].mLeft));
		float k2 = fabs((float)(vcListLast[nSize].mBottom - vcListLast[0].mBottom)/(vcListLast[nSize].mRight - vcListLast[0].mRight));
		if (k1 > k2)
		fRotateAngle = atanf(k1);
		else
		fRotateAngle = atanf(k2);*/

		//////////////////////////////////////////////////////////////////////////
		img.GetPart(cimg,vcListLast[i].mLeft-2>=0?vcListLast[i].mLeft-2:0,	
			vcListLast[i].mTop-2>=0?vcListLast[i].mTop-2:0,
			vcListLast[i].mRight+2<GetWidthPixel()?vcListLast[i].mRight+2:GetWidthPixel()-1,
			vcListLast[i].mBottom+2<GetHeight()?vcListLast[i].mBottom+2:GetHeight()-1);
		CString str;
		str.Format(L"C:\\\\%d.bmp",i);
		cimg.SaveToFile(str);
		int w = cimg.GetWidthPixel();
		int h = cimg.GetHeight();

		LPBYTE* Simple = new LPBYTE[h];

		ofstream of;
		str.Format(_T("%d"),i);
		of.open(_T("C:\\CutTest\\test")+str+_T(".txt"));
		for (int j=0; j<h; j++)
		{
			Simple[j] = new BYTE[w];
			for (int k=0; k<w; k++)
			{
				Simple[j][k] = (BYTE)cimg.GetPixel(k,j);
				if ((int)Simple[j][k] == 255)
					of << "1";
				else
					of << "0";
			}
			of<<endl;
		}
		of.close();

		//CutEdge(h,w,i,Simple);
		//////////////////////////////////////////////////////////////////////////
		//int flag=0;
		//int m, n;
		//int from_X, from_Y, to_X, to_Y;
		////初始点行坐标
		//for (m=0;m<h;m++)
		//{
		//	for (n=0;n<w;n++)
		//	{
		//		if(255 == (int)Simple[m][n])
		//		{
		//			position[i].startX = m;
		//			flag=1;
		//			break;
		//		}
		//	}
		//	if(1 == flag)
		//		break;
		//	
		//}
		//flag=0;
		////初始点列坐标
		//for (n=0;n<w;n++)
		//{
		//	for (m=0;m<h;m++)
		//	{
		//		if (255 == (int)Simple[m][n])
		//		{
		//			position[i].startY = n;
		//			flag=1;
		//			break;
		//		}
		//	}
		//	if(1 == flag)
		//		break;
		//}
		//flag=0;
		////终点行坐标
		//for (m=h-1;m>0;m--)
		//{
		//	for (n=w-1;n>0;n--)
		//	{
		//		if(255 == (int)Simple[m][n])
		//		{
		//			position[i].endX = m;
		//			flag=1;
		//			break;
		//		}
		//	}
		//	if(1 == flag)
		//		break;
		//}
		//flag=0;
		////终点列坐标
		//for (n=w-1;n>0;n--)
		//{
		//	for (m=h-1;m>0;m--)
		//	{
		//		if (255 == (int)Simple[m][n])
		//		{
		//			position[i].endY = n;
		//			flag=1;
		//			break;
		//		}
		//	}
		//	if(1 == flag)
		//		break;
		//}
		//flag=0;
		///*cout<<from_X<<endl;
		//cout<<from_Y<<endl;
		//cout<<to_X<<endl;
		//cout<<to_Y<<endl;*/
		//str.Format(_T("%d"),i);
		//of.open(_T("C:\\CutTest2\\test")+str+_T(".txt"));
		//for (m=0;m<to_X-from_X;m++)
		//{
		//	for (n=0;n<to_Y-from_Y;n++)
		//	{
		//		Simple[m][n] = Simple[from_X+m][from_Y+n];
		//		if ((int)Simple[m][n] == 255)
		//			of << "1";
		//		else
		//			of << "0";
		//	}
		//	of<<endl;
		//}
		//of.close();
		//////////////////////////////////////////////////////////////////////////
		int sample[ZOOMH][ZOOMW];
		CharToStandard(sample, Simple, h, w, i);

		//float wf,hf;

		//wf = (float)ZOOMW/(float)(position[i].endY - position[i].startY);
		//hf = (float)ZOOMH/(float)(position[i].endX - position[i].startX);

		//wf = (float)ZOOMW/(float)w;
		//hf = (float)ZOOMH/(float)h;

		//
		//str.Format(_T("%d"),i);
		//of.open(_T("C:\\SampleTest\\test")+str+_T(".txt"));
		//LPBYTE* sample = new LPBYTE[ZOOMH];
		//for (int j=0; j<ZOOMH;j++) 
		//{
		//	sample[j] = new BYTE[ZOOMW];
		//	for(int k=0;k<ZOOMW;k++)
		//	{
		//		long oi,oj;
		//		//lpGrayBits[i*RowLength1+j]=(lpRgbBits[i*RowLength2+3*j]*114+lpRgbBits[i*RowLength2+3*j+1]*587+lpRgbBits[i*RowLength2+3*j+2]*299)>>10;
		//		oi = (long) (j / hf +0.5);
		//		oj = (long) (k / wf +0.5); 
		//		if((oj >= 0)&&(oj<w) &&(oi>=0)&&(oi<h) )
		//		{
		//			sample[j][k]  =  Simple[oi][oj];
		//			if ((int)sample[j][k] == 255)
		//				of << "1";
		//			else
		//				of << "0";
		//		} 
		//	}
		//	of << endl;
		//}
		//of.close();
		result += CharRecognize(sample,i);
		//AfxMessageBox(str);
	}
	return result;
}

void CImageRecognize::DrawCharRect(vector<CharacterRect>& vcListLast,CImageRecognize& img)
{
	int nSize = vcListLast.size();
	for(int i=0; i<nSize; i++)
	{
		for(int j=vcListLast[i].mTop;j<vcListLast[i].mBottom;j++)
		{
			img.SetPixel(vcListLast[i].mLeft,j,RGB(255,0,0));
			img.SetPixel(vcListLast[i].mRight,j,RGB(255,0,0));
		}
		for(int j=vcListLast[i].mLeft;j<vcListLast[i].mRight;j++)
		{
			img.SetPixel(j,vcListLast[i].mTop,RGB(255,0,0));
			img.SetPixel(j,vcListLast[i].mBottom,RGB(255,0,0));
		}
	}
}

bool CImageRecognize::ClearRegin(BYTE byColor,CRect* rc/* = NULL*/)
{
	int flag = 0;
	if (rc == NULL)
	{
		rc = new CRect(0,0,GetWidthPixel()-1,GetHeight()-1);
		flag = 1;
	}
	for(int i=rc->left;i<=rc->right;i++)
	{
		for(int j=rc->top;j<=rc->bottom;j++)
		{
			if (GetGray(i,j) == byColor)
			{
				//SetPixel(i,j,RGB(255,255,255));
				SetPixel(i,j,RGB(0,0,0));
			}
		}
	}
	if (flag) delete rc;
	return true;
}

bool CImageRecognize::ConnectionReginSet(int x,int y,int& sum,int& nTop,int& nBottom,int& nLeft,int& nRight,BYTE byColor,BYTE nConRgn,CRect* rc/* = NULL*/)
{
	int flag = 0;
	if (rc == NULL)
	{
		rc = new CRect(0,0,GetWidthPixel()-1,GetHeight()-1);
		flag = 1;
	}
	SetPixel(x,y,RGB(nConRgn,nConRgn,nConRgn));
	//SetPixel(x,y,RGB(0,0,nConRgn));
	sum ++;
	BYTE nWidth = rc->right-rc->left;
	BYTE nHeight = rc->bottom-rc->top;

	//if (sum > 4000)
	//	return true;

	if (x-1>=rc->left && GetGray(x-1,y) == byColor)
	{
		ConnectionReginSet(x-1,y,sum,nTop,nBottom,nLeft,nRight,byColor,nConRgn,rc);
		if(nLeft > x-1)
		{
			nLeft = x-1;
		}

	}

	if (y-1>=rc->top && GetGray(x,y-1) == byColor)
	{
		ConnectionReginSet(x,y-1,sum,nTop,nBottom,nLeft,nRight,byColor,nConRgn,rc);
		if(nTop > y-1)
		{
			nTop = y-1;
		}

	}

	if (y+1<=rc->bottom && GetGray(x,y+1) == byColor)
	{
		ConnectionReginSet(x,y+1,sum,nTop,nBottom,nLeft,nRight,byColor,nConRgn,rc);
		if(nBottom < y+1)
		{
			nBottom = y+1;
		}
	}

	if (x+1<=rc->right && GetGray(x+1,y) == byColor)
	{
		ConnectionReginSet(x+1,y,sum,nTop,nBottom,nLeft,nRight,byColor,nConRgn,rc);
		if (nRight < x+1)
		{
			nRight = x+1; 
		}
	}

	if (flag) delete rc;
	return true;
}

bool CImageRecognize::ExceptBorder(BYTE bColor,CRect* rc)
{
	int flag = 0;
	int iWidth = GetWidthPixel();
	int iHight = GetHeight();
	if (rc == NULL)
	{
		rc = new CRect(0,0,iWidth-1,iHight-1);
		flag = 1;
	}
	for(int i=rc->left;i<=rc->right;i++)
	{
		SetPixel(i,rc->top,RGB(bColor,bColor,bColor));
	}

	for(int i=rc->left;i<=rc->right;i++)
	{
		SetPixel(i,rc->bottom,RGB(bColor,bColor,bColor));
	}

	for(int i=rc->top;i<=rc->bottom;i++)
	{
		SetPixel(rc->left,i,RGB(bColor,bColor,bColor));
	}

	for(int i=rc->top;i<rc->bottom;i++)
	{
		SetPixel(rc->right,i,RGB(bColor,bColor,bColor));
	}
	if (flag) delete rc;
	return true;

}




bool CImageRecognize::DetectLine(int min,int max,CImageRecognize * toImg)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = GetHeight()*2/5;i<GetHeight()*7/8;i++)
	{
		preby = GetGray(i,0);
		for (int j = 1 ; j < GetWidthPixel() -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
		//changeNum = GetBetterValue(changeNum,i,GetHeight());
		if (changeNum>min && changeNum<max)
		{
			for (int k = 0 ; k <GetWidthPixel() ; k++)
			{
				toImg->SetPixel(k,i,RGB(255,0,0));
			}
		}
		changeNum = 0;
	}
	return true;
}

bool CImageRecognize::DetectLine_area(int min,int max,int high,int low,CImageRecognize* toImg)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = 0;i<GetWidthPixel()-1;i++)
	{
		preby = GetGray(i,0);
		for (int j = 1 ; j < low-high -1 ; j++ )
		{
			if (preby != GetGray(i,j))
			{
				preby = GetGray(i,j);
				changeNum++;
			}
		}
		if (changeNum>min && changeNum<max)
		{
			for (int k = 0 ; k <GetHeight() ; k++)
			{
				toImg->SetPixel(i,k,RGB(255,0,0));
			}
		}
		changeNum = 0;
	}
	return true;
}

bool CImageRecognize::ModelTo(CImageRecognize* img)
{
	img->ImResize(GetHeight(),GetWidthPixel());

	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			img->SetPixel(i,j,GetPixel(i,j));
		}
	}

	img->SetPixel(GetWidthPixel()/2,GetHeight()/2,RGB(255,0,0));

	return true;
}


bool CImageRecognize::DetectLineX(int min,int max,int ax,int ay, int bx, int by)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = ay;i<by;i++)
	{
		preby = GetGray(i,0);
		for (int j = ax ; j < bx -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
	}
	return changeNum;
}

bool CImageRecognize::DetectLineY(int min,int max,int ax,int ay, int bx, int by)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = ax;i<bx;i++)
	{
		preby = GetGray(i,0);
		for (int j = ay ; j < by -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
	}
	return changeNum;
}


// 区域跳点检测
bool CImageRecognize::AreaDetect(int xmin, int xmax, int ymin, int ymax, CImageRecognize* toImg)
{
	int changeNumX,changeNumY;
	BYTE preby;
	//初始化大小
	//for (int i = GetWidthPixel()/5; i<GetWidthPixel()/2; i++ )
	//{
	int i = 120;
	for(int j = 1; i+j<GetWidthPixel()-1; j=j+3)
	{
		for (int k = 1; i/3 + k < GetHeight()-1; k=k+3)
		{
			changeNumX = DetectLineX(10,20,j,k,j+i,k+i/3);
			changeNumY = DetectLineX(5,10,j,k,j+i,k+i/3);
			if (changeNumX<xmax && changeNumX>xmax && changeNumY<ymax && changeNumY>ymax)
			{
				DrawRecTangle(j,k,j+i,k+i/3,RGB(255,0,0),toImg);
			}
			changeNumX = 0;
			changeNumY = 0;
		}
	}
	//}
	return false;
}


// 画矩形
bool CImageRecognize::DrawRecTangle(int ax, int ay, int bx, int by, COLORREF color, CImageRecognize* toImg)
{
	for (int i =ax; i<bx; i++)
	{
		toImg->SetPixel(i,ay,color);
	}
	for (int i =ay; i<by; i++)
	{
		toImg->SetPixel(bx,i,color);
	}
	for (int i =ax; i<bx; i++)
	{
		toImg->SetPixel(i,by,color);
	}
	for (int i =ay; i<by; i++)
	{
		toImg->SetPixel(ax,i,color);
	}
	return true;
}

void CImageRecognize::ChangeDIBToGray(void)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetGray(i,j);

			SetPixel(i,j,RGB(by,by,by));
		}
	}
}

bool CImageRecognize::GetClosestLine(int &high, int &low)
{
	const int length = GetHeight()/10;
	high = 0;
	low = 0;
	int sum=0;
	int max=0;
	for (int i = 0; i + length < GetHeight(); i++)
	{
		for (int j = i; j < i+length; j++)
		{
			if (IsMarkLine(j))
			{
				++sum;
			}
		}
		if (sum >= max)
		{
			high = i;
			low = i + length;
			max = sum;
		}
		sum = 0;
	}
	return true;
}

bool CImageRecognize::GetPart(CImageRecognize &cutImg, int ax, int ay, int bx, int by)
{
	cutImg.Resize(bx-ax,by-ay,RGB(255,255,255));
	for (int i = 0; i <by-ay; i++)
	{
		for (int j = 0; j < bx-ax; j++)
		{
			cutImg.SetPixel(j,i,GetPixel(ax + j, ay + i));
		}
	}
	return true;
}
bool CImageRecognize::Resize(int w, int h, BYTE byColor)
{
	if (!IsValidate())
		return false;

	for(int i=0;i<m_pBMIH->biHeight;i++)
	{
		delete m_lpData[i];
	}

	delete m_lpData;

	this->m_pBMIH->biHeight = h;
	this->m_pBMIH->biWidth = w;

	int nWidthBytes = WIDTHBYTES(m_pBMIH->biWidth * m_pBMIH->biBitCount);

	m_lpData = new LPBYTE[h];

	for(int i=0;i<h;i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memset(m_lpData[i],byColor,nWidthBytes);
	}

	return true;
}
bool CImageRecognize::DetectLine_areaX(int min, int max, int& left, int& right, int high, int low, CImageRecognize* toImg)
{
	int height,width,changeNum,mymax=0,pos=0;
	BYTE preby;
	vector<DetectArea> vda;
	height = low - high;
	width = height * 4;

	preby = GetGray(0,0);
	for(int l=0; l+width<GetWidthPixel(); l++ )//确定扫描区域
	{
		DetectArea da;
		changeNum = 0;
		for(int i = high;i<low;i++)
		{
			preby = GetGray(i,0);
			for (int j = l ; j < l+width ; j++ )
			{
				if (preby != GetGray(j,i))
				{
					preby = GetGray(j,i);
					changeNum++;
				}
			}
		}
		da.changeNum=changeNum;
		da.high=high;
		da.low=low;
		da.left=l;
		da.right=l+width;
		vda.push_back(da);
	}
	for (int i = 0; i < vda.size(); i++)
	{
		if (vda[i].changeNum > mymax)
		{
			mymax = vda[i].changeNum;
			pos = i;
		}
	}
	left = pos;
	right = pos+width;

	CString str;
	str.Format(L"max=%d",mymax);
	AfxMessageBox(str);
	return true;
}

bool CImageRecognize::JumpPointDetect( CImageRecognize* imgDetected)
{
	int changeNum,sum=0;
	vector<int> vJumpPointDetect;
	BYTE preby = GetGray(0,0);
	for(int i = 0;i<GetHeight();i++)
	{
		changeNum = 0;
		preby = GetGray(i,0);
		for (int j = 1 ; j < GetWidthPixel() -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
		//changeNum = GetBetterValue(changeNum,i,GetHeight());
		sum+=changeNum;
		vJumpPointDetect.push_back(changeNum);
	}
	for(int i = 0;i<GetHeight();i++)
	{
		for (int j = 0; j<vJumpPointDetect[i]*5; j++)
		{
			if(j>GetWidthPixel()-6)
				continue;
			imgDetected->SetPixel(j,i,RGB(255,0,0));
		}
	}
	for(int i = 0 ; i < GetWidthPixel(); i+=25)
		imgDetected->SetPixel(i,GetHeight()-1,RGB(0,255,0));

	CString str;
	str.Format(L"%d",sum);
	AfxMessageBox(str);
	return true;
}

int CImageRecognize::GetBetterValue(int changeTime,int yPos, int height)
{
	int result;
	float pos=100*yPos/height;

	if (changeTime>35)
	{
		if(yPos>height*2/3)
		{
			result = changeTime/(-pos/134+1.5);
		}
		else
		{
			result = changeTime/pos/67;
		}
	}
	else
	{
		if(yPos>height*2/3)
		{
			result = changeTime*(-pos/134+1.5);
		}
		else
		{
			result = changeTime*pos/67;
		}
	}
	return result;
}


void CImageRecognize::GreyPlus()
{
	int iWidth = GetWidthPixel();
	int iHeight = GetHeight();
	long lMulh = iWidth*iHeight;
	BYTE byColor;
	float fPointNum[256],fColorP[256],fColorSumP[256];

	for (int i=0; i<256; i++)
	{
		fPointNum[i] = 0;
		fColorP[i] = 0;
		fColorSumP[i] = 0;
	}

	//统计每个色素的色素点个数
	for (int i=0; i<iWidth; i++)
	{
		for (int j=0; j<iHeight; j++)
		{
			byColor = GetPixel(i,j);
			fPointNum[byColor]++;
		}
	}

	for(int i=0; i<256; i++)
	{
		fColorP[i] = fPointNum[i]/lMulh;
	}
	for (int i=0; i<256; i++)
	{
		for (int j=0; j<=i; j++)
		{
			fColorSumP[i] += fColorP[j];
		}
	}

	for (int i=0; i<iWidth; i++)
	{
		for(int j=0; j<iHeight; j++)
		{
			byColor = GetPixel(i,j);
			SetPixel(i,j,RGB(fColorSumP[byColor]*255+0.5,fColorSumP[byColor]*255+0.5,fColorSumP[byColor]*255+0.5));
		}
	}
}

bool CImageRecognize::DetectLine_areaXX(CImageRecognize* toImg, int ax, int ay, int bx, int by)
{
	vector<int> greatSum;
	int sum;
	for (int i = 0; i<GetWidthPixel(); i++)
	{
		sum = 0;
		for (int j=0; j<GetHeight(); j++)
		{
			if (GetPixel(i,j) == RGB(255,255,255))
			{
				++sum;
			}
		}
		greatSum.push_back(sum);
	}
	for (int i=0; i<GetWidthPixel(); i++)
	{
		for (int j=0; j<greatSum[i];j++)
		{
			toImg->SetPixel(i,j,RGB(255,0,0));
		}
	}
	int lowpoint;
	for (int i=0; i<GetHeight(); i++)
	{
		lowpoint = 0;
		for (int j=0;j<GetWidthPixel();j++)
		{
			if (toImg->GetPixel(j,i)!=RGB(255,0,0))
			{
				++lowpoint;
			}
		}
		if (lowpoint>=8)
		{
			for (int j=0;j<GetWidthPixel();j++)
			{
				toImg->SetPixel(j,i,RGB(0,255,0));
			}
			for (int j=0;j<lowpoint;j++)
			{
				if (greatSum[j]<=i)
				{
					for (int k=0; k<GetHeight();k++)
					{
						toImg->SetPixel(j,k,RGB(255,255,255));
					}
				}
			}
			break;
		}
	}
	return true;
}

CString CImageRecognize::RecognizePlate(CString filePath)
{
	int high,low;
	int left,right;

	CImageRecognize rawImg,targetImg,effctedImg,redImg,cutedImg;
	rawImg.AttachFromFile(filePath);
	targetImg=rawImg;
	effctedImg=rawImg;
	redImg=rawImg;
	cutedImg=rawImg;
	//图像与处理部分
	effctedImg.Index2Gray();//灰度化
	//金鹏的灰度加强，暂时不用
	//effctedImg.GreyPlus();
	effctedImg.ExceptBorder();//去边

	effctedImg.ThreshTrans(110);//二值化

	//effctedImg.ThreshTrans(effctedImg.GetWidthPixel(),effctedImg.GetHeight());
	float sep[3][3] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
	effctedImg.Template(sep,1);//边缘化


	////////effctedImg.DetectLine_areaX(0,0,left,right,high,low,NULL);//选取车牌横向位置

	effctedImg.GetPlateLocation(15,30,&redImg,&targetImg,high,low,left,right);


	//水平蓝线
	for (int k = 0 ; k <effctedImg.GetWidthPixel() ; k++)
	{
		targetImg.SetPixel(k,high,RGB(0,0,255));
	}

	for (int k = 0 ; k <effctedImg.GetWidthPixel() ; k++)
	{
		targetImg.SetPixel(k,low,RGB(0,0,255));
	}

	//竖直蓝线
	for (int k = 0 ; k <effctedImg.GetHeight() ; k++)
	{
		targetImg.SetPixel(left,k,RGB(0,0,255));
	}

	for (int k = 0 ; k <effctedImg.GetHeight() ; k++)
	{
		targetImg.SetPixel(right,k,RGB(0,0,255));
	}



	cutedImg = effctedImg;

	//切割图像获得结果
	rawImg.GetPart(cutedImg,left,high,right,low);


	//结果保存
	CString str;
	//int pos = filePath.FindOneOf(L".") ;
	//filePath=filePath.Left(pos);
	str.Format(L".\\Plate\\%s.bmp",getFileName(filePath));
	redImg.SaveToFile(L"C:\\w_redImg.bmp");
	effctedImg.SaveToFile(L"C:\\w_effctedImg.bmp");
	targetImg.SaveToFile(L"C:\\w_targetImg.bmp");
	rawImg.SaveToFile(L"C:\\w_rawImg.bmp");
	cutedImg.SaveToFile(L"C:\\w_cutedImg.bmp");
	cutedImg.SaveToFile(str);

	//////////////////////////////////////////////////////////////////////////
	//img.AttachFromFile(L"C:\\w_cutedImg.bmp");
	CImageRecognize ximg;
	cutedImg.Index2Gray();
	cutedImg.ExceptBorder(0);
	cutedImg.ThreshTrans(125);
	//img.ThreshTrans(img.GetWidthPixel(),img.GetHeight());
	//img.SaveToFile(L"C:\\erzhihua.bmp");
	ximg = cutedImg;
	vector<CImageRecognize::CharacterRect> vcList;
	CString result;
	result = cutedImg.FindCharacter(ximg,5,1,5,255,vcList);//切割、识别，返回车牌号。
	//////////////////////////////////////////////////////////////////////////

	//result.Format(L"UUUUUUU");
	return result;
}

bool CImageRecognize::GetPlateLocation(int min, int max, CImageRecognize* redImg, CImageRecognize* targetImg, int& high, int& low, int& left, int& right)
{
	int changeNum = 0;
	vector<int> changeNumY;
	BYTE preby = GetGray(0,0);
	for(int i = 0;i<GetHeight();i++)
	{
		preby = GetGray(i,0);
		for (int j = 1 ; j < GetWidthPixel() -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
		changeNumY.push_back(changeNum);
		//获得优化的跳变数
		//changeNum = GetBetterValue(changeNum,i,GetHeight());
		//
		if (changeNum>min && changeNum<max)
		{
			for (int k = 0 ; k < changeNum*3 ; k++)
			{
				redImg->SetPixel(k,i,RGB(255,0,0));
			}
		}
		changeNum = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	//以上是跳变检测，并在redImg上画出跳变数
	//////////////////////////////////////////////////////////////////////////

	const int length = GetHeight()/9;
	high = 0;
	low = 0;
	int sum=0;
	int maxJP=0;
	for (int i = GetHeight()*0.4; i + length < GetHeight()*0.85; i++)
	{
		for (int j = i; j < i+length; j++)
		{
			if (changeNumY[j]>min&&changeNumY[j]<max)
			{
				++sum;
			}
		}
		if (sum >= maxJP)
		{
			high = i;
			low = i + length;
			maxJP = sum;
		}
		sum = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//以上是纵坐标车牌区域定位，算法还有待提高
	//////////////////////////////////////////////////////////////////////////
	high-=3;
	low+=2;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	int height,width,changeNumX,mymax=0,pos_left=0,pos_right=0;
	vector<DetectArea> vda;
	height = low - high;
	width = height * 4;

	preby = GetGray(0,0);
	for (int dwide = height*3.9;dwide<height*4.2;dwide+=5)
	{
		for(int l=0; l+dwide<GetWidthPixel(); l++ )//确定扫描区域
		{
			DetectArea da;
			changeNumX = 0;
			for(int i = high;i<low;i++)
			{
				preby = GetGray(i,0);
				for (int j = l ; j < l+dwide ; j++ )
				{
					if (preby != GetGray(j,i))
					{
						preby = GetGray(j,i);
						changeNumX++;
					}
				}
			}
			da.changeNum=changeNumX;
			da.high=high;
			da.low=low;
			da.left=l;
			da.right=l+dwide;
			vda.push_back(da);
		}
	}
	for (int i = 0; i < vda.size(); i++)
	{
		if (vda[i].changeNum > mymax)
		{
			mymax = vda[i].changeNum;
			pos_left = vda[i].left;
			pos_right = vda[i].right;
		}
	}
	left = pos_left;
	right = pos_right;
	//////////////////////////////////////////////////////////////////////////
	//以上是X轴扫描车牌
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool CImageRecognize::DoubleLinarStretch(CRect rectSrc, CRect rectDes)
{
	CImageRecognize img;
	img = *this;
	img.Resize(rectDes.Width(),rectDes.Height(),0);

	for(int i=0;i<rectDes.Width();i++)
	{
		for(int j=0;j<rectDes.Height();j++)
		{
			double x = double(i)/double(rectDes.Width())*double(rectSrc.Width()) + rectSrc.left;
			double y = double(j)/double(rectDes.Height())*double(rectSrc.Height()) + rectSrc.top;

			int a = int(x);
			int b = int(y);

			if (a > GetWidthPixel() - 2)
				a = GetWidthPixel() - 2;

			if (b > GetHeight() - 2)
				b = GetHeight() - 2;

			int A ;
			A = (x-a)*GetRValue(GetPixel(a+1,b+1)) + (a+1-x)*GetRValue(GetPixel(a,b+1));
			int B ;
			B= (x-a)*GetRValue(GetPixel(a+1,b)) + (a+1-x)*GetRValue(GetPixel(a,b));

			int Cr = (b+1-y)*B + (y-b)*A;

			A = (x-a)*GetBValue(GetPixel(a+1,b+1)) + (a+1-x)*GetBValue(GetPixel(a,b+1));
			B= (x-a)*GetBValue(GetPixel(a+1,b)) + (a+1-x)*GetBValue(GetPixel(a,b));

			int Cb = (b+1-y)*B + (y-b)*A;

			A = (x-a)*GetGValue(GetPixel(a+1,b+1)) + (a+1-x)*GetGValue(GetPixel(a,b+1));
			B= (x-a)*GetGValue(GetPixel(a+1,b)) + (a+1-x)*GetGValue(GetPixel(a,b));

			int Cg = (b+1-y)*B + (y-b)*A;

			img.SetPixel(i,j,RGB(Cr,Cg,Cb));
		}
	}

	*this = img;

	return true;
}


//void CImageRecognize::GetSomeVect(vector<CharacterRect>& vcList,int iMaxW,int iMaxH)
//{
//	int iWidth = GetWidthPixel();
//	int iHeight = GetHeight();
//
//	for (int i=0; i<iWidth-iMaxW; i++)
//	{
//		for (int j=0; j<iHeight-iMaxH; j++)
//		{
//			De
//		}
//	}
//}
CString CImageRecognize::GetCharFromPic(void)
{
	return CString();
}
/******************************
/*字符识别
********************************/
CString CImageRecognize::CharRecognize(int sample[ZOOMH][ZOOMW], int num)
{
	//int** module = NULL;
	int minNum = 9999;
	int sum = 0;
	int fitNum = 0, maxNum = 0;
	int i, j, m;

	for (m = 0; m < 37; m++)
	{
		sum = 0;
		LoadMod(m, num);
		for (i = 0; i < ZOOMH; i++)
		{
			for (j = 0; j < ZOOMW; j++)
			{
				if (sample[i][j] != module[i][j])
				{
					sum++;
				}
			}
		}
		if (minNum > sum)
		{
			minNum = sum;
			fitNum = m;
		}
	}
	/*maxNum = sum[0];
	fitNum = 0;
	for (m = 1; m < 36; m++)
	{
	if (sum[m] > maxNum)
	{
	maxNum = sum[m];
	fitNum = m;
	}
	}*/
	CString str = NULL;
	if(0 == num)
	{
		str= HFonts[fitNum];
	}
	else
	{
		if(fitNum == 0 || fitNum == 11 || fitNum == 13 || fitNum == 26 ||fitNum == 8 || fitNum == 24)
			fitNum = isbd0q(sample);
		str.Format(_T("%c"),Fonts[fitNum]);
	}


	//str.Format(_T("%c"),Fonts[fitNum]);

	return str;
}
/************************************************************************/
/*去边                                                                  */
/************************************************************************/
/*void CImageRecognize::CutEdge(int nImageHeight, int nImageWidth, int num, LPBYTE* Simple)
{
int i,j,flag=0;
//初始点行坐标
for (i=0;i<nImageHeight;i++)
{
for (j=0;j<nImageWidth;j++)
{
if(255 == (int)Simple[i][j])
{
position[num].startX = i;
flag=1;
break;
}
}
if(1 == flag)
break;

}
flag=0;
//初始点列坐标
for (j=0;j<nImageWidth;j++)
{
for (i=0;i<nImageHeight;i++)
{
if (255 == (int)Simple[i][j])
{
position[num].startY = j;
flag=1;
break;
}
}
if(1 == flag)
break;
}
flag=0;
//终点行坐标
for (i=nImageHeight-1;i>0;i--)
{
for (j=nImageWidth-1;j>0;j--)
{
if(255 == (int)Simple[i][j])
{
position[num].endX = i;
flag=1;
break;
}
}
if(1 == flag)
break;
}
flag=0;
//终点列坐标
for (j=nImageWidth-1;j>0;j--)
{
for (i=nImageHeight-1;i>0;i--)
{
if (255 == (int)Simple[i][j])
{
position[num].endY = j;
flag=1;
break;
}
}
if(1 == flag)
break;
}
flag=0;

for (i=0;i<position[num].endX-position[num].startX;i++)
{
for (j=0;j<position[num].endY-position[num].startY;j++)
{
Simple[i][j] = Simple[position[num].startX+i][position[num].startY+j];
}
}
}

void CImageRecognize::CutEdge(int nImageHeight, int nImageWidth, int num)
{
int i,j,flag=0;
//初始点行坐标
for (i=0;i<nImageHeight;i++)
{
for (j=0;j<nImageWidth;j++)
{
if(255 == data[i][j])
{
position[num].startX = i;
flag=1;
break;
}
}
if(1 == flag)
break;

}
flag=0;
//初始点列坐标
for (j=0;j<nImageWidth;j++)
{
for (i=0;i<nImageHeight;i++)
{
if (255 == data[i][j])
{
position[num].startY = j;
flag=1;
break;
}
}
if(1 == flag)
break;
}
flag=0;
//终点行坐标
for (i=nImageHeight-1;i>0;i--)
{
for (j=nImageWidth-1;j>0;j--)
{
if(255 == data[i][j])
{
position[num].endX = i;
flag=1;
break;
}
}
if(1 == flag)
break;
}
flag=0;
//终点列坐标
for (j=nImageWidth-1;j>0;j--)
{
for (i=nImageHeight-1;i>0;i--)
{
if (255 == data[i][j])
{
position[num].endY = j;
flag=1;
break;
}
}
if(1 == flag)
break;
}
flag=0;

for (i=0;i<position[num].endX-position[num].startX;i++)
{
for (j=0;j<position[num].endY-position[num].startY;j++)
{
data[i][j] = data[position[num].startX+i][position[num].startY+j];
}
}
}*/

/************************************************************************/
/* 归一化                                                               */
/************************************************************************/
void CImageRecognize::CharToStandard(int standard[ZOOMH][ZOOMW], LPBYTE* resorce, int nImageHeight, int nImageWidth, int num)
{
	//CutEdge(nImageHeight, nImageWidth, num);

	int i, j;
	float wf,hf;

	//wf = (float)zoomW/(float)(position[num].endY-position[num].startY);
	//hf = (float)zoomH/(float)(position[num].endX-position[num].startX);
	wf = (float)ZOOMW/(float)nImageWidth;
	hf = (float)ZOOMH/(float)nImageHeight;

	//ofstream of;
	CString str;
	//str.Format(_T("%c"),Fonts[count]);
	//of.open(_T("C:\\ModuleTest\\moduleTest")+str+_T(".txt"));
	for (i=0; i<ZOOMH;i++) 
	{
		for(j=0;j<ZOOMW;j++)
		{
			long oi,oj;
			//lpGrayBits[i*RowLength1+j]=(lpRgbBits[i*RowLength2+3*j]*114+lpRgbBits[i*RowLength2+3*j+1]*587+lpRgbBits[i*RowLength2+3*j+2]*299)>>10;
			oi = (long) (i / hf +0.5);
			oj = (long) (j / wf +0.5); 
			if((oj >= 0)&&(oj<nImageWidth) &&(oi>=0)&&(oi<nImageHeight) )
			{
				standard[i][j]  =  (int)resorce[oi][oj];
				//if (255 == (int)standard[i][j])
				//of << "1";
				//else
				//of << "0";
			} 
		}
		//of<<endl;
	}
	//of.close();
}


/****************************************
/*载入模板
*/
void CImageRecognize::LoadMod(int count, int num)
{
	CImageRecognize image;

	CString str = NULL;
	if (0 == num)
	{
		//str.Format(_T("D:\\modules\\%s.bmp"),HFonts[count]);
		str = HFonts[count];
	}
	else
	{
		//str.Format(_T("D:\\modules\\%c.bmp"),Fonts[count]);
		str.Format(_T("%c"),Fonts[count]);
	}
	image.AttachFromFile(_T(".\\modules\\")+str+_T(".bmp")); 
	int nImageHeight=image.GetHeight(); 
	int nImageWidth=image.GetWidthPixel();


	int   i,j,step;
	//int sum;
	//   读取图片数据 
	LPBYTE* data = new LPBYTE[nImageHeight];
	for(i=0;i <nImageHeight;i++) 
	{ 	
		data[i] = new BYTE[nImageWidth];
		for(j=0,step=0; j<nImageWidth;j++) 
		{ 
			data[i][j]=(BYTE)image.GetGray(j,i);

			/*if(j%3 == 2)
			step++;*/
		} 
	}
	//CutEdge(nImageHeight, nImageWidth);
	CharToStandard(module, data, nImageHeight, nImageWidth, num);
	return;
}
int CImageRecognize::isbd0q (int sample[ZOOMH][ZOOMW])
{
	int count;
	int m_result=0;
	int i,j;
	int sum=0;
	int height = 32;
	int wide = 16;
	for(i=height*3/4;i<height;i++)
	{
		for (j=wide/2;j<wide;j++)
		{
			if ( 255 == (int)sample[i][j])
			{
				sum++;
			}
		}
	}
	if (sum > 0.8*(height/4)*(wide/2))
	{
		m_result = 26;//is Q
		/*cout<<"sum is "<<sum<<endl;*/
	}
	else
	{

		sum=0;
		for (j=0;j<wide/6;j++)
		{
			for (i=0;i<height/2;i++)
			{
				if (255 == (int)sample[i][j])
				{
					sum++;
				}
				m_result ++;
			}
		}
		/*cout<<"isB,D sum = "<<sum<<endl;
		cout<<"循环此次= "<<m_result<<endl;*/
		if (sum >= 0.8*(wide/6)*(height/2))
		{
			sum=0;
			//B,D
			for (i=2*height/5;i< 3*height/5;i++)
			{
				for (j=0;j<wide;j++)
				{
					if ( 255 == (int)sample[i][j])
					{
						sum++;
					}
				}
			}
			if(sum >= 0.5*wide*(height/5))
				m_result = 11;//is B
			else
				m_result = 13;//is D
			/*cout<<"sum is "<<sum<<endl;*/
		} 
		else
		{
			//8,0
			sum=0;
			for (i=2*height/5;i< 3*height/5;i++)
			{
				for (j=0;j<wide;j++)
				{
					if ( 255 == (int)sample[i][j])
					{
						sum++;
					}
				}
			}
			if(sum >= 0.5*wide*(height/5))
				m_result = 8;//is 8
			else
				m_result = 0;//is 0
			/*cout<<"sum is "<<sum<<endl;*/
		}
	}
	return m_result;

}

void CImageRecognize::GetRect(CRect* rect)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		bool flag = false;
		for(int j=0;j<GetHeight();j++)
		{
			if (GetGray(i,j) == 255)
			{
				continue;
			}
			else
			{
				rect->left = i;
				flag = true;

				break;
			}
		}

		if (flag == true)
		{
			break;
		}
	}

	for(int i=GetWidthPixel()-1;i>=0;i--)
	{
		bool flag = false;
		for(int j=0;j<GetHeight();j++)
		{
			if (GetGray(i,j) == 255)
			{
				continue;
			}
			else
			{
				rect->right = i;
				flag = true;

				break;
			}
		}

		if (flag == true)
		{
			break;
		}
	}

	for(int i=0;i<GetHeight();i++)
	{
		bool flag = false;
		for(int j=0;j<GetWidthPixel();j++)
		{
			if (GetGray(j,i) == 255)
			{
				continue;
			}
			else
			{
				rect->top = i;
				flag = true;

				break;
			}
		}

		if (flag == true)
		{
			break;
		}
	}

	for(int i=GetHeight()-1;i>=0;i--)
	{
		bool flag = false;
		for(int j=0;j<GetWidthPixel();j++)
		{
			if (GetGray(j,i) == 255)
			{
				continue;
			}
			else
			{
				rect->bottom = i;
				flag = true;

				break;
			}
		}

		if (flag == true)
		{
			break;
		}
	}
}