#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

class CFigure
{
	//vector<Point> contour;
	int x;
	int y;
	int w;
	int h;

	double cx;
	double cy;
public:
	CFigure(void);
	~CFigure(void);
};

