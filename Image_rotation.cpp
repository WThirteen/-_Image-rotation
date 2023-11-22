#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

using namespace std;
using namespace cv;

#define PI 3.1415926

/// ͼ����ת
int main() {
	Mat img = imread("D:/_jpg_all/Test/3.jpg");
	// ����ͼ��
	Mat img1;
	resize(img, img1, Size(500, 500));
	// ��Сͼ�� ���ڼ���
	float row = img1.rows;
	float col = img1.cols;
	float r = img1.channels();
	Mat gray;
	if (r > 1) {
		cvtColor(img1, gray, COLOR_BGR2GRAY);
	}
	gray.convertTo(gray, CV_32FC1);
	//���û����һ����grayΪ8λ����
	// 
	float theta = 60;

	// ��ת����
	Mat rot = (Mat_<float>(2, 2) << cos(theta * PI / 180), sin(theta * PI / 180), -sin(theta * PI / 180), cos(theta * PI / 180));
	// ��� ʵ�ּ��� ��ת�󶥵�����
	Mat a = rot * (Mat_<float>(2, 1) << 0, 0);
	Mat b = rot * (Mat_<float>(2, 1) << 0, col - 1);
	Mat c = rot * (Mat_<float>(2, 1) << row - 1, 0);
	Mat d = rot * (Mat_<float>(2, 1) << row - 1, col - 1);
	//cout << a << endl<<b<<endl<<c<<endl<<d;

	// ������ת����Ŀ�� weigth
	float w1 = abs(b.at<float>(1, 0) - c.at<float>(1, 0));
	float w2 = abs(d.at<float>(1, 0) - a.at<float>(1, 0));
	//�ж���Ŀ��
	float w;
	if (w1 > w2) {
		w = round(w1);
	}
	else {
		w = round(w2);
	}

	// ������ת����ĸ߶� hight
	float h1 = abs(b.at<float>(0, 0) - c.at<float>(0, 0));
	float h2 = abs(d.at<float>(0, 0) - a.at<float>(0, 0));
	//�ж���Ŀ��
	float h;
	if (h1 > h2) {
		h = round(h1);
	}
	else {
		h = round(h2);
	}

	//�����¾��� ���ڷ�����ת��ͼ��
	Mat new_img = Mat::zeros(h, w, CV_32FC1);
	// ����ƫ��
	Mat delta_h = (Mat_<float>(4, 1) << a.at<float>(0, 0), b.at<float>(0, 0), c.at<float>(0, 0), d.at<float>(0, 0));
	Mat delta_w = (Mat_<float>(4, 1) << a.at<float>(1, 0), b.at<float>(1, 0), c.at<float>(1, 0), d.at<float>(1, 0));
	//Ѱ����С
	float min_h = abs(*(min_element(delta_h.begin<float>(), delta_h.end<float>())));
	float min_w = abs(*(min_element(delta_w.begin<float>(), delta_w.end<float>())));

	for (int i = 0 - min_h; i < h - min_h; i++) {
		for (int j = 0 - min_w; j < w - min_w; j++) {
			// �õ�����ת�����꣬��ԭͼ��Ӧ������
			Mat pix = rot.inv() * (Mat_<float>(2, 1) << i, j);
			// ����λС��
			float pix1[2] = { pix.at<float>(0,0),pix.at<float>(1,0) };
			// ˫���Բ�ֵѰ��С���Ĳ�ֵ �õ��õ�����ֵ
			// ����ò�ֵ
			if (pix1[0] >= 0 && pix1[0] <= row - 1 && pix1[1] >= 0 && pix1[1] <= col - 1) {
				float left_up[2] = { floor(pix1[0]),floor(pix1[1]) };
				float right_up[2] = { floor(pix1[0]),ceil(pix1[1]) };
				float left_dowm[2] = { ceil(pix1[0]),floor(pix1[1]) };
				float right_dowm[2] = { ceil(pix1[0]),ceil(pix1[1]) };

				float w1 = pix1[0] - floor(pix1[0]);// (x2+x)/(x2-x1)  x2-x1=1��
				float w2 = pix1[1] - floor(pix1[1]);//(y2+y)/(y2-y1)  y2-y1=1��

				///*round �������� ���ǵ���Ϊ���������*/
				new_img.at<float>(i+min_h, j+min_w) = round(gray.at<float>(left_up[0], left_up[1]) * (1 - w1) * (1 - w2)\
					+ gray.at<float>(right_up[0], right_up[1]) * w1 * (1 - w2)\
					+ gray.at<float>(left_dowm[0], left_dowm[1]) * (1 - w1) * w2\
					+ gray.at<float>(right_dowm[0], right_dowm[1]) * w1 * w2);
			}
		}
	}
	new_img.convertTo(new_img, CV_8UC1);
	imshow("img", new_img);
	waitKey(0);

	return 0;
}
