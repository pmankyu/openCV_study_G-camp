/*
Author: Kang Yi (Handong Global University)
Date : 2018.07.27
Anyone can use this source code at free of charge with this author information
*/

#include <opencv2/opencv.hpp>
#include "myopencv.h"

using namespace std;
using namespace cv;

int checkPoly(vector<Point>& poly, float margin)
{
	vector<float> lines;

	//calculate distances between vertices
	for (int i = 0; i < poly.size(); i++) {
		lines.push_back(getDistance(poly[i], poly[(i + 1) % poly.size()]));
	}
	sort(lines.begin(), lines.end());	//sort by ascending order

	vector<float>sideLenRatio;
	for (int i = 0; i < lines.size(); i++) {
		sideLenRatio.push_back(lines[i] / lines[0]);
	}

	if (lines.size() == 3) {
		if (((sideLenRatio[0] >= (1 - margin)) && (sideLenRatio[0] <= (1 + margin))) &&
			((sideLenRatio[1] >= (1 - margin)) && (sideLenRatio[1] <= (1 + margin))) &&
			((sideLenRatio[2] >= (1.4 - margin)) && (sideLenRatio[2] <= (1.4 + margin))))
			return 3; 	//직각삼각형 - 1:1:1.4
		else return -1;
	}
	else if (lines.size() == 4) {
		if (((sideLenRatio[0] >= (1 - margin)) && (sideLenRatio[0] <= (1 + margin))) &&
			((sideLenRatio[1] >= (1 - margin)) && (sideLenRatio[1] <= (1 + margin))) &&
			((sideLenRatio[2] >= (1 - margin)) && (sideLenRatio[2] <= (1 + margin))) &&
			((sideLenRatio[3] >= (1 - margin)) && (sideLenRatio[3] <= (1 + margin))))
			return 4; //정사각형 - 1:1:1:1

		else if (((sideLenRatio[0] >= (1 - margin)) && (sideLenRatio[0] <= (1 + margin))) &&
			((sideLenRatio[1] >= (1 - margin)) && (sideLenRatio[1] <= (1 + margin))) &&
			((sideLenRatio[2] >= (1.4 - margin)) && (sideLenRatio[2] <= (1.4 + margin))) &&
			((sideLenRatio[3] >= (1.4 - margin)) && (sideLenRatio[3] <= (1.4 + margin))))
			return 5; //평행사변 - 1:1:1.4:1.4

		else
			return -1;
	}
	else return -1;
}

void sol_problem_6(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;
	Mat canny;

	//cvtColor(img, img, COLOR_BGR2GRAY);
	Canny(img, canny, 10, 30); // use color information for edge detection
	dilate(canny, canny, Mat(), Point(-1, -1), 1);
	imshow("canny", canny);

	//find contours
	Mat edges = canny.clone();
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	int size = contours.size();
	cout << size << " contours" << endl;
	char text[20];

	vector<vector<Point>> polygon_pts(size);
	vector<Scalar> color(size);
	Mat draw = img.clone();
	for (int i = 0; i < size; i++) {
		//drawContours(draw, contours, i, Scalar(70, 70, 70), 10);
		approxPolyDP(contours[i], polygon_pts[i], 20, true);
		color[i] = Scalar((rand()) % 255, (rand()) % 255, (rand()) % 255);
		polylines(draw, polygon_pts[i], true, color[i], 2);
	}

	for (int i = 0; i < size; i++) {
		RotatedRect minrect = minAreaRect(polygon_pts[i]);
		if (minrect.size.area() < 900)
			continue;
		Moments mu = moments(polygon_pts[i], false);
		Point2f center(mu.m10 / mu.m00, mu.m01 / mu.m00);
		char *type;
		switch (checkPoly(polygon_pts[i], 0.1)) {
		case 3: type = "T";
			break;
		case 4: type = "SR";
			break;
		case 5: type = "PR";
			break;
		default:
			type = "ERR";
		}
		sprintf(text, "%s(%d)", type, i);
		putText(draw, text, center, FONT_HERSHEY_PLAIN, 1.5, color[i], 2);
	}
	imshow("draw", draw);
	waitKey(0);
	destroyAllWindows();
}