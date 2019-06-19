#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
using namespace cv;
using namespace std;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const double PI_F = 3.14159265358979323846f;

int threshole_value = 0;
int threshold_type = 3;


//vector<vector<Point>> contours;



/*
void Thresh_FindContours() {

int iMax = 1000;

findContours(dst,contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

//vector<vector<Point>>::iterator iter = contours.begin();
//while(iter != contours.end()) {
//	if((*iter).size() > iMax) iter = contours.erase(iter);
//	else ++iter;
//}


drawContours(temp_result, contours, -1, Scalar(255, 255, 255), 1);
imshow("Contours", dst);
}
*/

/*
void Threshold_Demo() {
Mat img_blurred;
Mat img_thresh;
Mat contours;
//temp_result.create(src.size(), src.type());
//contours.create(src.size(), src.type());


GaussianBlur(src, img_blurred, Size(5,5),0);
for(int i = 0;i < 1; i++)	GaussianBlur(img_blurred, img_blurred, Size(5,5),0);
imshow("gaussian", img_blurred);


adaptiveThreshold(dst, img_thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 19, 9);
//threshold(src, dst, threshole_value, 255, threshold_type);
imshow("threshold", img_thresh);

findContours(img_thresh,contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
temp_result.create(src.size(), src.type());

drawContours(temp_result, contours, -1, Scalar(255, 255, 255), 1);
imshow("Contours", dst);
//while(true) {
//int c = waitKey(20);
//if(c == 'z')  {
//printf("111\n");
//Thresh_FindContours();
//printf("222\n");
//Thresh_FindContours();
//printf("333\n");
//Thresh_FindContours();
//	return;
//}
//}
}


vector<Rect> contours_dict;
void makeBox() {
//Mat BoxMat;
//BoxMat.create(src.size(), src.type());
//
//Mat_<uchar>::const_iterator iter = contours.begin<uchar>();
//while(iter != contours.end<uchar>()) {
//	Rect rect =	boundingRect(*iter);
//	contours_dict.push_back(rect);
//	rectangle(BoxMat, rect, Scalar(255, 255, 255), 1);
//	++iter;
//}
//imshow("Box", BoxMat);
}
int iNumberBoxCount;
vector<Rect> possible_contours;
void pickPossibleNumberBox() {
//나는 아직 딥러닝을 못한다.
//그렇기에 숫자간의 넓이 비율을 이용한다.
const int MIN_AREA = 0;
const int MIN_WIDTH = 2;
const int MIN_HEIGHT = 8;
const double MAX_RATIO = 1.0;
long area;
double ratio;
Mat NumberBoxMat;
NumberBoxMat.create(src.size(), src.type());
for(vector<Rect>::iterator iter = contours_dict.begin(); iter != contours_dict.end(); ++iter) {
area = (*iter).width * (*iter).height;
ratio = (*iter).width / (*iter).height;

if( area >= MIN_AREA &&
(*iter).width > MIN_WIDTH && (*iter).height > MIN_HEIGHT && 
ratio < MAX_RATIO) {
printf("Width :: %d, Height :: %d\n", (*iter).width, (*iter).height);
printf("area :: %d, ratio :: %lf\n", area, ratio);
iNumberBoxCount++;
possible_contours.push_back((*iter));
rectangle(NumberBoxMat, (*iter), Scalar(255, 255, 255), 2);
}
}
imshow("NumberBox", NumberBoxMat);
}


vector<Rect> matched_result_idx;// 조건에 맞는것들 중에서 3개 이상씩 맞아 떨어지는애들을 넣는거
vector<Rect> matched_contours_idx; // 일단 조건에 맞는애들 넣어서 나중에 출력
Mat CheckDebug;

vector<Rect> find_chars(vector<Rect> contours_list, int iCountFunction) {
printf("넘어온 contours_list의 size 값은 :: %d\n", contours_list.size());
int outNum = 0;
int inNum = 0;
for(vector<Rect>::iterator iter = contours_list.begin(); iter != contours_list.end(); ++iter) {
matched_contours_idx.clear();
for(vector<Rect>::iterator iter2 = contours_list.begin(); iter2 != contours_list.end(); ++iter2) {
if(iter == iter2) continue;
//배열에서 1개를 지정(iter)하고 자기자신을 제외한 애들(iter2)을 비교.

int cx1 = ((*iter).x	 + (*iter).width)/2;
int cy1 = ((*iter).y	 + (*iter).height)/2;
int cx2 = ((*iter2).x	 + (*iter2).width)/2;
int cy2 = ((*iter2).y	 + (*iter2).height)/2;
//두 객체의 각각 중심좌표구하기 구하기
double dx = abs( cx1 - cx2 );
double dy = abs(cy1 - cy2);
//
double diagonal_length1 = sqrt((*iter).width * (*iter).width  + (*iter).height * (*iter).height);
double distance = norm(Point(cx1, cy1) - Point(cx2, cy2));
double angle_diff;
if(dx == 0) angle_diff = 90;
else angle_diff = cvFastArctan(dy, dx) * ( 180 / PI_F);

double area_diff = abs( (*iter).area() - (*iter2).area()) / (*iter).area();
double width_diff = abs((*iter).width - (*iter2).width) / (*iter).width;
double height_diff = abs((*iter).height - (*iter2).height)/(*iter).height;

if(distance < diagonal_length1 * MAX_DIAG_MULTIPLYER && angle_diff < MAX_ANGLE_DIFF && area_diff < MAX_AREA_DIFF && width_diff < MAX_WIDTH_DIFF && height_diff < MAX_HEIGHT_DIFF) {
bool bContinue = false;
for(vector<Rect>::iterator iter3 = matched_contours_idx.begin(); iter3 != matched_contours_idx.end(); ++iter3) {
if(*iter2 == *iter3) {
bContinue = true;
break;
}
}
if(bContinue) continue;
matched_contours_idx.push_back((*iter2));
}
}

if(matched_contours_idx.size() < MIN_N_MATCHED ) {// 갯수를 파악해서 3개보다 작으면 나가리
matched_contours_idx.clear();
continue;
}

printf("matched_contours_idx size :: %d\n", matched_contours_idx.size());
printf("matched_result_idx size :: %d\n", matched_result_idx.size());
for(vector<Rect>::iterator iter2 = matched_contours_idx.begin(); iter2 != matched_contours_idx.end(); ++iter2) {
bool bContinue = false;
for(vector<Rect>::iterator iter3 = matched_result_idx.begin(); iter3 != matched_result_idx.end(); ++iter3) {
if(*iter2 == *iter3) {
bContinue = true;
break;
}	
}
if(bContinue) continue;
matched_result_idx.push_back(*iter2);

}
printf("matched_result size !! :: %d\n", matched_result_idx.size());
//찾아진 조건에 맞는 애들을 result 배열에 모두다 넣음.




vector<Rect> unmatched_contours;
for(vector<Rect>::iterator iter = possible_contours.begin(); iter != possible_contours.end(); ++iter) {
bool bContinue = false;
for(vector<Rect>::iterator iter2 = matched_contours_idx.begin(); iter2 != matched_contours_idx.end(); ++iter2) {
if(*iter == *iter2) {
bContinue = true;
break;
}	

//unmatched_contours.push_back(*iter);

}
if(bContinue) continue;
}

printf("넘겨주는 unmatched_contours 의 size :: %d\n", unmatched_contours.size());
vector<Rect> recursive_unmatched_contours = find_chars(unmatched_contours, iCountFunction+1);


for(vector<Rect>::iterator iter2 = recursive_unmatched_contours.begin(); iter2 != recursive_unmatched_contours.end(); ++iter2) {
bool bContinue = false;
for(vector<Rect>::iterator iter3 = matched_result_idx.begin(); iter3 != matched_result_idx.end(); ++iter3) {
if(*iter2 == *iter3) {
bContinue = true;
break;
}	
}
if(bContinue) continue;
matched_result_idx.push_back(*iter2);

}

}

return matched_result_idx;
}
void drawFindChar() {
printf("-1 . DrawFindChar 부분 possible_contours 넘겨주는 size값 :: %d\n", possible_contours.size());
matched_result = find_chars(possible_contours, 1);
//printf("0 . DrawFindChar 부분 matched 넘겨주는 size값 :: %d\n", matched_result.size());
//matched_result = find_chars(matched_result, 1);
//printf("1 . DrawFindChar 부분 matched 넘겨주는 size값 :: %d\n", matched_result.size());
//matched_result = find_chars(matched_result, 1);
//printf("2 . DrawFindChar 부분 matched 넘겨주는 size값 :: %d\n", matched_result.size());
//matched_result = find_chars(matched_result, 1);
//printf("3 . DrawFindChar 부분 matched 넘겨주는 size값 :: %d\n", matched_result.size());
printf("matched_result size :: %d\n", matched_result.size());
for(vector<Rect>::iterator iter = matched_result.begin(); iter != matched_result.end(); ++iter) {

rectangle(temp_result, (*iter), Scalar(255, 255, 255), 2);
}


}

int main() {

img_ori = imread("4.jpg", IMREAD_GRAYSCALE);
imshow("Before", img_ori);



//namedWindow(windowName, CV_WINDOW_AUTOSIZE);
//createTrackbar("Type : ", windowName, &threshold_type, 4, Threshold_Demo);
//createTrackbar("Value : ", windowName, &threshole_value, 255, Threshold_Demo);
Threshold_Demo();
makeBox();
pickPossibleNumberBox();
//GaussianBlur(dst, dst, Size(3,3),0);
//imshow("gaussian2", dst);
drawFindChar();
imshow("CheckImage", temp_result);
printf("ㅋ");
waitKey(0);
return 0;
}


*/

