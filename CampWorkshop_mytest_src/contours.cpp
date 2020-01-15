#include "opencv2/opencv.hpp"
#include "myopencv.h"

using namespace std;
using namespace cv;

void approxContour(char * name)
{
	Mat src = imread(name);
	if (src.empty())
		return;

	Mat canny;
	Canny(src, canny, 40, 100);
	dilate(canny, canny, Mat(), Point(-1, -1), 1);
	imshow("Canny", canny);

	//find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(canny.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	if (contours.size() <= 0) {
		cout << "No components" << endl;
		return;
	}

	Mat result (src.size(),CV_8UC3, Scalar(0,0,0));
	Mat res(src.size(), CV_8UC1, Scalar(0));
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(result, contours, i, Scalar(255, 255, 255), 1);
		vector<Point> poly;
		approxPolyDP(contours[i], poly, 3, 1);
		polylines(res, poly, true, Scalar(255), 1);
	}

	imshow("src", src);
	imshow("polyDP", res);
	imshow("contours", result);
	
	waitKey(0);

	destroyAllWindows();
}

void contours(char* name)
{
	Mat src = imread(name);
	Mat gray;

	Mat canny;
	GaussianBlur(src, src, Size(7, 7), 1.5, 1.5);
	Canny(src, canny, 80, 130);
	dilate(canny, canny, Mat(), Point(-1, -1), 2);
	imshow("Canny", canny);

	//find contours
	int cnt = 0;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(canny.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	if ((cnt  = contours.size()) <= 0) {
		cout << "No components" << endl;
		return;
	}

	Mat result = src.clone();
	for (size_t i = 0; i < cnt; i++)
	{
		Scalar color(rand()%256, rand()%256, rand()%256);
		drawContours(result, contours, i, color, -1);
		//Rect bb = boundingRect(contours[i]);
		//char msg[10];
		//sprintf(msg, "%d-th cnot", i);
		//putText(src, msg, (bb.br() + bb.tl()) / 2, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255) - color, 2);
	}

	imshow("src", src);
	imshow("result", result);
	waitKey(0);
	destroyAllWindows();
}


void extract_line(char* name)
{
	Mat img = imread(name, IMREAD_COLOR);
	if (img.empty()) {
		cout << "Cannot read image" << endl;
		waitKey(0);
		return;
	}

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat edge;
	//edge ÃßÃâ (Canny, Sobel µî)
	GaussianBlur(gray, gray, Size(3, 3), 2);
	Canny(gray, edge, 60, 150, 3);
	imshow("edges", edge);
	//Hough Transform

	vector<Vec4i> lines;
	HoughLinesP(edge, lines, 1, CV_PI/180.0, 100, 30, 10);

	//draw lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		line(img, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 0, 200), 2);
	}

	imshow("Lines", img);
	imshow("Original", gray);
	waitKey(0);

	destroyAllWindows();

}

void img_box(char *name)
{
	Mat src = imread(name);
	if (src.empty())
		return;
	cout << name << ":";

	Mat mat = img_contour_Mat(src);
	imshow(name, mat);
	waitKey(0);
}

