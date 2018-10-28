#include <iostream>
#include <fstream>
#include "opencv2\opencv.hpp"
#include <cmath>
#include <ctime>

using namespace cv;
using namespace std;

int consize = 1000;

string color_name[7] = { "RED", "ORGANE", "YELLOW", "GREEN", "SKY", "BLUE", "PURPLE" };

#define PLAY  0
#define GEN   1

#define VIDEOFILE "sample_video/bird_10.mp4"
double Distance(const Point& p1, const Point& p2) {

	double distance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

	return distance;
}


class problem
{
public:
	double len[21];
	int num;
	int flip;
	int flag=0;
	double angle[7];
	double at = 20; // error margin for angle
	string name;
	int matching(double length[],double angle[], double tmp);
};

int problem::matching(double length[], double ang[], double tmp)
{
	if (ang[0] > 350)
		ang[0] = 0;
	if (ang[1] > 350)
		ang[1] = 0;
	if (ang[2] > 350)
		ang[2] = 0;
	if (ang[3] > 350)
		ang[3] = 0;
	if (ang[4] > 170)
		ang[4] = 0;
	if (ang[5] > 350)
		ang[5] = 0;
	if (ang[6] > 80)
		ang[6] = 0;
	if ((length[0] < len[0] + tmp) && (length[0] > len[0] - tmp) &&
		(length[1] < len[1] + tmp) && (length[1] > len[1] - tmp) &&
		(length[2] < len[2] + tmp) && (length[2] > len[2] - tmp) &&
		(length[3] < len[3] + tmp) && (length[3] > len[3] - tmp) &&
		(length[4] < len[4] + tmp) && (length[4] > len[4] - tmp) &&
		(length[5] < len[5] + tmp) && (length[5] > len[5] - tmp) &&
		(length[6] < len[6] + tmp) && (length[6] > len[6] - tmp) &&
		(length[7] < len[7] + tmp) && (length[7] > len[7] - tmp) &&
		(length[8] < len[8] + tmp) && (length[8] > len[8] - tmp) &&
		(length[9] < len[9] + tmp) && (length[9] > len[9] - tmp) &&
		(length[10] < len[10] + tmp) && (length[10] > len[10] - tmp) &&
		(length[11] < len[11] + 0.2) && (length[11] > len[11] - tmp) &&
		(length[12] < len[12] + tmp) && (length[12] > len[12] - tmp) &&
		(length[13] < len[13] + tmp) && (length[13] > len[13] - tmp) &&
		(length[14] < len[14] + tmp) && (length[14] > len[14] - tmp) &&
		(length[15] < len[15] + tmp) && (length[15] > len[15] - tmp) &&
		(length[16] < len[16] + tmp) && (length[16] > len[16] - tmp) &&
		(length[17] < len[17] + tmp) && (length[17] > len[17] - tmp) &&
		(length[18] < len[18] + tmp) && (length[18] > len[18] - tmp) &&
		(length[19] < len[19] + tmp) && (length[19] > len[19] - tmp) &&
		(length[20] < len[20] + tmp) && (length[20] > len[20] - tmp) &&
		(ang[0]<angle[0] + at) && (ang[0]>angle[0] - at) &&
		(ang[1]<angle[1] + at) && (ang[1]>angle[1] - at) &&
		(ang[2]<angle[2] + at) && (ang[2]>angle[2] - at) &&
		(ang[3]<angle[3] + at) && (ang[3]>angle[3] - at) &&
		(ang[4]<angle[4] + at) && (ang[4]>angle[4] - at) &&
		(ang[5]<angle[5] + at) && (ang[5]>angle[5] - at) &&
		(ang[6]<angle[6] + at) && (ang[6]>angle[6] - at))
	{
		flag = 1;
		return 1;
	}
	else {
		return 0;
	}
}

class shape 
{
public:
	Point wp;
	Point pnt[4];
	int plag=0;
	double angle;
	bool flip = true;
	void getPoints(Point p1, Point p2, Point p3, Point p4=Point(0,0), int n=0);
	void init();
};


