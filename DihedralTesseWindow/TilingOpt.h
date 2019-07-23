#ifndef TILINGOPT_H
#define TILINGOPT_H

#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <ctime>
#include <io.h>
#include <list>
#include <direct.h>
#include <QString>
#include <QVector>
#include <QPolygon>

using namespace cv;
using namespace std;


namespace Tiling_tiles{

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

	class PolygonTile{
	public:
		PolygonTile();
		PolygonTile(string filename);
		PolygonTile(const QVector<QPointF> &v);
		//void Pro_clear();

		//input
		void imgtocout(string imagepath, int raw = 0); //0.直接计算 1.先处理
		vector<Point2f> readTxt(string filename);


		//draw polygon
		void drawPolygon();
		/*void loadTileData(string tile_data);
		vector<Point2f> readTxt();
		void loadPoints(vector<Point2f> con_point);*/

		//void getpath();
		
		//void contour_sam_cur();
		//vector<vector<double>> compute_TAR(vector<Point2f> &contour_, double &shape_complexity, double frac = 0.25);

		//vector<int> cand_tiling_v(int max_cur_num);       //求轮廓上值最大的10个不临近的凸点
		//vector<int> partition_points(string imaname);  //求得用做划分的点


		//flipping
		//vector<Point2f> flip_contour(vector<Point2f> cont_s, int flag = 0);
	public:
		QPolygonF poly;
		QVector<QPointF> Qcontour;
		vector<Point2f> contour;
		Point2f poly_c;
		string imagepath;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
		string txtpath;
		//vector<double> cconvex;
		vector<vector<Point2f>> contour_sample;
		vector<vector<Point2f>> contour_sample_flip;
		//vector<vector<double>> contour_curva;
		//vector<vector<double>> contour_curva_flip;
		//char cur_string[6][600];
		//vector<vector<Point2f>> contour_saliency;  //显著性可选
		//vector<vector<Point2f>> contour_saliency_inver;
		double l_perimeter;
	};

	//class Tiling_opt{
	//public:
	//	Tiling_opt();
	//	~Tiling_opt();
	//	void Tiling_clear();

	//	//load dataset
	//	void load_dataset();

	//};


	void fileout(string filepath, vector<Point> contour_);
	Point2f center_p(vector<Point2f> contour_);
	vector<Point2f> vecp_qt2cv(QVector<QPointF>);
	QVector<QPointF> vecp_cv2qt(vector<Point2f>);
	
	//draw tool
	void draw_poly(Mat &drawing_, vector<Point2f> contour_s, Point2f center, int color = -1);

	//bbx
	QRectF b_box(vector<Point2f> contour);
	
}


#endif