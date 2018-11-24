#include "cv.hpp"
#include <iostream>


using namespace cv;
using namespace std;


int main() {
	VideoCapture cap("Road_image.mp4");
	Mat frame, gray, blur, result, white_edge, white_rect, yellow_edge, yellow_rect, YCbCr;
	int time;

	Rect rect1(250, 270, 200, 130);  // yellow
	Mat bgyellow(Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)), CV_8UC1, Scalar(0));
	rectangle(bgyellow, rect1, Scalar(255), -1);
	Rect rect2(450, 270, 230, 130); // white
	Mat bgwhite(Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)), CV_8UC1, Scalar(0));
	rectangle(bgwhite, rect2, Scalar(255), -1);

	vector<Vec4i> lines;
	float slope, x, y;
	float sum_x1, sum_x2, sum_y1, sum_y2;
	int count_x1, count_x2, count_y1, count_y2;
	int x1, y1, x2, y2;

	vector<Mat> channels(3);

	while (1) {
		time = cap.get(CAP_PROP_POS_MSEC);
		if (cap.grab() == 0 or time == 10000) break;
		cap.retrieve(frame);
		result = frame.clone();
		
		//white
		cvtColor(frame, gray, CV_BGR2GRAY);
		GaussianBlur(gray, blur, Size(3, 3), 0, 0);
		Canny(blur, white_edge, 40, 80, 3);
		white_edge.copyTo(white_rect, bgwhite);
		HoughLinesP(white_rect, lines, 1, CV_PI / 180, 60, 40, 120);

		sum_x1 = sum_x2 = sum_y1 = sum_y2 = 0;
		count_x1 = count_x2 = count_y1 = count_y2 = 0;
		for (int i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];

			x = l[2] - l[0];
			y = l[3] - l[1];
			
			if (x == 0) {
				slope = 100.0f;
			}
			else {
				slope = (-1)*(y / float(x));
			}

			if (-1.0f< slope and slope< -0.35f) {
				sum_x1 += l[0];
				sum_y1 += l[1];
				sum_x2 += l[2];
				sum_y2 += l[3];
				count_x1 += 1;
				count_x2 += 1;
				count_y1 += 1;
				count_y2 += 1;				
			}		
		}

		x1 = sum_x1 / count_x1;
		y1 = sum_y1 / count_y1;
		x2 = sum_x2 / count_x2;
		y2 = sum_y2 / count_y2;
		line(result, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 3, 8);


		// yellow
		cvtColor(frame, YCbCr, CV_BGR2YCrCb);
		GaussianBlur(YCbCr, YCbCr, Size(3, 3), 0, 0);
		split(YCbCr, channels);
		Canny(channels[2], yellow_edge, 6, 12, 3);
		yellow_edge.copyTo(yellow_rect, bgyellow);
		HoughLinesP(yellow_rect, lines, 1, CV_PI / 180, 45, 60, 200);

		sum_x1 = sum_x2 = sum_y1 = sum_y2 = 0;
		count_x1 = count_x2 = count_y1 = count_y2 = 0;
		for (int i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];

			x = l[2] - l[0];
			y = l[3] - l[1];

			if (x == 0) {
				slope = 100.0f;
			}
			else {
				slope = (-1)*(y / float(x));
			}

			if (0.3f< slope and slope <2.0f) {
				sum_x1 += l[0];
				sum_y1 += l[1];
				sum_x2 += l[2];
				sum_y2 += l[3];
				count_x1 += 1;
				count_x2 += 1;
				count_y1 += 1;
				count_y2 += 1;
			}
		}

		x1 = sum_x1 / count_x1;
		y1 = sum_y1 / count_y1;
		x2 = sum_x2 / count_x2;
		y2 = sum_y2 / count_y2;
		line(result, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 3, 8);

		imshow("result", result);
		waitKey(33);
	}

	waitKey(0);
	return 0;
}


