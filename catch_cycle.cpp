/*

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include "Color_indentify.h"
//---------------------[declare]------------------------
//
//------------------------------------------------------

using namespace std;
using namespace cv;

Mat frame;
Mat midImage;
Mat dst_img;

int main(int argc, char const *argv[]) {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "!!!!!WEBCAM NOT OPEN!!!!!"<<endl;
		return -1;
	} //if_loop_1
	while (char(waitKey(1)) != 27) {

		cap >> frame;
//----------BGR2GRAY-------------------------------------
//		cvtColor(cap_frame, mid_img, COLOR_BGR2GRAY);
//		GaussianBlur(mid_img, mid_img, Size(5, 5), 2, 2);
//----------HOUGH_CIRCLES--------------------------------
		resize(frame, midImage, Size(360, 280), 0, 0, INTER_AREA);
		vector<Vec3f> circles;
		Canny(src_img, mid_img, 50, 150,5);
		HoughCircles(mid_img, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);
//----------Draw_CIRCLE-----------------------------------
		for (size_t i = 0; i < circles.size(); i++) 
		{
			Point center(circles[i][0], circles[i][1]);
			int radius = circles[i][2]; 
			circle(src_img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//			circle(src_img, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}

		namedWindow("Hough_Circles", WINDOW_NORMAL);
		imshow("Hough_Circles", mid_img);
		imshow("Hough_Circles", src_img);
		if (waitKey(1) == 'q') {
			cout << "~~~~bye~~~~";
			break;
		} //if_loop_2
	} //while_loop
	return 0;
} //main_loop


/*
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
//#include <ros/ros.h>
//#include <geometry_msgs/Point.h>

using namespace cv;



int main(int argc, char** argv) {
	Mat frame, gray, canny_output;
	Mat structuringElement = getStructuringElement(MORPH_RECT, Size(10, 10));

//	bool show_frame = false;
//	if (argc>1) {
//		if (argv[1][0] == 'y' || argv[1][0] == 'Y') {
//			show_frame = true;
//		}
//	}
	//VideoCapture cap(1);
	VideoCapture cap(0);
//	ros::init(argc, argv, "ball_detect_node");
//	ros::NodeHandle nh;
//	geometry_msgs::Point msg;

//	ros::Publisher pub = nh.advertise<geometry_msgs::Point>("ball_position", 1000);	//cam_ball

	//std::vector<Vec3f> circles;

	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;

	//std::deque<Point2f> trac(32);

	while (char(waitKey(1)) != 27){//ros::ok()) {
		cap.read(frame);

		cvtColor(frame, gray, CV_BGR2GRAY);

		GaussianBlur(gray, gray, Size(9, 9), 2, 2);
		threshold(gray, gray, 50, 255, THRESH_BINARY_INV);
		//imshow("thre",gray);
		dilate(gray, gray, structuringElement);
		//imshow("dilate",gray);
		erode(gray, gray, structuringElement);
		//imshow("erode",gray);

		Canny(gray, canny_output, 100, 200, 3);
		findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);


		//std::vector<std::vector<Point> > contours_output(contours.size());
		std::vector<Point2f> center(contours.size());
		std::vector<float> radius(contours.size());

		//Mat drawing = Mat::zeros( frame.size(), CV_8UC3);

		RNG rng(12345);
		for (int i = 0; i< contours.size(); i++)
		{
			//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			minEnclosingCircle((Mat)contours[i], center[i], radius[i]);
			if (radius[i]>40) {
				circle(frame, center[i], (int)radius[i], Scalar(0, 0, 255), 2, 8, 0);
				circle(frame, center[i], 2, Scalar(0, 255, 0), 2, 8, 0);
//				msg.x = (int)center[i].x;
//				msg.y = (int)center[i].y;
//				msg.z = 1;

				//trac.push_front(center[i]);

//				ROS_INFO_STREAM("Found a ball at (" << msg.x << ", " << msg.y << ")");
				break;
			}
			center[0].x = 0;
			center[0].y = 0;
//			msg.z = 0;

			//trac.push_front(center[0]);
		}
		/*
		std::deque<Point2f>::iterator it = trac.begin();
		int cnt=0;
		while(it != trac.end()){
		int thickness = 4 - cnt++/8;
		Point2f p = *it;
		Point2f p2 = *(it+1);
		if(p.x==0 || p2.x==0) {
		it++;
		continue;
		}
		else if(thickness>0) line(frame, *it, *(it+1), Scalar(0, 255, 0), thickness );
		else{
		trac.pop_back();
		continue;
		}
		it++;
		}
		if (trac.size()>=32) trac.pop_back();
		ROS_DEBUG_STREAM(trac.size());
		if (msg.z == 0) ROS_ERROR_STREAM("Where's the ball???");
		pub.publish(msg);
		line(frame, Point(320,220), Point(320,260), Scalar(0, 0, 0), 2, 8);
		line(frame, Point(300,240), Point(340,240), Scalar(0, 0, 0), 2, 8);
		if(show_frame) imshow("From Webcam", frame);
		
//		if (waitKey(10) == 27) break;
	}
	return 0;
}*/