// SetCharDlg.cpp: 实现文件
//

#include "pch.h"
#include "2022302131012.h"
#include "afxdialogex.h"
#include "SetCharDlg.h"


// CSetCharDlg 对话框

IMPLEMENT_DYNAMIC(CSetCharDlg, CDialogEx)

CSetCharDlg::CSetCharDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARBOX, pParent)
	, C_str(_T(""))
	, x(0)
	, y(0)
{
	m_clrText = RGB(0, 0, 0);
}

CSetCharDlg::~CSetCharDlg()
{
}

void CSetCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, ID_EDIT_CHAR, m_strString);
	DDX_Text(pDX, ID_EDIT_CHAR, C_str);
	DDX_Text(pDX, ID_EDIT_X, x);
	DDX_Text(pDX, ID_EDIT_Y, y);
}


//BEGIN_MESSAGE_MAP(CSetCharDlg, CDialogEx)
//	ON_BN_CLICKED(IDC_BUTTON1, &CSetCharDlg::OnBnClickedButton1)
//END_MESSAGE_MAP()


// CSetCharDlg 消息处理程序


void CSetCharDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
BEGIN_MESSAGE_MAP(CSetCharDlg, CDialogEx)
	ON_BN_CLICKED(IDC_WORDSTYLE, &CSetCharDlg::OnClickedWordstyle)
END_MESSAGE_MAP()


void CSetCharDlg::OnClickedWordstyle()
{
	// TODO: 在此添加控件通知处理程序代码
	CFontDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_fnt.DeleteObject();
		LOGFONT LogFnt;
		dlg.GetCurrentFont(&LogFnt);//保存所选字体
		m_fnt.CreateFontIndirect(&LogFnt);//创建所选字体
		m_clrText = dlg.GetColor();//获得所选颜色
	}
}
