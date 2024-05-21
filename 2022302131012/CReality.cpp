#include "pch.h"
#include "CReality.h"

void CReality::AddObject3D(CObject3D* pObject)
{
	m_count++;
	for (int i = 0; i < pObject->m_SurfaceList.GetSize(); i++)
	{
		TriSurface surface = (TriSurface)pObject->m_SurfaceList.GetAt(i);
		//背向视点方向的面不可见
		if (GetVectorPM(surface.xn, surface.yn, surface.zn, lxn, lyn, lzn) >= 0)
		{
			surface.no = m_count;//设置当前面对应的光照参数
			m_SurfaceList.Add(surface);
		}
	}
	//设置景物参数
	m_ParamList.Add(pObject->m_Param);
}

void CReality::AddLight(LightParam lightParam)
{
	//计算距离衰减
	double f = 1.0 / (lightParam.c1 + lightParam.c2 * lightParam.m_distance +
		lightParam.c3 * lightParam.m_distance * lightParam.m_distance);
	if (f <= 0 || f > 1)
		f = 1.0;
	lightParam.f = f;
	//添加到光源列表中
	m_LightList.Add(lightParam);
}

void CReality::SetIA(double r, double g, double b)
{
	ira = r; iga = g; iba = b;
}

double CReality::GetVectorPM(double xn1, double yn1, double zn1, double xn2, double yn2, double zn2)
{
	return (xn1 * xn2 + yn1 * yn2 + zn1 * zn2) /
		(sqrt(xn1 * xn1 + yn1 * yn1 + zn1 * zn1) * sqrt(xn2 * xn2 + yn2 * yn2 + zn2 * zn2));
}

CPoint CReality::Projection(Point3D point3d)
{
	CPoint point;
	point.x = (long)(point3d.x + point3d.z * 0.5 * cos(0));
	point.y = 800 - (long)(point3d.y + point3d.z * 0.5 * sin(0));
	return point;
}

void CReality::Lambert(CDC* pDC)
{
	int no = 0;
	//景物表面三颜色分量的泛光反射率和漫反射率
	double kra=0, kga=0, kba=0, krd, kgd, kbd;
	double cosnl;//景物表面单位法向量与入射光单位向量间夹角的余弦值
	double ird, igd, ibd;//景物表面三角面反射的三颜色光强度
	//循环处理每一个三角面
	for (int i = 0; i < m_SurfaceList.GetSize(); i++)
	{
		//获得景物表面三角面
		TriSurface surface = (TriSurface)m_SurfaceList.GetAt(i);
		//获得当前景物的参数
		if (no != surface.no)
		{
			no = surface.no;
			Param param = (Param)m_ParamList.GetAt(no - 1);
			kra = param.kra; kga = param.kga; kba = param.kba;
			krd = param.krd; kgd = param.kgd; kbd = param.kbd;
		}
		//计算景物表面三角面反射的三颜色光强度
		ird = ira * kra; igd = iga * kga; ibd = iba * kba;
		//循环光源列表
		for (int j = 0; j < m_LightList.GetSize(); j++)
		{
			//获得光源参数
			LightParam light = (LightParam)m_LightList.GetAt(j);
			//计算景物表面单位法向量与入射光单位向量间夹角的余弦值
			cosnl = GetVectorPM(surface.xn, surface.yn, surface.zn,
				light.xn, light.yn, light.zn);
			//累加计算景物表面在多个光源照射下的三颜色分量光强度
			ird = ird + light.f * krd * light.irl * cosnl;
			igd = igd + light.f * kgd * light.igl * cosnl;
			ibd = ibd + light.f * kbd * light.ibl * cosnl;
		}
		if (ird < 0) ird = 0; if (ird > 255) ird = 255;
		if (igd < 0) igd = 0; if (igd > 255) igd = 255;
		if (ibd < 0) ibd = 0; if (ibd > 255) ibd = 255;
		//计算三角面的投影
		CPoint p[3];
		p[0] = Projection(surface.p1);
		p[1] = Projection(surface.p2);
		p[2] = Projection(surface.p3);
		//绘制三角面
		CPen pen(PS_SOLID, 1, RGB((int)ird, (int)igd, (int)ibd));
		CBrush brush(RGB((int)ird, (int)igd, (int)ibd));
		pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);
		pDC->Polygon(p, 3);
		brush.DeleteObject();
		pen.DeleteObject();
	}
}

void CReality::Clear()
{
	m_SurfaceList.RemoveAll();
	m_LightList.RemoveAll();
	m_ParamList.RemoveAll();
	m_count = 0;
}

