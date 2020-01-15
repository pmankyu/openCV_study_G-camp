
#include "opencv2/opencv.hpp"
#include "myopencv.h"
using namespace cv;
using namespace std;

int count_coin1(char *name)
{
	Mat image = imread(name);
	Mat gray;

	float width = 500;
	float height = width * ((double)image.rows / (double)image.cols);

	resize(image, image, Size(width, height));
	imshow("src", image);

	cvtColor(image, gray, COLOR_BGR2GRAY);
	//GaussianBlur(gray, gray, Size(3, 3), 1.5, 1.5);
	GaussianBlur(gray, gray, Size(7, 7), 1.5, 1.5);

	std::vector<Vec3f> coin;
	// A vector data type to store the details of coins.
	
	imshow("gray", gray);
	Canny(gray, gray, 180, 120, 3);
	imshow("canny", gray);
	//HoughCircles(gray, coin, HOUGH_GRADIENT, 2, 30, 120, 120, 30, 150);
	HoughCircles(gray, coin, HOUGH_GRADIENT, 2, 30, 180, 120, 30, 100); // best option

	// Argument 1: Input image mode
	// Argument 2: A vector that stores 3 values: x,y and r for each circle.
	// Argument 3: CV_HOUGH_GRADIENT: Detection method.
	// Argument 4: The inverse ratio of resolution.
	// Argument 5: Minimum distance between centers.
	// Argument 6: Upper threshold for Canny edge detector.
	// Argument 7: Threshold for center detection.
	// Argument 8: Minimum radius to be detected. Put zero as default
	// Argument 9: Maximum radius to be detected. Put zero as default

	int L = coin.size();
	// Get the number of coins.

	std::cout << "\n The number of coins is: " << L << "\n\n";

	// To draw the detected circles.
	for (size_t i = 0; i < coin.size(); i++)
	{
		Point center(cvRound(coin[i][0]), cvRound(coin[i][1]));
		// Detect center
		// cvRound: Rounds floating point number to nearest integer.
		int radius = cvRound(coin[i][2]); // To get the radius from the second argument of vector coin.
		circle(image, center, 4, Scalar(0, 255, 0), -1, 8, 0);
		// circle center

		//  To get the circle outline.     
		circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0); // circle outline
		std::cout << " Center location for circle " << i + 1 << " :"
			<< center << "\n Diameter : " << 2 * radius << "\n";
	}
	std::cout << "\n";
	imshow("res", image);
	waitKey(0);
	return L;
}

