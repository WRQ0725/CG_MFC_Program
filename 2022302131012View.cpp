
// 2022302131012View.cpp: CMy2022302131012View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "2022302131012.h"
#endif

#include "2022302131012Doc.h"
#include "2022302131012View.h"
#include "SetCharDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CMFCStatusBar m_wndStatusBar;

// CMy2022302131012View

IMPLEMENT_DYNCREATE(CMy2022302131012View, CView)

BEGIN_MESSAGE_MAP(CMy2022302131012View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2022302131012View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_DDALINE, &CMy2022302131012View::OnDrawDdaline)
	ON_COMMAND(ID_DRAW_BCIRCLE, &CMy2022302131012View::OnDrawBcircle)
	ON_COMMAND(ID_DRAW_MIDLINE, &CMy2022302131012View::OnDrawMidline)
	ON_COMMAND(ID_DRAW_PNCIRCLE, &CMy2022302131012View::OnDrawPncircle)
	ON_COMMAND(ID_CURVE_BEZIER, &CMy2022302131012View::OnCurveBezier)
	ON_WM_LBUTTONDBLCLK()
//	ON_COMMAND(ID_COLORSET, &CMy2022302131012View::OnColorset)
ON_COMMAND(ID_COLORSET, &CMy2022302131012View::OnColorset)
//ON_COMMAND(ID_WORD, &CMy2022302131012View::OnWord)
ON_COMMAND(ID_strSet, &CMy2022302131012View::OnStrset)
ON_COMMAND(ID_CURVE_BSAMPLE, &CMy2022302131012View::OnCurveBsample)
ON_COMMAND(ID_CURVE_HERMITE, &CMy2022302131012View::OnCurveHermite)
ON_COMMAND(ID_TRANS_MOVE, &CMy2022302131012View::OnTransMove)
ON_COMMAND(ID_TRANS_SYMMETRY, &CMy2022302131012View::OnTransSymmetry)
ON_COMMAND(ID_Clearall, &CMy2022302131012View::OnClearall)
ON_COMMAND(ID_TRANS_ROTATE, &CMy2022302131012View::OnTransRotate)
ON_COMMAND(ID_TRANS_ZOOM, &CMy2022302131012View::OnTransZoom)
ON_COMMAND(ID_FILL_SEED, &CMy2022302131012View::OnFillSeed)
ON_COMMAND(ID_FILL_EDGE, &CMy2022302131012View::OnFillEdge)
ON_COMMAND(ID_FILL_SCANLINE, &CMy2022302131012View::OnFillScanline)
END_MESSAGE_MAP()

// CMy2022302131012View 构造/析构

CMy2022302131012View::CMy2022302131012View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMy2022302131012View::~CMy2022302131012View()
{
}

BOOL CMy2022302131012View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy2022302131012View 绘图

void CMy2022302131012View::OnDraw(CDC* /*pDC*/)
{
	CMy2022302131012Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMy2022302131012View 打印


void CMy2022302131012View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy2022302131012View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy2022302131012View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy2022302131012View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMy2022302131012View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	/*ClientToScreen(&point);
	OnContextMenu(this, point);*/
}

void CMy2022302131012View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy2022302131012View 诊断

