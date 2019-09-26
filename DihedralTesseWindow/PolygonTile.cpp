#include "TilingOpt.h"

using namespace std;

namespace Tiling_tiles{

	PolygonTile::PolygonTile()
	{};

	PolygonTile::PolygonTile(string filename)	
	{
		string t = filename.substr(filename.size() - 3, filename.size() - 1);
		//cout << t<<endl;
		if (t == "txt")  //已有点数据
		{
			txtpath = filename;
			contour = readTxt(filename);			
		}
		else if (t == "jpg" || t == "png")
		{
			imagepath = filename;
			imgtocout(imagepath);
			contour = readTxt("output.txt");
		}
		if (contour.empty())
		{
			cout << "path empty!";
			exit(0);
		}
		Qcontour = vecp_cv2qt(contour);
		poly = QPolygonF(Qcontour);
		poly_c = center_p(contour);
	}

	PolygonTile::PolygonTile(const vector<Point2f> &v)
	{
		contour = v;
		Qcontour = vecp_cv2qt(contour);
		poly = QPolygonF(Qcontour);
		poly_c = center_p(contour);
	}

	PolygonTile::PolygonTile(const QVector<QPointF> &v)
	{
		poly = QPolygonF(v);
		Qcontour = v;
		contour = vecp_qt2cv(Qcontour);
		poly_c = center_p(contour);
	}

	void PolygonTile::poly_clear()
	{
		poly.clear(); //~QPolygonF();
		contour.swap(vector<Point2f>());
		Qcontour.swap(QVector<QPointF>());

		poly_c = Point2f(0, 0);
		c_length = 0;
		imagepath.clear();
		txtpath.clear();
		
		//cconvex.swap(vector<double>());
		contour_sample.swap(vector<vector<Point2f>>());
		contour_sample_flip.swap(vector<vector<Point2f>>());
		//contour_curva.swap(vector<vector<double>>());
		//contour_curva_flip.swap(vector<vector<double>>());
		
		
	}