void shape::getPoints(Point p1, Point p2, Point p3, Point p4,int n)
{
	double linescale[4];
	double linescalemax = 0;
	int linescaleidx = 0;
	if (n > 0)//하늘색
	{
		wp.x = (p1.x + p2.x + p3.x + p4.x) / 4;
		wp.y = (p1.y + p2.y + p3.y + p4.y) / 4;
		pnt[0] = p1;
		pnt[1] = p2;
		pnt[2] = p3;
		pnt[3] = p4;
		linescale[0] = Distance(pnt[0], pnt[2]);
		linescale[1] = Distance(pnt[1], pnt[3]);

		if (linescale[0] > linescale[1])
		{
			linescalemax = linescale[0];
			linescaleidx = 0;
		}
		else
		{
			linescalemax = linescale[1];
			linescaleidx = 1;
		}

		linescale[2] = Distance(pnt[linescaleidx], pnt[linescaleidx + 1]);
		linescale[3] = Distance(pnt[linescaleidx], pnt[(linescaleidx + 3) % 4]);


		if (linescale[2] > linescale[3])
			flip = false;

		if (flip)
		{
			linescale[0] = Distance(pnt[0], pnt[1]);
			linescale[1] = Distance(pnt[1], pnt[2]);
			if (linescale[0] > linescale[1])
			{
				angle =180- acos(abs(pnt[0].x - pnt[(1) % 3].x) / linescale[0]) * 180 / 3.14;
				
			}
			else
			{
				angle = acos(abs(pnt[2].x - pnt[(1) % 3].x) / linescale[1]) * 180 / 3.14;
				if (pnt[0].x > pnt[3].x)
					angle = 180 - angle;

			}
		}
		else 
		{
			linescale[0] = Distance(pnt[0], pnt[1]);
			linescale[1] = Distance(pnt[1], pnt[2]);

			if (linescale[0] > linescale[1])
			{
				angle = acos(abs(pnt[2].x - pnt[(1) % 3].x) / linescale[1]) * 180 / 3.14;
				if (pnt[0].x > pnt[3].x)
					angle = 180 - angle;

			}
			else
			{
				
				angle =180- acos(abs(pnt[0].x - pnt[(1) % 3].x) / linescale[0]) * 180 / 3.14;

			}
			angle = (angle + 50) ;
			if (angle > 180)
				angle = angle - 180;
		}
	}
	else if(p4 == Point(0, 0))//세모
	{
		wp.x = (p1.x + p2.x + p3.x) / 3;
		wp.y = (p1.y + p2.y + p3.y) / 3;
		pnt[0] = p1;
		pnt[1] = p2;
		pnt[2] = p3;

		linescale[0] = Distance(pnt[0], pnt[1]);
		linescale[1] = Distance(pnt[1], pnt[2]);
		linescale[2] = Distance(pnt[2], pnt[0]);

		for (int i = 0; i < 3; i++)
		{
			if (linescalemax < linescale[i])
			{
				linescalemax = linescale[i];
				linescaleidx = i;
			}
		}

		angle = acos(abs(pnt[linescaleidx].x - pnt[(linescaleidx + 1) % 3].x) / linescale[linescaleidx]) * 180 / 3.14;
		if ((pnt[linescaleidx].y + pnt[(linescaleidx + 1) % 3].y) / 2 < pnt[(linescaleidx + 2) % 3].y)
			angle = 180 - angle;
		if ((pnt[linescaleidx].x + pnt[(linescaleidx + 1) % 3].x) / 2 > pnt[(linescaleidx + 2) % 3].x)
			angle = 360 - angle;
	}
	else//보라색
	{
		wp.x = (p1.x + p2.x + p3.x + p4.x) / 4;
		wp.y = (p1.y + p2.y + p3.y + p4.y) / 4;
		pnt[0] = p1;
		pnt[1] = p2;
		pnt[2] = p3;
		pnt[3] = p4;
		linescale[0] = Distance(pnt[3], pnt[2]);
		

		angle = acos(abs(pnt[2].x - pnt[3].x) / linescale[0]) * 180 / 3.14;
		angle = 90 - angle;

	}
	plag = 1;
}

void shape::init()
{
	wp = Point(0, 0);
	plag = 0;
	pnt[0] = Point(0, 0);
	pnt[1] = Point(0, 0);
	pnt[2] = Point(0, 0);
	pnt[3] = Point(0, 0);
	flip = true;
}

