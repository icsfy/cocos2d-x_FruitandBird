#ifndef  _Collision_H_
#define  _Collision_H_

#include <math.h>

static bool collision(double sMinX, double sMinY, double sMaxX, double sMaxY, double gMinX, double gMinY, double gMaxX, double gMaxY)
{
	//重叠xy
	double xOverlap = 0;
	double yOverlap = 0;
	//重叠面积
	int impacetArea = 0;
	//左侧碰撞
	if(sMinX<=gMaxX && sMinX>=gMinX && sMaxX>gMaxX)
	{
		xOverlap = abs(gMaxX - sMinX);
		//上方碰撞
		if(sMaxY>gMinY && sMaxY<gMaxY)
		{
			yOverlap = abs(gMinY - sMaxY);
		}else if(sMinY<gMaxY && sMinY>gMinY) //下方碰撞
		{
			yOverlap = abs(gMaxY - sMinY);
		}
	}else if(sMaxX>=gMinX && sMaxX<=gMaxX && sMinX<=gMinX) //右侧碰撞
	{
		xOverlap = abs(gMinX - sMaxX);
		//上方碰撞
		if(sMaxY>gMinY && sMaxY<gMaxY)
		{
			yOverlap = abs(gMinY - sMaxY);
		}else if(sMinY<gMaxY && sMinY>gMinY) //下方碰撞
		{
			yOverlap = abs(gMaxY - sMinY);
		}
	}else if(sMaxX<=gMaxX && sMaxX>=gMinX && sMinX>=gMinX && sMaxX>=gMinX)//包含
	{
		xOverlap = abs(sMaxX - sMinX);
		//上方碰撞
		if(sMaxY>gMinY && sMaxY<gMaxY)
		{
			yOverlap = abs(gMinY - sMaxY);
		}else if(sMinY<gMaxY && sMinY>gMinY) //下方碰撞
		{
			yOverlap = abs(gMaxY - sMinY);
		}
	}
	//计算重叠面积
	impacetArea = xOverlap * yOverlap;

	if(impacetArea>0)
	{
		return true;
	}

	return false;

}

#endif
