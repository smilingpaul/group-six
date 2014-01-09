#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CSupplatectrl1 包装类

class CSupplatectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CSupplatectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x9BFF5271, 0x2917, 0x463D, { 0x8C, 0x77, 0x9F, 0xB6, 0xD2, 0xB5, 0x97, 0xA3 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, 
		CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
		UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
			pPersist, bStorage, bstrLicKey); 
	}

	// 属性
public:


	// 操作
public:

	// _DSupPlate

	// Functions
	//

	void SetVideoDetectParam()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SendComOrder(long lOrder)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lOrder);
	}
	void AllowDetect(long lOrder, BOOL bDetect)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lOrder, bDetect);
	}
	long GetDirNum()
	{
		long result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetDirRoadNum(long lDirIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lDirIndex);
		return result;
	}
	void TesttHandleBreak(long lDirIndex, long lRoadIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lDirIndex, lRoadIndex);
	}
	long GetDetectFlux(long lDirIndex, long lRoadIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lDirIndex, lRoadIndex);
		return result;
	}
	long GetPeccancyCount(long lDirIndex, long lRoadIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lDirIndex, lRoadIndex);
		return result;
	}
	float GetSpeed(long lDirIndex, long lRoadIndex)
	{
		float result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_R4, (void*)&result, parms, lDirIndex, lRoadIndex);
		return result;
	}
	void RecogniseBuf(long * lpImageBuf, long nImgWidth, long nImgHeight)
	{
		static BYTE parms[] = VTS_PI4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpImageBuf, nImgWidth, nImgHeight);
	}
	void RecognisePicFile(LPCTSTR strPicFile)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strPicFile);
	}
	void SetPlateMode(long nPlateMode)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nPlateMode);
	}
	long SaveImageFile(LPCTSTR strSavePicFile, long * lpSaveImageBuf, long nSaveImgW, long nSaveImgH)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PI4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strSavePicFile, lpSaveImageBuf, nSaveImgW, nSaveImgH);
		return result;
	}
	long GetPlateImage(long nPlateInndex, long * lpPlateImageBuf, long nPlateImgW, long nPlateImgH)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nPlateInndex, lpPlateImageBuf, nPlateImgW, nPlateImgH);
		return result;
	}
	long GetRecogImgWH(LPCTSTR strPicFile, long * nImageW, long * nImageH)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strPicFile, nImageW, nImageH);
		return result;
	}
	long GetRecogImgBuf(LPCTSTR strPicFile, long * lpImgBuf)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strPicFile, lpImgBuf);
		return result;
	}
	CString GetPlateNumber(long nPlateIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nPlateIndex);
		return result;
	}
	long GetPlateColorNumber(long nPlateIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nPlateIndex);
		return result;
	}
	CString GetPlateColorString(long nPlateIndex, long nLanguageType)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nPlateIndex, nLanguageType);
		return result;
	}
	long GetPlateRect(long nPlateIndex, long * nPlateLeft, long * nPlateRight, long * nPlateTop, long * nPlateBottom)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nPlateIndex, nPlateLeft, nPlateRight, nPlateTop, nPlateBottom);
		return result;
	}
	long GetNearImage(long * lpNearImg)
	{
		long result;
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lpNearImg);
		return result;
	}
	long GetFarImage(long nFarImgIndex, long * lpFarImg)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nFarImgIndex, lpFarImg);
		return result;
	}
	void DlgSetVideoDetectMode()
	{
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RecogKinescope(LPCTSTR strKinescope)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strKinescope);
	}
	long SetKinescopeStat(long nOrder)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nOrder);
		return result;
	}
	void OnOcrDBManage()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetPassedCarInfo(LPUNKNOWN PassedCarInfo)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, PassedCarInfo);
	}
	void SetRecogParam()
	{
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RegistSupPlate()
	{
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long SetVideoDetectMode(long nDetectMode)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nDetectMode);
		return result;
	}
	CString GetModulPath()
	{
		CString result;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void RecogniseCamera()
	{
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetCameraPin()
	{
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetCameraFilter()
	{
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void FinishSetLightStat()
	{
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetRoadLightStat(long nDirIndex, long nRoadIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nDirIndex, nRoadIndex);
		return result;
	}
	void SetRoadLightStat(long nDirIndex, long nRoadIndex, long bIsRedLight)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nDirIndex, nRoadIndex, bIsRedLight);
	}
	long GetDirRoadID(long nDirIndex, long * lDirID, long nRoadIndex, long * lRoadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nDirIndex, lDirID, nRoadIndex, lRoadID);
		return result;
	}
	void RecogKinescopeRealTime(LPCTSTR strKinescope)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strKinescope);
	}
	void KinescopeDetectCar(LPCTSTR strKinescope)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strKinescope);
	}
	void SetRecogSourceCompressble(long nCompress)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nCompress);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// Properties
	//

	long GetDisplayIndex()
	{
		long result;
		GetProperty(0x1, VT_I4, (void*)&result);
		return result;
	}
	void SetDisplayIndex(long propVal)
	{
		SetProperty(0x1, VT_I4, propVal);
	}


};