int get_problem_from_file(problem p[])
{
	char inputString[1000];
	ifstream inFile("output.txt");
	string names = "sdfd ";

	int p_num = 0;
	int i = 0;

	while (!inFile.eof()) {
		inFile.getline(inputString, 100);
		if (i < 21)
			p[p_num].len[i] = atof(inputString);
		else if (i < 28)
			p[p_num].angle[i - 21] = atof(inputString);
		else if (atoi(inputString) != 9999)
			p[p_num].name = inputString;

		i++;
		if (atoi(inputString) == 9999)
		{
			i = 0;
			p_num++;
		}
	}

	inFile.close();
	return p_num;
}

int menu_display(void)
{
	int select;
	system("cls");
	printf("칠교놀이\n\n");
	printf("1. 문제 맞추기\n");
	printf("2. 문제 만들기\n");
	printf("3. 캘리브레이션\n");
	printf("4. 프로그램 종료\n\n");
	printf("메뉴번호 입력후 Enter>");
	scanf("%d", &select);
	return select;
}

// Global Variable for calibration
Point boundary[4];

int red_eps = 30;
int orange_eps = 53;
int yellow_eps = 64;
int green_eps = 52;
int sky_eps = 34;
int blue_eps = 25;
int purple_eps = 30;



void find_shapes_from_contours(Mat col[7], shape shapes[7], Mat &img, Mat &img2, int mode)
{

	int shape_eps[7] = { red_eps, orange_eps, yellow_eps, green_eps, sky_eps, blue_eps, purple_eps };

	vector<vector<Point>>cont[8]; //cont[0]-red, cont[1]-orange, cont[2]-yellow, cont[3]-green, cont[4]-sky, cont[5]-blue, cont[6]-purple
	vector<Vec4i>hierarchy;

	// Find contour 
	for (int i = 0; i < 7; i++){
		//Canny(col[i], col[i], 100, 255);
		findContours(col[i], cont[i], hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
	}

	for (int i = 0; i < 7; i++)
		shapes[i].init();

	for (int shape_id = 0; shape_id < 7; shape_id++)
	{
		int max = 0, maxidx = 0;

		if (cont[shape_id].size() > 0)
		{
			for (int i = 0; i < cont[shape_id].size(); i++)
			{
				if (contourArea((cont[shape_id])[i]) > max)
				{
					max = contourArea((cont[shape_id])[i]);
					maxidx = i;
				}
			}

			if (contourArea((cont[shape_id])[maxidx]) >= consize)
			{
				vector<Point> app;

				shapes[shape_id].plag = 1;
				//approxPolyDP((cont[shape_id])[maxidx], app, shape_eps[shape_id], false);
				approxPolyDP((cont[shape_id])[maxidx], app, 30, false);
				if (app.size() == 4 && (shape_id < 4 || shape_id == 5))
				{
					drawContours(img2, vector<vector<Point>>(1, app), 0, Scalar(100, 0, 125), 3);
					shapes[shape_id].getPoints(app[0], app[1], app[2]);
					if (mode == GEN) {
						putText(img, color_name[shape_id], shapes[shape_id].wp, 1, 1, Scalar(0, 0, 0));
						char msg[256];
						sprintf(msg, "%.0lf", shapes[shape_id].angle);
						putText(img, msg, shapes[shape_id].wp + Point(0, 15), 1, 1, Scalar(0, 0, 0));
					}
				}
				else if (app.size() == 5 && (shape_id == 4 || shape_id == 6))
				{
						shapes[shape_id].getPoints(app[0], app[1], app[2], app[3], 1);
						drawContours(img2, vector<vector<Point>>(1, app), 0, Scalar(255, 216, 0), 4);
						if (mode == GEN) {
							putText(img, color_name[shape_id], shapes[shape_id].wp, 1, 1, Scalar(0, 0, 0));
							if (shape_id == 4 && shapes[4].flip)
								putText(img, "Flip", shapes[4].wp - Point(0, 15), 1, 1, Scalar(0, 0, 255));
							char msg[256];
							sprintf(msg, "%.0lf", shapes[shape_id].angle);
							putText(img, msg, shapes[shape_id].wp + Point(0, 15), 1, 1, Scalar(0, 0, 0));
						}
				}
			}
		}
	}


	return;
}


void get_shape_features(shape shapes[], double length[], double shapeangle[])
{
	double lines[3];
	lines[0] = Distance(shapes[0].pnt[0], shapes[0].pnt[1]);
	lines[1] = Distance(shapes[0].pnt[1], shapes[0].pnt[2]);
	lines[2] = Distance(shapes[0].pnt[2], shapes[0].pnt[0]);

	double maxl = 0;
	int maxli = 0;
	for (int i = 0; i < 3; i++)
	{
		if (maxl < lines[i])
		{
			maxl = lines[i];
			maxli = i;
		}
	}

	int cl = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = i; j < 6; j++)
		{
			length[cl] = Distance(shapes[i].wp, shapes[j + 1].wp);

			length[cl] = length[cl] / lines[maxli];
			cout << cl << ": " << length[cl] << endl;
			cl++;
		}
	}

	for (int i = 0; i < 7; i++)
		shapeangle[i] = shapes[i].angle;

	return;
}