#ifdef _DEBUG
void CMy2022302131012View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2022302131012View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2022302131012Doc* CMy2022302131012View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2022302131012Doc)));
	return (CMy2022302131012Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2022302131012View 消息处理程序


void CMy2022302131012View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMy2022302131012Doc* pDoc = GetDocument(); //获得文档类指针
	CClientDC ht(this); //定义当前绘图设备
	if (MenuID == 1) { //DDA直线
		if (PressNum == 0) { //第一次按键将第一点保留在文档类数组中
			pDoc->group[PressNum] = point;
			mPointOrign = point; mPointOld = point;;//记录第一点
			PressNum++;
			SetCapture();
		}
		else if (PressNum == 1) { //第二次按键保留第二点，用文档类画线
			pDoc->group[PressNum] = point;
			PressNum = 0; //程序画图
			pDoc->DDALine(&ht);
			ReleaseCapture();
		}
	}
	if (MenuID == 2) { //中点画直线
		if (PressNum == 0) { //第一次按键将第一点保留在文档类数组中
			pDoc->group[PressNum] = point;
			mPointOrign = point; mPointOld = point;;//记录第一点
			PressNum++;
			SetCapture();
		}
		else if (PressNum == 1) { //第二次按键保留第二点，用文档类画线
			pDoc->group[PressNum] = point;
			PressNum = 0; //程序画图
			pDoc->MidLine(&ht);
			ReleaseCapture();
		}
	}

	if (MenuID == 3) {//Bresenham圆
		if (PressNum == 0) {//第一次按键将第一点保留在mPointOrign
			pDoc->group[PressNum] = point;
			PressNum++;
			mPointOrign = point;
			mPointOld = point;//记录第一点
			SetCapture();
		}
		else if (PressNum == 1) {//第二次按键调用文档类画圆程序画图
			PressNum = 0;
			pDoc->BCircle(&ht, mPointOrign, point); ReleaseCapture();
		}
	}

	if (MenuID == 4) {//正负法画圆
		if (PressNum == 0) {//第一次按键将第一点保留在mPointOrign
			pDoc->group[PressNum] = point;
			PressNum++;
			mPointOrign = point;
			mPointOld = point;//记录第一点
			SetCapture();
		}
		else if (PressNum == 1) {//第二次按键调用文档类画圆程序画图
			PressNum = 0;
			pDoc->PNCircle(&ht, mPointOrign, point); ReleaseCapture();
		}
	}
	if (MenuID == 5|| MenuID==7|| MenuID==9) {//三种曲线选点并做十字标志
		pDoc->group[pDoc->PointNum++] = point;
		ht.MoveTo(point.x - 5, point.y);
		ht.LineTo(point.x + 5, point.y);
		ht.MoveTo(point.x, point.y - 5);
		ht.LineTo(point.x, point.y + 5);
		SetCapture(); PressNum = 1;
	}

	if ((MenuID == 6|| MenuID==8 || MenuID==10) && PressNum == 0) {//在控制点数组中，逐个寻找
		for (int i = 0; i < pDoc->PointNum; i++)
		{
			if ((point.x >= pDoc->group[i].x - 5) && (point.x <= pDoc->group[i].x + 5)
				&& (point.y >= pDoc->group[i].y - 5) && (point.y <= pDoc->group[i].y + 5))
			{
				SaveNumber = i;
				PressNum = 1;
			}
		}
	}

	if (MenuID == 11) {//平移
		ht.SetROP2(R2_NOT);//设置异或方式
		if (PressNum == 0) {
			PressNum++;
			mPointOrign = point;
			mPointOld = point;//记录第一点
			SetCapture();
		}
		else if (PressNum == 1) { //根据两点间距计算平移量
			for (int i = 0; i < pDoc->PointNum; i++)//根据平移量计算新图形坐标
			{
				pDoc->group[i].x += point.x - mPointOrign.x;
				pDoc->group[i].y += point.y - mPointOrign.y;
			}
			ht.MoveTo(mPointOrign);//擦除橡皮筋
			ht.LineTo(point);
			pDoc->DrawGraph(&ht);//生成新图形
			ReleaseCapture();
			PressNum = 0;
		}
	}

	if (MenuID == 12) {
		//旋转
		if (PressNum == 0) {
			PressNum++;
			mPointOrign = point;
			mPointOld = point;//记录第一点
			SetCapture();
		}
		else if (PressNum == 1) {//根据两点间距计算旋转角度‘
			double dx = point.x - mPointOrign.x;
			double dy = -(point.y - mPointOrign.y);
			double k = dy / dx;
			double a = 0;//旋转角

			//根据直线的斜率确定旋转角，统一到顺时针旋转上
			if (dx > 0 && dy > 0) {
				a = 2 * pai - atan(k);//顺时针旋转a(锐角)，第一象限
			}
			else if (dx > 0 && dy < 0) {
				a = atan(-k);//逆时针旋转a(锐角)，第四象限
			}
			else if (dx < 0 && dy>0) {
				a = 2 * pai - (pai - atan(-k));//顺时针旋转a(钝角)，第二象限
			}
			else if (dx < 0 && dy < 0) {
				a = pai - atan(k);//逆时针旋转a(钝角)，第三象限
			}

			//旋转公式为rotateX=cos(a)*sourceX+sin(a)*sourceY；rotateY=-sin(a)*sourceX+cos(a)*sourceY
			for (int i = 0; i < pDoc->PointNum; i++) {
				pDoc->rotatePoint(mPointOrign, pDoc->group[i], a);
			}
			ht.DPtoLP(&point);
			ht.SetROP2(R2_NOT);
			ht.MoveTo(mPointOrign);//擦除橡皮筋
			ht.LineTo(point);
			pDoc->DrawGraph(&ht);//生成新图形
			ReleaseCapture();
			PressNum = 0;
		}
	}

	if (MenuID == 13) {
		//缩放
		if (PressNum == 0) {
			PressNum++;
			mPointOrign = point;
			mPointOld = point;//记录第一点
			SetCapture();
		}
		else if (PressNum == 1) {//根据两点间距计算缩放量
			int dx = point.x - mPointOrign.x;
			int dy = point.y - mPointOrign.y;

			//根据直线的斜率确定缩放比例，d>0为放大，d<0为缩小
			//根据平移量计算新图形坐标
			pDoc->group[0].x -= (dx / 2);
			pDoc->group[0].y += (dy / 2);
			pDoc->group[1].x += (dx / 2);
			pDoc->group[1].y += (dy / 2);
			pDoc->group[2].x += (dx / 2);
			pDoc->group[2].y -= (dy / 2);
			pDoc->group[3].x -= (dx / 2);
			pDoc->group[3].y -= (dy / 2);
			pDoc->group[4].x -= (dx / 2);
			pDoc->group[4].y += (dy / 2);

			ht.DPtoLP(&point);
			ht.SetROP2(R2_NOT);
			ht.MoveTo(mPointOrign);//擦除橡皮筋
			ht.LineTo(point);
			pDoc->DrawGraph(&ht);//生成新图形
			ReleaseCapture();
			PressNum = 0;
		}
	}

	if (MenuID == 15) {//对称变换
		if (PressNum == 0) {
			PressNum++;
			mPointOrign = point;
			mPointOld = point;//记录第一点
			SetCapture();
		}
		else if (PressNum == 1) {
			pDoc->Symmetry(mPointOrign, point);//进行对称变换
			pDoc->DrawGraph(&ht);//生成新图形
			ReleaseCapture();
			PressNum = 0;
		}
	}

	if (MenuID == 20) {//种子填充:画边界
		if (PressNum == 0) {
			mPointOrign = point;
			mPointOld = point;
			mPointOld1 = point;//记录第一点
			PressNum++;
			SetCapture();
		}
		else {
			ht.SetROP2(R2_NOT);
			ht.MoveTo(mPointOrign);//擦除橡皮筋
			ht.LineTo(point);
			pDoc->group[0] = mPointOrign;//借助DDA直线函数画边界
			pDoc->group[1]=point;
			pDoc->DDALine(&ht);
			mPointOrign = point;
			mPointOld = point;
			PressNum++;
		}
	}
	if (MenuID == 21) {//确定种子点，填充
		pDoc->SeedFill(&ht, point);
		PressNum = 0; MenuID = 20;//设置决定顶点操作方式
	}

	if (MenuID == 22|| MenuID==23) {//边缘填充选顶点,//种子填充算法
		pDoc->group[PressNum++] = point;
		pDoc->PointNum++;
		mPointOrign = point;
		mPointOld = point;
		SetCapture();
	}
	CView::OnLButtonDown(nFlags, point);
		
	}

