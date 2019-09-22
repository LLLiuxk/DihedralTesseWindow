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
#include "Tool.h"


using namespace cv;
using namespace std;


namespace Tiling_tiles{

	class PolygonTile{
	public:
		PolygonTile();
		PolygonTile(string filename);
		PolygonTile(const QVector<QPointF> &v);
		void poly_clear();

		//input
		void setpath();
		void imgtocout(string imagepath, int raw = 0); //0.直接计算 1.先处理
		vector<Point2f> readTxt(string filename);
		void loadTile(int allpath,string filename);
		void loadPoints(vector<Point2f> con_point);

		void contour_sampling();

		//candidate tiling vertices
		vector<int> cand_tiling_v(int max_cur_num);  //输入最大的n个点  

		//partition
		vector<int> partition_points();  //求得用做划分的点

		//flipping
		vector<Point2f> flip_contour(vector<Point2f> cont_s, int flag = 0);//0水平翻转

		//draw polygon
		void drawPolygon();
		/*void loadTileData(string tile_data);
		vector<Point2f> readTxt();
		void loadPoints(vector<Point2f> con_point);*/

		//void getpath();
		

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

		double c_length;
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

	class Tiling_opt{
	public:
		Tiling_opt();
		~Tiling_opt();
		void Tiling_clear();

		void initial(string file);
		//load dataset
		void load_dataset();
		void tiliing_generation();

		void com_all_TARs(int num_c);

	public:
		PolygonTile *poly_first;
		PolygonTile *poly_mid;
		PolygonTile *poly_second;
		PolygonTile *poly_tem;

		int all_types;
		vector<vector<Point2f>> contour_dataset;
		vector<vector<vector<double>>> all_con_tars;
		vector<vector<vector<double>>> all_con_tars_flip;
		vector<vector<vector<double>>> all_fea_tars;
		vector<vector<vector<double>>> all_fea_tars_flip;

	};
}


#endif