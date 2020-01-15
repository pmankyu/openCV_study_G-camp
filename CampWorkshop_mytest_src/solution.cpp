#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void sol_problem_5(char *name) {
	Mat img = imread(name, 1);
	if (img.empty())
		return;

	Mat mask, dst,res;

	Mat kernel_x = getGaussianKernel(img.cols, 80); // column vector with x coord size
	Mat kernel_y = getGaussianKernel(img.rows, 80); // column vector with y coord size
	Mat kernel_x_transpose;

	transpose(kernel_x, kernel_x_transpose); // make row vector by transposing column vector of x size
	Mat kernel = kernel_y * kernel_x_transpose; // make 2D kernel
	normalize(kernel, kernel, 0, 1, NORM_MINMAX); // normalize the kernel [0..1]

	vector<Mat> channels(3);
	split(img, channels);

	for (int i = 0; i < 3; i++)
	{
		channels[i].convertTo(dst, CV_64F); // CV_8U -> CV_64F
		multiply(kernel, dst, dst); // dst = mask * dst
		convertScaleAbs(dst, dst); // CV_64F -> CV_8U
		channels[i] = dst;
	}
	
	merge(channels, res);
	imshow("kernel", kernel);
	imshow("res", res);

	waitKey(0);

}

void sol_problem_4(char *name) {
	VideoCapture cap(name);
	if (cap.isOpened() == false)
		return;
	Mat frame, hsv, gray, bin, bin2, mask;
	vector<Mat> channels(3);
	bool stop = false;

	while (1) {
		if (!stop) {
			cap >> frame;
			if (frame.empty())
				break;
			resize(frame, frame, Size(0, 0), 0.8, 0.8);
			cvtColor(frame, hsv, COLOR_BGR2HSV);
			cvtColor(frame, gray, COLOR_BGR2GRAY);
			split(hsv, channels);
			inRange(hsv, Scalar(0, 70, 80), Scalar(30, 255, 255), bin);
			inRange(hsv, Scalar(170, 70, 80), Scalar(180, 255, 255), bin2);
			bitwise_or(bin, bin2, mask); // RED MASK
			bitwise_and(gray, ~mask, gray); // GRAY range
			cvtColor(gray, gray, COLOR_GRAY2BGR); // change # of channel to 3
			cvtColor(mask, mask, COLOR_GRAY2BGR); // change # of channel to 3
			bitwise_and(frame, mask, frame);
			bitwise_or(frame, gray, frame);
		}
		imshow("result", frame);
		imshow("red", mask);
		int key= waitKey(30);
		if (key == 27)
			break;
		else if (key == ' ')
			stop = !stop;
	}
}

void sol_problem_3(char *name)
{
	VideoCapture cap(name);
	if (cap.isOpened() == false)
		return;

	Mat frame, prev, dst;
	cap >> prev;
	cvtColor(prev, prev, COLOR_BGR2GRAY);
	while (1) {
		cap >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		imshow("live", frame);
		absdiff(frame, prev, dst);
		threshold(dst, dst, 10, 255, THRESH_BINARY);
		imshow("diff", dst);
		prev = frame.clone();
		int key = waitKey(33);
		if (key == 27)
			break;
	}
}

void sol_problem_2(char *name) {
	VideoCapture cap(name);
	Mat frame, display;
	bool stop = false;
	bool graymode = false;

	while (1) {
		if (!stop) {
			cap >> frame;
			if (frame.empty())
				break;
		}

		if (graymode == true)
			cvtColor(frame, display, COLOR_BGR2GRAY);
		else
			display = frame;
		imshow("display", display);

		int key = waitKey(30);
		if (key == 27)
			break;
		else if (key == ' ')
			stop = !stop;
		else if (key == '0')
			graymode = true;
		else if (key == '1')
			graymode = false;		
	}
}

void sol_problem_1(char *name) {
	Mat img = imread(name); 
	Mat hsv;
	vector<Mat> colors(7);
	cvtColor(img, hsv,COLOR_BGR2HSV);
	inRange(hsv, Scalar(0,200,0), Scalar(5,255,255), colors[0]); //red  (0)
	inRange(hsv, Scalar(10,200,0), Scalar(20,255,255), colors[1]); // orange (15)
	inRange(hsv, Scalar(20, 200, 0), Scalar(30, 255, 255), colors[2]); // yellow (27.75)
	inRange(hsv, Scalar(50, 200, 0), Scalar(65, 255, 255), colors[3]); // green (60)
	inRange(hsv, Scalar(80, 200, 0), Scalar(100, 255, 255), colors[4]);//  light blue (99.75)
	inRange(hsv, Scalar(105, 200, 0), Scalar(125, 255, 255), colors[5]); // blue (109.5)
	inRange(hsv, Scalar(130, 200, 0), Scalar(150, 255, 255), colors[6]); //purple (133.5)

	imshow("src", img);
	char cname[20];
	//for (int i = 0; i < colors.size(); i++)
	//{
	//	if (!colors[i].empty()) {
	//		sprintf(cname, "color-%d", i);
	//		imshow(cname, colors[i]);
	//	}
	//}

	Mat eroded, dilated;
	Mat merged(img.size(), CV_8UC1, Scalar(0));
	for (int i = 0; i < colors.size(); i++) {
		if (colors[i].empty()) 
			continue;
		erode(colors[i], eroded, Mat(),Point(-1,-1),2);
		dilate(colors[i], dilated, Mat(), Point(-1, -1),2);
		colors[i] = dilated-eroded;
		//sprintf(cname, "contour-%d", i);
		//imshow(cname, colors[i]);
		bitwise_or (merged, colors[i], merged);
	}

	bitwise_not(merged, merged);
	imshow("merged", merged);
	cvtColor(merged, merged, COLOR_GRAY2BGR);
	bitwise_and(merged, img, img);

	imshow("result", img);
	waitKey(0);
}