void CMy2022302131012View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC ht(this); //定义当前绘图设备
	OnPrepareDC(&ht);
	ht.DPtoLP(&point);
	if (MenuID == 5 && pDoc->PointNum > 3) {
		pDoc->Bezier(&ht, 1);//绘制Bezier函数
		MenuID = 6; //将下面的操作改为修改控制点位置
		PressNum = 0;
	}
	if (MenuID == 7 && pDoc->PointNum > 3) {
		pDoc->Bsample(&ht, 1);//绘制B样条函数
		MenuID = 8; //将下面的操作改为修改控制点位置
		PressNum = 0;
	}
	if (MenuID == 9 && pDoc->PointNum > 3) {
		pDoc->Hermite(&ht, 1);//绘制B样条函数
		MenuID = 10; //将下面的操作改为修改控制点位置
		PressNum = 0;
	}
	if (MenuID == 6 && PressNum == 1) {
		PressNum = 0;
	}
	if (MenuID == 8 && PressNum == 1) {
		PressNum = 0;
	}
	if (MenuID == 10 && PressNum == 1) {
		PressNum = 0;
	}
	if (MenuID == 20 && PressNum > 0) {//种子填充
		ht.SetROP2(R2_NOT);
		ht.MoveTo(mPointOrign);//擦除橡皮筋
		ht.LineTo(point);
		pDoc->group[0] = mPointOld1;//封闭多边形
		pDoc->group[1] = mPointOrign;
		pDoc->DDALine(&ht);
		PressNum = 0; MenuID = 21;//改变操作方式为种子点选取
		ReleaseCapture();
	}
	if (MenuID == 22) {//边缘填充选点结束
		ht.SetROP2(R2_NOT);
		ht.MoveTo(mPointOrign);//擦除橡皮筋
		ht.LineTo(point);
		pDoc->group[PressNum] = pDoc->group[0];
		pDoc->PointNum++;
		ht.MoveTo(pDoc->group[PressNum - 1]);
		ht.LineTo(pDoc->group[0]);
		for (int i = 0; i < PressNum; i++) {
			ht.LineTo(pDoc->group[i + 1]);
		}
		pDoc->EdgeFill(&ht);
		PressNum = 0; pDoc->PointNum = 0;
		ReleaseCapture();
	}

	if (MenuID == 23) {
		ht.SetROP2(R2_NOT);
		ht.MoveTo(mPointOrign);//擦除橡皮筋
		ht.LineTo(point);
		pDoc->group[PressNum] = pDoc->group[0];//封闭多边形
		ht.MoveTo(pDoc->group[PressNum - 1]);//擦除
		ht.LineTo(pDoc->group[0]);
		for (int i = 0; i < PressNum; i++)//擦除
			ht.LineTo(pDoc->group[i + 1]);
		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));//设置多边形边界颜色（即画笔）
		CPen* pOldPen = ht.SelectObject(&pen);
		CBrush brush(RGB(0, 255, 0)); //设置多边形填充颜色（即画刷）
		CBrush* pOldBrush = ht.SelectObject(&brush);
		ht.SetROP2(R2_COPYPEN); //设置直接画方式
		ht.Polygon(pDoc->group, PressNum);//调用多边形扫描线填充函数
		ht.SelectObject(pOldPen);//恢复系统的画笔、画刷颜色设置
		ht.SelectObject(pOldBrush);
		PressNum = 0; pDoc->PointNum = 0;//初始化参数，为下一次操作做准备
		ReleaseCapture();
	}
	CView::OnRButtonDown(nFlags, point);
}


