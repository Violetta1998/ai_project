#include <opencv2\opencv.hpp>
#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

void translateImg(Mat copyImg) {
	Size sizeImg(copyImg.cols, copyImg.rows);//width, height
	float M[2][3] = { { 1,0,rand()%100+40 },{ 0,1,rand() % 100 + 40 } };
	Mat imgAffine = copyImg.clone();
	Mat M2(2, 3, CV_32F, M);//convert mass to Mat
	warpAffine(copyImg, imgAffine, M2, sizeImg);
	imshow("affine", imgAffine);
}

void rotateImg(Mat copyImg) {
	Mat M = getRotationMatrix2D(Point2f(copyImg.cols / 2, copyImg.rows / 2), rand()%180+30, 1);
	Mat imgAffine = copyImg.clone();
	warpAffine(copyImg, imgAffine, M, Size(copyImg.cols, copyImg.rows));
	imshow("affine", imgAffine);
}

void affineTransormation(Mat copyImg, Point p1, Point p2, Point p3) {//надо?
	int random = rand() % 100 + 10;
	float pts1[3][2] = { {p1.x,p1.y},{p2.x,p2.y},{p3.x,p3.y} };
	float pts2[3][2] = { {p1.x+random,p1.y+random},{p2.x-random,p2.y-random},{p3.x+random,p3.y-random} };
	Mat M = getAffineTransform(Mat(3,2,CV_32F,pts1), Mat(3, 2, CV_32F, pts2));
	
	Mat imgAffine = copyImg.clone();
	warpAffine(copyImg, imgAffine, M, Size(copyImg.cols, copyImg.rows));
	imshow("affine", imgAffine);
}

void scaleImg(Mat copyImg) {
	Mat imgAffine = copyImg.clone();
	resize(copyImg, imgAffine, Size(copyImg.cols, copyImg.cols),2,2,INTER_CUBIC);
	imshow("affine", imgAffine);
}

