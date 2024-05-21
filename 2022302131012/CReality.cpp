#include "pch.h"
#include "CReality.h"

void CReality::AddObject3D(CObject3D* pObject)
{
	m_count++;
	for (int i = 0; i < pObject->m_SurfaceList.GetSize(); i++)
	{
		TriSurface surface = (TriSurface)pObject->m_SurfaceList.GetAt(i);
		//�����ӵ㷽����治�ɼ�
		if (GetVectorPM(surface.xn, surface.yn, surface.zn, lxn, lyn, lzn) >= 0)
		{
			surface.no = m_count;//���õ�ǰ���Ӧ�Ĺ��ղ���
			m_SurfaceList.Add(surface);
		}
	}
	//���þ������
	m_ParamList.Add(pObject->m_Param);
}

void CReality::AddLight(LightParam lightParam)
{
	//�������˥��
	double f = 1.0 / (lightParam.c1 + lightParam.c2 * lightParam.m_distance +
		lightParam.c3 * lightParam.m_distance * lightParam.m_distance);
	if (f <= 0 || f > 1)
		f = 1.0;
	lightParam.f = f;
	//��ӵ���Դ�б���
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
	//�����������ɫ�����ķ��ⷴ���ʺ���������
	double kra=0, kga=0, kba=0, krd, kgd, kbd;
	double cosnl;//������浥λ������������ⵥλ������нǵ�����ֵ
	double ird, igd, ibd;//������������淴�������ɫ��ǿ��
	//ѭ������ÿһ��������
	for (int i = 0; i < m_SurfaceList.GetSize(); i++)
	{
		//��þ������������
		TriSurface surface = (TriSurface)m_SurfaceList.GetAt(i);
		//��õ�ǰ����Ĳ���
		if (no != surface.no)
		{
			no = surface.no;
			Param param = (Param)m_ParamList.GetAt(no - 1);
			kra = param.kra; kga = param.kga; kba = param.kba;
			krd = param.krd; kgd = param.kgd; kbd = param.kbd;
		}
		//���㾰����������淴�������ɫ��ǿ��
		ird = ira * kra; igd = iga * kga; ibd = iba * kba;
		//ѭ����Դ�б�
		for (int j = 0; j < m_LightList.GetSize(); j++)
		{
			//��ù�Դ����
			LightParam light = (LightParam)m_LightList.GetAt(j);
			//���㾰����浥λ������������ⵥλ������нǵ�����ֵ
			cosnl = GetVectorPM(surface.xn, surface.yn, surface.zn,
				light.xn, light.yn, light.zn);
			//�ۼӼ��㾰������ڶ����Դ�����µ�����ɫ������ǿ��
			ird = ird + light.f * krd * light.irl * cosnl;
			igd = igd + light.f * kgd * light.igl * cosnl;
			ibd = ibd + light.f * kbd * light.ibl * cosnl;
		}
		if (ird < 0) ird = 0; if (ird > 255) ird = 255;
		if (igd < 0) igd = 0; if (igd > 255) igd = 255;
		if (ibd < 0) ibd = 0; if (ibd > 255) ibd = 255;
		//�����������ͶӰ
		CPoint p[3];
		p[0] = Projection(surface.p1);
		p[1] = Projection(surface.p2);
		p[2] = Projection(surface.p3);
		//����������
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
	//�����Ӧ��ͶӰ��������С���м�y����ֵ
	int ymax, ymin, ymid;
	//ɨ����ɨ�����������������ϵ������
	double nax, nay, naz, nbx, nby, nbz;
	//�����αߵ��ɨ�����ϵ���������
	double dnax, dnay, dnaz, dnbx, dnby, dnbz, dnpx=0, dnpy=0, dnpz=0;
	//������������б�ʵĵ���
	double f1, f2, f3;
	//��������ɨ���������ε�x�����
	double xa, xb;
	//Ҫ������յ������
	double xn, yn, zn;
	//ѭ���������б�
	for (int i = 0; i < m_SurfaceList.GetSize(); i++)
	{
		//��þ������������
		TriSurface surface = (TriSurface)m_SurfaceList.GetAt(i);
		//��ö���
		Point3D p1 = surface.p1;
		Point3D p2 = surface.p2;
		Point3D p3 = surface.p3;
		//�Զ������ͶӰ
		CPoint tp1 = Projection(p1);
		CPoint tp2 = Projection(p2);
		CPoint tp3 = Projection(p3);
		//����ͶӰ������
		p1.tx = tp1.x; p1.ty = tp1.y;
		p2.tx = tp2.x; p2.ty = tp2.y;
		p3.tx = tp3.x; p3.ty = tp3.y;
		//��ͶӰ��y����ֵ����
		//������Ϊ�˱�֤p1���Ӧ��ͶӰ��y����ֵ���
		//p3���Ӧ��ͶӰ��y����ֵ��С
		//�����ͱ�֤����ɨ���߽���ɨ��ʱp1p3���ϵ�����������������¼���
		//����������������˵��ɨ������ɨ��P2p3�ߣ���ɨ��p1p2��
		Sort(&p1, &p2, &p3);
		//���������ߵ�б�ʵĵ���
		if (p1.ty != p3.ty) f1 = (double)(p3.tx - p1.tx) / (p3.ty - p1.ty);
		if (p1.ty != p2.ty) f2 = (double)(p2.tx - p1.tx) / (p2.ty - p1.ty);
		if (p2.ty != p3.ty) f3 = (double)(p3.tx - p2.tx) / (p3.ty - p2.ty);
		//���ö����Ӧ��ͶӰ��������С���м�y����ֵ
		//����ymin��ɨ������ʼֵ
		//ymax��ɨ������ֵֹ
		//ymid�����ж���Ҫ���¼�����������
		ymax = (int)p1.ty; ymid = (int)p2.ty; ymin = (int)p3.ty;
		//�ж��Ƿ�p2p3��ƽ����x��
		if (p2.ty == p3.ty)
		{
			//p2p3��ƽ����x�ᣬɨ���߲���ɨ���ñ�
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
			//p2p3�߲�ƽ����x�ᣬɨ������ɨ���ñ�
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
		while (y <= ymax)//��ɨ����ɨ��������
		{
			//��ɫ���ε�x����Բ����
			//����ɨ�����ϵ����������
			if (xa != xb)
			{
				dnpx = (nbx - nax) / (xb - xa);
				dnpy = (nby - nay) / (xb - xa);
				dnpz = (nbz - naz) / (xb - xa);
			}
			if (xa < xb)
			{
				//����ɨ����������˵������
				xn = nax; yn = nay; zn = naz;
				//��ɨ��������ɫ�����е�ÿ����������
				for (int x = (int)xa; x <= (int)xb; x++)
				{
					//��øõ������պ�Ӧ���Ƶ���ɫ
					COLORREF color = PhongValue(surface.no, xn, yn, zn);
					//���Ƶ�
					pDC->SetPixel(x, y, color);
					//������һ���������
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
			//ɨ����ɨ��p2p3�ߣ���Ҫ���¼�����������
			if (y >= ymid)
			{
				dnax = (p1.xn - p2.xn) / (p1.ty - p2.ty);
				dnay = (p1.yn - p2.yn) / (p1.ty - p2.ty);
				dnaz = (p1.zn - p2.zn) / (p1.ty - p2.ty);
				f3 = f2;
			}
			//���������α�����һ��ɨ�����ϵ������
			nax = nax + dnax; nay = nay + dnay; naz = naz + dnaz;
			nbx = nbx + dnbx; nby = nby + dnby; nbz = nbz + dnbz;
			//���������α�����һ��ɨ�����ϵ��x����
			xa = xa + f3; xb = xb + f1;
			//������һ��ɨ����
			y++;
		}
	}
}

COLORREF CReality::PhongValue(int no, double xn, double yn, double zn)
{
	//�����������ɫ�����ķ��ⷴ����
	double kra, kga, kba;
	//�����������ɫ��������������
	double krd, kgd, kbd;
	//�����������ɫ�����ľ��淴����
	double krs, kgs, kbs;
	//����߹�ָ��
	int n;
	//���뾵�淨����
	double hxn, hyn, hzn;
	double cosnl;//������浥λ������������ⵥλ������нǵ�����ֵ
	double cosnh;//������浥λ�����������뾵�淨�����нǵ�����ֵ
	double ird, igd, ibd;//������������淴�������ɫ��ǿ��

	//��õ�ǰ����Ĳ���
	Param param = (Param)m_ParamList.GetAt(no - 1);
	kra = param.kra; kga = param.kga; kba = param.kba;
	krd = param.krd; kgd = param.kgd; kbd = param.kbd;
	krs = param.krs; kgs = param.kgs; kbs = param.kbs;
	n = param.n;

	//���㾰����������淴�������ɫ��ǿ��
	ird = ira * kra; igd = iga * kga; ibd = iba * kba;
	//ѭ����Դ�б�
	for (int j = 0; j < m_LightList.GetSize(); j++)
	{
		//��ù�Դ����
		LightParam light = (LightParam)m_LightList.GetAt(j);
		//���㾰����浥λ������������ⵥλ������нǵ�����ֵ
		cosnl = GetVectorPM(xn, yn, zn, light.xn, light.yn, light.zn);
		//�������뾵�淨����
		hxn = (light.xn + lxn) / 2; hyn = (light.yn + lyn) / 2; hzn = (light.zn + lzn) / 2;
		//���㾰����浥λ�����������뾵�淨�����нǵ�����ֵ
		cosnh = GetVectorPM(xn, yn, zn, hxn, hyn, hzn);
		cosnh = pow(cosnh, n);//����cosnh��n����
		//�ۼӼ��㾰������ڶ����Դ�����µ�����ɫ������ǿ��
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