void CReality::PhongScan(CDC* pDC)
{
	//顶点对应的投影点的最大、最小、中间y坐标值
	int ymax, ymin, ymid;
	//扫描线扫过的三角形两个边上点的向量
	double nax, nay, naz, nbx, nby, nbz;
	//三角形边点和扫描线上点向量增量
	double dnax, dnay, dnaz, dnbx, dnby, dnbz, dnpx=0, dnpy=0, dnpz=0;
	//三角形三条边斜率的倒数
	double f1, f2, f3;
	//三角形在扫描线上区段的x坐标对
	double xa, xb;
	//要计算光照点的向量
	double xn, yn, zn;
	//循环三角面列表
	for (int i = 0; i < m_SurfaceList.GetSize(); i++)
	{
		//获得景物表面三角面
		TriSurface surface = (TriSurface)m_SurfaceList.GetAt(i);
		//获得顶点
		Point3D p1 = surface.p1;
		Point3D p2 = surface.p2;
		Point3D p3 = surface.p3;
		//对顶点进行投影
		CPoint tp1 = Projection(p1);
		CPoint tp2 = Projection(p2);
		CPoint tp3 = Projection(p3);
		//设置投影点坐标
		p1.tx = tp1.x; p1.ty = tp1.y;
		p2.tx = tp2.x; p2.ty = tp2.y;
		p3.tx = tp3.x; p3.ty = tp3.y;
		//按投影点y坐标值排序
		//排序是为了保证p1点对应的投影点y坐标值最大
		//p3点对应的投影点y坐标值最小
		//这样就保证了在扫描线进行扫描时p1p3边上点的向量增量不用重新计算
		//对于另外两条边来说，扫描线先扫过P2p3边，再扫过p1p2边
		Sort(&p1, &p2, &p3);
		//计算三条边的斜率的倒数
		if (p1.ty != p3.ty) f1 = (double)(p3.tx - p1.tx) / (p3.ty - p1.ty);
		if (p1.ty != p2.ty) f2 = (double)(p2.tx - p1.tx) / (p2.ty - p1.ty);
		if (p2.ty != p3.ty) f3 = (double)(p3.tx - p2.tx) / (p3.ty - p2.ty);
		//设置顶点对应的投影点的最大、最小、中间y坐标值
		//其中ymin是扫描线起始值
		//ymax是扫描线终止值
		//ymid用于判断需要重新计算向量增量
		ymax = (int)p1.ty; ymid = (int)p2.ty; ymin = (int)p3.ty;
		//判断是否p2p3边平行于x轴
		if (p2.ty == p3.ty)
		{
			//p2p3边平行于x轴，扫描线不用扫过该边
			xa = p2.tx; xb = p3.tx;
			nax = p2.xn; nay = p2.yn; naz = p2.zn;
			nbx = p3.xn; nby = p3.yn; nbz = p3.zn;
			dnax = (p1.xn - p2.xn) / (p1.ty - p2.ty);
			dnay = (p1.yn - p2.yn) / (p1.ty - p2.ty);
			dnaz = (p1.zn - p2.zn) / (p1.ty - p2.ty);
			dnbx = (p1.xn - p3.xn) / (p1.ty - p3.ty);
			dnby = (p1.yn - p3.yn) / (p1.ty - p3.ty);
			dnbz = (p1.zn - p3.zn) / (p1.ty - p3.ty);
			f3 = f2;
		}
		else
		{
			//p2p3边不平行于x轴，扫描线先扫过该边
			xa = xb = p3.tx;
			dnax = (p2.xn - p3.xn) / (p2.ty - p3.ty);
			dnay = (p2.yn - p3.yn) / (p2.ty - p3.ty);
			dnaz = (p2.zn - p3.zn) / (p2.ty - p3.ty);
			dnbx = (p1.xn - p3.xn) / (p1.ty - p3.ty);
			dnby = (p1.yn - p3.yn) / (p1.ty - p3.ty);
			dnbz = (p1.zn - p3.zn) / (p1.ty - p3.ty);
			nax = nbx = p3.xn; nay = nby = p3.yn; naz = nbz = p3.zn;
		}
		int y = ymin;
		while (y <= ymax)//用扫描线扫描三角形
		{
			//着色区段的x坐标对不相等
			//计算扫描线上点的向量增量
			if (xa != xb)
			{
				dnpx = (nbx - nax) / (xb - xa);
				dnpy = (nby - nay) / (xb - xa);
				dnpz = (nbz - naz) / (xb - xa);
			}
			if (xa < xb)
			{
				//设置扫描线上最左端点的向量
				xn = nax; yn = nay; zn = naz;
				//对扫描线上着色区段中的每个点计算光照
				for (int x = (int)xa; x <= (int)xb; x++)
				{
					//获得该点计算光照后应绘制的颜色
					COLORREF color = PhongValue(surface.no, xn, yn, zn);
					//绘制点
					pDC->SetPixel(x, y, color);
					//计算下一个点的向量
					xn = xn + dnpx; yn = yn + dnpy; zn = zn + dnpz;
				}
			}
			else
			{
				xn = nbx; yn = nby; zn = nbz;
				for (int x = (int)xb; x <= (int)xa; x++)
				{
					COLORREF color = PhongValue(surface.no, xn, yn, zn);
					pDC->SetPixel(x, y, color);
					xn = xn + dnpx; yn = yn + dnpy; zn = zn + dnpz;
				}
			}
			//扫描线扫完p2p3边，需要重新计算向量增量
			if (y >= ymid)
			{
				dnax = (p1.xn - p2.xn) / (p1.ty - p2.ty);
				dnay = (p1.yn - p2.yn) / (p1.ty - p2.ty);
				dnaz = (p1.zn - p2.zn) / (p1.ty - p2.ty);
				f3 = f2;
			}
			//计算三角形边在下一条扫描线上点的向量
			nax = nax + dnax; nay = nay + dnay; naz = naz + dnaz;
			nbx = nbx + dnbx; nby = nby + dnby; nbz = nbz + dnbz;
			//计算三角形边在下一条扫描线上点的x坐标
			xa = xa + f3; xb = xb + f1;
			//进入下一条扫描线
			y++;
		}
	}
}

