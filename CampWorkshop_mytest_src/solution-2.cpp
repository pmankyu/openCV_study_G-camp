/* 
Author: Kang Yi (Handong Global University)
Date : 2018.07.27
Anyone can use this source code at free of charge with this author information 
*/


#include <opencv2/opencv.hpp>
#include "myopencv.h"

using namespace cv;
using namespace std;

void sol_problem_7(char* name)
{
	int count = 0;
	Mat src = imread(name);
	if (src.empty())
		return;

	Mat canny;
	Canny(src, canny, 30, 60);
	dilate(canny, canny, Mat(), Point(-1, -1), 1);
	imshow("Canny", canny);

	//find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(canny.clone(), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	if (contours.size() <= 0) {
		cout << "No components" << endl;
		return;
	}

	int triangle = 0, srect = 0, prect = 0;

	Mat result = src.clone();
	for (size_t i = 0; i < contours.size(); i++)
	{
		vector<Point> poly;
		approxPolyDP(contours[i], poly, 10, true);
		int ret = 0;
		ret = checkPoly(poly, 0.7);

		if (ret != -1) {
			count++;
			polylines(result, poly, true, Scalar(0, 0, 0), 2);
			RotatedRect minrect = minAreaRect(poly);
			if (minrect.size.width * minrect.size.height < 90)
				continue;
			if (ret == 3) {
				triangle++;
				putText(result, "T", minrect.center, FONT_HERSHEY_COMPLEX, 0.6, Scalar(0, 0, 0), 2);
			}
			else if (ret == 4) {
				srect++;
				putText(result, "SR", minrect.center, FONT_HERSHEY_COMPLEX, 0.6, Scalar(0, 0, 0), 2);
			}
			else if (ret == 5) {
				prect++;
				putText(result, "PR", minrect.center, FONT_HERSHEY_COMPLEX, 0.6, Scalar(0, 0, 0), 2);
			}
		}
	}

	cout << "The number of polygons: " << count << endl;
	cout << "Tri: " << triangle << " SR:" << srect << " PR:" << prect << endl;
	imshow("src", src);
	//	imshow("gray", gray);
	imshow("result", result);
	waitKey(0);
	destroyAllWindows();
}


void sol_problem_9(char *name) {
	VideoCapture cap(name);
	if (!cap.isOpened())
		return;
	Mat frame, FG;
	bool stop = false;

	Ptr<BackgroundSubtractorMOG2> FMptr;
	FMptr = createBackgroundSubtractorMOG2(500, 16, true);
	FMptr->setVarThreshold(20);
	FMptr->setShadowValue(50);
	//FMptr->setShadowThreshold();
	FMptr->setNMixtures(5);
	FMptr->setBackgroundRatio(0.7);
	FMptr->setDetectShadows(true);
	FMptr->setVarThresholdGen(30);

	while (1) {
		if (!stop) {
			cap >> frame;
			if (frame.empty())
				break;
			resize(frame, frame, Size(0, 0), 0.5, 0.5);
			FMptr->apply(frame, FG, 0.1);
		}

		imshow("fg", FG);
		threshold(FG, FG, 100, 255, THRESH_BINARY);
		imshow("thresh", FG);
		imshow("src", frame);
		int key = waitKey(30);
		if (key == 27)
			break;
		else if (key == ' ')
			stop = !stop;
	}

	destroyAllWindows();
}
