#pragma once
#include "afxdialogex.h"


// CSetCharDlg 对话框

class CSetCharDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetCharDlg)

public:
	CSetCharDlg(CWnd* pParent = nullptr);   // 标准构造函数
	CFont m_fnt;//保存字体
	COLORREF m_clrText;//保存颜色
	virtual ~CSetCharDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg void OnBnClickedButton1();
//	CEdit m_strString;
	CString C_str;
	int x;
	int y;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickedWordstyle();
};
