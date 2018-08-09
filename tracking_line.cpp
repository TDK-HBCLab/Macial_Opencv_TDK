
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
//#include <vector>

using namespace cv;
using namespace std;



int main()
{

	VideoCapture capture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 480);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
	//capture.set();
	namedWindow("test", WINDOW_AUTOSIZE);
	int thres_value = 70;
	createTrackbar("threshold", "test", &thres_value, 255);


	while (char(waitKey(1)) != 27) {
		Mat frame;
		capture >> frame;
		Mat midImage = frame;
		flip(midImage, midImage, 1);

		int height = midImage.rows;
		int width = midImage.cols;
		int ROI_height = height / 5;
		int ROI_width = width;

		cvtColor(midImage, midImage, COLOR_BGR2GRAY);
		GaussianBlur(midImage, midImage, Size(5, 5), 2, 2);
		threshold(midImage, midImage, thres_value, 255, THRESH_BINARY_INV);//反向閾值，因boundingRect為抓取黑色外框。
		//bitwise_not(midImage, midImage);
		//Canny(midImage, midImage, 100, 200, 3);

		//--ROI from frame---
		Mat ROI_TOP = frame(Rect(0, 0, ROI_width, ROI_height));
		Mat ROI_MID = frame(Rect(0, 2 * ROI_height, ROI_width, ROI_height));
		Mat ROI_BUT = frame(Rect(0, 4 * ROI_height, ROI_width, ROI_height));
		//imshow("mid", frame);

		//--ROI from midImage---(欲處理圖像)
		Mat trans_TOP = midImage(Rect(0, 0, ROI_width, ROI_height));
		Mat trans_MID = midImage(Rect(0, 2 * ROI_height, ROI_width, ROI_height));
		Mat trans_BUT = midImage(Rect(0, 4 * ROI_height, ROI_width, ROI_height));
		//drawing ROI 
		rectangle(frame, Rect(0, 0, ROI_width, ROI_height), Scalar(0, 255, 0), 2);
		rectangle(frame, Rect(0, 2 * ROI_height, ROI_width, ROI_height), Scalar(0, 255, 0), 2);
		rectangle(frame, Rect(0, 4 * ROI_height, ROI_width, ROI_height), Scalar(0, 255, 0), 2);

		vector<vector<Point>> TOP_contours, MID_contours, BUT_contours;
		vector<Vec4i> TOP_hierarchy, MID_hierarchy, BUT_hierarchy;

		//findContours
		findContours(trans_TOP, TOP_contours, TOP_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		findContours(trans_MID, MID_contours, MID_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		findContours(trans_BUT, BUT_contours, BUT_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		vector<Rect> TOP_boundRect(TOP_contours.size());
		vector<Rect> MID_boundRect(MID_contours.size());
		vector<Rect> BUT_boundRect(BUT_contours.size());

		Point2i TOP_center, MID_center, BUT_center;
		Point2i abs_top_center, abs_mid_center, abs_but_center;
		Point2i TOP_rect_o, MID_rect_o, BUT_rect_o;

		
		for (int i = 0; i < TOP_contours.size(); i++) {
			TOP_boundRect[i] = boundingRect((Mat)TOP_contours[i]);
			//定義ROI_TOP(藍框)的矩形坐上角點，使其藍框的height能與綠框相符
			TOP_rect_o = Point(TOP_boundRect[i].tl().x, 0);
			//將trans_TOP所得(X,Y)數據丟入ROI_TOP，並使其映射回frame;重複MID,BUT; tl()::point(x,y),br()::point(x+width,y+height)
			rectangle(ROI_TOP, TOP_rect_o, TOP_boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
			//Rect_center & draw_center_point(x,y)
			TOP_center.x = 0.5*(TOP_boundRect[i].tl().x + TOP_boundRect[i].br().x);
			TOP_center.y = 0.5*(TOP_boundRect[i].tl().y + TOP_boundRect[i].br().y);
			cout << "TOP_center(x,y) = " << " ( " << TOP_center.x << " , " << TOP_center.y << " ) ";
			circle(ROI_TOP, TOP_center, 3, Scalar(255, 0, 0), -1, 8);
			abs_top_center = Point(TOP_center.x, TOP_center.y);//absolute_top_center_point in frame
		}
/*
			if (((int) (0.5*ROI_height + TOP_rect_o.y)) == abs_top_center.y) {
				cout << "TOP_center(x,y) = " << " ( " << TOP_center.x << " , " << TOP_center.y << " ) ";
			} //if_TOP
			else {
				cout << "TOP_center point is not at ROI_Rect_center!!!!" << endl;
			} //else

		} //for_TOP
*/

		for (int i = 0; i < MID_contours.size(); i++) {
			MID_boundRect[i] = boundingRect((Mat)MID_contours[i]);
			MID_rect_o = Point(MID_boundRect[i].tl().x, 2 * ROI_height);
			cout << MID_rect_o.y << endl;
			rectangle(ROI_MID, MID_rect_o, MID_boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
			MID_center.x = 0.5*(MID_boundRect[i].tl().x + MID_boundRect[i].br().x);
			MID_center.y = 0.5*(MID_boundRect[i].tl().y + MID_boundRect[i].br().y);
			cout << "MID_center(x,y) = " << " ( " << MID_center.x << " , " << 2 * ROI_height + MID_center.y << " ) ";
			circle(ROI_MID, MID_center, 3, Scalar(255, 0, 0), -1, 8);
			abs_mid_center = Point(MID_center.x, 2 * ROI_height + MID_center.y);
		}
/*			if (((int)(0.5*ROI_height + MID_rect_o.y)) == abs_mid_center.y) {
				cout << "MID_center(x,y) = " << " ( " << MID_center.x << " , " << 2 * ROI_height + MID_center.y << " ) ";
			} //if_TOP
			else {
				cout << "MID_center point is not at ROI_Rect_center!!!!" << endl;
			} //else

		} //for_MID
*/

		for (int i = 0; i < BUT_contours.size(); i++) {
			BUT_boundRect[i] = boundingRect((Mat)BUT_contours[i]);
			BUT_rect_o = Point(BUT_boundRect[i].tl().x, 4 * ROI_height);
			rectangle(ROI_BUT, BUT_rect_o, BUT_boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
			BUT_center.x = 0.5*(BUT_boundRect[i].tl().x + BUT_boundRect[i].br().x);
			BUT_center.y = 0.5*(BUT_boundRect[i].tl().y + BUT_boundRect[i].br().y);
			//x可得到確切值，但y須加上ROI在frame上y的偏移量(2*ROI_height)
			cout << "BUT_center(x,y) = " << " ( " << BUT_center.x << " , " << 4 * ROI_height+BUT_center.y << " ) " << endl;
			circle(ROI_BUT, BUT_center, 3, Scalar(255, 0, 0), -1, 8);
			abs_but_center = Point(BUT_center.x, 4 * ROI_height + BUT_center.y);
		}
/*
			if (((int)(0.5*ROI_height + BUT_rect_o.y)) == abs_but_center.y) {
				cout << "BUT_center(x,y) = " << " ( " << BUT_center.x << " , " << 4 * ROI_height+BUT_center.y << " ) ";
			} //if_TOP
			else {
				cout << "BUT_center point is not at ROI_Rect_center!!!!" << endl;
			} //else

		} //for_BUT
*/

		//define_baseline
		int baseline_pos = width / 2;
		line(frame, Point(baseline_pos, 0), Point(baseline_pos, height), Scalar(0, 255, 255), 1, 8);

		//drawing line to connect center points
		line(frame, abs_top_center, abs_mid_center, Scalar(0, 0, 255), 2, 8);
		line(frame, abs_mid_center, abs_but_center, Scalar(0, 0, 255), 2, 8);


		namedWindow("frame", WINDOW_AUTOSIZE);
		imshow("test_ROI", midImage);
		imshow("frame", frame);

		

	}
}
		












//--------------------------------------test-end-------------------------------------

//		Mat ROI_MID = midImage(Rect(0, 2 * ROI_height, ROI_width, ROI_height));
//		Mat ROI_BUT = midImage(Rect(0, 4 * ROI_height, ROI_width, ROI_height));
		//------BGR2GRAY--GaussBlur--Threshold----------------------------------------------------------------
		//cvtColor(ROI_TOP, ROI_TOP, COLOR_BGR2GRAY);
		//cvtColor(ROI_MID, ROI_MID, COLOR_BGR2GRAY);
		//cvtColor(ROI_BUT, ROI_BUT, COLOR_BGR2GRAY);
		//GaussianBlur(ROI_TOP, ROI_TOP, Size(5, 5), 2, 2);
		//GaussianBlur(ROI_MID, ROI_MID, Size(5, 5), 2, 2);
		//GaussianBlur(ROI_BUT, ROI_BUT, Size(5, 5), 2, 2);
		//threshold(ROI_TOP, ROI_TOP, 150, 255, THRESH_BINARY);
		//threshold(ROI_MID, ROI_MID, 150, 255, THRESH_BINARY);
		//threshold(ROI_BUT, ROI_BUT, 150, 255, THRESH_BINARY);
		//------findContours--DrawRect--------------------------------------------------------	

/*
		Point TOP_center, MID_center, BUT_center;

		vector<vector<Point>> TOP_contours, MID_contours, BUT_contours;
		vector<Vec4i> TOP_hierarchy, MID_hierarchy, BUT_hierarchy;
		//Point2i rect_center[3];//[0]:TOP,[1]:MID,[2]:BUT

		findContours(ROI_TOP, TOP_contours, TOP_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		findContours(ROI_MID, MID_contours, MID_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		findContours(ROI_BUT, BUT_contours, BUT_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i <= TOP_contours.size(); i++) {
			Rect target_TOP_rect = boundingRect(TOP_contours[i]);
			rectangle(frame, target_TOP_rect, Scalar(255, 0, 0), 2);
			//Point2f vertex[4]; //[0]:(x,y),[1]
			//rect_center[0] = (Point2i)target_rect.;//TOP_center -> rect_center[0]
		//	cout << " TOP : " << " x = " << target_rect.x << " , " << " y = " << target_rect.y << endl;

//		Canny(ROI_MID, ROI_MID, 100, 200, 3);
//		findContours(ROI_MID, MID_contours, MID_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			Rect target_MID_rect = boundingRect(MID_contours[i]);
			rectangle(frame, target_MID_rect, Scalar(255, 0, 0), 2);
			//rect_center[1] = (Point2i)target_rect.center;//MID_center -> rect_center[1]
		//	cout << " MID : " << " x = " << target_rect.x << " , " << " y = " << target_rect.y << endl;


//		Canny(ROI_BUT, ROI_BUT, 100, 200, 3);
//		findContours(ROI_BUT, BUT_contours, BUT_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			Rect target_BUT_rect = boundingRect(BUT_contours[i]);
			rectangle(frame, target_BUT_rect, Scalar(255, 0, 0), 2);
			//rect_center[2] = (Point2i)target_rect.center;//BUT_center -> rect_center[2]
		//	cout << " BUT : " << " x = " << target_rect.x << " , " << " y = " << target_rect.y << endl;
		}//ROI

//------Drawing--Line------------------------------------------------------------------------
		rectangle(frame, Rect(0, 0, ROI_width, ROI_height), Scalar(0, 255, 0), 2);
		rectangle(frame, Rect(0, 2 * ROI_height, ROI_width, ROI_height), Scalar(0, 255, 0), 2);
		rectangle(frame, Rect(0, 4 * ROI_height, ROI_width, ROI_height), Scalar(0, 255, 0), 2);
		imshow("test", frame);

//		line(frame, rect_center[2], rect_center[1], Scalar(255, 0, 0), 3, CV_AA);
//		line(frame, rect_center[1], rect_center[0], Scalar(0, 255, 0), 3, CV_AA);

	}//while
}//main


*/