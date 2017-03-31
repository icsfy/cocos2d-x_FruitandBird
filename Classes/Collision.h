#ifndef  _Collision_H_
#define  _Collision_H_

#include <math.h>

static bool collision(double sMinX, double sMinY, double sMaxX, double sMaxY, double gMinX, double gMinY, double gMaxX, double gMaxY)
{
	//�ص�xy
	double xOverlap = 0;
	double yOverlap = 0;
	//�ص����
	int impacetArea = 0;
	//�����ײ
	if(sMinX<=gMaxX && sMinX>=gMinX && sMaxX>gMaxX)
	{
		xOverlap = abs(gMaxX - sMinX);
		//�Ϸ���ײ
		if(sMaxY>gMinY && sMaxY<gMaxY)
		{
			yOverlap = abs(gMinY - sMaxY);
		}else if(sMinY<gMaxY && sMinY>gMinY) //�·���ײ
		{
			yOverlap = abs(gMaxY - sMinY);
		}
	}else if(sMaxX>=gMinX && sMaxX<=gMaxX && sMinX<=gMinX) //�Ҳ���ײ
	{
		xOverlap = abs(gMinX - sMaxX);
		//�Ϸ���ײ
		if(sMaxY>gMinY && sMaxY<gMaxY)
		{
			yOverlap = abs(gMinY - sMaxY);
		}else if(sMinY<gMaxY && sMinY>gMinY) //�·���ײ
		{
			yOverlap = abs(gMaxY - sMinY);
		}
	}else if(sMaxX<=gMaxX && sMaxX>=gMinX && sMinX>=gMinX && sMaxX>=gMinX)//����
	{
		xOverlap = abs(sMaxX - sMinX);
		//�Ϸ���ײ
		if(sMaxY>gMinY && sMaxY<gMaxY)
		{
			yOverlap = abs(gMinY - sMaxY);
		}else if(sMinY<gMaxY && sMinY>gMinY) //�·���ײ
		{
			yOverlap = abs(gMaxY - sMinY);
		}
	}
	//�����ص����
	impacetArea = xOverlap * yOverlap;

	if(impacetArea>0)
	{
		return true;
	}

	return false;

}

#endif
