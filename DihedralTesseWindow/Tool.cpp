#include<stdio.h>
#include<math.h>
#include "TilingOpt.h"

namespace Tiling_tiles{

	vector<pair<string, Scalar>> colorbar = {
		{ "black", Scalar(0, 0, 0) },
		{ "white", Scalar(255, 255, 255) },
		{ "blue", Scalar(255, 0, 0) },
		{ "green", Scalar(0, 255, 0) },
		{ "red", Scalar(0, 0, 255) },
		{ "red1", Scalar(127, 110, 174) },
		{ "gray", Scalar(194, 194, 194) },
		{ "blue1", Scalar(251, 228, 169) },
		{ "blue2", Scalar(251, 204, 176) },
		{ "blue3", Scalar(251, 181, 105) },
		{ "green1", Scalar(130, 174, 89) },
		{ "green2", Scalar(222, 250, 167) },
		{ "yellow", Scalar(0, 255, 255) },
		{ "orange1", Scalar(70, 124, 217) },
		{ "orange2", Scalar(3, 142, 249) },
		{ "pink", Scalar(237, 171, 245) },
		{ "lightorange2", Scalar(175, 211, 249) },
		{ "lightpink", Scalar(244, 211, 247) } };
	

	void MyLine(Mat img, Point2f start, Point2f end, string color1)
	{
		Scalar color = Scalar(128);
		int cosize = colorbar.size();
		for (int i = 0; i < cosize; i++)
		{
			if (color1.compare(colorbar[i].first) == 0)
			{
				color = colorbar[i].second;
				break;
			}
		}
		int thickness = 2;
		int lineType = 8;
		line(img,
			start,
			end,
			color,
			thickness,
			lineType);
	}
	Mat draw_polygen(string win_name, vector<Point2f> contour_s)
	{
		int col = 800;
		int row = 800;
		Mat drawing_pro = Mat(row, col, CV_8UC3, Scalar(255, 255, 255));
		int n = contour_s.size();
		//cout << "n: " << n << endl;
		Point rook_points[1][2000];
		for (int t = 0; t < n; t++)
		{
			rook_points[0][t] = contour_s[t];
		}
		const Point* ppt[1] = { rook_points[0] };
		int npt[] = { n };
		fillPoly(drawing_pro,
			ppt,
			npt,
			1,
			Scalar(0, 0, 0) //黑色
			//Scalar(255, 255, 255) //白色
			);
		//imshow(win_name, drawing_pro);
		return drawing_pro;
	}
	void draw_poly(Mat &drawing_, vector<Point2f> contour_s, Point2f center, int color)
	{
		Scalar col_sca = Scalar(0, 0, 0);
		if (color < colorbar.size() && color>0)
			col_sca = colorbar[color].second;
		Point2f shift = center - center_p(contour_s);
		int n = contour_s.size();
		//cout << "n: " << n << endl;
		Point rook_points[1][2000];
		for (int t = 0; t < n; t++)
		{
			rook_points[0][t] = contour_s[t] + shift;
		}
		const Point* ppt[1] = { rook_points[0] };
		int npt[] = { n };
		fillPoly(drawing_,
			ppt,
			npt,
			1,
			col_sca //黑色
			//Scalar(255, 255, 255) //白色
			);
		//circle(drawing_, contour_s[0] + shift, 4, Scalar(255), 3);
		//circle(drawing_, center, 4, Scalar(0, 255, 255), -1);
	}