int main(int argc, char* argv[]) {
	Mat img=imread("back.jpg",1);
	Mat copyImg = img.clone();
	Mat gray; cvtColor(copyImg, gray, CV_BGR2GRAY);
	threshold(gray, gray, 128, 0, THRESH_BINARY);
	Mat grayCopy = gray.clone();
	
	Scalar color(0, 0, 0);
	Scalar color2(255, 255, 255);
	double thickness = 1.5;

	namedWindow("image with lines", WINDOW_FREERATIO);
	namedWindow("mask with points", WINDOW_FREERATIO);
	namedWindow("affine", WINDOW_FREERATIO);

	string sNull("00000");
	for (int i = 0; i < 400;i++) {
		srand(time(NULL));
		int figure = rand() % 5 + 1;
		switch (1) {
		case 1: {//checkerboards
			int x1 = rand() % 20 + 1;
			int y1 = rand() % 20 + 1;
			int y2 = rand() % 40 + 25;
			int diaposonX = rand() % 20 + 5;
			int diaposonY = y2 - y1;
			int x2 = x1 + diaposonX;
			int countX(0);
			int countY(0);
			int repeat = rand() % 10 + 4;
			if (repeat % 2 != 0) { repeat -= 1;}
			
			//cv::RotatedRect rect(Point2f(x2-x1,y2-y1),Size2f(diaposonY,diaposonX),33);
			for (int i = 0; i < repeat; i++) {
				Point p1(x1 + countX*diaposonX, y1 + countY*diaposonY);
				Point p2(x2 + countX*diaposonX, y2 + countY*diaposonY);
				if (p1.x < 128 && p2.x < 128 && p1.y < 128 && p2.y < 128) {
					rectangle(copyImg, p1, p2, color, thickness);

					circle(gray, p1, 1, color2, -1);
					circle(gray, p2, 1, color2, -1);
					circle(gray, Point(x1 + countX*diaposonX, y2 + countY*diaposonY), 1, color2, -1);
					circle(gray, Point(x2 + countX*diaposonX, y1 + countY*diaposonY), 1, color2, -1);
					countX++;
				}
				
				if (i == repeat - 1 && countY<repeat) {
					i = 0;
					countX = 0;
					countY++;
				}
			}
			//affineTransormation(copyImg, Point((x1 + countX*diaposonX, y1 + countY*diaposonY)), Point((x2 + countX*diaposonX, y2 + countY*diaposonY)), Point(x1, y1));
			break;
		}
		case 2: {//не пересекающиеся линии
			int a = rand() % 11 + (-5);//наклон
			int x = 15;
			int d = rand() % 50 + 1;//пересечение с oY
			int diaposon = rand() % 30 + 15;
			int count = rand() % 8 + 3;
			//y1 = a*x+d
			for (int i = 0; i < count; i++) {
				Point p1(diaposon, a*x + d); Point p2(x + diaposon, (a*(x + diaposon) + d));
				if (p1.x < 128 && p1.y < 128 && p1.y>0 && p2.y>0 && p2.x < 128 && p2.y < 128) {
					line(copyImg, p1, p2, color, thickness);
					circle(gray, p1, 1, color2, -1);
					circle(gray, p2, 1, color2, -1);
				}
				
				diaposon = rand() % (x + diaposon + 10) + 15;
				a = rand() % 11 + (-5);
				d = rand() % 50 + 1;
			}
			break;
		}
		case 3: {//triangle/quads
			int x1 = rand() % 20 + 5;
			int x2 = rand() % 40 + 25;
			int x3 = rand() % 60 + 45;

			int y1 = rand() % 20 + 5;
			int y3 = rand() % 20 + 25;
			int y2 = rand() % 60 + 45;
			srand(time(NULL));
			int param = rand() % 2 + 1;
			switch (param) {
			case 1: {
				line(copyImg, Point(x1, y1), Point(x2, y2), color, thickness);//triangle
				line(copyImg, Point(x2, y2), Point(x3, y3), color, thickness);
				line(copyImg, Point(x3, y3), Point(x1, y1), color, thickness);

				circle(gray, Point(x1, y1), 1, color2, -1);
				circle(gray, Point(x2, y2), 1, color2, -1);
				circle(gray, Point(x3, y3), 1, color2, -1);
				break;
			}
			case 2: {
				line(copyImg, Point(x1, y1), Point(x2, y3), color, thickness);//quads
				line(copyImg, Point(x2, y3), Point(x3, y3), color, thickness);
				line(copyImg, Point(x3, y3), Point(x3, y1), color, thickness);
				line(copyImg, Point(x3, y1), Point(x1, y1), color, thickness);

				circle(gray, Point(x1, y1), 1, color2, -1);
				circle(gray, Point(x2, y3), 1, color2, -1);
				circle(gray, Point(x3, y3), 1, color2, -1);
				circle(gray, Point(x3, y1), 1, color2, -1);
				break;
			}

			}
			break;
		}
		case 4: {//stars
			int diaposon = rand() % 20 + (-10);
			int x = rand() % 15 + 5;
			int a = rand() % 5 + 1;
			int d = rand() % 20 + 10;
			int a1(a);
			int d1(d);
			srand(time(NULL));
			int count = rand() % 10 + 3;
			for (int i = 0; i < count; i++) {
				Point p1(x, a1*x + d1);
				Point p2(x + diaposon, (a*(x + diaposon) + d));
				if (p1.x < 128 && p1.y < 128 && p1.y>0 && p2.y>0 && p2.x < 128 && p2.y < 128) {
					line(copyImg, p1, p2, color, thickness);
					circle(gray, p1, 1, color2, -1);
					circle(gray, p2, 1, color2, -1);

					diaposon = rand() % 20 + (-10);
					a = rand() % 10 + 1;
					d = rand() % 20 + 10;
				}
			}
			break; 
		}
		case 5: {//cube
			int x1 = rand() % 10 + 3;
			int x2 = rand() % 30 + 15;
			int x3 = rand() % 50 + 35;
			int diaposonX = x3 - (x2 - x1);

			int y1 = rand() % 10 + 3;
			int y2 = rand() % 30 + 15;
			int y3 = rand() % 50 + 35;
			int diaposonY = y2 - y1;

			rectangle(copyImg, Point(x1, y1), Point(x2, y2), color, thickness);
			line(copyImg, Point(x1, y2), Point(diaposonX, y2 + diaposonY), color, thickness);
			line(copyImg, Point(x2, y2), Point(x3, y2 + diaposonY), color, thickness);
			line(copyImg, Point(diaposonX, y2 + diaposonY), Point(x3, y2 + diaposonY), color, thickness);
			line(copyImg, Point(x3, y2 + diaposonY), Point(x3, y2), color, thickness);
			line(copyImg, Point(x2, y1), Point(x3, y2), color, thickness);

			circle(gray, Point(x1, y1), 1, color2, -1);
			circle(gray, Point(x1, y2), 1, color2, -1);
			circle(gray, Point(x2, y2), 1, color2, -1);
			circle(gray, Point(x3, y2), 1, color2, -1);
			circle(gray, Point(x2, y1), 1, color2, -1);
			circle(gray, Point(diaposonX, y2 + diaposonY), 1, color2, -1);
			circle(gray, Point(x3, y2 + diaposonY), 1, color2, -1);
			circle(gray, Point(diaposonX, y2 + diaposonY), 1, color2, -1);
			break;
		}
		default:
			break;
		}
		if (i > 9) {
			sNull="0000";
		}
		if (i > 99) {
			sNull = "000";
		}
		imshow("image with lines", copyImg);
		imshow("mask with points", gray);
		
		/*const string nameOrig("C:/images/original/" + sNull+std::to_string(i) + ".png");
		const string nameMask("C:/images/mask/" + sNull+std::to_string(i) + ".png");
		imwrite(nameOrig, copyImg);
		imwrite(nameMask, gray);*/
		copyImg = img.clone();
		gray = grayCopy.clone();
		char c = (char)waitKey(30000);
		if (waitKey(30)==27) {
			break;
		}

	}

		waitKey(0);
		return 0;
}