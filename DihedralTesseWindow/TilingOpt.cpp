#include "TilingOpt.h"

namespace Tiling_tiles{

	Tiling_opt::Tiling_opt()
	{
		poly_first = new PolygonTile();
		poly_mid = new PolygonTile();
		poly_second = new PolygonTile();
		poly_tem = new PolygonTile();
		all_types = 500;
		//memset(dp, 0, sizeof(dp));
		//memset(dp_inver, 0, sizeof(dp_inver));

	}

	Tiling_opt::~Tiling_opt()
	{
	}

	void Tiling_opt::Tiling_clear()
	{
		/*memset(dis, 0, sizeof(dis));
		memset(dis_cur, 0, sizeof(dis_cur));
		memset(distance, 0, sizeof(distance));
		memset(step, 0, sizeof(step));
		prototile_first->Pro_clear();
		prototile_mid->Pro_clear();
		prototile_second->Pro_clear();
		prototile_tem->Pro_clear();
		contour_dataset.swap(vector<vector<Point2f>>());*/
	}

	void Tiling_opt::initial(string file)
	{
		poly_first = new PolygonTile(file);
		poly_first->drawPolygon();
		//load_dataset();
	}

	void Tiling_opt::load_dataset()
	{
		if (!contour_dataset.empty()) contour_dataset.swap(vector<vector<Point2f>>());
		for (int i = 0; i <= all_types; i++)
		{		
			string image = int2string(i);
			poly_second->loadTile(0, image);
			
			//cout << image << endl;	
			if (!poly_second->contour.empty())
				contour_dataset.push_back(poly_second->contour);
		}
		cout << "load over!" << endl;
	}

	void Tiling_opt::tiliing_generation()
	{
		clock_t start, midtime, finish;
		start = clock();
		bool check_self_intersect = true;
		int num_c = 1;//选择(num_c+1)*100个点
		vector<int> p_p_index = poly_first->partition_points();
		vector<Point2f> contour_ = poly_first->contour;
		vector<Point2f> cont_orig = poly_first->contour_sample[1];
		int contsize = contour_.size();
		Point2f cent_cont = center_p(contour_);
		//vector<Point2f> contour_ = prototile_first->contour;
		load_dataset();
		com_all_TARs(num_c);
		string rootname = "D:\\VisualStudioProjects\\DihedralTessellation\\result_tuning_score_new\\" + prototile_first->contourname;
		//string rootname = "D:\\VisualStudioProjects\\DihedralTessellation\\result_tuning\\" + prototile_first->contourname;
		const char *na = rootname.c_str();
		if (_access(na, 0) != -1)

		{
			printf("The  file/dir had been Exisit ");
			return;
		}
		mkdir(na);
	}

	void Tiling_opt::com_all_TARs(int num_c)
	{
		if (!all_con_tars.empty())
		{
			cout << "the TARs have been exist" << endl;
			return;
		}
		int all_num = contour_dataset.size();
		for (int i = 0; i < all_num; i++)
		{
			poly_second->loadPoints(contour_dataset[i]);
			poly_second->contour_sampling();
			double shape_com;
			vector<vector<double>> tar_all = poly_second->compute_TAR(prototile_second->contour_sample[num_c], shape_com);//(num_c+1)*100 points
			vector<vector<double>> tar_all_flip = poly_second->compute_TAR(prototile_second->contour_sample_flip[num_c], shape_com);
			////将特征点的tar值记录下来用作快速筛选算法
			//vector<vector<double>> tar_fea;
			//vector<vector<double>> tar_fea_flip;
			//vector<int> cand_points_index = feature_points(prototile_second->contour_sample[num_c], 1, 3, cos(PI * 160 / 180));
			//for (int j = 0; j < cand_points_index.size(); j++)
			//{
			//	tar_fea.push_back(tar_all[cand_points_index[j]]);
			//}
			//all_fea_tars.push_back(tar_fea);
			//cand_points_index = feature_points(prototile_second->contour_sample_flip[num_c], 1, 3, cos(PI * 160 / 180));
			//for (int j = 0; j < cand_points_index.size(); j++)
			//{
			//	tar_fea_flip.push_back(tar_all_flip[cand_points_index[j]]);
			//}
			//all_fea_tars_flip.push_back(tar_fea_flip);

			vector<vector<double>> tar_fea;
			vector<vector<double>> tar_fea_flip;
			vector<int> cand_points_index = most_convex_p(prototile_second->contour_sample[num_c], curvature_com(prototile_second->contour_sample[num_c]), 30);
			for (int j = 0; j < cand_points_index.size(); j++)
			{
				tar_fea.push_back(tar_all[cand_points_index[j]]);
			}
			all_fea_tars.push_back(tar_fea);
			cand_points_index = most_convex_p(prototile_second->contour_sample_flip[num_c], curvature_com(prototile_second->contour_sample_flip[num_c]), 30);
			for (int j = 0; j < cand_points_index.size(); j++)
			{
				tar_fea_flip.push_back(tar_all_flip[cand_points_index[j]]);
			}
			all_fea_tars_flip.push_back(tar_fea_flip);

			all_con_tars.push_back(tar_all);
			all_con_tars_flip.push_back(tar_all_flip);
			all_shape_complexity.push_back(shape_com);


			//double shape_com1;
			//vector<vector<double>> tar_all1 = prototile_second->compute_TAR(prototile_second->contour_sample[0], shape_com1);//(num_c+1)*100 points
			//vector<vector<double>> tar_all_flip1 = prototile_second->compute_TAR(prototile_second->contour_sample_flip[0], shape_com1);
		}
		cout << "all TARs of contours have been computed" << endl;
	}


}