	void draw_allplane(Mat &drawing_, vector<Point2f> contour_, vector<int> vec_, double scale, int type)
	{
		int color = 1;//white
		int drawrow = drawing_.rows;
		int drawcol = drawing_.cols;
		int border = -300 * scale;
		int con_size = contour_.size();
		if (scale != 1)
		{
			for (int i = 0; i < con_size; i++)
			{
				contour_[i] = scale*contour_[i];
			}
		}
		double length_ = arcLength(contour_, 1) / con_size;
		vector<Point2f> allcent_in_plane;
		vector<Point2f> allcent_in_plane_;
		//vector<vector<Point2f>> four_place;
		//vector<Point2f> one_loca;
		if (type == 0) //translation
		{
			stack<Point2f> cent_stack;
			Point2f step[4];
			step[0] = contour_[vec_[2]] - contour_[vec_[0]];
			step[1] = contour_[vec_[3]] - contour_[vec_[1]];
			step[2] = -step[0];
			step[3] = -step[1];

			Point2f cenp = Point2f(drawrow / 2, drawcol / 2);
			draw_poly(drawing_, contour_, cenp, color);
			cent_stack.push(cenp);
			while (!cent_stack.empty())
			{
				//cout << cent_stack.size() << endl;
				if (cent_stack.size() == 500000) break;
				Point2f top_p = cent_stack.top();
				cent_stack.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp = top_p + step[i];
					if ((cenp.x<drawcol + border) && (cenp.x>-border) && (cenp.y<drawrow + border) && (cenp.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane.begin(); it != allcent_in_plane.end(); it++)
						{
							double leng = length_two_point2f(cenp, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack.push(cenp);
							allcent_in_plane.push_back(cenp);
							draw_poly(drawing_, contour_, cenp,color);
						}
					}
				}
			}
		}
		else if (type == 1) //rotation
		{
			Point2f center = center_p(contour_);
			vector<Point2f> contour_r;
			Point2f rota_cent = contour_[vec_[0]];
			Mat rot_mat = getRotationMatrix2D(rota_cent, 180, 1);
			transform(contour_, contour_r, rot_mat);
			Point2f shift = center_p(contour_r) - center;

			stack<Point2f> cent_stack;
			stack<Point2f> cent_stack_r;
			Point2f step[4];
			step[0] = 2 * (contour_[vec_[1]] - contour_[vec_[0]]);
			step[1] = 2 * (contour_[vec_[3]] - contour_[vec_[0]]);
			step[2] = -step[0];
			step[3] = -step[1];

			Point2f cenp = Point2f(drawrow / 2, drawcol / 2);
			Point2f cenp_r = cenp + shift;
			draw_poly(drawing_, contour_, cenp, color);
			draw_poly(drawing_, contour_r, cenp_r, color);
			cent_stack.push(cenp);
			cent_stack_r.push(cenp_r);
			while (!cent_stack.empty())
			{
				Point2f top_p = cent_stack.top();
				cent_stack.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp = top_p + step[i];
					if ((cenp.x<drawcol + border) && (cenp.x>-border) && (cenp.y<drawrow + border) && (cenp.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane.begin(); it != allcent_in_plane.end(); it++)
						{
							double leng = length_two_point2f(cenp, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack.push(cenp);
							allcent_in_plane.push_back(cenp);
							draw_poly(drawing_, contour_, cenp,color);
						}
					}
				}
			}
			while (!cent_stack_r.empty())
			{
				Point2f top_p = cent_stack_r.top();
				cent_stack_r.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp_r = top_p + step[i];
					if ((cenp_r.x<drawcol + border) && (cenp_r.x>-border) && (cenp_r.y<drawrow + border) && (cenp_r.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane_.begin(); it != allcent_in_plane_.end(); it++)
						{
							double leng = length_two_point2f(cenp_r, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack_r.push(cenp_r);
							allcent_in_plane_.push_back(cenp_r);
							draw_poly(drawing_, contour_r, cenp_r,color);
						}
					}
				}
			}
		}
		else if (type == 2) //flipping (1-3)
		{
			Point2f line1 = contour_[vec_[2]] - contour_[vec_[0]];
			Point2f line2 = contour_[vec_[3]] - contour_[vec_[1]];
			Point2f center = center_p(contour_);
			Point2f line1_cent = 0.5 * contour_[vec_[2]] + 0.5 * contour_[vec_[0]];
			Point2f s_axis(line2.y, -line2.x);
			Line_Seg symmetry_axis(line1_cent + 5 * s_axis, line1_cent - 5 * s_axis);
			//计算翻转后的轮廓，因为直接任意轴对称翻转比较耗时，因此先以x轴为对称轴翻转，然后旋转相应角度
			double rota_angle = acos(cos_two_vector(Point2f(0, 1), s_axis)) / PI * 180;
			if (sin_two_vector(Point2f(0, 1), s_axis) > 0) rota_angle = -rota_angle;
			//cout << rota_angle << endl;
			Point2f cross;
			if (line_intersection(Line_Seg(symmetry_axis), Line_Seg(center - 5 * line2, center + 5 * line2), cross) != 1) cout << "Error!" << endl;  //没有交点说明出错了
			Point2f cent_shift = 2 * (cross - center);
			vector<Point2f> contour_r = flip_only_coord(contour_);
			Mat rot_mat = getRotationMatrix2D(center, 2 * rota_angle, 1);
			transform(contour_r, contour_r, rot_mat);
			//将以上所得翻转的轮廓沿相对应的轴平移，计算该位置
			double cos_ax_1_3 = cos_two_vector(s_axis, line1);
			Point2f line3 = s_axis;
			if (cos_ax_1_3 < 0) line3 = -s_axis;
			//这里line3代表对称轴上与line1成锐角夹角方向的向量，长度为line1在对称轴上的投影
			line3 = line3*(abs(line1.x*line3.x + line1.y*line3.y) / (line3.x*line3.x + line3.y*line3.y));
			Point2f shift = cent_shift + line3;

			for (int i = 0; i < con_size; i++)
			{
				contour_r[i] += shift;
			}
			stack<Point2f> cent_stack;
			stack<Point2f> cent_stack_r;
			Point2f step[4];
			step[0] = 2 * line3;
			step[1] = line2;
			step[2] = -step[0];
			step[3] = -step[1];

			Point2f cenp = Point2f(drawrow / 2, drawcol / 2);
			Point2f cenp_r = cenp + shift;
			draw_poly(drawing_, contour_, cenp,color);
			draw_poly(drawing_, contour_r, cenp_r,color);
			cent_stack.push(cenp);
			cent_stack_r.push(cenp_r);
			while (!cent_stack.empty())
			{
				Point2f top_p = cent_stack.top();
				cent_stack.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp = top_p + step[i];
					if ((cenp.x<drawcol + border) && (cenp.x>-border) && (cenp.y<drawrow + border) && (cenp.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane.begin(); it != allcent_in_plane.end(); it++)
						{
							double leng = length_two_point2f(cenp, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack.push(cenp);
							allcent_in_plane.push_back(cenp);
							draw_poly(drawing_, contour_, cenp,color);
						}
					}
				}
			}
			while (!cent_stack_r.empty())
			{
				Point2f top_p = cent_stack_r.top();
				cent_stack_r.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp_r = top_p + step[i];
					if ((cenp_r.x<drawcol + border) && (cenp_r.x>-border) && (cenp_r.y<drawrow + border) && (cenp_r.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane_.begin(); it != allcent_in_plane_.end(); it++)
						{
							double leng = length_two_point2f(cenp_r, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack_r.push(cenp_r);
							allcent_in_plane_.push_back(cenp_r);
							draw_poly(drawing_, contour_r, cenp_r,color);
						}
					}
				}
			}
		}
		else if (type == 3) //flipping (2-4)
		{
			Point2f line1 = contour_[vec_[2]] - contour_[vec_[0]];
			Point2f line2 = contour_[vec_[3]] - contour_[vec_[1]];
			Point2f center = center_p(contour_);
			Point2f line2_cent = 0.5 * contour_[vec_[3]] + 0.5 * contour_[vec_[1]];
			Point2f s_axis(line1.y, -line1.x);
			Line_Seg symmetry_axis(line2_cent + 5 * s_axis, line2_cent - 5 * s_axis);
			//计算翻转后的轮廓，因为直接任意轴对称翻转比较耗时，因此先以x轴为对称轴翻转，然后旋转相应角度
			double rota_angle = acos(cos_two_vector(Point2f(0, 1), s_axis)) / PI * 180;
			if (sin_two_vector(Point2f(0, 1), s_axis) > 0) rota_angle = -rota_angle;
			//cout << rota_angle << endl;
			Point2f cross;
			if (line_intersection(Line_Seg(symmetry_axis), Line_Seg(center - 5 * line1, center + 5 * line1), cross) != 1) cout << "Error!" << endl;  //没有交点说明出错了
			Point2f cent_shift = 2 * (cross - center);
			vector<Point2f> contour_r = flip_only_coord(contour_);
			Mat rot_mat = getRotationMatrix2D(center, 2 * rota_angle, 1);
			transform(contour_r, contour_r, rot_mat);
			//将以上所得翻转的轮廓沿相对应的轴平移，计算该位置
			double cos_ax_2_4 = cos_two_vector(s_axis, line2);
			Point2f line3 = s_axis;
			if (cos_ax_2_4 < 0) line3 = -s_axis;
			//这里line3代表对称轴上与line2成锐角夹角方向的向量，长度为line2在对称轴上的投影
			line3 = line3*(abs(line2.x*line3.x + line2.y*line3.y) / (line3.x*line3.x + line3.y*line3.y));
			Point2f shift = cent_shift + line3;
			for (int i = 0; i < con_size; i++)
			{
				contour_r[i] += shift;
			}
			stack<Point2f> cent_stack;
			stack<Point2f> cent_stack_r;
			Point2f step[4];
			step[0] = line1;
			step[1] = 2 * line3;
			step[2] = -step[0];
			step[3] = -step[1];

			Point2f cenp = Point2f(drawrow / 2, drawcol / 2);
			Point2f cenp_r = cenp + shift;
			draw_poly(drawing_, contour_, cenp,color);
			draw_poly(drawing_, contour_r, cenp_r,color);
			cent_stack.push(cenp);
			cent_stack_r.push(cenp_r);
			while (!cent_stack.empty())
			{
				Point2f top_p = cent_stack.top();
				cent_stack.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp = top_p + step[i];
					if ((cenp.x<drawcol + border) && (cenp.x>-border) && (cenp.y<drawrow + border) && (cenp.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane.begin(); it != allcent_in_plane.end(); it++)
						{
							double leng = length_two_point2f(cenp, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack.push(cenp);
							allcent_in_plane.push_back(cenp);
							draw_poly(drawing_, contour_, cenp,color);
						}
					}
				}
			}
			while (!cent_stack_r.empty())
			{
				Point2f top_p = cent_stack_r.top();
				cent_stack_r.pop();
				for (int i = 0; i < 4; i++)
				{
					cenp_r = top_p + step[i];
					if ((cenp_r.x<drawcol + border) && (cenp_r.x>-border) && (cenp_r.y<drawrow + border) && (cenp_r.y>-border))
					{
						int flag = 0;
						for (vector<Point2f>::iterator it = allcent_in_plane_.begin(); it != allcent_in_plane_.end(); it++)
						{
							double leng = length_two_point2f(cenp_r, *it);
							if (leng < length_)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 0)
						{
							cent_stack_r.push(cenp_r);
							allcent_in_plane_.push_back(cenp_r);
							draw_poly(drawing_, contour_r, cenp_r,color);
						}
					}
				}
			}
		}
	}