void CMy2022302131012View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int xx, yy;
	char p1[20];
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDC.DPtoLP(&point);
	pDC.SetROP2(R2_NOT);//设置异或方式xx=point.x; yy = point.y; //取出坐标信息
	xx = point.x; yy = point.y; //取出坐标信息
	sprintf_s(p1, "%4d", xx); //转化为字符串
	m_wndStatusBar.SetPaneText(2, p1, TRUE); //在第2个区域显示x坐标
	sprintf_s(p1, "%4d", yy); //转化为字符串
	m_wndStatusBar.SetPaneText(3, p1, TRUE); //在第3个区域显示y坐标

	//以下是各个方法的橡皮筋技术实现
	if ((MenuID == 1 || MenuID == 2 || MenuID==11 || MenuID==15 || MenuID==12 || MenuID==13) && PressNum ==1) {
		if (mPointOld != point) {
			pDC.MoveTo(mPointOrign); pDC.LineTo(mPointOld);//擦旧线
			pDC.MoveTo(mPointOrign);
			pDC.LineTo(point);//画新线
			mPointOld = point;
		}
	}
	if ((MenuID == 20|| MenuID==22||MenuID==23) && PressNum > 0 ) {
		if (mPointOld != point) {
			pDC.MoveTo(mPointOrign); pDC.LineTo(mPointOld);//擦旧线
			pDC.MoveTo(mPointOrign);
			pDC.LineTo(point);//画新线
			mPointOld = point;
		}
	}

	if ((MenuID == 3 || MenuID==4) && PressNum == 1) {
		pDC.SelectStockObject(NULL_BRUSH);//画空心圆
		if (mPointOld != point) {
			r = (int)sqrt((mPointOrign.x - mPointOld.x) * (mPointOrign.x-mPointOld.x) + (mPointOrign.y - mPointOld.y) * (mPointOrign.y - mPointOld.y));
			pDC.Ellipse(mPointOrign.x - r, mPointOrign.y - r, mPointOrign.x + r, mPointOrign.y + r);//擦旧圆
			r = (int)sqrt((mPointOrign.x - point.x) * (mPointOrign.x - point.x) +
				(mPointOrign.y - point.y) * (mPointOrign.y - point.y));
			pDC.Ellipse(mPointOrign.x - r, mPointOrign.y - r, mPointOrign.x + r, mPointOrign.y + r);//画新圆
			mPointOld = point;
		}
	}

	if (MenuID == 6 && PressNum > 0) {
		if (pDoc->group[SaveNumber] != point)
		{
			pDC.MoveTo(pDoc->group[SaveNumber].x - 5, pDoc->group[SaveNumber].y);
			pDC.LineTo(pDoc->group[SaveNumber].x + 5, pDoc->group[SaveNumber].y);
			pDC.MoveTo(pDoc->group[SaveNumber].x, pDoc->group[SaveNumber].y - 5);
			pDC.LineTo(pDoc->group[SaveNumber].x, pDoc->group[SaveNumber].y + 5);
			pDoc->Bezier(&pDC, 1);//擦除十字标志和旧线
			pDC.MoveTo(point.x - 5, point.y);
			pDC.LineTo(point.x + 5, point.y);
			pDC.MoveTo(point.x, point.y - 5);
			pDC.LineTo(point.x, point.y + 5);
			pDoc->group[SaveNumber] = point;//记录新控制点
			pDoc->Bezier(&pDC, 1);//画十字标志和新曲线
		}
	}
	if (MenuID == 8 && PressNum > 0) {
		if (pDoc->group[SaveNumber] != point)
		{
			pDC.MoveTo(pDoc->group[SaveNumber].x - 5, pDoc->group[SaveNumber].y);
			pDC.LineTo(pDoc->group[SaveNumber].x + 5, pDoc->group[SaveNumber].y);
			pDC.MoveTo(pDoc->group[SaveNumber].x, pDoc->group[SaveNumber].y - 5);
			pDC.LineTo(pDoc->group[SaveNumber].x, pDoc->group[SaveNumber].y + 5);
			pDoc->Bsample(&pDC, 1);//擦除十字标志和旧线
			pDC.MoveTo(point.x - 5, point.y);
			pDC.LineTo(point.x + 5, point.y);
			pDC.MoveTo(point.x, point.y - 5);
			pDC.LineTo(point.x, point.y + 5);
			pDoc->group[SaveNumber] = point;//记录新控制点
			pDoc->Bsample(&pDC, 1);//画十字标志和新曲线
		}
	}
	if (MenuID == 10 && PressNum > 0) {
		if (pDoc->group[SaveNumber] != point)
		{
			pDC.MoveTo(pDoc->group[SaveNumber].x - 5, pDoc->group[SaveNumber].y);
			pDC.LineTo(pDoc->group[SaveNumber].x + 5, pDoc->group[SaveNumber].y);
			pDC.MoveTo(pDoc->group[SaveNumber].x, pDoc->group[SaveNumber].y - 5);
			pDC.LineTo(pDoc->group[SaveNumber].x, pDoc->group[SaveNumber].y + 5);
			pDoc->Hermite(&pDC, 1);//擦除十字标志和旧线
			pDC.MoveTo(point.x - 5, point.y);
			pDC.LineTo(point.x + 5, point.y);
			pDC.MoveTo(point.x, point.y - 5);
			pDC.LineTo(point.x, point.y + 5);
			pDoc->group[SaveNumber] = point;//记录新控制点
			pDoc->Hermite(&pDC, 1);//画十字标志和新曲线
		}
	}
	CView::OnMouseMove(nFlags, point);
}



