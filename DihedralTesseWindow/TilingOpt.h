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

	typedef struct innerPat_pare
	{
		int type; //0:trans,1:rota,2:flip(13),3:flip(24)
		vector<int> in_interval;
		vector<Point2f> in_contour;
	}inPat;

	class PolygonTile{
	public:
		PolygonTile();
		PolygonTile(string filename);
		PolygonTile(const vector<Point2f> &v);
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

		//compute TAR
		vector<vector<double>> compute_TAR(vector<Point2f> &contour_, double &shape_complexity, double frac = 0.25);

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
		int tiliing_generation(); //1 successd 0 failed

		void com_all_TARs(int num_c);

		//tiling rules
		int Rotation_rule(vector<int> part_points_index, vector<Point2f> &contour_s, string rootname);
		int Tanslation_rule(vector<int> part_points_index, vector<Point2f> &contour_s, string rootname);
		int Flipping_rule(vector<int> part_points_index, vector<Point2f> &contour_s, string rootname);

		//rotation
		bool rotation_placement(vector<int> results, vector<Point2f> &contour_s, vector<Point2f> &return_B, vector<int> &return_p, Mat &countname);
		vector<vector<int>> find_rota_tilingV(vector<Point2f> cont, vector<int> mark_13);

		//translation
		bool translation_placement(vector<int> results, vector<Point2f> &contour_s, vector<Point2f> &return_B, vector<int> &return_p, Mat &countname);

		//flipping
		bool flipping_placement(vector<int> results, vector<Point2f> &contour_s, vector<Point2f> &return_B, vector<int> &return_p, Mat &countname, int type);

		vector<Point2f> extract_contour(vector<Point2f> contour_, vector<int> mark_p, vector<int> &midmark_p, vector<vector<Point2f>> &four_place, int type);

		//collision		
		bool coll_detec_bbx(vector<Point2f> contour1, vector<Point2f> contour2, int threshold);
		bool vertex_angle(vector<Point2f> angle1, vector<Point2f> angle2);

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
		vector<double> all_shape_complexity;
		vector<inPat> all_inner_conts;
		vector<Mat> all_tiling_Mat;


	};
}


#endif