	void draw_result(Mat &drawing_, vector<Point2f> contour_, vector<int> vec_, double scale, int type, Point2f shift)
	{
		int drawrow = drawing_.rows;
		int drawcol = drawing_.cols;
		int border = 0;//30 * scale;
		int csize1 = contour_.size();
		vector<Point2f> cont1;
		if (scale != 1)
		{
			for (int i = 0; i < csize1; i++)
			{
				cont1.push_back(scale*contour_[i]);
			}
		}
		else cont1 = contour_;
		Point2f cen_con1 = center_p(cont1);
		int c_a_width = 3;
		vector<vector<Point2f>> connection_a;

		if (type == 0) //translation
		{
			Point2f step[4];
			step[0] = cont1[vec_[2]] - cont1[vec_[0]];
			step[1] = cont1[vec_[3]] - cont1[vec_[1]];
			step[2] = -step[0];
			step[3] = -step[1];

			Point2f cenp = Point2f(drawcol / 2, drawrow / 2) + shift;
			vector<Point2f> connect_area;

			move_con(cont1, cenp - center_p(cont1));
			draw_poly(drawing_, cont1, cenp);
			connect_area.push_back(cont1[(vec_[2] + csize1 - c_a_width) % csize1]);
			connect_area.push_back(cont1[(vec_[2] + c_a_width) % csize1]);

			int new_poly = 30;
			for (int t = 1; new_poly > 0; t = t + 2)
			{
				int bey = 0;
				for (int index = 0; index < 4; index++)
				{
					int r = index / 2;
					for (int n = 0; n < t + r; n++)
					{
						move_con(cont1, step[index]);
						cenp += step[index];
						Point2f conn_p = cont1[vec_[index]];//cenp + connec_cent[index];
						connect_area.push_back(cont1[(vec_[index] + csize1 - c_a_width) % csize1]);
						connect_area.push_back(cont1[(vec_[index] + c_a_width) % csize1]);

						if ((conn_p.x<drawcol + border) && (conn_p.x>-border) && (conn_p.y<drawrow + border) && (conn_p.y>-border))
						{
							new_poly--;
							draw_poly(drawing_, cont1, cenp);
							connection_a.push_back(connect_area);
						}
						else
						{
							bey = 1;
							break;
						}
						connect_area.swap(vector<Point2f>());
						if (n < t + r - 1)
						{
							connect_area.push_back(cont1[(vec_[(index + 2) % 4] + csize1 - c_a_width) % csize1]);
							connect_area.push_back(cont1[(vec_[(index + 2) % 4] + c_a_width) % csize1]);
						}
						else
						{
							connect_area.push_back(cont1[(vec_[(index + 3) % 4] + csize1 - c_a_width) % csize1]);
							connect_area.push_back(cont1[(vec_[(index + 3) % 4] + c_a_width) % csize1]);
						}
					}
					if (bey == 1) break;
				}
				if (bey == 1) break;
			}
			for (int g = 0; g < connection_a.size(); g++)
			{
				draw_poly(drawing_, connection_a[g], center_p(connection_a[g]));
			}
		}
		else if (type == 1) //rotation
		{

		}
		else if (type == 2) //reflection(1-3)
		{

		}
		else if (type == 3) //reflection(2-4)
		{
		}
	}
	void draw_two(Mat &drawing_, vector<Point2f> &contour_1, vector<int> vec_1, vector<Point2f> &contour_2, vector<int> vec_2, double scale, int type)
	{
		//contour_2务必由contour_1提取得到
		int drawrow = drawing_.rows;
		int drawcol = drawing_.cols;
		//double length_m = 0;//drawrow / 200 * 2;
		int border = 0;//30 * scale;
		int csize1 = contour_1.size();
		int csize2 = contour_2.size();
		vector<Point2f> cont1;
		vector<Point2f> cont2;
		if (scale != 1)
		{
			for (int i = 0; i < csize1; i++)
			{
				cont1.push_back(scale*contour_1[i]);
			}
			for (int i = 0; i < csize2; i++)
			{
				cont2.push_back(scale*contour_2[i]);
			}
		}
		else
		{
			cont1 = contour_1;
			cont2 = contour_2;
		}
		Point2f cen_con1 = center_p(cont1);
		Point2f cen_con2 = center_p(cont2);
		Point2f shift_ = cen_con2 - cen_con1;
		int c_a_width = 5;
		vector<vector<Point2f>> connection_a;
		vector<vector<Point2f>> connection_a2;

		if (type == 0) //translation
		{
			Point2f step[4];
			step[0] = cont1[vec_1[2]] - cont1[vec_1[0]];
			step[1] = cont1[vec_1[3]] - cont1[vec_1[1]];
			step[2] = -step[0];
			step[3] = -step[1];

			Point2f cenp = Point2f(drawcol / 2, drawrow / 2);
			Point2f cenp2 = cenp + shift_;
			vector<Point2f> connect_area;
			vector<Point2f> connect_area2;

			move_con(cont1, cenp - center_p(cont1));
			move_con(cont2, cenp2 - center_p(cont2));
			draw_poly(drawing_, cont1, cenp);
			Point2f conn_11 = cont1[(vec_1[2] + csize1 - c_a_width) % csize1];
			Point2f conn_12 = cont1[(vec_1[2] + c_a_width) % csize1];
			/*for (int add = 1; length_two_point2f(conn_11, conn_12) < length_m; add++)
			{
			cout << "length1:  " << length_two_point2f(conn_11, conn_12) << endl;
			conn_11 = cont1[(vec_1[2] + csize1 - add - c_a_width) % csize1];
			conn_12 = cont1[(vec_1[2] + add + c_a_width) % csize1];
			}*/
			connect_area.push_back(conn_11);
			connect_area.push_back(conn_12);

			draw_poly(drawing_, cont2, cenp2, 1);
			Point2f conn_21 = cont2[(vec_2[0] + csize2 - c_a_width) % csize2];
			Point2f conn_22 = cont2[(vec_2[0] + c_a_width) % csize2];
			/*for (int add = 1; length_two_point2f(conn_21, conn_22) < length_m; add++)
			{
			cout << "length2:  " << length_two_point2f(conn_21, conn_22) << endl;
			conn_21 = cont2[(vec_2[0] + csize2 - add - c_a_width) % csize2];
			conn_22 = cont2[(vec_2[0] + add + c_a_width) % csize2];
			}	*/
			connect_area2.push_back(conn_21);
			connect_area2.push_back(conn_22);
			//for (int gg = 0; gg < 2; gg++)
			//{
			//	cout << connect_area[gg] << " ";
			//	circle(drawing_, connect_area[gg], 5, Scalar(255, 128, 0), -1);
			//	cout << endl;
			//	cout << connect_area2[gg] << " ";
			//	circle(drawing_, connect_area2[gg], 5, Scalar(128, 255, 0), -1);
			//}
			//cout << endl;
			int new_poly = 60;
			for (int t = 1; new_poly > 0; t = t + 2)
			{
				int bey = 0;
				for (int index = 0; index < 4; index++)
				{
					int r = index / 2;
					for (int n = 0; n < t + r; n++)
					{
						move_con(cont1, step[index]);
						move_con(cont2, -step[index]);
						cenp += step[index];
						cenp2 += -step[index];
						Point2f conn_p = cont1[vec_1[index]];//cenp + connec_cent[index];		
						Point2f conn_p2 = cont2[vec_2[(index + 2) % 4]];

						Point2f conn_13 = cont1[(vec_1[index] + csize1 - c_a_width) % csize1];
						Point2f conn_14 = cont1[(vec_1[index] + c_a_width) % csize1];
						/*for (int add = 1; length_two_point2f(conn_13, conn_14) < length_m; add++)
						{
						cout << "length3:  " << length_two_point2f(conn_13, conn_14) << endl;
						conn_13 = cont1[(vec_1[index] + csize1 - add - c_a_width) % csize1];
						conn_14 = cont1[(vec_1[index] + add + c_a_width) % csize1];
						}*/
						connect_area.push_back(conn_13);
						connect_area.push_back(conn_14);

						draw_poly(drawing_, cont2, cenp2, 1);
						Point2f conn_23 = cont2[(vec_2[(index + 2) % 4] + csize2 - c_a_width) % csize2];
						Point2f conn_24 = cont2[(vec_2[(index + 2) % 4] + c_a_width) % csize2];
						/*for (int add = 1; length_two_point2f(conn_23, conn_24) < length_m; add++)
						{
						cout << "length4:  " << length_two_point2f(conn_23, conn_24) << endl;
						conn_23 = cont2[(vec_2[(index + 2) % 4] + csize2 - add - c_a_width) % csize2];
						conn_24 = cont2[(vec_2[(index + 2) % 4] + add + c_a_width) % csize2];
						}*/
						connect_area2.push_back(conn_23);
						connect_area2.push_back(conn_24);
						if ((conn_p.x<drawcol + border) && (conn_p.x>-border) && (conn_p.y<drawrow + border) && (conn_p.y>-border))
						{
							if ((conn_p2.x<drawcol + border) && (conn_p2.x>-border) && (conn_p2.y<drawrow + border) && (conn_p2.y>-border))
							{
								new_poly--;
								draw_poly(drawing_, cont1, cenp);
								//draw_poly(drawing_, connect_area, center_p(connect_area));
								//circle(drawing_, conn_p, 9, Scalar(255, 0, 0), -1);
								/*for (int gg = 0; gg < 4; gg++)
								{
								circle(drawing_, connect_area[gg], 2, Scalar(255, 128, 0), -1);
								}*/
								draw_poly(drawing_, cont2, cenp2, 1);
								connection_a.push_back(connect_area);
								connection_a2.push_back(connect_area2);
								//circle(drawing_, conn_p2, 9, Scalar(0, 255, 0), -1);

								//draw_poly(drawing_, connect_area2, center_p(connect_area2), 1);
							}
							else
							{
								bey = 1;
								break;
							}
						}
						else
						{
							bey = 1;
							break;
						}
						connect_area.swap(vector<Point2f>());
						connect_area2.swap(vector<Point2f>());
						if (n < t + r - 1)
						{
							conn_11 = cont1[(vec_1[(index + 2) % 4] + csize1 - c_a_width) % csize1];
							conn_12 = cont1[(vec_1[(index + 2) % 4] + c_a_width) % csize1];
							/*for (int add = 1; length_two_point2f(conn_11, conn_12) < length_m; add++)
							{
							cout << "length5:  " << length_two_point2f(conn_11, conn_12) << endl;
							conn_11 = cont1[(vec_1[(index + 2) % 4] + csize1 - add - c_a_width) % csize1];
							conn_12 = cont1[(vec_1[(index + 2) % 4] + add + c_a_width) % csize1];
							}*/
							conn_21 = cont2[(vec_2[index] + csize2 - c_a_width) % csize2];
							conn_22 = cont2[(vec_2[index] + c_a_width) % csize2];
							/*for (int add = 1; length_two_point2f(conn_21, conn_22) < length_m; add++)
							{
							cout << "length6:  " << length_two_point2f(conn_21, conn_22) << endl;
							conn_21 = cont2[(vec_2[index] + csize2 -add - c_a_width) % csize2];
							conn_22 = cont2[(vec_2[index] + add + c_a_width) % csize2];
							}	*/
						}
						else
						{
							conn_11 = cont1[(vec_1[(index + 3) % 4] + csize1 - c_a_width) % csize1];
							conn_12 = cont1[(vec_1[(index + 3) % 4] + c_a_width) % csize1];
							/*for (int add = 1; length_two_point2f(conn_11, conn_12) < length_m; add++)
							{
							conn_11 = cont1[(vec_1[(index + 3) % 4] + csize1 - add - c_a_width) % csize1];
							conn_12 = cont1[(vec_1[(index + 3) % 4] + add + c_a_width) % csize1];
							}*/
							conn_21 = cont2[(vec_2[(index + 1) % 4] + csize2 - c_a_width) % csize2];
							conn_22 = cont2[(vec_2[(index + 1) % 4] + c_a_width) % csize2];
							/*for (int add = 1; length_two_point2f(conn_21, conn_22) < length_m; add++)
							{
							conn_21 = cont2[(vec_2[(index + 1) % 4] + csize2 - add - c_a_width) % csize2];
							conn_22 = cont2[(vec_2[(index + 1) % 4] + add + c_a_width) % csize2];
							}*/
						}
						connect_area.push_back(conn_11);
						connect_area.push_back(conn_12);
						connect_area2.push_back(conn_21);
						connect_area2.push_back(conn_22);
					}
					if (bey == 1) break;
				}
				if (bey == 1) break;
			}

			for (int g = 0; g < connection_a.size(); g++)
			{
				draw_poly(drawing_, connection_a[g], center_p(connection_a[g]));
			}
			for (int g = 0; g < connection_a2.size(); g++)
			{
				draw_poly(drawing_, connection_a2[g], center_p(connection_a2[g]), 1);
			}

		}
	}