int count_coin2(char *name) {
	Mat image = imread(name);
	Mat gray, thresh, edges, morph;

	float width = 500;
	float height = width * ((double)image.rows / (double)image.cols);

	resize(image, image, Size(width, height));

	imshow("input", image);

	GaussianBlur(image, image, Size(7, 7), 1.5, 1.5);
	cvtColor(image, gray, COLOR_BGR2GRAY);
	//threshold(gray, thresh, 180, 255, THRESH_BINARY);
	threshold(gray, thresh, 0, 255, THRESH_OTSU | THRESH_BINARY);
	medianBlur(thresh, thresh, 3);
	imshow("thresh", thresh);

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(thresh, morph, MORPH_CLOSE, element, Point(-1, -1), 1);
	//dilate(thresh, morph, element,Point(-1,-1), 1);
	//erode(morph, morph, element);
	imshow("dilate", morph);

	vector<vector<Point>> cont;
	vector<Vec4i> hierarchy;
	findContours(morph.clone(), cont, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	int coin_num = 0;
	for (int i = 0; i < cont.size(); i++) {
		Rect bb = boundingRect(cont[i]);
		if (bb.area() < 80)
			continue;
		Scalar color = Scalar(rand() % 256, rand() % 256, rand() % 256);
		drawContours(image, cont, i, color, 4);
		char text[100];
		sprintf(text, "%d", ++coin_num);
		putText(image, text, Point((bb.br() + bb.tl()) / 2), FONT_HERSHEY_PLAIN, 4.5, color, 3);
		std::cout << "Coin " << coin_num << " bb= " << bb << std::endl;
	}
	cout << "coint # = " << coin_num << endl;
	imshow("gray", gray);


	imshow("cont", image);

	waitKey(0);
	return coin_num;
}

void contrast(Mat & gray) {

	Mat edges;
	Canny(gray, edges, 30, 120, 3);
	dilate(edges, edges, Mat(), Point(-1, -1), 2);
	imshow("edge", edges);
	
	Mat mask(edges.size(), CV_8UC1, Scalar(0));
	vector<vector<Point>> cont;
	findContours(edges, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < cont.size(); i++) {
		if (boundingRect(cont[i]).area() > 200) {
			drawContours(mask, cont, i, Scalar(255), -1);
		}
	}
	imshow("mask_rect", mask);
	bitwise_and(gray, mask, gray);
	add(gray,Mat(gray.size(),CV_8UC1, Scalar(20)), gray, mask);
	gray = gray.mul(Mat(gray.size(), CV_8UC1, Scalar(1.2)));
	//gray = apply_gamma_transform(gray, 0.5);
	imshow("contrast", gray);
}

void DTofDT(Mat & sure_fg) {
	vector<vector<Point>> cont;
	int peanuts = 0;
	Mat undef(sure_fg.size(), CV_8UC1, Scalar(0));
	findContours(sure_fg, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < cont.size(); i++) {
		RotatedRect rrect = minAreaRect(cont[i]);
		float w = rrect.size.width;
		float h = rrect.size.height;
		float ratio = (w > h) ? (w / h) : (h / w);
		if (ratio > 2.0) {
			peanuts++;
			drawContours(undef, cont, i, Scalar(255), -1);
			drawContours(sure_fg, cont, i, Scalar(0), -1);
		}
	}

	if (peanuts <= 0)
		return;
	
	Mat dist2, dist_norm2, sure_fg2;
	//cvtColor(sure_fg, sure_fg3U, COLOR_GRAY2BGR);
	distanceTransform(undef, dist2, DIST_L2, 3);
	normalize(dist2, dist_norm2, 0, 1, NORM_MINMAX);
	imshow("**dist_thresh2", dist_norm2);
	threshold(dist_norm2, dist_norm2, 0.5, 1.0, THRESH_BINARY);
	dist_norm2 = dist_norm2 * 255;
	dist_norm2.convertTo(sure_fg2, CV_8U);
	Mat element5x5 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(sure_fg2, sure_fg2, MORPH_OPEN, element5x5, Point(-1, -1), 1);
	imshow("**sure_fg2", sure_fg2);
	bitwise_or(sure_fg, sure_fg2, sure_fg);
	
	return ;
}

int count_coin3(char *name, bool preprocessing) {
	Mat img = imread(name);
	Mat src_gray, src, gray, bin, open, dist, dist_norm, sure_bg, sure_fg, morph;

	float width = 500;
	float height = width * ((double)img.rows / (double) img.cols);

	resize(img, src, Size(width, height));
	imshow("input", src);

	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	GaussianBlur(src_gray, gray, Size(11, 11), 1.5, 1.5);
	imshow("gray", gray);

	if(preprocessing)
	   contrast(gray); //*** special preprocessing

	threshold(gray, bin, 0, 255, THRESH_OTSU | THRESH_BINARY);
	//imshow("OTSU", bin);
	medianBlur(bin, bin,11);
	medianBlur(bin, bin, 5);
	imshow("binary image (OTSU + median)", bin);

	Mat element5x5 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat element3x3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(bin, open, MORPH_OPEN, element3x3,Point(-1,-1), 2);
	//imshow("opening", open);

	dilate(open, sure_bg, element3x3, Point(-1, -1), 3);
	//imshow("sure_bg (close)", sure_bg);

	Mat dist_8U;
	distanceTransform(open, dist, DIST_L2, 5);
	normalize(dist, dist_norm, 0, 1, NORM_MINMAX);
	dist_norm = dist_norm * 255;
	dist_norm.convertTo(dist_8U, CV_8U);
	imshow("normalize", dist_8U);

	double minv, maxv;
	minMaxIdx(dist_8U, &minv, &maxv);
	threshold(dist_8U, sure_fg, maxv*0.6, 255, THRESH_BINARY);
	dilate(sure_fg, sure_fg, element5x5, Point(-1,-1),2);
	imshow("dist_thresh", sure_fg);


	//*** special post processing
	if (preprocessing)
	    DTofDT(sure_fg);
    
	imshow("final fg", sure_fg);

	Mat unknown;
	subtract(sure_bg, sure_fg, unknown);
	imshow("unknown", unknown);

	// Find total markers
	vector<vector<Point> > contours;
	findContours(sure_fg.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(sure_bg.size(), CV_8UC1);
	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(markers, contours, i, Scalar(static_cast<int>(i) + 1), -1);
	}
	markers = markers + 1; // make bg == 1
	int coin_num = contours.size();

	// Draw the background marker
	bitwise_not(unknown, unknown); // make unknown region a zero
	bitwise_and(unknown, markers, markers);

	Mat colormap;
	applyColorMap(markers*(255.0 / (coin_num + 1)), colormap, COLORMAP_JET);
	imshow("colormapped markers", colormap);

	markers.convertTo(markers, CV_32S);
	//imshow("Markers", markers * 10000);

	Mat imgResult(src);
	// Perform the watershed algorithm
	watershed(imgResult, markers);

	//imshow("watershed result", markers*(80000.0/(float)coin_num));
	
    // Generate random colors
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size() + 1; i++)
	{
		int b = theRNG().uniform(0, 256);
		int g = theRNG().uniform(0, 256);
		int r = theRNG().uniform(0, 256);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
	// Create the result image
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index == 255) {    // background
				dst.at<Vec3b>(i, j) = Vec3b(255, 255, 0);
			}
			else if (index == -1) { // border
				dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				dst.at<Vec3b>(i, j) = colors[index - 1];
			}
		}
	}
	// Visualize the final image
	imshow(name, dst);

	waitKey(0);
	return coin_num;
}