void CMy2022302131012View::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDC.DPtoLP(&point);
	pDC.SetROP2(R2_NOT);//设置异或方式
	if (MenuID == 6) {
		for (int i = 0; i < pDoc->PointNum; i++) {//消除所有光标
			pDC.MoveTo(pDoc->group[i].x - 5, pDoc->group[i].y);
			pDC.LineTo(pDoc->group[i].x + 5, pDoc->group[i].y);
			pDC.MoveTo(pDoc->group[i].x, pDoc->group[i].y - 5);
			pDC.LineTo(pDoc->group[i].x, pDoc->group[i].y + 5);
		}
		pDoc->Bezier(&pDC, 0);//绘制Bezier函数
		MenuID = 5; //将下面的操作改回Bezier曲线方式
		PressNum = 0;
		pDoc->PointNum = 0;
		ReleaseCapture();
	}
	if (MenuID == 8) {
		for (int i = 0; i < pDoc->PointNum; i++) {//消除所有光标
			pDC.MoveTo(pDoc->group[i].x - 5, pDoc->group[i].y);
			pDC.LineTo(pDoc->group[i].x + 5, pDoc->group[i].y);
			pDC.MoveTo(pDoc->group[i].x, pDoc->group[i].y - 5);
			pDC.LineTo(pDoc->group[i].x, pDoc->group[i].y + 5);
		}
		pDoc->Bsample(&pDC, 0);//绘制B样条函数
		MenuID = 7; //将下面的操作改回B样条曲线方式
		PressNum = 0;
		pDoc->PointNum = 0;
		ReleaseCapture();
	}
	if (MenuID == 10) {
		for (int i = 0; i < pDoc->PointNum; i++) {//消除所有光标
			pDC.MoveTo(pDoc->group[i].x - 5, pDoc->group[i].y);
			pDC.LineTo(pDoc->group[i].x + 5, pDoc->group[i].y);
			pDC.MoveTo(pDoc->group[i].x, pDoc->group[i].y - 5);
			pDC.LineTo(pDoc->group[i].x, pDoc->group[i].y + 5);
		}
		pDoc->Hermite(&pDC, 0);//绘制Hermite函数
		MenuID = 9; //将下面的操作改回Hermite曲线方式
		PressNum = 0;
		pDoc->PointNum = 0;
		ReleaseCapture();
	}
	CView::OnLButtonDblClk(nFlags, point);
}


