
// 2022302131012Doc.h: CMy2022302131012Doc 类的接口
//


#pragma once


class CMy2022302131012Doc : public CDocument
{
protected: // 仅从序列化创建
	CMy2022302131012Doc() noexcept;
	DECLARE_DYNCREATE(CMy2022302131012Doc)

// 特性
public:
	CPoint group[100];//定义数组，存放鼠标选点
	int PointNum;
	COLORREF m_crColor;//保存图形颜色
// 操作
public:
	void DDALine(CClientDC* DCPoint);//定义函数
	void MidLine(CClientDC* DCPoint);
	void midline(CClientDC* DCPoint, int flag, int x0, int y0, int x1, int y1);
	void BCircle(CClientDC* DCPoint, CPoint p1, CPoint p2);
	void PNCircle(CClientDC* DCPoint, CPoint p1, CPoint p2);
	void Bezier(CClientDC* DCPoint, int mode);
	void Bezier_4(CClientDC* DCPoint, int mode, CPoint p1, CPoint p2, CPoint p3, CPoint p4);
	void Bezier_3(CClientDC* DCPoint, int mode, CPoint p1, CPoint p2, CPoint p3);
	void Bsample(CClientDC* DCPoint, int mode);
	void Hermite(CClientDC* DCPoint, int mode);
	void GenerateGraph(CClientDC* DCPoint);
	void DrawGraph(CClientDC* DCPoint);
	void Symmetry(CPoint p1, CPoint p2);
	void rotatePoint(CPoint center, CPoint& point, double angle);
	void GenerateGraph2(CClientDC* DCPoint);
	void SeedFill(CClientDC* pDC, CPoint seedpoint);
	void EdgeFill(CClientDC* pDC);
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CMy2022302131012Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