	Point2f center_p(vector<Point2f> contour_)
	{
		//利用轮廓的矩
		Moments mu = moments(contour_);
		return Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	}

	double contour_length(vector<Point2f> contour)
	{
		double length = 0;
		int i = 0;
		for (; i < contour.size() - 1; i++)
		{
			length += length_two_point2f(contour[i], contour[i + 1]);
		}
		length += length_two_point2f(contour[i], contour[0]);
		return length;
	}

	double length_two_point2f(Point2f &u, Point2f &v)
	{
		return sqrt((u.x - v.x)*(u.x - v.x) + (u.y - v.y)*(u.y - v.y));
	}

	double length_two_point_tar(vector<double> &p1, vector<double> &p2)
	{
		int Ts = p1.size();
		if (Ts != p2.size())
		{
			cout << "point number not equal" << endl;
			return 0;
		}

		double result = 0;
		for (int i = 0; i < Ts; i++)
		{
			result += abs(p1[i] - p2[i]);
		}
		//cout << "test: "<<result << endl;
		return result / Ts;
	}

	//double area_poly(vector<Point2f> &cont)
	//{
	//	int csize = cont.size();
	//	if (csize < 3) return 0;
	//	double sum = 0;
	//	for (int i = 0; i < csize; i++)
	//		sum += cont[i].x * cont[(i + 1) % csize].y - cont[i].y * cont[(i + 1) % csize].x;
	//	return fabs(sum / 2.0);
	//}
	void move_con(vector<Point2f> &con, Point2f sh)
	{
		int csize = con.size();
		for (int i = 0; i < csize; i++)
		{
			con[i] += sh;
		}
	}