void CMy2022302131012View::OnDrawDdaline()
{
	// TODO: 在此添加命令处理程序代码
	PressNum = 0; MenuID = 1;

}


void CMy2022302131012View::OnDrawBcircle()
{
	// TODO: 在此添加命令处理程序代码
	PressNum = 0; MenuID = 3;
}


void CMy2022302131012View::OnDrawMidline()
{
	// TODO: 在此添加命令处理程序代码
	PressNum = 0; MenuID = 2;
}


void CMy2022302131012View::OnDrawPncircle()
{
	// TODO: 在此添加命令处理程序代码
	PressNum = 0; MenuID = 4;
}


void CMy2022302131012View::OnCurveBezier()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	pDoc->PointNum = 0;//初始化
	PressNum = 0; MenuID = 5;

}




void CMy2022302131012View::OnColorset()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CColorDialog dlg(pDoc->m_crColor);//创建对话框对象，并让其默认颜色为
	//上一次选择的颜色
	if (dlg.DoModal() == IDOK)
		pDoc->m_crColor = dlg.GetColor();

}


//void CMy2022302131012View::OnWord()
//{
	// TODO: 在此添加命令处理程序代码
//	CDC* pDC = GetDC();
//	CSetCharDlg dlg;
//	if (dlg.DoModal() == IDOK) {
//		CFont* pfntOld = pDC->SelectObject(&dlg.m_fnt);//保存旧字体
//		pDC->SetTextColor(dlg.m_clrText);//设置颜色
//		pDC->TextOut(dlg.x, dlg.y, dlg.C_str);//画到屏幕上
//		pDC->SelectObject(pfntOld);//还原旧字体
//	}
//	ReleaseDC(pDC);
//}


