#include "TilingOpt.h"

using namespace std;

namespace Tiling_tiles{

	PolygonTile::PolygonTile()
	{};

	PolygonTile::PolygonTile(string filename)
	{
		string t = filename.substr(filename.size() - 3, filename.size() - 1);
		cout << t;
		if (t == "txt")  //���е�����
		{
			txtpath = filename;
			contour = readTxt(filename);
			Qcontour = vecp_cv2qt(contour);
			poly = QPolygonF(Qcontour);
			poly_c = center_p(contour);
			
		}
		else if (t == "jpg" || t == "png")
		{
			imagepath = filename;
			imgtocout(imagepath);
			contour = readTxt("output.txt");
			Qcontour = vecp_cv2qt(contour);
			poly = QPolygonF(Qcontour);
			poly_c = center_p(contour);
		}
	}
	PolygonTile::PolygonTile(const QVector<QPointF> &v)
	{
		poly = QPolygonF(v);
		Qcontour = v;
		contour = vecp_qt2cv(Qcontour);
		poly_c = center_p(contour);
	}

	void PolygonTile::imgtocout(string imagepath, int raw)
	{
		int show = 1;
		Mat src;
		Mat src_gray;
		Mat src_2;
		int thresh = 100;
		int max_thresh = 255;

		//read image
		String imageName = imagepath;
		//String imageName = dataroot + tile_image + ".png"; // by default
		cout << imageName << endl;
		src = imread(imageName, IMREAD_COLOR);
		//src = imread(imageName, CV_LOAD_IMAGE_UNCHANGED);
		if (src.empty())
		{
			cerr << "No image supplied ..." << endl;
			return;
		}
		/*����Ĵ�����̣�
		1.������һ��ͼ��ת���ɻҶ�ͼ
		2.ģ����������һ������ɸѡ����
		3.ת����ֵͼ
		4.ģ��������ȡ����*/
		if (raw == 1)
		{
			cvtColor(src, src_gray, COLOR_BGR2GRAY);
			//blur(src_gray, src_gray, Size(3, 3));
			GaussianBlur(src_gray, src_gray, Size(3, 3), 10, 10);
			//δ�������ͼ��Ҫ�Ĳ���֮ǰ����õ����Ĳ��ᴦ���ͷ
			threshold(src_gray, src_gray, 128, 255, cv::THRESH_BINARY); //255 white
			//imwrite(dataroot +"new\\"+ tile_image + ".png", src_gray);
			imwrite(imagepath, src_gray);

			/*if (imread(dataroot + tile_image + "2.png", IMREAD_COLOR).empty())
			imwrite(dataroot + tile_image + "2.png", src_gray);
			else
			{
			cout << tile_image + "2.png has already exist!!!" << endl;
			imwrite(dataroot + tile_image + "0000.png", src_gray);
			}*/
			blur(src_gray, src_gray, Size(3, 3));
			imshow("src_gray_blur", src_gray);
		}
		else
		{
			cvtColor(src, src_gray, COLOR_BGR2GRAY);
			blur(src_gray, src_gray, Size(3, 3));
			//imshow("src_gray_blur", src_gray);
		}
		Mat canny_output;
		//���ǵ������ж������
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//��candy���ɻҶ�ͼ�������ͼ
		Canny(src_gray, canny_output, thresh, thresh * 2, 3);
		//imshow("canny_output", canny_output);
		//������ͼ�������������
		findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		cout << "contours num:" << contours.size() << endl;

		if (show == 1)
		{
			Mat drwa = Mat::zeros(800, 800, CV_8UC3);
			int i = 0;
			for (; i < contours[0].size() / 4; i++)
			{
				circle(drwa, contours[0][i], 1, Scalar(255, 0, 0), -1);
			}
			for (; i < contours[0].size() / 2; i++)
			{
				circle(drwa, contours[0][i], 1, Scalar(0, 255, 0), -1);
			}
			for (; i < contours[0].size(); i++)
			{
				circle(drwa, contours[0][i], 1, Scalar(0, 0, 255), -1);
			}
			imshow("contour sampling", drwa);
		}

		//��ʱ��洢
		string filepath = "output.txt";
		fileout(filepath, contours[0]);

	}

	vector<Point2f> PolygonTile::readTxt(string filename)
	{
		vector<Point2f> con_point;
		//��ȡһ��������������ļ�����ʽ��Ӧ��һ�����������㱣����ļ�
		ifstream in(filename);
		if (!in.is_open())
		{
			cout << filename << endl;
			cout << "Error opening file" << endl;
			return con_point;
		}
		//��������ÿ���ַ�
		//cout << "Opening file!!!" << endl;
		vector<char> each_point;
		int aa = 0;
		int bb = 0;
		int nn = 0;
		char cc;
		char buf[200];
		//for ()
		in.getline(buf, 200);
		//cout << "num: " << buf << endl;
		while (!in.eof())
		{
			aa = 0;
			bb = 0;
			nn = 0;
			int f = 0;
			in.getline(buf, 200);
			cc = buf[nn++];
			while ((cc >= '0' && cc <= '9') || cc == ',' || cc == ' ')
			{
				f = 1;
				if ((cc >= '0' && cc <= '9'))
				{
					each_point.push_back(cc);
				}
				if (cc == ',' || cc == ' ')
				{
					for (int i = 0; i < each_point.size(); i++)
					{
						aa = aa * 10 + (each_point[i] - 48);
					}
					each_point.swap(vector<char>());
				}
				cc = buf[nn++];
			}
			if (f)
			{
				for (int i = 0; i < each_point.size(); i++)
				{
					bb = bb * 10 + (each_point[i] - 48);
				}
				each_point.swap(vector<char>());
				con_point.push_back(Point(aa, bb));
			}
		}
		in.close();
		return con_point;
	}

	void PolygonTile::drawPolygon()
	{
		Mat drawing_pro = Mat(600, 600, CV_8UC3, Scalar(255, 255, 255));
		draw_poly(drawing_pro, contour, Point2f(300,300));
		imshow("Input image", drawing_pro);
	}

}