	vector<int> feature_points(vector<Point2f> contour_, double dmin, double dmax, double angle_cos)
	{
		vector<int> index_num;
		double angle_back = 2; //顶部对应点的值
		double angle_start = 2; //起始点对应的值
		int contoursize = contour_.size();
		double arl = arcLength(contour_, true);
		dmin = dmin * arl / contoursize;
		dmax = dmax * arl / contoursize;

		for (int i = 0; i < contoursize; i++)
		{
			int k = 1;
			double length_l = length_two_point2f(contour_[i], contour_[(i + contoursize - k) % contoursize]);
			double length_r = length_two_point2f(contour_[i], contour_[(i + k) % contoursize]);
			while (length_l < dmin || length_r < dmin)
			{
				k++;
				length_l = length_two_point2f(contour_[i], contour_[(i + contoursize - k) % contoursize]);
				length_r = length_two_point2f(contour_[i], contour_[(i + k) % contoursize]);
			}
			//cout << "ok" << endl;
			double length_op = 0;
			double angle = 2;
			int f = 0;

			while (length_l < dmax && length_r < dmax)
			{
				length_op = length_two_point2f(contour_[(i + contoursize - k) % contoursize], contour_[(i + k) % contoursize]);
				double angle1 = cos_3edges(length_l, length_r, length_op);
				if (angle1 < angle_cos)  //角度大于angle_cos的度数
				{
					f = 1;
					break;
				}
				else
				{
					if (angle1 < angle) angle = angle1;    //满足条件的所有角里度数最大的那个
					k++;
					length_l = length_two_point2f(contour_[i], contour_[(i + contoursize - k) % contoursize]);
					length_r = length_two_point2f(contour_[i], contour_[(i + k) % contoursize]);
				}
			}
			if (f == 0 && angle != 2)
			{
				if (index_num.empty())
				{
					angle_start = angle;
					angle_back = angle;
					index_num.push_back(i);
				}
				else
				{
					if (length_two_point2f(contour_[index_num.back()], contour_[i]) > dmax)
					{
						angle_back = angle;
						index_num.push_back(i);
					}
					else
					{
						if (angle > angle_back)
						{
							index_num.pop_back();
							angle_back = angle;
							if (index_num.empty())
								angle_start = angle;
							index_num.push_back(i);
						}
					}
				}
			}
		}

		if (length_two_point2f(contour_[index_num.back()], contour_[index_num[0]]) < dmax)
		{
			if (angle_back > angle_start)
			{
				index_num[0] = index_num.back();
			}
			index_num.pop_back();
		}
		cout << "Feature Points num: "<< index_num.size() << endl;
		return index_num;
	}

