#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

void face_blur( char* name) {
	Mat img = imread(name);
	Mat orig;
	
	vector<Mat> img_ch, ch_hsv;
	char window[10];

	resize(img, img, Size(500, 500.0 * (double)img.rows / (double)img.cols));
	imshow("original", img);

	Mat img_hsv;
	cvtColor(img, img_hsv, COLOR_BGR2HSV);
	imshow("img_hsv", img_hsv);
	split(img_hsv, ch_hsv); //ch[0] = H
	
	Mat face1, face2, face_mask;
	inRange(ch_hsv[0], 0, 25, face1);
	inRange(ch_hsv[0], 155, 180, face2);
	face_mask = face1 | face2;
	imshow("after binarization", face_mask);

	Mat blur_face, non_face, non_face_mask;
	//filter2D(ch_hsv[2], ch_hsv[2], -1, face_mask);
	blur(ch_hsv[2], blur_face, Size(11, 11));
	bitwise_and(blur_face, face_mask, blur_face);
	
	bitwise_not(face_mask, non_face_mask);
	bitwise_and(ch_hsv[2], non_face_mask, non_face);
	bitwise_or(blur_face, non_face, ch_hsv[2]);

//	add(ch_hsv[1], Mat(blur_face.size(), CV_8UC1, Scalar(60)), ch_hsv[1], face_mask);
	add(ch_hsv[2], Mat(blur_face.size(), CV_8UC1, Scalar(20)), ch_hsv[2], face_mask);

	Mat result;
	merge(ch_hsv, result);
	cvtColor(result, result, COLOR_HSV2BGR);
	imshow("after blur", result);
	
	waitKey(0);

	img.release();
	img_hsv.release();
	result.release();
	face_mask.release();
	non_face_mask.release();
	blur_face.release();
	face1.release();
	face2.release();
	return ;
}