bool cmp(const Rect& a, const Rect& b) {
	return a.x < b.x ? true : false;
}

vector<Rect> find_chars(vector<Rect> contour_list) {

	// 각 사격형 중심좌표간의 거리 ( 한 지점의 대각선 길이의 N배율 만큼의 길이 )
	const double MAX_DIAG_MULTIPLYER=	2;
	//각 사각형 중심좌표간의 기울기
	const double MAX_ANGLE_DIFF	=	25.0;
	// 각 사각형의 넓이비율 최대치
	const double MAX_AREA_DIFF	=	1.8;
	// 각 사각형의 넓이비율 최소치
	//const double MIN_AREA_DIFF	=	1.0;
	// 너비비율
	const double MAX_WIDTH_DIFF		=2.0;
	// 높이비율
	const double MAX_HEIGHT_DIFF	=1.2;
	// 만족하는 번호의 조건이 3개 미만의 경우 처단하라!!!!!!
	const int MIN_N_MATCHED		= 3;



	vector<Rect> matched_result;
	for( vector<Rect>::iterator iter1 = contour_list.begin(); iter1 != contour_list.end(); ++iter1) {
		vector<Rect> matched_contours;
		for( vector<Rect>::iterator iter2 = contour_list.begin(); iter2 != contour_list.end(); ++iter2) {
			if( *iter1 == *iter2 ) continue;

			//if((*iter2).width > (*iter2).height*2 || (*iter2).height > (*iter2).width*2) continue;
			//생각을 해보니 1같은 경우에는 가로와 세로의 비율이 어마어마하다. 
			int cx1 = ((*iter1).x	 + (*iter1).width)/2;
			int cy1 = ((*iter1).y	 + (*iter1).height)/2;
			int cx2 = ((*iter2).x	 + (*iter2).width)/2;
			int cy2 = ((*iter2).y	 + (*iter2).height)/2;
			
			int iMinW = (*iter1).width > (*iter2).width ? (*iter2).width : (*iter1).width;
			int iMaxW = (*iter1).width < (*iter2).width ? (*iter2).width : (*iter1).width;
			int iMinH = (*iter1).height > (*iter2).height ? (*iter2).height : (*iter1).height;
			int iMaxH = (*iter1).height < (*iter2).height ? (*iter2).height : (*iter1).height;
			int iMinA = (*iter1).area() > (*iter2).area() ? (*iter2).area() : (*iter1).area();
			int iMaxA = (*iter1).area() < (*iter2).area() ? (*iter2).area() : (*iter1).area();

			//if((*iter2).x < cx1 || ((*iter1).x + (*iter1).width) > cx2) {
			//	continue;
			//}
			if((((*iter2).x < ((*iter1).x + (*iter1).width)) && 
				(((*iter2).x + (*iter2).width > (*iter1).x))) ||
				(((*iter1).x < ((*iter2).x + (*iter2).width)) && 
				((*iter1).x + (*iter1).width > ((*iter2).x + (*iter2).width)))) {
					//printf("나는 사라져요!!! x : %d /// y : %d \n", (*iter2).x, (*iter2).y);
					//printf("얘때문이에요!!!! x : %d /// y : %d \n", (*iter1).x, (*iter1).y);
				continue;
			}
			//두 객체의 각각 중심좌표구하기 구하기
			double dx = abs( (*iter1).x - (*iter2).x )*1.0;
			double dy = abs((*iter1).y - (*iter2).y)*1.0;
			//
			//double diagonal_length1 = sqrt(((*iter1).width * (*iter1).width)  + ((*iter1).height * (*iter1).height));
			//double distance = norm(Point(abs(cx1-cx2), abs(cy1-cy2)));
			double distance =  sqrt(abs(cx1-cx2)*abs(cx1-cx2) + abs(cy1-cy2)*abs(cy1-cy2));
			double angle_diff;
			double radian = atan2f((float)dy, (float)dx);
			if( dx == 0 ) angle_diff = 90;
			else angle_diff = radian * 180 / PI_F;
			
			//printf("나는 X1 : %d 와  X2 : %d 이고, 나의 Angle :: %f\n",(*iter1).x,(*iter2).x, cvFastArctan(dy, dx));
			double area_diff	=	(double)iMaxA/iMinA;
			double width_diff	=	(double)iMaxW/iMinW;
			double height_diff	=	(double)iMaxH/iMinH;
			
			if(
				(abs(cx1-cx2) < iMaxW*1.3 && abs( cy1-cy2) < iMinH) && 
				//distance < diagonal_length1 * MAX_DIAG_MULTIPLYER && //a와 b의 거리가 나의 대각선의 배수만큼의 거리를 벗어나면 안된다.
				angle_diff < MAX_ANGLE_DIFF && //두 Rect간의 중심좌표의 각도가 일정 수치를 넘어서지 않는다.
				 area_diff < MAX_AREA_DIFF && //두 Rect간의 크기 비율이 일정 수치를 넘어서지 않는다.
				width_diff < MAX_WIDTH_DIFF && height_diff < MAX_HEIGHT_DIFF //각 Rect간의 Width값과 Height값의 차이비율
				)  {
					//이미 들어가버린 친구 기준에서도 ++이 되어줘야함..
					//bool bContinue = false;
					//for(vector<Rect>::iterator iter3 = matched_contours.begin(); iter3 != matched_contours.end(); ++iter3) {
					//	if(*iter2 == *iter3) {
					//		bContinue = true;
					//		break;
					//	}
					//}
					//if(bContinue) continue;
					//printf("이게 뭐하는건지 iMaxW : %d / iMaxH : %d\n", iMaxW, iMaxH);
					matched_contours.push_back((*iter2));
					printf("제 1 CX, CY :: %d %d // 제 2 CX, CY :: %d %d\n", cx1, cy1,cx2, cy2);
					printf("제 1 Area : %d, 제 2 Area : %d\n", (*iter1).area(), (*iter2).area());	
					printf("제 1 Width : %d, 제 2 Width : %d\n", (*iter1).width, (*iter2).width);
					printf("제 1 height : %d, 제 2 height : %d\n", (*iter1).height, (*iter2).height);
					printf("제 1 좌표 :: X : %d, Y : %d\n제 2 좌표 :: X : %d, Y : %d\n", (*iter1).x, (*iter1).y, (*iter2).x, (*iter2).y);
					printf(" distance ::%.4lf\tangle_diff ::%.4lf\tarea_diff ::%.4lf\twidth_diff ::%.4lf\theight_diff ::%.4lf\n\n ",
					distance, angle_diff ,area_diff  ,width_diff  , height_diff );
					
			//printf("넌 대체 뭐가 문제니 :::: %lf %lf\n" , area_diff*1000000000 , MAX_AREA_DIFF*1000000000);
			}else if((*iter2).x == 414 && (*iter1).x == 374) {
				printf("나는 얘때메 사라져요 ㅡㅡ; X : %d , Y : %d\n", (*iter1).x, (*iter1).y);
			}
		}

		//printf("내가 기준점이에요!!!! 나의 x좌표 :: %d, y좌표 :: %d, width :: %d, height :: %d\n", (*iter1).x, (*iter1).y, (*iter1).width, (*iter1).height);
		matched_contours.push_back(*iter1);
		//printf("matched_contours의 사이즈는 :: %d입니다 \n", matched_contours.size());
		if(matched_contours.size() < MIN_N_MATCHED) {
			matched_contours.clear();
			continue;
		}



		//for(vector<Rect>::iterator iter2 = matched_contours.begin(); iter2 != matched_contours.end(); ++iter2) {
		//	printf("나의 x좌표 :: %d, y좌표 :: %d, width :: %d, height :: %d\n", (*iter2).x, (*iter2).y, (*iter2).width, (*iter2).height);
		//}
		for( vector<Rect>::iterator iter2 = matched_contours.begin(); iter2 != matched_contours.end(); ++iter2) {
			bool bContinueCheck = false;
			for( vector<Rect>::iterator iter3 = matched_result.begin();  iter3 != matched_result.end(); ++iter3) {
				if(*iter2 == *iter3) {
					bContinueCheck = true;
					break;
				}
			}
			if(bContinueCheck) continue;

			matched_result.push_back(*iter2);
		}


		vector<Rect> unmatched_contour;

		for( vector<Rect>::iterator iter2 = contour_list.begin(); iter2 != contour_list.end(); ++iter2) {
			bool bContinueCheck = false;
			for( vector<Rect>::iterator iter3 = matched_contours.begin(); iter3 != matched_contours.end(); ++iter3) {
				if(*iter2 == *iter3) {
					bContinueCheck = true;
					break;
				}
			}
			if(bContinueCheck) continue;
			unmatched_contour.push_back(*iter2);
		}


		//printf("find_char 들어가요!!!\n");
		vector<Rect> recursive_contour_list = find_chars(unmatched_contour);
		//printf("find_char 나옵니다ㅏㅏㅏㅏㅏㅏㅏ!!!\n");

		for( vector<Rect>::iterator iter2 = recursive_contour_list.begin(); iter2 != recursive_contour_list.end(); ++iter2) {
			matched_result.push_back(*iter2);
		}
		//if(iter1 != contour_list.begin() && iter1 != (contour_list.begin())+1&& iter1 != (contour_list.begin())+2)
		//break;
	}
	return matched_result;
}
void sharpen(const Mat &image, Mat &result) {
	
	int nchannels = image.channels();

	for(int i = 1; i < image.rows -1; i++) {
		const uchar* previous = image.ptr<const uchar>(i-1);
		const uchar* current = image.ptr<const uchar>(i);
		const uchar* next = image.ptr<const uchar>(i+1);

		uchar* output = result.ptr<uchar>(i);

		for(int j = nchannels; j < (image.cols -1)*nchannels; j++) {
			*output++ = saturate_cast<uchar>(5* current[j] - current[j - nchannels] - current[j+nchannels] - previous[j] - next[j]);
		}
	}
	result.row(0).setTo(Scalar(0, 0, 0));
	result.row(result.rows-1).setTo(Scalar(0, 0, 0));
	result.col(0).setTo(Scalar(0, 0, 0));
	result.col(result.cols -1).setTo(Scalar(0, 0, 0));
}
void main() {
	/*
	Mat before = imread("1.jpg");
	imshow("be", before);
	Mat after;
	after.create(before.rows, before.cols, before.type());
	sharpen(before, after);

	imshow("Thresasdash", after);


	
	Mat after2;
	after2.create(before.rows, before.cols, before.type());
	sharpen(after, after2);

	imshow("Thresasdash2", after2);



	
	Mat after3;
	after3.create(before.rows, before.cols, before.type());
	sharpen(after2, after3);

	imshow("Thresasdash3", after3);
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////v


	const int MIN_AREA	= 80		;
	const int MIN_WIDTH	= 2			;
	const int MIN_HEIGHT = 8			;
	const double MIN_RATIO	= 0.25		;
	const double MAX_RATIO	= 2.0		;
	
	Mat img_ori, gray;
	Mat dst, dectected_edges;
	int height, width, channel;
	img_ori = imread("2.jpg", IMREAD_COLOR);

	channel = img_ori.channels();

	imshow("Ori", img_ori);


	cvtColor(img_ori, gray, COLOR_BGR2GRAY);
	imshow("Gray", gray);


	Mat img_blurred;
	
	GaussianBlur(gray, img_blurred, Size(5, 5), 0);
	// 노이즈를 줄이기 위하여 사용
	imshow("Gaussian", img_blurred);

	Mat img_thresh;
	adaptiveThreshold(img_blurred, img_thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 19, 9);
	// 이미지의 threshold를 지정하여  일정 지정값 이하는 0으로 나머지는 255로 바꿔줌 ( 검, 흰으로 바꿔줌)

	imshow("Thresh", img_thresh);


	//Mat contours
	vector<vector<Point>> contours;
	//contours.create(gray.size(), gray.type());
	findContours(img_thresh, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	Mat temp_result;
	temp_result.create(img_ori.size(), img_ori.type());
	drawContours(temp_result, contours, -1, Scalar(255, 255, 255));
	imshow("Contours", temp_result);

	temp_result.create(gray.size(), gray.type());

	vector<Rect> contours_dict;

	//printf("Contours Size :: %d\n", contours.size());
	for( vector<vector<Point>>::iterator iter = contours.begin(); iter != contours.end(); ++iter) {
		Rect contour = boundingRect(*iter);
		rectangle(temp_result, contour, Scalar(255, 255, 255), 2);

		contours_dict.push_back(contour);
	}
	
	sort(contours_dict.begin(), contours_dict.end(), cmp);
	imshow("bounding", temp_result);


	vector<Rect> possible_contours;
	int iCount = 0;

	for(vector<Rect>::iterator iter = contours_dict.begin(); iter != contours_dict.end(); ++iter) {
		int area = (*iter).width * (*iter).height;
		double ratio = (*iter).width / (*iter).height;

		if(area >= MIN_AREA &&
			(*iter).width >= MIN_WIDTH && (*iter).height >= MIN_HEIGHT &&
			ratio <= MAX_RATIO) {
				possible_contours.push_back(*iter);
		}
	}

	//////////////////////
	sort(possible_contours.begin(), possible_contours.end(), cmp);
	temp_result = Mat::zeros(gray.size(), gray.type());
	for(vector<Rect>::iterator iter = possible_contours.begin(); iter != possible_contours.end(); ++iter) {
		rectangle(temp_result, (*iter), Scalar(255, 255, 255), 1);
	}
	imshow("possible contours", temp_result);
	
	vector<Rect> matched_result = find_chars(possible_contours);



	temp_result = Mat::zeros(gray.size(), gray.type());
	int iMinX = gray.size().width+10;
	int iMaxX = 0;

	int iMinY = gray.size().height+10;
	int iMaxY = 0;
	sort(matched_result.begin(), matched_result.end(), cmp);
	for(vector<Rect>::iterator iter = matched_result.begin(); iter != matched_result.end(); ++iter) {
		if((*iter).x+(*iter).width > iMaxX) iMaxX = (*iter).x+(*iter).width;
		if((*iter).x < iMinX) iMinX = (*iter).x;
		if((*iter).y+(*iter).height > iMaxY) iMaxY = (*iter).y+(*iter).height;
		if((*iter).y < iMinY) iMinY = (*iter).y;
		rectangle(temp_result, *iter, Scalar(255, 255, 255), 1);
		rectangle(img_ori, (*iter), Scalar(255, 0, 0), 1);
		//printf("LAST!!! 제 1 좌표 :: X : %d, Y : %d\n\n", (*iter).x, (*iter).y);

	}

	imshow("Wow Box!", temp_result);

	Mat img_cropped;
	
	img_cropped = Mat::zeros(img_ori.size(), img_ori.type());
	printf("MinX : %d, MaxX : %d, MinY : %d, MaxY : %d\n", iMinX, iMaxX, iMinY, iMaxY);
	if(iMaxX <= iMinX || iMaxY <= iMinY) {
		printf("\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\nFAIL !!!!!\tFAIL !!!!!\tFAIL !!!!!\tFAIL !!!!!\t\n/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
		printf("\n사진 파일과 알고리즘간에 예상하지못한 예외가 발생하였습니다.\n\n");
		printf("/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\nFAIL !!!!!\tFAIL !!!!!\tFAIL !!!!!\tFAIL !!!!!\t\n/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
		waitKey(0);
		return;
	}
	Rect rec = Rect(iMinX, iMinY,iMaxX-iMinX, iMaxY-iMinY);

	//img_ori
	rectangle(img_ori,rec, Scalar(0, 255, 0), 2);
	imshow("TestPoss", img_ori);

	Mat NumberBoard = img_ori(rec);
	imshow("NumberBoard", NumberBoard);



	waitKey(0);
}