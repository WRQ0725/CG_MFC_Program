
// 2022302131012View.h: CMy2022302131012View 类的接口
//

#pragma once


class CMy2022302131012View : public CView
{
protected: // 仅从序列化创建
	int MenuID, PressNum,SaveNumber;
	CPoint mPointOrign, mPointOld,mPointOld1;
	int r;
	double pai = 3.1415926;
	CMy2022302131012View() noexcept;
	DECLARE_DYNCREATE(CMy2022302131012View)
// 特性
public:
	CMy2022302131012Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMy2022302131012View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawDdaline();
	afx_msg void OnDrawBcircle();
	afx_msg void OnDrawMidline();
	afx_msg void OnDrawPncircle();
	afx_msg void OnCurveBezier();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnColorset();
	afx_msg void OnColorset();
//	afx_msg void OnWord();
	afx_msg void OnStrset();
	afx_msg void OnCurveBsample();
	afx_msg void OnCurveHermite();
	afx_msg void OnTransMove();
	afx_msg void OnTransSymmetry();
	afx_msg void OnClearall();
	afx_msg void OnTransRotate();
	afx_msg void OnTransZoom();
	afx_msg void OnFillSeed();
	afx_msg void OnFillEdge();
	afx_msg void OnFillScanline();
};

#ifndef _DEBUG  // 2022302131012View.cpp 中的调试版本
inline CMy2022302131012Doc* CMy2022302131012View::GetDocument() const
   { return reinterpret_cast<CMy2022302131012Doc*>(m_pDocument); }
#endif

