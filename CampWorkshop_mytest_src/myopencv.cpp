#include "opencv2/opencv.hpp"
#include "myopencv.h"

using namespace cv;
using namespace std;

int testVideo();
void mytest();
void imgtest();
void morph_test(char *name);
void Split_merge_test();
int Video_test();

int main(int argc, char **argv){
	char * name;
	if (argc < 2){
		//cout << "Too few arguments" << endl;
		//return 0;
	}
	else
		name = argv[1];

	testVideo();
	//Video_test();
	//Split_merge_test();
	//imgtest();

	//testVideo();
	//mytest();
	//morph_test("images/Fig09_mask.tif");
	//basic_test_constructor();
	//basic_test_subMat("images/Fig06_lenna.tif");
	//basic_test_split("images/Fig06_color_bars.tif");
	//color_test("images/Fig06_lenna.tif");
	//  basic_test_deepCopy("images/Fig06_lenna.tif");
	//basic_traversal("images/Fig06_lenna.tif");
	//negative_test("images/Fig06_lenna.tif");
	// draw_log_scale();
	//process_ImageLog("images/Fig03_DFT.tif");
	//process_ImageLog_math("images/Fig03_DFT.tif");
	//process_Log_LUT("images/Fig03_DFT.tif");
	//draw_exp_scale();
	//process_Image_exp("images/Fig02_cameraman.tif");

	//smoothing("Images/lenna_gray_400.tif");
	//face_blur("images/face_senior.jpg");
	//GaussianTest("images/lenna_gray_400.tif");
	//myfilter_test("images/lenna_gray_400.tif");
	//myfilter2_test("images/Fig05_gaussian.tif");
	//Laplacian_Test("images/Fig06_lenna.tif");
   // smoothing("images/Fig05_gaussian.tif");
    //morph_testing("images/Fig09_mask.tif");
	// gamma_tranformTest("images/Fig03_spine.tif");
	//gamma_tranformTest("images/Fig03_aerial.tif");
	//histogram_Test("images/Fig03_pollen2.tif");
	// gamma_tranformTest("images/Fig02_Einstein.tif");
	// process_Gamma_LUT("images/Fig03_spine.tif");
	//negativeTest("images/Fig03_Xray.tif");
	//histogram_Test("images/Fig02_EinStein.tif");
	//histogram_Test("images/Fig03_pollen2.tif");
	//histogram_Test("images/Fig03_pollen4.tif");
	//histogram_Test("images/Fig10_std_10.tif");
	// histogram_Test("images/Fig06_lenna.tif");
	// negativeTest("images/Fig06_lenna.tif");
   	// sharpening("images/Fig03_dipxe.tif");
	//frame_diff();
	//histogram_Test("images/Fig10_std_50.tif");

	//color_darken("images/Fig06_strawberry.tif");
	//color_inverse("images/Fig06_strawberry.tif");
	//test_color();
	//color_correction("images/Fig06_chalk.tif");
	// color_enhance("images/Fig06_chalk.tif");
	// color_ratio("images/Fig06_woman_baby.tif");
	//color_smooth("images/Fig06_lenna.tif");
	//color_laplacian("images/Fig06_lenna.tif");
	//color_segment("images/face.jpg");
	//mouse_event("images/face.jpg");
	// sol_problem_1("images/Tangram/bird.png");
	//  sol_problem_4("images/lane.avi");
	// sol_problem_5("images/flower.png");

	//change_colortone("images/Fig06_lenna.tif");
	//  change_contrast_brightness("images/Fig06_lenna.tif");
	//change_colortone("images/Fig06_Flower.tif");

	//thresh_test(name); // ("images/lenna_gray_400.tif");
	//thresh_otsu(name); // ("images/Fig10_std_50.tif");
	//adaptive_threshold_test(name); // ("images/lenna_gray_400.tif");

	
	//frame_MOG2("images/Highway-Surveillance.mp4");

	//vignette("images/flower.png");
	
	//edges("images/Fig04_house.tif");
	//Sobel_edges("images/Fig04_house.tif");
	//Scharr_edges("images/Fig04_house.tif");
	//Sobel_Magnitudes("images/Fig04_house.tif");

    // Canny_test_thresholds("images/Fig04_house.tif");
	//noisy_Canny("images/lenna_gray_400.tif");
	//Canny_edges("noisy_lenna_20.tif");
	//Laplacian_Test("images/Fig04_house.tif");
	//extract_line("images/Tangram/bird.png");
	// extract_line("images/Fig04_house.tif");
	// contours("images/Tangram/bird.png");
	//contours("images/Tangram/bird_test.png");
	 // approxContour("images/Tangram/bird_cropping.jpg");
	//count_Polygon("images/Tangram/bird_cropping.jpg");
    //morph_testing("images/Fig09.tif");
	//morph_testing("images/Fig09_text.tif");

	//sol_problem_6("images/Tangram/bird.png");
	//sol_problem_7("images/Tangram/bird_cropping.jpg");
	//sol_problem_9("images/highway-surveillance.mp4");
	//testVideo();

	//img_box("../testimg/t_1.JPG");
	
	//watershed_test(name);
/*
  int selector = 0;
  if (argc > 2) {
	  selector = atoi(argv[2]);
	  switch (selector) {
	  case 0: kmeans_test(name); break;
	  case 1: cout << "#of components = " << connected_component(name) << endl; break;
		  // weak for noisy image coin_B10.jpg, coin_B11.jpg, coin_B3.jpg, coin_B9.jpg
	  case 2:  std::cout << "# of Coins = " << count_coin1(name) << std::endl; break;
		  // weak for noisy and touching image: coint_B2, B3, B4, B5,
	  case 3: std::cout << "# of Coins = " << count_coin2(name); break;
		  // weak for overlapped image : coin_B7, B8, B12, B15
	  case 4: std::cout << "# of coins = " << count_coin3(name, false) << std::endl; break;
	  case 5: std::cout << "# of coins = " << count_coin3(name, true) << std::endl; break;
	  default: cout << "none selected" << endl;
	  }
  }
  */
}

