#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#include "myopencv.h"


Point2f abs_diff_ratio(Point2f ref, Point2f eval)
{
	float x_error_ratio = abs(ref.x - eval.x) / ref.x;
	float y_error_ratio = abs(ref.y - eval.y) / ref.y;

	return Point2f(x_error_ratio, y_error_ratio);
}

Mat img_contour_Mat(Mat &src) {
// Assume the input image is a binary
        Mat last_sample;
        Mat canny;
        Canny(src, canny, 80, 130);
        dilate(canny, canny, Mat(), Point(-1, -1), 1);
        //imshow("Canny", canny);

        int inputWidth = canny.cols;
        int inputHeight = canny.rows;

        Point2f center(inputWidth / 2, inputHeight / 2);
        cout << canny.cols << ","<< canny.rows << ":("<< center.x <<","<< center.y <<")"<< endl;

        //find contours
        int cnt = 0;
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(canny.clone(), contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        cnt = contours.size();

        // Delete from here
    /*  Mat result = src.clone();
        for (size_t i = 0; i < cnt; i++)
        {
                Scalar color(rand() % 256, rand() % 256, 255);
                drawContours(result, contours, i, color, 1);
                Rect bb = boundingRect(contours[i]);
                char msg[10];
                sprintf(msg, "%d-th cnot", i);
                cout << msg << bb.tl() << ","<< bb.br()<< " = "<< (bb.br() - bb.tl()) <<":"<< (bb.br() + bb.tl()) / 2 << endl;
                //putText(src, msg, (bb.br() + bb.tl()) / 2, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255) - color, 2);
        }
        imshow("src", src);
        imshow("result", result);
        waitKey(0);
        destroyAllWindows();
      */
    // Upto here in executable

        if (cnt <= 0) {
                cout << "ERROR: No component !!" << endl;
                last_sample = Mat(0, 0, 0, 0);
        }
        else if (cnt == 1) {
                Rect bb = boundingRect(contours[0]);
                Point2f bb_center((bb.br() + bb.tl()) / 2);
                Point2f error_ratio = abs_diff_ratio(bb_center, center);
                if (error_ratio.x < 0.3 && error_ratio.y < 0.3)
                        last_sample = Mat(src, bb);
        }
        else {
                Point2f leastError(1.0, 1.0);
                int best_cont = 0;
                for (size_t i = 0; i < cnt; i++)
                {
                        Rect bb = boundingRect(contours[i]);
                        Point2f sizeError = abs_diff_ratio(Point2f(bb.x, bb.y), Point2f(inputWidth, inputHeight));
                        if ((bb.size().width >= inputWidth - 10 && bb.size().height >= inputHeight - 10) ||
                                (sizeError.x <= 0.2 && sizeError.y <= 0.2))  {
                                best_cont = i;
                                break;
                        }

                        Point2f bb_center((bb.br() + bb.tl()) / 2);
                        Point2f error(abs(center.x - bb_center.x) / center.x, abs(center.y - bb_center.y) / center.y);
                        if (error.x <= leastError.x && error.y <= leastError.y) {
                                best_cont = i;
                        }

                }
                Rect best_bb = boundingRect(contours[best_cont]);
                last_sample = Mat(src, best_bb);
        }

        return last_sample;
}