COLORREF CReality::PhongValue(int no, double xn, double yn, double zn)
{
	//景物表面三颜色分量的泛光反射率
	double kra, kga, kba;
	//景物表面三颜色分量的漫反射率
	double krd, kgd, kbd;
	//景物表面三颜色分量的镜面反射率
	double krs, kgs, kbs;
	//镜面高光指数
	int n;
	//理想镜面法向量
	double hxn, hyn, hzn;
	double cosnl;//景物表面单位法向量与入射光单位向量间夹角的余弦值
	double cosnh;//景物表面单位法向量与理想镜面法向量夹角的余弦值
	double ird, igd, ibd;//景物表面三角面反射的三颜色光强度

	//获得当前景物的参数
	Param param = (Param)m_ParamList.GetAt(no - 1);
	kra = param.kra; kga = param.kga; kba = param.kba;
	krd = param.krd; kgd = param.kgd; kbd = param.kbd;
	krs = param.krs; kgs = param.kgs; kbs = param.kbs;
	n = param.n;

	//计算景物表面三角面反射的三颜色光强度
	ird = ira * kra; igd = iga * kga; ibd = iba * kba;
	//循环光源列表
	for (int j = 0; j < m_LightList.GetSize(); j++)
	{
		//获得光源参数
		LightParam light = (LightParam)m_LightList.GetAt(j);
		//计算景物表面单位法向量与入射光单位向量间夹角的余弦值
		cosnl = GetVectorPM(xn, yn, zn, light.xn, light.yn, light.zn);
		//计算理想镜面法向量
		hxn = (light.xn + lxn) / 2; hyn = (light.yn + lyn) / 2; hzn = (light.zn + lzn) / 2;
		//计算景物表面单位法向量与理想镜面法向量夹角的余弦值
		cosnh = GetVectorPM(xn, yn, zn, hxn, hyn, hzn);
		cosnh = pow(cosnh, n);//计算cosnh的n次幂
		//累加计算景物表面在多个光源照射下的三颜色分量光强度
		ird = ird + light.f * (krd * light.irl * cosnl +
			krs * light.irl * cosnh);
		igd = igd + light.f * (kgd * light.igl * cosnl +
			kgs * light.igl * cosnh);
		ibd = ibd + light.f * (kbd * light.ibl * cosnl +
			kbs * light.ibl * cosnh);
	}

	if (ird < 0) ird = 0; if (ird > 255) ird = 255;
	if (igd < 0) igd = 0; if (igd > 255) igd = 255;
	if (ibd < 0) ibd = 0; if (ibd > 255) ibd = 255;
	return RGB((int)ird, (int)igd, (int)ibd);
}

void CReality::Sort(Point3D* p1, Point3D* p2, Point3D* p3)
{
	Point3D* p[3];
	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	for (int i = 0; i < 2; i++)
		for (int j = i + 1; j < 3; j++)
		{
			if (p[i]->ty < p[j]->ty)
			{
				double c;
				long lc;
				lc = p[i]->tx; p[i]->tx = p[j]->tx; p[j]->tx = lc;
				lc = p[i]->ty; p[i]->ty = p[j]->ty; p[j]->ty = lc;
				c = p[i]->xn; p[i]->xn = p[j]->xn; p[j]->xn = c;
				c = p[i]->yn; p[i]->yn = p[j]->yn; p[j]->yn = c;
				c = p[i]->zn; p[i]->zn = p[j]->zn; p[j]->zn = c;
			}
		}
}