Mat get_transform_matrix(Point boundary[4], Size &warpSize)
{
	vector<Point2f> corners(4);

	corners[0] = boundary[1];//좌하
	corners[1] = boundary[2];//우하
	corners[2] = boundary[0];//우상
	corners[3] = boundary[3];//좌상
	
	vector<Point2f> warpCorners(4);
	warpCorners[1] = Point2f(0, 0);
	warpCorners[0] = Point2f(warpSize.width, 0);
	warpCorners[3] = Point2f(0, warpSize.height);
	warpCorners[2] = Point2f(warpSize.width, warpSize.height);

	Mat trans = getPerspectiveTransform(corners, warpCorners);
	return trans;
}

void split_puzzle_by_color(Mat & hsvframe, Mat col[])
{
	inRange(hsvframe, Scalar(164, 100, 100), Scalar(180, 255, 255), col[0]); //red
	inRange(hsvframe, Scalar(12, 100, 100), Scalar(20, 255, 255), col[1]); // orange
	inRange(hsvframe, Scalar(21, 70, 100), Scalar(34, 255, 255), col[2]); // yellow
	inRange(hsvframe, Scalar(40, 70, 100), Scalar(70, 255, 255), col[3]); // green
	inRange(hsvframe, Scalar(85, 70, 100), Scalar(104, 128, 255), col[4]); // sky blue
	inRange(hsvframe, Scalar(105, 120, 100), Scalar(120, 255, 255), col[5]); // blue
	inRange(hsvframe, Scalar(120, 60, 100), Scalar(162, 110, 255), col[6]); // purple
	inRange(hsvframe, Scalar(0, 50, 100), Scalar(255, 255, 255), col[7]);
}

