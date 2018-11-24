#include "cv.hpp"
#include <iostream>


using namespace cv;
using namespace std;


int main() {
	VideoCapture cap("Road_image.mp4");
	Mat frame, YcbCr, gray, blur, white_edge, white_rect, yellow_edge, yellow_rect, result;
	int time;

	Rect rect1(250,270,200,130);  // yellow
	Mat bgyellow(Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)), CV_8UC1, Scalar(0));
	rectangle(bgyellow, rect1, Scalar(255), -1);
	Rect rect2(450, 270, 230, 130); // white
	Mat bgwhite(Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)), CV_8UC1, Scalar(0));
	rectangle(bgwhite, rect2, Scalar(255), -1);

	float rho, theta, a, b, x0, y0, slope, sum_theta, sum_rho;
	int theta_count, rho_count;
	Point p1, p2;
	vector <Vec2f> lines;

	vector<Mat> channels(3);

	while (1) {
		time = cap.get(CAP_PROP_POS_MSEC);
		if (cap.grab() == 0 or time ==10000) break;
		cap.retrieve(frame);
		cvtColor(frame, gray, CV_BGR2GRAY);
		GaussianBlur(gray, blur, Size(3, 3), 0, 0);
		result = frame.clone();


		// white
		Canny(blur, white_edge, 40, 80, 3);
		white_edge.copyTo(white_rect, bgwhite);
		HoughLines(white_rect, lines, 1, CV_PI / 180, 70);

		sum_theta = 0;
		theta_count = 0;
		sum_rho = 0;
		rho_count = 0;
		for (int i = 0; i < lines.size(); i++) {
			rho = lines[i][0];
			theta = lines[i][1];
			a = cos(theta);
			b = sin(theta);
			x0 = a * rho;
			y0 = b * rho;

			p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
			p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

			if (x0 == 0) {
				slope == 1/100.0f;
			}
			else {
				slope = (y0 / float(x0));
			}			
			slope = 1 / slope;

			if (-0.8f < slope and slope < -0.4f) {
				sum_theta += theta;
				theta_count+=1;
				sum_rho += rho;
				rho_count+=1;
			}
		}
		a = cos(sum_theta / theta_count);
		b = sin(sum_theta / theta_count);

		x0 = a * (sum_rho / rho_count);
		y0 = b * (sum_rho / rho_count);

		slope = (y0 / float(x0));
		slope = 1 / slope;
		cout << "white_slope : " << slope << endl;

		p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
		p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

		line(result, p1, p2, Scalar(0, 255, 0), 3, 8);

		// yellow
		cvtColor(frame, YcbCr, CV_BGR2YCrCb);
		GaussianBlur(YcbCr, YcbCr, Size(3, 3), 0, 0);
		split(YcbCr, channels);
		Canny(channels[2], yellow_edge, 8, 16, 3);
		yellow_edge.copyTo(yellow_rect, bgyellow);		
		HoughLines(yellow_rect, lines, 1, CV_PI / 180, 30);

		sum_theta = 0;
		theta_count = 0;
		sum_rho = 0;
		rho_count = 0;
		for (int i = 0; i < lines.size(); i++) {
			rho = lines[i][0];
			theta = lines[i][1];
			a = cos(theta);
			b = sin(theta);

			x0 = a * rho;
			y0 = b * rho;

			p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
			p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

			if (x0 == 0) {
				slope == 1 / 100.0f;
			}
			else {
				slope = (y0 / float(x0));
			}
			slope = 1 / slope;

			if (0.7f < slope and slope < 1.5f) {
				sum_theta += theta;
				theta_count += 1;
				sum_rho += rho;
				rho_count += 1;
			}
		}
		a = cos(sum_theta / theta_count);
		b = sin(sum_theta / theta_count);

		x0 = a * (sum_rho / rho_count);
		y0 = b * (sum_rho / rho_count);

		slope = (y0 / float(x0));
		slope = 1 / slope;
		cout << "yellow_slope : " << slope << endl << endl;;

		p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
		p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

		line(result, p1, p2, Scalar(0, 255, 0), 3, 8);

		imshow("result", result);
		
		waitKey(33);

	}

	waitKey(0);
	return 0;
}