void CMy2022302131012View::OnStrset()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CSetCharDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CFont* pfntOld = pDC->SelectObject(&dlg.m_fnt);//保存旧字体
		pDC->SetTextColor(dlg.m_clrText);//设置颜色
		pDC->TextOut(dlg.x, dlg.y, dlg.C_str);//画到屏幕上
		pDC->SelectObject(pfntOld);//还原旧字体
	}
	ReleaseDC(pDC);
}


void CMy2022302131012View::OnCurveBsample()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	pDoc->PointNum = 0;//初始化
	PressNum = 0; MenuID = 7;
}


void CMy2022302131012View::OnCurveHermite()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	pDoc->PointNum = 0;//初始化
	PressNum = 0; MenuID = 9;
}


void CMy2022302131012View::OnTransMove()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDoc->GenerateGraph(&pDC);//调用文档类函数在屏幕上生成图形
	PressNum = 0; MenuID = 11;
}


void CMy2022302131012View::OnTransSymmetry()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDoc->GenerateGraph(&pDC);
	PressNum = 0; MenuID = 15;
}


void CMy2022302131012View::OnClearall()
{
	// TODO: 在此添加命令处理程序代码
	// 清空绘制区域
	CRect rectDlg;
	GetClientRect(rectDlg);			// 获得窗体的大小
	int pointWidth = rectDlg.Width();		// 获取窗体宽度
	int pointHeight = rectDlg.Height();		// 获取窗体高度
	RedrawWindow(CRect(0, 0, pointWidth, pointHeight));		// 重绘指定区域
}


void CMy2022302131012View::OnTransRotate()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDoc->GenerateGraph2(&pDC);//调用文档类函数在屏幕上生成图形
	PressNum = 0; MenuID = 12;
}


void CMy2022302131012View::OnTransZoom()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDoc->GenerateGraph2(&pDC);//调用文档类函数在屏幕上生成图形
	PressNum = 0; MenuID = 13;
}


void CMy2022302131012View::OnFillSeed()
{
	// TODO: 在此添加命令处理程序代码
	PressNum = 0; MenuID = 20;
}


void CMy2022302131012View::OnFillEdge()
{
	// TODO: 在此添加命令处理程序代码
	PressNum = 0; MenuID = 22;
}


void CMy2022302131012View::OnFillScanline()
{
	// TODO: 在此添加命令处理程序代码
	CMy2022302131012Doc* pDoc = GetDocument();//获得文档类指针
	pDoc->PointNum = 0;//实际上不需要该变量，但为了借鉴边缘填充的部分功能，与边缘填充保持一致
	PressNum = 0; MenuID = 23;
}