	void bbx_center_point(vector<vector<Point2f>> all_point, vector<Point2f> &five_p)
	{
		five_p.swap(vector<Point2f>());
		vector<Point2f> contour;
		for (int i = 0; i < all_point.size(); i++)
		{
			for (int j = 0; j < all_point[i].size(); j++)
			{
				contour.push_back(all_point[i][j]);
			}
		}
		double bbx_max_x = -10000;
		double bbx_max_y = -10000;
		double bbx_min_x = 10000;
		double bbx_min_y = 10000;
		double center_x = 0;
		double center_y = 0;

		for (int i = 0; i < contour.size(); i++)
		{
			center_x += contour[i].x;
			center_y += contour[i].y;
			if (contour[i].x < bbx_min_x) bbx_min_x = contour[i].x;
			if (contour[i].x > bbx_max_x) bbx_max_x = contour[i].x;
			if (contour[i].y < bbx_min_y) bbx_min_y = contour[i].y;
			if (contour[i].y > bbx_max_y) bbx_max_y = contour[i].y;
		}
		center_x = center_x / contour.size();
		center_y = center_y / contour.size();
		five_p.push_back(Point2f(center_x, center_y));
		five_p.push_back(Point2f(bbx_min_x, bbx_max_y));
		five_p.push_back(Point2f(bbx_min_x, bbx_min_y));
		five_p.push_back(Point2f(bbx_max_x, bbx_min_y));
		five_p.push_back(Point2f(bbx_max_x, bbx_max_y));

	}

	vector<Point2f> b_box(vector<Point2f> contour)
	{
		vector<Point2f> four_cor;
		double bbx_max_x = -10000;
		double bbx_max_y = -10000;
		double bbx_min_x = 10000;
		double bbx_min_y = 10000;
		for (int i = 0; i < contour.size(); i++)
		{
			if (contour[i].x < bbx_min_x) bbx_min_x = contour[i].x;
			if (contour[i].x > bbx_max_x) bbx_max_x = contour[i].x;
			if (contour[i].y < bbx_min_y) bbx_min_y = contour[i].y;
			if (contour[i].y > bbx_max_y) bbx_max_y = contour[i].y;
		}
		four_cor.push_back(Point2f(bbx_min_x, bbx_max_y));
		four_cor.push_back(Point2f(bbx_min_x, bbx_min_y));
		four_cor.push_back(Point2f(bbx_max_x, bbx_min_y));
		four_cor.push_back(Point2f(bbx_max_x, bbx_max_y));
		return four_cor;
	}
	vector<Point2f> b_box_int(vector<Point> contour)//返回的点是从左上方逆时针
	{
		vector<Point2f> four_cor;
		double bbx_max_x = -10000;
		double bbx_max_y = -10000;
		double bbx_min_x = 10000;
		double bbx_min_y = 10000;
		for (int i = 0; i < contour.size(); i++)
		{
			if (contour[i].x < bbx_min_x) bbx_min_x = contour[i].x;
			if (contour[i].x > bbx_max_x) bbx_max_x = contour[i].x;
			if (contour[i].y < bbx_min_y) bbx_min_y = contour[i].y;
			if (contour[i].y > bbx_max_y) bbx_max_y = contour[i].y;
		}
		four_cor.push_back(Point2f(bbx_min_x, bbx_max_y));
		four_cor.push_back(Point2f(bbx_min_x, bbx_min_y));
		four_cor.push_back(Point2f(bbx_max_x, bbx_min_y));
		four_cor.push_back(Point2f(bbx_max_x, bbx_max_y));
		return four_cor;
	}

	QRectF Qbbox(vector<Point2f> contour)
	{
		double bbx_max_x = -10000;
		double bbx_max_y = -10000;
		double bbx_min_x = 10000;
		double bbx_min_y = 10000;
		for (int i = 0; i < contour.size(); i++)
		{
			if (contour[i].x < bbx_min_x) bbx_min_x = contour[i].x;
			if (contour[i].x > bbx_max_x) bbx_max_x = contour[i].x;
			if (contour[i].y < bbx_min_y) bbx_min_y = contour[i].y;
			if (contour[i].y > bbx_max_y) bbx_max_y = contour[i].y;
		}
		double width = bbx_max_x - bbx_min_x;
		double height = bbx_max_y - bbx_min_y;
		QRectF rect = QRectF(bbx_min_x, bbx_min_y, width, height);
		return rect;
	}

