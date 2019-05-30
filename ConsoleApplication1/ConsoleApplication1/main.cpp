#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;
Mat src, src_gray;
Mat dst, dectected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const double PI_F = 3.14159265358979323846f;

string windowName = "threshold";
int threshole_value = 0;
int threshold_type = 3;

void Thresh_FindContours() {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	
	int cMax = 1000;

	
	vector<vector<Point>>::iterator iter = contours.begin();
	while(iter != contours.end()) {
	}

	findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	drawContours(dst, contours, -1, Scalar(255, 255, 255), 2);
	namedWindow("Contours");
	imshow("Contours", dst);
}
void Threshold_Demo(int, void*) {
	threshold(src, dst, threshole_value, 255, threshold_type);
	imshow(windowName, dst);
	while(true) {
	int c = waitKey(20);
	if(c == 'z')  {
		Thresh_FindContours();
		return;
	}
	}
}

int main() {
	src = imread("Car.jpg", IMREAD_GRAYSCALE);
	dst.create(src.size(), src.type());
	imshow("Before", src);
	GaussianBlur(src, dst, Size(9,9),0);
	imshow("gaussian", dst);
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	createTrackbar("Type : ", windowName, &threshold_type, 4, Threshold_Demo);
	createTrackbar("Value : ", windowName, &threshole_value, 255, Threshold_Demo);
	Threshold_Demo(0, 0);
	waitKey(0);
	return 0;
}