void kmeans_test(char * name) {
	Mat src = imread(name);
	float width = 500;
	float height = width * ((double)src.rows / (double)src.cols);
	resize(src, src, Size(width, height));
	imshow("original", src);

	blur(src, src, Size(5, 5));
	imshow("blurred", src);

	Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);
	Mat bestLabels, centers, clustered;
	vector<Mat> bgr;
	cv::split(src, bgr);
	// i think there is a better way to split pixel bgr color
	for (int i = 0; i<src.cols*src.rows; i++) {
		p.at<float>(i, 0) = (i / src.cols) / src.rows;
		p.at<float>(i, 1) = (i%src.cols) / src.cols;
		p.at<float>(i, 2) = bgr[0].data[i] / 255.0;
		p.at<float>(i, 3) = bgr[1].data[i] / 255.0;
		p.at<float>(i, 4) = bgr[2].data[i] / 255.0;
	}

	int K = 2;
	cv::kmeans(p, K, bestLabels,
		TermCriteria(cv::TermCriteria::Type::EPS + cv::TermCriteria::Type::MAX_ITER, 10, 1.0),
		3, KMEANS_PP_CENTERS, centers);

	vector<Vec3b>  colors(K); 
	for (int i = 0; i<K; i++) {
		colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);
	}
	// i think there is a better way to do this mayebe some Mat::reshape?
	clustered = Mat(src.rows, src.cols, CV_8UC3);
	for (int i = 0; i<src.cols*src.rows; i++) {
		int y = i / src.cols;
		int x = i % src.cols;
		clustered.at<Vec3b>(y, x) = colors[bestLabels.at<int>(i)];
	}

//	clustered.convertTo(clustered, CV_8U);
	imshow("clustered", clustered);

	waitKey(0);

}

#include <algorithm>

int connected_component(char *name) 
{

	cv::Mat img, img_edge, labels, centroids, img_color, stats;

	// load image or show help if no image was provided
	
	if ((img = imread(name, IMREAD_GRAYSCALE)).empty())
	{
		cout << "Error File: " << name << endl;
		return -1;
	}

	float width = 500;
	float height = width * ((double)img.rows / (double)img.cols);
	resize(img, img, Size(width, height));
	GaussianBlur(img, img, Size(7, 7), 1.5, 1.5);
	imshow("img", img);

	threshold(img, img_edge, 100, 255, THRESH_BINARY| THRESH_OTSU);
	erode(img_edge, img_edge, Mat(), Point(-1, -1), 2);
	dilate(img_edge, img_edge, Mat(), Point(-1, -1), 2);
	imshow("Image after threshold and open", img_edge);

	int i, nccomps = cv::connectedComponentsWithStats(
		img_edge,
		labels,
		stats,
		centroids
	);
	cout << "Total Connected Components Detected: " << nccomps << endl;
	int comps = 0;
	vector<cv::Vec3b> colors(nccomps + 1);
	colors[0] = cv::Vec3b(0, 0, 0); // background pixels remain black.
	for (i = 1; i <= nccomps; i++) {
		
		if (stats.at<int>(i - 1, cv::CC_STAT_AREA) < 100)
			colors[i] = cv::Vec3b(0, 0, 0); // small regions are painted with black too.
		else {
			colors[i] = cv::Vec3b(rand() % 256, rand() % 256, 255);
			comps++;
		}
	}
	img_color = cv::Mat::zeros(img.size(), CV_8UC3);
	for (int y = 0; y < img_color.rows; y++)
		for (int x = 0; x < img_color.cols; x++)
		{
			int label = labels.at<int>(y, x);
			CV_Assert(0 <= label && label <= nccomps);
			img_color.at<cv::Vec3b>(y, x) = colors[label];
		}

	cv::imshow("Labeled map", img_color);
	cv::waitKey(0);
	return comps;
}

