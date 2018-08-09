/*

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

		for (int i = 0; i < TOP_contours.size(); i++) {
			TOP_boundRect[i] = boundingRect((Mat)TOP_contours[i]);
			//將trans_TOP所得(X,Y)數據丟入ROI_TOP，並使其映射回frame;重複MID,BUT; tl()::point(x,y),br()::point(x+width,y+height)
			rectangle(ROI_TOP, TOP_boundRect[i].tl(), TOP_boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
			//Rect_center & draw_center_point(x,y)
			TOP_center.x = 0.5*(TOP_boundRect[i].tl().x + TOP_boundRect[i].br().x);
			Point2i center(0,0);
			center.x = 0.5*(center.x + TOP_center.x);	//總矩形中心
			TOP_center.y = 0.5*(TOP_boundRect[i].tl().y + TOP_boundRect[i].br().y);
			cout << "TOP_center(x,y) = " << " ( " << TOP_center.x << " , " << TOP_center.y << " ) ";
			
			abs_top_center = Point(TOP_center.x, TOP_center.y);//absolute_top_center_point in frame
			circle(frame, abs_top_center, 3, Scalar(255, 0, 0), -1, 8);
		}	//for_TOP
/*
			if ((TOP_center.x == 0) && (TOP_center.y == 0)) {
				cout << endl << "not found TOP_center" << endl;
			}
			else {
				//absolute_top_center_point in frame
				if (TOP_boundRect.size() == 1) {
					abs_top_center = Point(TOP_center.x, TOP_center.y);//absolute_top_center_point in frame
				}
				else {
					//計算多區塊合併之正中心值
					for (int j = 0; j <= TOP_boundRect.size(); j++) {
						TOP_center.x += 0.5*(TOP_boundRect[j].tl().x + TOP_boundRect[j].br().x);
						TOP_center.x /= (j + 1);
						abs_top_center = Point(TOP_center.x, TOP_center.y);
					}
				}
			}
			cout << "TOP_center(x,y) = " << " ( " << abs_top_center.x << " , " << abs_top_center.y << " ) ; ";
*/
/*
		for (int i = 0; i < MID_contours.size(); i++) {
			MID_boundRect[i] = boundingRect((Mat)MID_contours[i]);
			rectangle(ROI_MID, MID_boundRect[i].tl(), MID_boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
			MID_center.x = 0.5*(MID_boundRect[i].tl().x + MID_boundRect[i].br().x);
			Point2i center(0, 0);
			center.x += MID_center.x;	//總矩形中心
			if ((i + 1) == MID_contours.size()) {
				MID_center.x = center.x / MID_contours.size();
			}
			MID_center.y = 0.5*(MID_boundRect[i].tl().y + MID_boundRect[i].br().y);
			cout << "MID_center(x,y) = " << " ( " << MID_center.x << " , " << 2 * ROI_height + MID_center.y << " )  ; ";
			circle(ROI_MID, MID_center, 3, Scalar(255, 0, 0), -1, 8);
			abs_mid_center = Point(MID_center.x, 2 * ROI_height + MID_center.y);
		} //for_MID
*/
/*
		if ((MID_center.x == 0) && (MID_center.y == 0)) {
			cout << endl << "not found MID_center" << endl;
		}
		else {
			//absolute_mid_center_point in frame
			if (MID_boundRect.size() == 1) {
				abs_mid_center = Point(MID_center.x, 2 * ROI_height + MID_center.y);//absolute_top_center_point in frame
			}
			else {
				//計算多區塊合併之正中心值
				for (int MID_J = 0; MID_J <= MID_boundRect.size(); MID_J++) {
					MID_center.x += 0.5*(MID_boundRect[MID_J].tl().x + MID_boundRect[MID_J].br().x);
					MID_center.x /= (MID_J + 1);
					abs_mid_center = Point(MID_center.x, 2 * ROI_height + MID_center.y);
				}
			}
		}
		cout << "MID_center(x,y) = " << " ( " << abs_mid_center.x << " , " << abs_mid_center.y << " ) ; ";
*/
/*
		for (int i = 0; i < BUT_contours.size(); i++) {
			BUT_boundRect[i] = boundingRect((Mat)BUT_contours[i]);
			rectangle(ROI_BUT, BUT_boundRect[i].tl(), BUT_boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
			BUT_center.x = 0.5*(BUT_boundRect[i].tl().x + BUT_boundRect[i].br().x);
			Point2i center(0, 0);
			center.x += BUT_center.x;	//總矩形中心
			if ((i + 1) == BUT_contours.size()) {
				BUT_center.x = center.x / BUT_contours.size();
			}
			BUT_center.y = 0.5*(BUT_boundRect[i].tl().y + BUT_boundRect[i].br().y);
			//x可得到確切值，但y須加上ROI在frame上y的偏移量(2*ROI_height)
			//cout << "BUT_center(x,y) = " << " ( " << BUT_center.x << " , " << 4 * ROI_height + BUT_center.y << " ) " << endl;
			circle(ROI_BUT, BUT_center, 3, Scalar(255, 0, 0), -1, 8);
			abs_but_center = Point(BUT_center.x, 4 * ROI_height + BUT_center.y);
		} //for_BUT
*/
/*
		if ((BUT_center.x == 0) && (BUT_center.y == 0)) {
			cout << endl << "not found BUT_center" << endl;
		}
		else {
			//absolute_but_center_point in frame
			if (BUT_boundRect.size() == 1) {
				abs_but_center = Point(BUT_center.x, 4 * ROI_height + BUT_center.y);//absolute_top_center_point in frame
			}
			else {
				//計算多區塊合併之正中心值
				for (int BUT_J = 0; BUT_J <= BUT_boundRect.size(); BUT_J++) {
					BUT_center.x += 0.5*(BUT_boundRect[BUT_J].tl().x + BUT_boundRect[BUT_J].br().x);
					BUT_center.x /= (BUT_J + 1);
					abs_but_center = Point(BUT_center.x, 4 * ROI_height + BUT_center.y);
				}
			}
		}
		cout << "BUT_center(x,y) = " << " ( " << abs_but_center.x << " , " << abs_but_center.y << " ) ; " << endl;
*/
/*
		//drawing line to connect center points
		if ((TOP_contours.size() > 0) || (MID_contours.size() > 0)) {
			line(frame, abs_top_center, abs_mid_center, Scalar(0, 0, 255), 2, 8);
		}
		if ((MID_contours.size() > 0) || (BUT_contours.size() > 0)) {
			line(frame, abs_mid_center, abs_but_center, Scalar(0, 0, 255), 2, 8);
		}
		imshow("test_ROI", midImage);
		imshow("frame", frame);

	}
}

*/