int Video_test(){
	Mat frame;
	VideoCapture cap(".//images//video.mp4");

	if(!cap.isOpened()){
		cerr << "file open fail!" << endl;
		return -1;
	}

	while(1){
		cap >> frame;
		if(frame.empty()){
			cerr << "empty frame" << endl;
			break;
		}

		imshow("vid", frame);
		waitKey(33);
	}
}

void Split_merge_test(){
	Mat img1 = imread(".//images//marco.jpg",1);
	Mat img2 = imread(".//images//rain.jpeg",1);

	Mat result;
	Mat img2_gray;
	vector<Mat> channels;

	resize(img1, img1, Size(300,300));
	resize(img2, img2, Size(300,300));
	cvtColor(img2, img2_gray, COLOR_BGR2GRAY);
	imshow("marco_orig",img1);
	imshow("rain_orig",img2);
	imshow("rain_gray",img2_gray);

	split(img1, channels);
	channels[1] += img2_gray;
	imshow("marco_ch[1]",channels[1]);
	merge(channels, result);
	imshow("result",result);

	waitKey(0);
}

int testVideo() {
	VideoCapture cap("/dev/video2");
	Mat frame;
	Mat prevframe;
	cap >> prevframe;
	cvtColor(prevframe, prevframe, COLOR_BGR2GRAY);
	Mat difframe;
	while (1) {
		cap >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		absdiff(frame, prevframe, difframe);
		prevframe = frame.clone();
		imshow("frame", frame);
		threshold(difframe, difframe, 30, 255, THRESH_BINARY);
		imshow("framdiff", difframe);
		int key = waitKey(33);
		if (key == ' ')
			break;
	}
	
}

void mytest() {
	Mat A(100, 200, CV_8UC3, Scalar(255,0,0));
	Mat T(A, Rect(30, 50, 100, 50));
	Mat B;

	//T.copyTo(B);
	B = T.clone();

	imshow("A1", A);
	imshow("B1", B);

	B = Scalar(0, 0, 255);
	imshow("A2", A);
	imshow("B2", B);
	waitKey(0);
}

void imgtest(){
	Mat img = imread(".//images//lena.jpg",1);
	namedWindow("image");
	imshow("image", img);
	waitKey(0);
}

void morph_test(char *name) {
	Mat img = imread(name);
	Mat gray;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat erd, dil, gray2;
	morphologyEx(gray, gray2, MORPH_OPEN, element);
	//erode(gray, erd, element, Point(-1, -1), 1);
	//dilate(erd, gray2, element, Point(-1, -1), 1);
	//imshow("erode", erd);
	imshow("erd-dil", gray2);

	dilate(gray, dil, element, Point(-1, -1), 1);
	imshow("dilate", dil);

	waitKey(0);
}