	vector<Point2f> sampling(vector<Point2f> &contour_, int points_num)
	{
		double length = contour_length(contour_);
		int sam_num = points_num * 100;
		double Lambda = length / sam_num;

		vector<Point2f> contour_sam;
		Point2f sample;

		contour_sam.push_back(contour_[0]);
		sample = contour_[0];
		int csize = contour_.size();
		for (int t = 1; t <= csize; t++)
		{
			double length_ = length_two_point2f(sample, contour_[t%csize]);
			if (length_ > Lambda)
			{
				Point2f vec = unit_vec(contour_[t%csize] - sample);
				sample = sample + Lambda * vec;
				contour_sam.push_back(sample);
				t = t - 1;
			}
			else if (t < contour_.size())
			{
				while ((length_ + length_two_point2f(contour_[t], contour_[(t + 1) % csize])) < Lambda)
				{
					length_ = length_ + length_two_point2f(contour_[t], contour_[(t + 1) % csize]);
					t++;
					if (t > (contour_.size() - 1)) break;
				}
				if (t >(contour_.size() - 1)) break;
				Point2f vec = unit_vec(contour_[(t + 1) % csize] - contour_[t]);
				sample = contour_[t] + (Lambda - length_) * vec;
				contour_sam.push_back(sample);
			}
		}
		if (length_two_point2f(contour_sam[0], contour_sam[contour_sam.size() - 1])<1) contour_sam.pop_back();

		return contour_sam;
	}

