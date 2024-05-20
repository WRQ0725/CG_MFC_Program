
// 2022302131012.h: 2022302131012 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号
#include "math.h"

// CMy2022302131012App:
// 有关此类的实现，请参阅 2022302131012.cpp
//

class CMy2022302131012App : public CWinAppEx
{
public:
	CMy2022302131012App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy2022302131012App theApp;