void play_game(problem problems[], int p_size, Mat &transMat)
{
	VideoCapture capture(VIDEOFILE);

	int mode = 0;
	int random = 1;
	int custom = 0;
	srand((unsigned int)time(NULL));
	random = rand() % p_size + 1;

	cout << "Select game mode (easy = 1, hard = 2, custom = 3): ";
	scanf("%d", &mode);
	if (mode == 2)
		random += 100;
	if (mode == 3)
		custom = 1;

	string modes = to_string(random);
	modes += ".PNG";

	Mat hsv, img, frame, range, sum, img2;
	vector<Mat>channels;
	vector<Mat>bgr;

	Mat col[8];   //col[0]-red, col[1]-orange, col[2]-yellow, col[3]-green, col[4]-sky, col[5]-blue, col[6]-purple
	Mat col2[8];  //col2[0]-red, col2[1]-orange, col2[2]-yellow, col2[3]-green, col2[4]-sky, col2[5]-blue, col2[6]-purple
	vector<vector<Point>>cont[8]; //cont[0]-red, cont[1]-orange, cont[2]-yellow, cont[3]-green, cont[4]-sky, cont[5]-blue, cont[6]-purple

	Size size(5, 5);
	Mat Kernel = getStructuringElement(MORPH_ELLIPSE, size);

	int count = 0;
	shape shapes[8];

	while (1)
	{

		Mat problem = imread(modes);

		capture >> img;

		//Warping
		//warpPerspective(img, img, transMat, warpSize);

		img.copyTo(img2);

		capture >> hsv;
		cvtColor(hsv, hsv, COLOR_BGR2HSV);
		cvtColor(img, frame, COLOR_BGR2HSV); //HSV로 영상 이진화

		split(hsv, channels);
		split(img, bgr);

		//GaussianBlur(frame, frame, Size(3, 3), 1);

		split_puzzle_by_color(frame, col);

		sum = col[0] + col[1] + col[2] + col[3] + col[4] + col[5] + col[6]; //???

		//침식, 팽창
		for (int i = 0; i < 8; i++) {
			erode(col[i], col[i], Kernel, Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
			dilate(col[i], col[i], Kernel, Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
			col[i].copyTo(col2[i]);
		}
	
		find_shapes_from_contours(col, shapes, img, img2, PLAY);

		double length[21];
		double shapeangle[7];
		get_shape_features(shapes, length, shapeangle);

		double tmp = 0.15;
		for (int i = 0; i < p_size ; i++)
			problems[i].matching(length, shapeangle, tmp);

		if ((shapes[0].plag && shapes[1].plag && shapes[2].plag && shapes[3].plag && shapes[4].plag && shapes[5].plag && shapes[6].plag))
		{
		}
		else
		{
			for (int i = 0; i<p_size; i++)
				problems[i].flag = 0;

			//cout << "Asdasd" << endl;
		}

		for (int i = 0; i < p_size; i++)
		{
			if (problems[i].flag)
				putText(img, problems[i].name, Point(50, 50), 1, 3, Scalar(0, 0, 0), 3);
		}

		if (!custom)
			imshow("Problem", problem);

		imshow("debug", img2);
		imshow("Original", img); //show the original image

		merge(channels, hsv);
		merge(bgr, img);

		count++;
		char key = waitKey(1);
		if (key == 'q')
			break;
		else if (key == 'e' && random>100)
		{
			random -= 100;
			modes = to_string(random);
			modes += ".PNG";
		}
		else if (key == 'h' && random<100)
		{
			random += 100;
			modes = to_string(random);
			modes += ".PNG";
		}
		else if (key == 'r')
		{
			random = rand() % p_size + 1;
			random += 100;
			modes = to_string(random);
			modes += ".PNG";
		}
	}
}

int  make_game_DB(problem problems[], int p_size, Mat &transMat)
{
	    Mat img, img2, hsv;
	    VideoCapture capture(VIDEOFILE);
	    cout << "문제의 이름을 입력하세요" << endl;
	
	    float length_accum[21] = { 0 };
	    float angle_accum[7] = { 0 };
	    string inputName;
	    cin >> inputName;
		
		for (int i = 0; i < 21; i++)
			length_accum[i] = 0;
		for (int i = 0; i < 7; i++)
			angle_accum[i] = 0;
		int count = 0;

		while (1)
		{
			capture >> img;
			vector<Point2f> corners(4);

			//Warping
			//warpPerspective(img, img, transMat, warpSize);
			img.copyTo(img2);
		
			//cvtColor(hsv, hsv, COLOR_BGR2HSV);
			cvtColor(img, hsv, COLOR_BGR2HSV); //HSV로 영상 이진화

			imshow("img", img);
			//waitKey(0);

			Mat col[8], col2[8];

			GaussianBlur(hsv, hsv, Size(3, 3), 1);

			split_puzzle_by_color(hsv, col);

			Mat Kernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));

			//침식, 팽창
			for (int i = 0; i < 8; i++) {  
				erode(col[i], col[i], Kernel, Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
				dilate(col[i], col[i], Kernel, Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
				dilate(col[i], col[i], Kernel, Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
				col[i].copyTo(col2[i]);
			}

			//컨투어 찾기
			shape shapes[7];
			find_shapes_from_contours(col, shapes, img, img2, GEN);
			
			for (int i = 0; i < 7; i++) {
				//Point2f sz(col2[i].cols/2.0, col2[i].rows/2.0);
				resize(col2[i], col2[i], Size(0,0), 0.25, 0.25);
				imshow(color_name[i], col2[i]);
			}
			
		
			double length[21];
			double shapeangle[7];

			get_shape_features(shapes, length, shapeangle);

			//double tmp = 0.1;
			//
			//for (int i = 0; i < p_size; i++)
			//	problems[i].matching(length, shapeangle, tmp);

			char key = waitKey(30);
	
			if (key == 'x')
				break;
			else if (key == 's' || key == ' ')
			{
				if (length[0] > 0.1&&
					length[1] > 0.1&&
					length[2] > 0.1&&
					length[3] > 0.1&&
					length[4] > 0.1&&
					length[5] > 0.1&&
					length[6] > 0.1&&
					length[7] > 0.1&&
					length[8] > 0.1&&
					length[9] > 0.1&&
					length[10] > 0.1&&
					length[11] > 0.1&&
					length[12] > 0.1&&
					length[13] > 0.1&&
					length[14] > 0.1&&
					length[15] > 0.1&&
					length[16] > 0.1&&
					length[17] > 0.1&&
					length[18] > 0.1&&
					length[19] > 0.1&&
					length[20] > 0.1&&
					shapeangle[0] >= 0 &&
					shapeangle[1] >= 0 &&
					shapeangle[2] >= 0 &&
					shapeangle[3] >= 0 &&
					shapeangle[4] >= 0 &&
					shapeangle[5] >= 0 &&
					shapeangle[6] >= 0)
				{
					count++;
					for (int i = 0; i < 21; i++) {
						length_accum[i] += length[i];
					}
					for (int i = 0; i < 7; i++) {
						angle_accum[i] += shapeangle[i];
					}

					putText(img2, "Capturing features", Point(20, 50), 1, 3, Scalar(0, 0, 0), 3);

					if (key == 's')
					{
						ofstream outFile("problems.txt", ios::app);

						outFile << endl;
						outFile.setf(ios::fixed);
						outFile.precision(2);
						for (int i = 0; i < 21; i++)
						{
							outFile << length_accum[i] / count << endl;
						}
						outFile.precision(0);
						for (int i = 0; i < 7; i++) {
							outFile << angle_accum[i] / count << endl;
						}
						outFile << inputName << endl;
						outFile << 9999 ;
						outFile.close();
						break;
					}
					for (int i = 0; i < 7; i++)
						col2[i].release();
				}
			}

			imshow("contours", img2);
			imshow("Original", img); //show the original image

			for (int i = 0; i < 7; i++)
				col2[i].release();

		}
		destroyAllWindows();

		return 0;
}

Mat get_calibration_points(Point boundary[])
{

	vector<vector<Point>>cont;
	vector<Vec4i>hierarchy;

	VideoCapture capture(0);
	Mat img, img2, frame, marker;
	
	while (1)
	{
		vector<Point> app;

		capture >> img;
	    img.copyTo(img2);
		cvtColor(img, frame, COLOR_BGR2HSV); //HSV로 영상 이진화

		vector<Mat> channels;
		split(img, channels);

		//GaussianBlur(frame, frame, Size(3, 3), 1);

		inRange(frame, Scalar(87, 0, 105), Scalar(179, 96, 255), marker);

		//col[0].copyTo(col2[0]);

		//컨투어 찾기

		findContours(marker, cont, hierarchy, CV_RETR_TREE, CHAIN_APPROX_TC89_L1, Point(0, 0));


		int max = 0, maxidx = 0;


		if (cont.size() > 0)
		{
			for (int i = 0; i < cont.size(); i++)
			{
				if (contourArea((cont)[i]) > max)
				{
					max = contourArea((cont)[i]);
					maxidx = i;
				}
			}

			if (contourArea((cont)[maxidx]) > consize)
			{
				approxPolyDP((cont)[maxidx], app, 5, false);
				drawContours(img2, cont, maxidx, 0);
				drawContours(img2, vector<vector<Point>>(1, app), 0, Scalar(0, 0, 255), 4);
				if (app.size() == 5)
				{
					drawContours(img, vector<vector<Point>>(1, app), 0, Scalar(0, 0, 255), 4);
					boundary[0] = app[0];
					boundary[1] = app[1];
					boundary[2] = app[2];
					boundary[3] = app[3];
					for (int i = 0; i < 4; i++)
						circle(img, app[i], 3, Scalar(0, 255, 0), 3);
				}
			}
		}

		imshow("img2", img2);
		imshow("marker", marker);
		imshow("Original", img); //show the original image


		//count++;
		waitKey(1);
		if (app.size() == 5)
			break;
	}

	Size warpSize(800, 800);
	Mat transMat = get_transform_matrix(boundary, warpSize);

	return transMat;

}


Mat init_transMat()
{
	//vector<Point> inputQuad(4), warpCorners(4);

	// Input Quadilateral or Image plane coordinates
	Point2f inputQuad[4];
	// Output Quadilateral or World plane coordinates
	Point2f outputQuad[4];

	//corners[1] = Point(0, 0); // left top
	//corners[0] = Point(799, 0); // left  bottom
	//corners[3] = Point(0, 799); // right top
	//corners[2] = Point(799, 799); // right bottom

	//warpCorners[1] = Point2f(0, 0);
	//warpCorners[0] = Point2f(800, 0);
	//warpCorners[3] = Point2f(0, 800);
	//warpCorners[2] = Point2f(800, 800);

	inputQuad[0] = Point2f(0, 0);
	inputQuad[1] = Point2f(800, 0);
	inputQuad[2] = Point2f(800, 800);
	inputQuad[3] = Point2f(0, 800);
	// The 4 points where the mapping is to be done , from top-left in clockwise order
	outputQuad[0] = Point2f(0, 0);
	outputQuad[1] = Point2f(800, 0);
	outputQuad[2] = Point2f(800, 800);
	outputQuad[3] = Point2f(0, 800);

	Mat lamda = getPerspectiveTransform(inputQuad, outputQuad);

								   // Get the transform Matrix
	cout << lamda;
	return lamda;
}


int main()
{
	Mat transMat = init_transMat();

	namedWindow("Control", CV_WINDOW_KEEPRATIO);

	cvCreateTrackbar("red", "Control", &red_eps, 100);
	cvCreateTrackbar("orange", "Control", &orange_eps, 100);
	cvCreateTrackbar("yellow", "Control", &yellow_eps, 100);
	cvCreateTrackbar("green", "Control", &green_eps, 100);
	cvCreateTrackbar("sky", "Control", &sky_eps, 100); 
	cvCreateTrackbar("blue", "Control", &blue_eps, 100);
	cvCreateTrackbar("purple", "Control", &purple_eps, 100);

	//문제 입력
	problem p[100];
	int p_size = get_problem_from_file(p);

	int c;
	while ((c = menu_display()) != 4)
	{
		switch (c)
		{
		case 1: // Play Game
			play_game(p,p_size, transMat);
			break;

		case 2: // Make Game
			make_game_DB(p, p_size, transMat);
			break;
	
		case 3: // Calibration
			transMat = get_calibration_points(boundary);	
		    break;

		default: break;
		}
	}
	return 0;
}


int test_perspective_transform()
{
	// Input Quadilateral or Image plane coordinates
	Point2f inputQuad[4];
	// Output Quadilateral or World plane coordinates
	Point2f outputQuad[4];

	// Lambda Matrix
	Mat lambda(2, 4, CV_32FC1);
	//Input and Output Image;
	Mat input, output;

	//Load the image
	input = imread("lenna.jpg", 1);
	// Set the lambda matrix the same type and size as input
	output = Mat::zeros(input.rows, input.cols, input.type());

	// The 4 points that select quadilateral on the input , from top-left in clockwise order
	// These four pts are the sides of the rect box used as input 
	inputQuad[0] = Point2f(0, 0);
	inputQuad[1] = Point2f(input.cols-1, 0);
	inputQuad[2] = Point2f(input.cols-1, input.rows-1);
	inputQuad[3] = Point2f(0, input.rows-1 );
	// The 4 points where the mapping is to be done , from top-left in clockwise order
	outputQuad[0] = Point2f(50, 50);
	outputQuad[1] = Point2f(input.cols - 1-50, 50);
	outputQuad[2] = Point2f(input.cols - 1, input.rows - 1);
	outputQuad[3] = Point2f(0, input.rows - 1);

	// Get the Perspective Transform Matrix i.e. lambda 
	lambda = getPerspectiveTransform(inputQuad, outputQuad);
	// Apply the Perspective Transform just found to the src image
	warpPerspective(input, output, lambda, output.size());

	cout << lambda << endl;

	//Display input and output
	imshow("Input", input);
	imshow("Output", output);

	waitKey(0);
	return 0;
}