	vector<Point2f> flip_only_coord(vector<Point2f> cont_s, int flag)
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
		}
		else if (flag == 1)
		{
			for (int i = 0; i < cont_size; i++)
			{
				cont_s[i].y = 2 * ccen.y - cont_s[i].y;
			}
		}
		return cont_s;
	}

	int line_intersection(Line_Seg line1, Line_Seg line2, Point2f &cross_p)
	{
		Point2f start1 = line1.start;
		Point2f end1 = line1.end;
		Point2f start2 = line2.start;
		Point2f end2 = line2.end;
		Point2f s10 = end1 - start1;
		Point2f s32 = end2 - start2;
		Point2f s02 = start1 - start2;
		float s_numer, t_numer, denom, t;
		denom = s10.x * s32.y - s32.x * s10.y;
		s_numer = s10.x * s02.y - s10.y * s02.x;
		t_numer = s32.x * s02.y - s32.y * s02.x;

		if (denom == 0)//平行或共线
		{
			if (s_numer == 0)//Collinear,返回离end1最近的点
			{
				double dis1 = sqrt((start2.x - end1.x)*(start2.x - end1.x) + (start2.y - end1.y)*(start2.y - end1.y));
				double dis2 = sqrt((end2.x - end1.x)*(end2.x - end1.x) + (end2.y - end1.y)*(end2.y - end1.y));
				if (dis1 > dis2)
				{
					cross_p = end2;
				}
				else{
					cross_p = start2;
				}
				return 2;
			}
			else return 0; // parallel
			cout << "denom == 0" << endl;
		}
		bool denomPositive = denom > 0;
		if ((s_numer < 0) == denomPositive)//参数是大于等于0且小于等于1的，分子分母必须同号且分子小于等于分母
			return 0; // No collision


		if ((t_numer < 0) == denomPositive)
			return 0; // No collision

		if (fabs(s_numer) > fabs(denom) || fabs(t_numer) > fabs(denom))
			return 0; // No collision
		// Collision detected
		t = t_numer / denom;
		//cout << "t:" << t << endl;
		cross_p.x = start1.x + t * s10.x;
		cross_p.y = start1.y + t * s10.y;
		return 1;
	}
	vector<Point2f> line_polygon(Line_Seg line1, vector<Point2f> contour)
	{
		vector<Point2f> all_inter;
		int contsize = contour.size();
		for (int i = 0; i < contsize; i++)
		{
			Point2f cen;
			Line_Seg line2(contour[i], contour[(i + 1) % contsize]);
			int f = line_intersection(line1, line2, cen);
			if (f == 1)
			{
				if (all_inter.empty() || length_two_point2f(all_inter.back(), cen)>0.01)
					all_inter.push_back(cen);
			}
			else if (f == 2) all_inter.push_back(0.5 * (line2.start + line2.end));
		}
		cout << "intersize :  " << all_inter.size()
			<< all_inter[0] << " " << all_inter[1] << endl;
		return all_inter;
	}

	vector<Point2f> poly_poly(vector<Point2f> contour, vector<Point2f> contour_)
	{
		vector<Point2f> all_inter;
		int contsize = contour.size();
		int consize2 = contour_.size();
		for (int i = 0; i < contsize; i++)
		{
			Point2f cen;
			Line_Seg line(contour[i], contour[(i + 1) % contsize]);
			vector<Point2f> interp = line_polygon(line, contour_);
			if (!interp.empty())
			{
				for (int j = 0; j < interp.size(); j++)
				{
					int f = 0;
					for (int t = 0; t < all_inter.size(); t++)
					{
						if (length_two_point2f(all_inter[t], interp[j]) < 0.01)
							f = 1;
					}
					if (f == 0)
					{
						all_inter.push_back(interp[j]);
					}
				}
			}
		}
		return all_inter;

	}


	bool self_intersect(vector<Point2f> &contour_, int &first, int &second)
	{
		int sizec = contour_.size();
		for (int i = 0; i < sizec - 2; i++)
		{
			Line_Seg line1(contour_[i], contour_[i + 1]);
			if (i == 0)
			{
				for (int j = 2; j < sizec - 1; j++)
				{
					Point2f crossp;
					Line_Seg line2(contour_[j], contour_[j + 1]);
					if (line_intersection(line1, line2, crossp) == 1)
					{
						first = i;
						second = j;
						return true;
					}
				}
			}
			else{
				for (int j = i + 2; j < sizec; j++)
				{
					Point2f crossp;
					Line_Seg line2(contour_[j], contour_[(j + 1) % sizec]);
					if (line_intersection(line1, line2, crossp) == 1)
					{
						first = i;
						second = j;
						return true;
					}
				}
			}
		}
		first = -1;
		second = -1;
		return false;
	}


	Point2f unit_vec(Point2f vec)
	{
		double fenmu = sqrt(vec.x*vec.x + vec.y*vec.y);
		Point2f unit = Point2f(vec.x / fenmu, vec.y / fenmu);
		return unit;
	}

	Point2f vertical_vec(Point2f vec)
	{
		Point2f vvec = Point2f(1, -vec.x / vec.y);
		return unit_vec(vvec);
	}

	double cos_3edges(double l1, double l2, double l3)
	{
		//l1,l2分别为左右两边,l3为对边
		return (l1 * l1 + l2 * l2 - l3 * l3) / (2 * l1 * l2);
	}

	double cos_two_vector(Point2f &v0, Point2f &v1)
	{
		return unit_vec(v0).x*unit_vec(v1).x + unit_vec(v0).y*unit_vec(v1).y;
	}

	double sin_two_vector(Point2f &v0, Point2f &v1)
	{
		return unit_vec(v0).x*unit_vec(v1).y - unit_vec(v0).y*unit_vec(v1).x;
	}

	double tar_2vector(Point2f &v0, Point2f &v1)
	{
		return v0.x*v1.y - v0.y*v1.x;
	}

	vector<double> curvature_com(const vector<Point2f> &contour_sam)
	{
		vector<double> eachOfcurvature;
		int c_s = contour_sam.size();
		//sin_two_vector>0 为一凸点,cos值+1.1; <0为一凹点,cos值-1.1
		//使用1.1是为了防止在无限接近0时来回加减出现近似误差
		//此处需要注意，opencv图像与正常坐标y轴相反
		//提取轮廓点在图上看是逆时针，实际在正常坐标系为顺时针，因此应用顺时针来计算凹凸
		for (int i = 0; i < c_s; i++)
		{
			double curvature = cos_two_vector(contour_sam[(i + c_s - 1) % c_s] - contour_sam[i], contour_sam[(i + 1) % c_s] - contour_sam[i]);
			if (sin_two_vector(contour_sam[(i + c_s - 1) % c_s] - contour_sam[i], contour_sam[(i + 1) % c_s] - contour_sam[i]) > 0)
				eachOfcurvature.push_back(curvature + 1.1);
			else eachOfcurvature.push_back(curvature - 1.1);
		}
		return eachOfcurvature;
	}

	vector<double> recover_consin(const vector<double> &former)
	{
		vector<double> real_cos;
		int size_f = former.size();
		for (int i = 0; i < size_f; i++)
		{
			if (former[i] <= 0) real_cos.push_back(former[i] + 1.1);
			else real_cos.push_back(former[i] - 1.1);
		}
		return real_cos;
	}


	vector<int> most_convex_p(vector<Point2f> contour_, vector<double> cont_c, int max_cur_num)
	{
		vector<int> index_num;
		int contoursize = contour_.size();
		for (int i = 0; i < contoursize; i++)
		{
			index_num.push_back(i);
		}
		vector<double> cont_c1 = recover_consin(cont_c);//确定是否加上凹点
		//sort_comb(cont_c1, index_num);
		sort_comb(cont_c1, index_num);
		//for (int i = 0; i < 50; i++)
		//	cout << cont_c[i] << endl;

		vector<int> cand_points_index;
		int t = 1;
		cand_points_index.push_back(index_num[0]);
		double length = contour_length(contour_);

		for (int i = 1; i < contoursize; i++)
		{
			if (t >= max_cur_num) break;
			else
			{
				int flag = 0;
				for (vector<int>::iterator it = cand_points_index.begin(); it != cand_points_index.end(); it++)
				{
					double leng = length_two_point2f(contour_[index_num[i]], contour_[*it]);
					if (leng < 0.008*length)
					{
						flag = 1;
						break;
					}
				}

				if (flag == 0)
				{
					cand_points_index.push_back(index_num[i]);
					t++;
				}
			}
		}
		if (cand_points_index.size()<max_cur_num)
			cout << "cand_points_index: " << cand_points_index.size() << "  <max_cur_num" << endl;
		return cand_points_index;
	}

	void sort_comb(vector<double> vect, vector<int> &index_num) //将下标和数值联合排序，只保留下标的排序,从大到小
	{
		int i, j;
		double temp;
		int num;
		for (i = 0; i < vect.size() - 1; i++)
			for (j = 0; j < vect.size() - 1 - i; j++)
				if (vect[j] < vect[j + 1])
				{
					temp = vect[j];
					vect[j] = vect[j + 1];
					vect[j + 1] = temp;
					num = index_num[j];
					index_num[j] = index_num[j + 1];
					index_num[j + 1] = num;
				}
	}

	void fileout(string filepath, vector<Point> contour_)
	{
		ofstream out(filepath);
		if (out.is_open())
		{
			out << contour_.size() << endl;//contours[0].size()
			for (int j = 0; j < contour_.size(); j++)
				out << contour_[j].x << "," << contour_[j].y << endl;
			//out << contour_[0].x << "," << contour_[0].y << endl;  //首尾连起来
		}
		cout << "Output contour size: " << contour_.size() << endl;
		out.close();
	}

	string int2string(int number)
	{
		char ch[8];
		int  divisor = 10000;
		int index = 0;
		if (number == 0)
		{
			ch[index] = '0';
			ch[index + 1] = '\0';
			return ch;
		}
		while (divisor != 0)
		{
			if (number / divisor == 0)
			{
				divisor = divisor / 10;
				if (index != 0)
				{
					ch[index] = '0';
					index++;
				}

			}
			else
			{
				ch[index] = number / divisor + '0';
				index++;
				number = number % divisor;
				divisor = divisor / 10;
			}
		}
		ch[index] = '\0';
		return ch;
	}

	string double2string(double number)
	{
		char ch[8];
		int mid = number * 1000;
		string result = int2string(mid);
		int length = result.length();
		int i = 0;
		int t = 0;
		while (i < length - 3)
		{
			ch[i++] = result[t++];
		}
		ch[i++] = '.';
		while (i <= length)
		{
			ch[i++] = result[t++];
		}
		ch[i] = '\0';
		return ch;
	}

	QVector<QPointF> vecp_cv2qt(vector<Point2f> contour)
	{
		QVector<QPointF> Qcontour;
		int size = contour.size();
		for (int i = 0; i < size; i++)
		{
			Qcontour.push_back(QPointF(contour[i].x, contour[i].y));
		}
		return Qcontour;
	}

	vector<Point2f> vecp_qt2cv(QVector<QPointF> Qcontour)
	{
		vector<Point2f> contour;
		int size = Qcontour.size();
		for (int i = 0; i < size; i++)
		{
			contour.push_back(Point2f(Qcontour[i].x(), Qcontour[i].y()));
		}
		return contour;
	}
	
}