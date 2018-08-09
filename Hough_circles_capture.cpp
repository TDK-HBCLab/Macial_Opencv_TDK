/*

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>


using namespace cv;
using namespace std;

vector<Vec3f> circles;

int main()
{
	//---------------------TrackBar----------------------------------------------------------
	namedWindow("Trackbar Window", CV_WINDOW_AUTOSIZE);  //TrackBar Windows
	//-----Gauss-ksize
	int k = 3;
	int n = 0;
//	k = 2*n + 3;  //ksize(k,k),k >= 3
	Size ksize = Size(k, k);
	createTrackbar("Gauss_ksize", "Trackbar Window", &n, 6);
	//-----houghCircle-dp,minDist,param1,param2,minR,maxR
	double dp = 1;
	double det = 0.1;
	int m=0;
//	dp = dp - (double)m * det;
	createTrackbar("hough_dp", "Trackbar Window", &m, 10);
	
	double minDist = 10;
	int a = 0;
	minDist = minDist + (double)a * 10.0;
	createTrackbar("minDist", "Trackbar Window", &a, 10);
	
	int param1 = 150;
	createTrackbar("param1", "Trackbar Window", &param1, 300);

	int param2 = 200;
	createTrackbar("param2", "Trackbar Window", &param2, 300);

	int minR = 0;
	createTrackbar("minRadius", "Trackbar Window", &minR, 300);

	int maxR = 0;
	createTrackbar("maxRadius", "Trackbar Window", &maxR, 300);


	VideoCapture capture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 480);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
	//ros::init(argc, argv, "circle_tracking_node");
	//ros::NodeHandle nh;
	//geometry_msgs::Point msg
	//ros::Publisher pub = nh.advertise<geometry_msgs::Point>("center_position",1000);
	while ((char(waitKey(1)) != 27)) {      //|| (ros::ok())) {

		Mat frame;
		capture >> frame;
		Mat midImage;
		//resize(frame, frame, Size(640, 480), 0, 0, CV_INTER_AREA);
		flip(frame, frame, 1);
		cvtColor(frame, midImage, COLOR_BGR2GRAY);
	
		k = 2 * n + 3;  //ksize(k,k),k >= 3
		Size ksize = Size(k, k);
		GaussianBlur(midImage, midImage, ksize, 2, 2);
		imshow("Gauss", midImage);//show

//		inRange(midImage, Scalar(0, 0, 0), Scalar(180, 255, 60), midImage);
//		bitwise_not(midImage, midImage);
//		cvtColor(frame, frame, COLOR_BGR2HSV);
//		frame.copyTo(frame, midImage);
//		cvtColor(frame, midImage, COLOR_HSV2BGR);
//		cvtColor(midImage, midImage, COLOR_BGR2GRAY);
		dp = dp - (double)m * det;
		minDist = minDist + (double)a * 10.0;
		HoughCircles(midImage, circles, HOUGH_GRADIENT, (double) dp,(double) minDist, param1, param2, minR, maxR);
		Point center;
		int radius;
		for (int i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			radius = cvRound(circles[i][2]);
			circle(frame, center, radius, Scalar(255, 0, 0), 3, CV_AA, 0);
			circle(frame, center, 2, Scalar(0, 0, 255), 3, CV_AA, 0);
			cout << "x= " << center.x << ", y= " << center.y << endl;
		}
		center.x = 0;
		center.y = 0;
		imshow("contours", frame);
	

	} //while
	return 0;
}


*/