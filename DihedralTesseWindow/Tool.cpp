#include<stdio.h>
#include<math.h>
#include "TilingOpt.h"

namespace Tiling_tiles{

	vector<pair<string, Scalar>> colorbar = { { "white", Scalar(255, 255, 255) },
	                                 { "gray", Scalar(194, 194, 194) }, 
									 { "blue1", Scalar(251, 228, 169) }, 
									 { "blue2", Scalar(251, 204, 176) },
									 { "green1", Scalar(130, 174, 89) }, 
									 { "green2", Scalar(222, 250, 167) }, 
									 { "red1", Scalar(127, 110, 174) },
									 { "orange1", Scalar(70, 124, 217) }, 
									 { "blue3", Scalar(251, 181, 105) }, 
									 { "pink", Scalar(237, 171, 245) },
									 { "orange2", Scalar(3, 142, 249)}, 
									 { "lightorange2", Scalar(175, 211, 249) }, 
									 { "lightpink", Scalar(244, 211, 247) }};
	

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


	Point2f center_p(vector<Point2f> contour_)
	{
		Moments mu = moments(contour_);
		return Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
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

	void draw_poly(Mat &drawing_, vector<Point2f> contour_s, Point2f center, int color)
	{
		Scalar col_sca = Scalar(0, 0, 0);
		if (color <= 12 && color >= 0)
		{
			col_sca = colorbar[color].second;
		}
		QRectF bbx = b_box(contour_s);
		Point2f bbx_c = Point2f(bbx.x() + bbx.width() / 2, bbx.y() + bbx.height() / 2);
		Point2f shift = center - bbx_c;
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
			col_sca
			//Scalar(255, 255, 255) //白色
			);
		//circle(drawing_, contour_s[0] + shift, 4, Scalar(255), 3);
		//circle(drawing_, center, 4, Scalar(0, 255, 255), -1);
	}

	QRectF b_box(vector<Point2f> contour)
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
		QRectF rect = QRectF(bbx_min_x, bbx_min_y,width,height);
		return rect;
	}
}