	void PolygonTile::setpath()
	{
		c_length = 0;
		imagepath = "D:\\VisualStudioProjects\\DihedralTesseWindow\\dataset\\";
		txtpath = "D:\\VisualStudioProjects\\DihedralTesseWindow\\test\\";
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
		/*这里的处理过程：
		1.将任意一张图像转化成灰度图
		2.模糊，利于下一步进行筛选过滤
		3.转化二值图
		4.模糊方便提取轮廓*/
		if (raw == 1)
		{
			cvtColor(src, src_gray, COLOR_BGR2GRAY);
			//blur(src_gray, src_gray, Size(3, 3));
			GaussianBlur(src_gray, src_gray, Size(3, 3), 10, 10);
			//未经处理的图需要四步，之前处理好的用四步会处理过头
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
		//考虑到可能有多个轮廓
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//用candy法由灰度图求出掩码图
		Canny(src_gray, canny_output, thresh, thresh * 2, 3);
		//imshow("canny_output", canny_output);
		//由掩码图求出有序轮廓点
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

		//逆时针存储
		string filepath = "output.txt";
		fileout(filepath, contours[0]);

	}

	vector<Point2f> PolygonTile::readTxt(string filename)
	{
		vector<Point2f> con_point;
		//读取一个存有轮廓点的文件，格式对应上一步计算轮廓点保存的文件
		ifstream in(filename);
		if (!in.is_open())
		{
			cout << "Error opening file: "<< filename << endl;
			return con_point;
		}
		//挨个处理每个字符
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

	void PolygonTile::loadTile(int allpath, string filename) //只读txt文件
	{
		poly_clear();
		if (allpath)
		{
			contour = readTxt(filename);
		}
		else
		{
			string filepath = "D:\\VisualStudioProjects\\DihedralTesseWindow\\test\\" + filename + ".txt";
			contour = readTxt(filepath);
		}
		if (contour.empty()) return;
		Qcontour = vecp_cv2qt(contour);
		poly = QPolygonF(Qcontour);
		poly_c = center_p(contour);
	}

	void PolygonTile::loadPoints(vector<Point2f> con_point)
	{
		poly_clear();
		contour.swap(con_point);
		Qcontour = vecp_cv2qt(contour);
		poly = QPolygonF(Qcontour);
		poly_c = center_p(contour);
	}

	void PolygonTile::contour_sampling()
	{
		double Lambda = 0;
		int sam_num = 0;
		c_length = contour_length(contour);
		// center point
		poly_c = center_p(contour);

		//sampling and computing curvature
		for (int i = 1; i < 6; i++)  //确定采样点数，此处为500点
		{
			vector<Point2f> contour_sam;
			vector<Point2f> contour_sam_flip;
			contour_sam = sampling(contour, i); //点数为 i*100

			contour_sample.push_back(contour_sam);
			//contour_curva.push_back(curvature_com(contour_sam));

			contour_sam_flip = flip_contour(contour_sam, 0); //0是水平翻转
			contour_sample_flip.push_back(contour_sam_flip);

		}
	}

	vector<int> PolygonTile::cand_tiling_v(int max_cur_num)
	{
		//排序，找最大的max_cur_num个凹凸点
		contour.swap(vector<Point2f>());
		int sam_num = contour_sample.size();
		contour = contour_sample[sam_num - 1];
		//cconvex = curvature_com(contour);
		cout << "contour_sample num: " << contour_sample[sam_num - 1].size() << endl;
		vector<int> cand_points_index;
		//cand_points_index = most_convex_p(contour, cconvex, max_cur_num);
		cand_points_index = feature_points(contour, 1, 3, cos(PI * 160 / 180));

		return cand_points_index;
	}

	vector<int> PolygonTile::partition_points()
	{
		int cur_p_num = 20;   //cur_p_num 个不相邻的最大cos值点
		int margin = 12;      //margin个点的采样间隔
		double ratio = 0.012; //筛选间隔与周长之比
		vector<int> max_order;
		contour_sampling();
		max_order = cand_tiling_v(cur_p_num);
		int contoursize = contour.size();
		vector<int> all_order = max_order;

		for (int i = 0; i < contoursize; i = i + margin)
		{
			int flag = 0;
			for (vector<int>::iterator it = max_order.begin(); it != max_order.end(); it++)
			{
				double leng = length_two_point2f(contour[i], contour[*it]);
				if (leng < ratio*c_length)
				{
					flag = 1;
					break;
				}
			}

			if (flag == 0)
			{
				all_order.push_back(i);
			}
		}
		cout << "Candidate tiling vertices num: " << all_order.size() << endl;
		sort_bub(all_order);

		// show convex points
		Mat drawing5 = Mat(800, 800, CV_8UC3, Scalar(255, 255, 255));

		for (int j = 0; j < contoursize; j++)
		{
			circle(drawing5, contour[j], 1, Scalar(0, 0, 0), -1);

			//MyLine(drawing4, prototile_first->contour_sample[sam_num][j] - shift1, prototile_first->contour_sample[sam_num][j + 1] - shift1, "red");
		}
		for (int j = 0; j < max_order.size(); j++)
		{
			circle(drawing5, contour[max_order[j]], 6, Scalar(0, 0, 255), -1);
		}

		for (int j = 0; j < all_order.size(); j++)
		{
			//circle(drawing5, contour[all_order[j]], 3, Scalar(128, 128, 128), -1);
		}

		imshow("convex points: ", drawing5);

		return all_order;
	}

	vector<Point2f> PolygonTile::flip_contour(vector<Point2f> cont_s, int flag)
	{
		Point2f ccen = center_p(cont_s);
		int cont_size = cont_s.size();

		//flag==0 水平翻转
		if (flag == 0)
		{
			for (int i = 0; i < cont_size; i++)
			{
				cont_s[i].x = 2 * ccen.x - cont_s[i].x;
			}
			for (int i = 0; i < cont_size / 2; i++)
			{
				Point2f mid = cont_s[i];
				cont_s[i] = cont_s[cont_size - 1 - i];
				cont_s[cont_size - 1 - i] = mid;
			}
		}
		else if (flag == 1)
		{
			for (int i = 0; i < cont_size; i++)
			{
				cont_s[i].y = 2 * ccen.y - cont_s[i].y;
			}
			for (int i = 0; i < cont_size / 2; i++)
			{
				Point2f mid = cont_s[i];
				cont_s[i] = cont_s[cont_size - 1 - i];
				cont_s[cont_size - 1 - i] = mid;
			}
		}
		return cont_s;
	}

	vector<vector<double>> PolygonTile::compute_TAR(vector<Point2f> &contour_, double &shape_complexity, double frac)
	{
		vector<vector<double>> all_tar;
		int consize = contour_.size();
		int tar_num = frac * consize - 1;
		shape_complexity = 0;
		//cout << "consize: " << consize << " tar_num: " << tar_num << endl;
		vector<double> maxtar(tar_num, 0);// 记录最大值来进行归一化
		for (int i = 0; i < consize; i++)
		{
			vector<double> one_p_tar;
			for (int j = 0; j < tar_num; j++)
			{
				Point2f vpsubts_vp = contour_[(i - j - 1 + consize) % consize] - contour_[i];
				Point2f vpplusts_vp = contour_[(i + j + 1) % consize] - contour_[i];
				double tar = 0.5 * tar_2vector(vpplusts_vp, vpsubts_vp);
				//cout << vpsubts_vp << "  " << vpplusts_vp << endl;
				one_p_tar.push_back(tar);
				if (abs(tar) > maxtar[j]) maxtar[j] = abs(tar);

			}
			all_tar.push_back(one_p_tar);
		}
		for (int i = 0; i < consize; i++)
		{
			double max_tar_one = 0;
			double min_tar_one = 10000;
			for (int j = 0; j < tar_num; j++)
			{
				all_tar[i][j] = all_tar[i][j] / maxtar[j];
				if (all_tar[i][j] > max_tar_one) max_tar_one = all_tar[i][j];
				if (all_tar[i][j] < min_tar_one) min_tar_one = all_tar[i][j];
			}
			shape_complexity += abs(max_tar_one - min_tar_one);
		}
		shape_complexity = shape_complexity / consize;
		//cout << all_tar[0].size() << "    shape_com: " << shape_complexity << endl;
		return all_tar;
	}

	//vector<vector<double>> PolygonTile::compute_TAR(vector<Point2f> &contour_, double &shape_complexity, double frac)
	//{
	//	vector<vector<double>> all_tar;
	//	int consize = contour_.size();
	//	int tar_num = frac * consize - 1;
	//	shape_complexity = 0;
	//	//cout << "consize: " << consize << " tar_num: " << tar_num << endl;
	//	vector<double> maxtar(tar_num, 0);
	//	for (int i = 0; i < consize; i++)
	//	{
	//		vector<double> one_p_tar;
	//		for (int j = 0; j < tar_num; j++)
	//		{
	//			Point2f vpsubts_vp = contour_[(i - j - 1 + consize) % consize] - contour_[i];
	//			Point2f vpplusts_vp = contour_[(i + j + 1) % consize] - contour_[i];
	//			double tar = 0.5 * tar_sin_2vector(vpplusts_vp, vpsubts_vp);
	//			//cout << vpsubts_vp << "  " << vpplusts_vp << endl;
	//			one_p_tar.push_back(tar);
	//			if (abs(tar) > maxtar[j]) maxtar[j] = abs(tar);

	//		}
	//		all_tar.push_back(one_p_tar);
	//	}
	//	for (int i = 0; i < consize; i++)
	//	{
	//		double max_tar_one = 0;
	//		double min_tar_one = 10000;
	//		for (int j = 0; j < tar_num; j++)
	//		{
	//			all_tar[i][j] = all_tar[i][j] / maxtar[j];
	//			if (all_tar[i][j] > max_tar_one) max_tar_one = all_tar[i][j];
	//			if (all_tar[i][j] < min_tar_one) min_tar_one = all_tar[i][j];
	//		}
	//		shape_complexity += abs(max_tar_one - min_tar_one);
	//	}
	//	shape_complexity = shape_complexity / consize;
	//	//cout << all_tar[0].size() << "    shape_com: " << shape_complexity << endl;
	//	return all_tar;
	//}


	


	void PolygonTile::drawPolygon()
	{
		Mat drawing_pro = Mat(600, 600, CV_8UC3, Scalar(255, 255, 255));
		Point2f cen_b = Point2f(300, 300) + center_p(contour) - center_p(b_box(contour));
		draw_poly(drawing_pro, contour, cen_b);
		imshow("Input image", drawing_pro);
	}

}