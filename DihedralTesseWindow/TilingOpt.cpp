#include "TilingOpt.h"

namespace Tiling_tiles{

	Tiling_opt::Tiling_opt()
	{
		poly_first = new PolygonTile();
		poly_mid = new PolygonTile();
		poly_second = new PolygonTile();
		poly_tem = new PolygonTile();
		all_types = 10;
		Tiling_index = 0;
		Candidate_index = 0;
		modify = false;
		chosen = false;
		chosen_index = 0;
		//memset(dp, 0, sizeof(dp));
		//memset(dp_inver, 0, sizeof(dp_inver));

	}

	Tiling_opt::~Tiling_opt()
	{
	}

	void Tiling_opt::Tiling_clear()
	{
		poly_first->poly_clear();
		poly_mid->poly_clear();
		poly_second->poly_clear();
		poly_tem->poly_clear();

		memset(dis, 0, sizeof(dis));
		memset(distance, 0, sizeof(distance));
		memset(step, 0, sizeof(step));

		contour_dataset.swap(vector<vector<Point2f>>());
		all_con_tars.swap(vector<vector<vector<double>>>());
		all_con_tars_flip.swap(vector<vector<vector<double>>>());
		all_fea_tars.swap(vector<vector<vector<double>>>());
		all_fea_tars_flip.swap(vector<vector<vector<double>>>());
		all_shape_complexity.swap(vector<double>());
		all_inner_conts.swap(vector<inPat>());

		Tiling_index = 0;
		Candidate_index = 0;
		all_tiling_Mat.swap(vector<Mat>());
		candidate_contours.swap(vector<vector<Point2f>>());
		cand_paths.swap(vector<vector<pair<int, int>>>());
		mid_inter.swap(vector<int>());

	}

	void Tiling_opt::initial(string file)
	{
		if (!poly_first->contour.empty())
		{
			Tiling_clear();
		}
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

	int Tiling_opt::tiliing_generation()
	{
		if (!all_inner_conts.empty()) return 2;
		int num_c = 1;//选择(num_c+1)*100个点
		if (poly_first->contour.empty())
		{
			cout << "contour is empty!"<<endl;
			return 0;
		}
		vector<int> p_p_index = poly_first->partition_points();
		vector<Point2f> contour_ = poly_first->contour;
		vector<Point2f> cont_orig = poly_first->contour_sample[1];
		int contsize = contour_.size();
		Point2f cent_cont = center_p(contour_);
		//vector<Point2f> contour_ = prototile_first->contour;
		load_dataset();
		com_all_TARs(num_c);
		string rootname = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\output";
		//string rootname = "D:\\VisualStudioProjects\\DihedralTessellation\\result_tuning\\" + prototile_first->contourname;
		const char *na = rootname.c_str();
		if (_access(na, 0) != -1)

		{
			cout<<"The  file/dir had been Exist "<<endl;
			//return 2;
		}
		//mkdir(na);

		int trans = Tanslation_rule(p_p_index, contour_, rootname);
		int rotas = 0;// Rotation_rule(p_p_index, contour_, rootname);
		int flips = 0;// Flipping_rule(p_p_index, contour_, rootname);
		int count = trans + rotas + flips;
		cout << "succeed count: " << count << " trans: " << trans << " rotat: " << rotas << " flips: " << flips << endl;
		//midtime = clock();
		//cout << "Time consumption: " << (double)(midtime - start) / CLOCKS_PER_SEC << " s " << endl;
		if (count == 0)
		{
			cout << "no right placement" << endl;
			return 3;
		}

		return 1;
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
			vector<vector<double>> tar_all = poly_second->compute_TAR(poly_second->contour_sample[num_c], shape_com);//(num_c+1)*100 points
			vector<vector<double>> tar_all_flip = poly_second->compute_TAR(poly_second->contour_sample_flip[num_c], shape_com);
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
			vector<int> cand_points_index = most_convex_p(poly_second->contour_sample[num_c], curvature_com(poly_second->contour_sample[num_c]), 30);
			for (int j = 0; j < cand_points_index.size(); j++)
			{
				tar_fea.push_back(tar_all[cand_points_index[j]]);
			}
			all_fea_tars.push_back(tar_fea);
			cand_points_index = most_convex_p(poly_second->contour_sample_flip[num_c], curvature_com(poly_second->contour_sample_flip[num_c]), 30);
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

	int Tiling_opt::Rotation_rule(vector<int> part_points_index, vector<Point2f> &contour_s, string rootname)
	{
		int rotas = 0;
		int ppindex = part_points_index.size();
		int margin = contour_s.size() / 10;
		int contsize = contour_s.size();
		Point2f cent_cont = center_p(contour_s);

		for (int i = 0; i < ppindex; i++)
		{
			for (int j = i + 1; j < ppindex; j++)
			{
				vector<Point2f> inner_contour;
				vector<int> mid_interval; //mid_interval存储的是组合成的inner 的分段连接点
				vector<int> mark_13;
				mark_13.push_back(part_points_index[i]);
				mark_13.push_back(part_points_index[j]);
				vector<vector<int>> all_result_index = find_rota_tilingV(contour_s, mark_13);
				int allresultsize = all_result_index.size();
				for (int num = 0; num < allresultsize; num++)
				{
					Mat drawing1 = Mat(800, 1600, CV_8UC3, Scalar(255, 255, 255));
					if (!rotation_placement(all_result_index[num], contour_s, inner_contour, mid_interval, drawing1))
					{
						cout << ++rotas << " Rotation succeed" << endl;
						inPat one_situation;
						one_situation.in_contour = inner_contour;
						one_situation.in_interval = mid_interval;
						one_situation.type = 1;
						all_inner_conts.push_back(one_situation);
						inner_contour.swap(vector<Point2f>());
						mid_interval.swap(vector<int>());

						Point2f shift2 = Point2f(400, 400) - cent_cont;
						for (int jj = 0; jj < contsize; jj++)
						{
							circle(drawing1, contour_s[jj] + shift2, 1, Scalar(0, 0, 0), -1);

							//MyLine(drawing4, prototile_first->contour_sample[sam_num][j] - shift1, prototile_first->contour_sample[sam_num][j + 1] - shift1, "red");
						}
						for (int jj = 0; jj < ppindex; jj++)
						{
							circle(drawing1, contour_s[part_points_index[jj]] + shift2, 4, Scalar(0, 0, 255), -1);
						}
						for (int jj = 0; jj < 4; jj++)
						{
							circle(drawing1, contour_s[all_result_index[num][jj]] + shift2, 8, Scalar(0, 255, 0), -1);
						}
						string filename = rootname + "\\" + int2string(rotas - 1) + "rota_PlacingResult.png";
						imwrite(filename, drawing1);
						Mat draw = drawing1(Rect(800, 0, 800, 800));
						all_tiling_Mat.push_back(draw);
					}
				}
			}
		}
		return rotas;

	}

	int Tiling_opt::Tanslation_rule(vector<int> part_points_index, vector<Point2f> &contour_s, string rootname)
	{
		int trans = 0;
		int ppindex = part_points_index.size();
		int margin = contour_s.size() / 10;
		int contsize = contour_s.size();
		Point2f cent_cont = center_p(contour_s);

		for (int i = 0; i < ppindex; i++)
		{
			for (int j = i + 1; j < ppindex; j++)
			{
				if (abs(part_points_index[j] - part_points_index[i]) < margin) continue;
				//cout << "i: " << p_p_index[i] << "   j: " << p_p_index[j% ppindex] << endl;
				for (int m = j + 1; m < ppindex; m++)
				{
					if (abs(part_points_index[m] - part_points_index[j]) < margin) continue;
					for (int n = m + 1; n < ppindex; n++)
					{
						//cout << i<<" "<<j<<" "<<m<<" "<<n << endl;
						if (abs(part_points_index[n] - part_points_index[m]) < margin) continue;
						vector<Point2f> inner_contour;
						vector<int> mid_interval; //mid_interval存储的是组合成的inner 的分段连接点
						vector<int> result_index; //1.translation：以下所有该类摆放都以1-3,2-4为轴摆放
						result_index.push_back(part_points_index[i]);
						result_index.push_back(part_points_index[j]);
						result_index.push_back(part_points_index[m]);
						result_index.push_back(part_points_index[n]);
						Mat drawing1 = Mat(800, 1600, CV_8UC3, Scalar(255, 255, 255));

						if (!translation_placement(result_index, contour_s, inner_contour, mid_interval, drawing1))
						{
							cout << ++trans << " Translation succeed" << endl;
							inPat one_situation;
							one_situation.in_contour = inner_contour;
							one_situation.in_interval = mid_interval;
							one_situation.type = 0;
							all_inner_conts.push_back(one_situation);

							Point2f shift2 = Point2f(400, 400) - cent_cont;
							for (int jj = 0; jj < contsize; jj++)
							{
								circle(drawing1, contour_s[jj] + shift2, 1, Scalar(0, 0, 0), -1);

								//MyLine(drawing4, prototile_first->contour_sample[sam_num][j] - shift1, prototile_first->contour_sample[sam_num][j + 1] - shift1, "red");
							}
							for (int jj = 0; jj < ppindex; jj++)
							{
								circle(drawing1, contour_s[part_points_index[jj]] + shift2, 4, Scalar(0, 0, 255), -1);
							}
							for (int jj = 0; jj < 4; jj++)
							{
								circle(drawing1, contour_s[result_index[jj]] + shift2, 8, Scalar(0, 255, 0), -1);
							}

							string filename = rootname + "\\" + int2string(trans - 1) +  "transPlacingResult.png";
							imwrite(filename, drawing1);
							Mat draw = drawing1(Rect(800, 0, 800, 800));
							all_tiling_Mat.push_back(draw);
						}					
					}
				}
			}
		}
		return trans;
	}

	int Tiling_opt::Flipping_rule(vector<int> part_points_index, vector<Point2f> &contour_s, string rootname)
	{
		int flips = 0;
		int ppindex = part_points_index.size();
		int margin = contour_s.size() / 10;
		int contsize = contour_s.size();
		Point2f cent_cont = center_p(contour_s);

		for (int i = 0; i < ppindex; i++)
		{
			for (int j = i + 1; j < ppindex; j++)
			{
				if (abs(part_points_index[j] - part_points_index[i]) < margin) continue;
				//cout << "i: " << p_p_index[i] << "   j: " << p_p_index[j% ppindex] << endl;
				for (int m = j + 1; m < ppindex; m++)
				{
					if (abs(part_points_index[m] - part_points_index[j]) < margin) continue;
					for (int n = m + 1; n < ppindex; n++)
					{
						//cout << i<<" "<<j<<" "<<m<<" "<<n << endl;
						if (abs(part_points_index[n] - part_points_index[m]) < margin) continue;
						vector<Point2f> inner_contour;
						vector<int> mid_interval; //mid_interval存储的是组合成的inner 的分段连接点
						vector<int> result_index; //1.translation：以下所有该类摆放都以1-3,2-4为轴摆放
						result_index.push_back(part_points_index[i]);
						result_index.push_back(part_points_index[j]);
						result_index.push_back(part_points_index[m]);
						result_index.push_back(part_points_index[n]);
						Mat drawing1 = Mat(800, 1600, CV_8UC3, Scalar(255, 255, 255));
						Mat drawing2 = Mat(800, 1600, CV_8UC3, Scalar(255, 255, 255));

						if (!flipping_placement(result_index, contour_s, inner_contour, mid_interval, drawing1, 0))
						{
							
							cout << ++flips << " Flipping succeed" << endl;
							inPat one_situation;
							one_situation.in_contour = inner_contour;
							one_situation.in_interval = mid_interval;
							one_situation.type = 2;
							all_inner_conts.push_back(one_situation);
							inner_contour.swap(vector<Point2f>());
							mid_interval.swap(vector<int>());

							Point2f shift2 = Point2f(400, 400) - cent_cont;
							for (int jj = 0; jj < contsize; jj++)
							{
								circle(drawing1, contour_s[jj] + shift2, 1, Scalar(0, 0, 0), -1);

								//MyLine(drawing4, prototile_first->contour_sample[sam_num][j] - shift1, prototile_first->contour_sample[sam_num][j + 1] - shift1, "red");
							}
							for (int jj = 0; jj < ppindex; jj++)
							{
								circle(drawing1, contour_s[part_points_index[jj]] + shift2, 4, Scalar(0, 0, 255), -1);
							}
							for (int jj = 0; jj < 4; jj++)
							{
								circle(drawing1, contour_s[result_index[jj]] + shift2, 8, Scalar(0, 255, 0), -1);
							}

							string filename = rootname + "\\" + int2string(flips - 1) + "flip(1-3)PlacingResult.png";
							imwrite(filename, drawing1);
							Mat draw = drawing1(Rect(800, 0, 800, 800));
							all_tiling_Mat.push_back(draw);
						}
						if (!flipping_placement(result_index, contour_s, inner_contour, mid_interval, drawing2, 1))
						{
							cout << ++flips << " Flipping succeed" << endl;
							inPat one_situation;
							one_situation.in_contour = inner_contour;
							one_situation.in_interval = mid_interval;
							one_situation.type = 3;
							all_inner_conts.push_back(one_situation);

							Point2f shift2 = Point2f(400, 400) - cent_cont;
							for (int jj = 0; jj < contsize; jj++)
							{
								circle(drawing2, contour_s[jj] + shift2, 1, Scalar(0, 0, 0), -1);
								//MyLine(drawing4, prototile_first->contour_sample[sam_num][j] - shift1, prototile_first->contour_sample[sam_num][j + 1] - shift1, "red");
							}
							for (int jj = 0; jj < ppindex; jj++)
							{
								circle(drawing2, contour_s[part_points_index[jj]] + shift2, 4, Scalar(0, 0, 255), -1);
							}
							for (int jj = 0; jj < 4; jj++)
							{
								circle(drawing2, contour_s[result_index[jj]] + shift2, 8, Scalar(0, 255, 0), -1);
							}

							string filename = rootname + "\\" + int2string(flips - 1) + "flip(2-4)PlacingResult.png";
							imwrite(filename, drawing2);
							Mat draw = drawing2(Rect(800, 0, 800, 800));
							all_tiling_Mat.push_back(draw);
						}
					}
				}
			}
		}
		return flips;
	}

	bool Tiling_opt::rotation_placement(vector<int> results, vector<Point2f> &contour_s, vector<Point2f> &return_B, vector<int> &return_p, Mat &countname)
	{

		Line_Seg line1(contour_s[results[2]], contour_s[results[0]]);
		Line_Seg line2(contour_s[results[3]], contour_s[results[1]]);
		//cout << results[0] << " " << results[1] << " " << results[2] << " " << results[3] << endl;
		//cout << line1.start << " " << line1.end << endl << line2.start << " " << line2.end << endl;
		Point2f cross_p;
		if (line_intersection(line1, line2, cross_p) != 1) return true;   //rotation 里
		if (abs(length_two_point2f(contour_s[results[2]], cross_p) - length_two_point2f(contour_s[results[0]], cross_p)) > 5) return true;
		if (abs(length_two_point2f(contour_s[results[3]], cross_p) - length_two_point2f(contour_s[results[1]], cross_p)) > 5) return true;
		int csize = contour_s.size();
		// 提取旋转围成的轮廓
		vector<vector<Point2f>> four_place;
		vector<Point2f> one_loca = contour_s;
		four_place.push_back(one_loca);

		for (int i = 0; i < 3; i++)
		{
			Point2f rota_cent = one_loca[results[i]];
			Mat rot_mat = getRotationMatrix2D(rota_cent, 180, 1);
			transform(one_loca, one_loca, rot_mat);
			four_place.push_back(one_loca);                                                                       //rotation的提取规律为1的1-4，4的4-3,3的3-2,2的2-1
		}
		vector<Point2f> a[4];                                                                                     //               ②    ③            共有①②③④四个图案，
		for (int i = 0; i < 4; i++)                                                                               //               /\    /\			   对于旋转的情况来说，接触点的角为同一个角，
		{                                                                                                         //              /3 \  /1 \           因此只需检测①和②中的顶点3，以此类推
			if (i == 0)                                                                                           //             /    \/    \ 
			{                                                                                                     //            4\   2/\2   /4
				for (int j = 0; j < 4; j++)                                                                       //              \  /  \  /
				{																								  //              1\/    \/3
					a[j].push_back(four_place[0][results[j]]);                                                    //               /\    /\						                                                                      
				}	                                                                                              //              /1 \  /3 \  
			}		                                                                                                  //             /    \/    \          为保证在一定范围内可靠，
			else                                                                                                  //            2\   4/\4  2/          比较时计算∠Pt-iPtPt+i的角度，
			{	                                                                                                  //              \  /  \  /           i从1取到3
				for (int j = 0; j < 4; j++)                                                                       //              3\/    \/1          
				{                                                                                                 //               ①    ④
					a[j].push_back(four_place[0][(results[j] - i + csize) % csize]);
					a[j].push_back(four_place[0][(results[j] + i) % csize]);
				}
			}
		}

		if (vertex_angle(a[0], a[0]) || vertex_angle(a[1], a[1]) || vertex_angle(a[2], a[2]) || vertex_angle(a[3], a[3])) return true; //到这里用时0.009s

		for (int i = 0; i < 4; i++)
		{
			for (int j = i + 1; j < 4; j++)
			{
				if (coll_detec_bbx(four_place[i], four_place[j], 10))
					return true;
			}
		}
		return_B = extract_contour(contour_s, results, return_p, four_place, 1);
		//visual presentation
		vector<Point2f> all_cent;
		Point2f allcenter = Point2f(0, 0);
		for (int i = 0; i < 4; i++)
		{
			Point2f t = center_p(four_place[i]);
			all_cent.push_back(t);
			allcenter += t;
		}
		Point2f shift = Point2f(1200, 400) - 0.1*allcenter;
		for (int i = 0; i < 4; i++)
		{
			vector<Point2f> one_;
			for (int j = 0; j < four_place[i].size(); j++)
			{
				Point2f p = four_place[i][j] * 0.4 + shift;
				one_.push_back(p);
			}
			draw_poly(countname, one_, all_cent[i] * 0.4 + shift);
		}
		return false;
	}

	vector<vector<int>> Tiling_opt::find_rota_tilingV(vector<Point2f> cont, vector<int> mark_13)
	{
		vector<vector<int>> all_result_index;
		int contsize = cont.size();
		double margin = arcLength(cont, 1) / contsize;
		Point2f center = 0.5 * cont[mark_13[0]] + 0.5 * cont[mark_13[1]];
		for (int i = mark_13[0] + 1; i < mark_13[1]; i++)
		{
			double length1 = length_two_point2f(center, cont[i]);
			double mincos = -0.996; //cos(175)=-0.996194
			int minnum = -1;
			for (int j = mark_13[1] + 1; j < mark_13[0] + contsize; j++)
			{
				int t = j % contsize;
				double length2 = length_two_point2f(center, cont[t]);
				//double length3 = length_two_point2f(cont[i], cont[j]);
				if (abs(length1 - length2) < margin)
				{
					double cos = cos_two_vector(cont[i] - center, cont[t] - center);
					if (cos < mincos)
					{
						mincos = cos;
						minnum = t;
					}
				}
			}
			if (minnum != -1)
			{
				vector<int> one;
				one.push_back(mark_13[0]);
				one.push_back(i);
				one.push_back(mark_13[1]);
				one.push_back(minnum);
				all_result_index.push_back(one);
			}
		}
		return all_result_index;
	}

	bool Tiling_opt::translation_placement(vector<int> results, vector<Point2f> &contour_s, vector<Point2f> &return_B, vector<int> &return_p, Mat &countname)
	{
		int csize = contour_s.size();
		Point2f line1 = contour_s[results[2]] - contour_s[results[0]];
		Point2f line2 = contour_s[results[3]] - contour_s[results[1]];
		vector<Point2f> shifting;
		shifting.push_back(line1);
		shifting.push_back(line2);
		shifting.push_back(line1 + line2);
		// 提取平移围成的轮廓
		vector<vector<Point2f>> four_place;
		vector<Point2f> one_loca;
		four_place.push_back(contour_s);
		for (int i = 0; i < 3; i++)
		{
			Point2f move = shifting[i];
			for (int j = 0; j < csize; j++)
			{
				one_loca.push_back(contour_s[j] + move);
			}
			four_place.push_back(one_loca);
			one_loca.swap(vector<Point2f>());
		}

		vector<Point2f> a[2];
		vector<Point2f> b[2];
		for (int i = 0; i < 4; i++)
		{                                                                                                     //translation的提取规律为1的4-3,2的1-4,4的2-1,3的3-2
			if (i == 0)
			{																								  //              ①     ③            共有①②③④四个图案，
				a[0].push_back(four_place[0][results[2]]);                                                    //               /\    /\			   检测①中的顶点3与②中顶点1，	
				a[1].push_back(four_place[0][results[3]]);                                                    //              /1 \  /1 \           以及①中顶点4与③中顶点2的角度之和
				b[0].push_back(four_place[1][results[0]]);                                                    //             /    \/    \         
				b[1].push_back(four_place[2][results[1]]);                                                    //            2\   4/\2   /4
			}                                                                                                 //              \  /  \  /
			else                                                                                              //              3\/    \/3
			{                                                                                                 //               /\    /\     
				a[0].push_back(four_place[0][(results[2] - i + csize) % csize]);                              //              /1 \  /1 \ 
				a[1].push_back(four_place[0][(results[3] - i + csize) % csize]);                              //             /    \/    \          为保证在一定范围内可靠，
				a[0].push_back(four_place[0][(results[2] + i) % csize]);                                      //            2\   4/\2  4/          比较时计算∠Pt-iPtPt+i的角度，
				a[1].push_back(four_place[0][(results[3] + i) % csize]);                                      //              \  /  \  /           i从1取到3
				b[0].push_back(four_place[1][(results[0] - i + csize) % csize]);                              //              3\/    \/3
				b[1].push_back(four_place[2][(results[1] - i + csize) % csize]);                              //               ②    ④
				b[0].push_back(four_place[1][(results[0] + i) % csize]);
				b[1].push_back(four_place[2][(results[1] + i) % csize]);
			}
		}

		if (vertex_angle(a[0], b[0]) || vertex_angle(a[1], b[1])) return true;  //到这里用时0.009s
		if (coll_detec_bbx(four_place[0], four_place[1], 10) || coll_detec_bbx(four_place[0], four_place[2], 10) || coll_detec_bbx(four_place[0], four_place[3], 0) || coll_detec_bbx(four_place[1], four_place[2], 0))
			return true;
		return_B = extract_contour(contour_s, results, return_p, four_place, 0);
		//visual presentation
		Point2f shift1 = Point2f(1200, 400) - (0.4*center_p(contour_s) + 0.2*line1 + 0.2*line2);
		for (int i = 0; i < 4; i++)
		{
			vector<Point2f> one_;
			for (int j = 0; j < four_place[i].size(); j++)
			{
				Point2f p = four_place[i][j] * 0.4 + shift1;
				one_.push_back(p);
			}
			draw_poly(countname, one_, center_p(one_));
		}

		return false;
	}

	bool Tiling_opt::flipping_placement(vector<int> results, vector<Point2f> &contour_s, vector<Point2f> &return_B, vector<int> &return_p, Mat &countname, int type)
	{
		//0:以1-3对应的中线为轴翻转 1:以2-4为轴 
		//不一定需要以1-3为轴，只要1,3两点到对称轴的距离相同
		//因此第一步是根据1,3两点确定翻转轴
		int csize = contour_s.size();
		Point2f cent = center_p(contour_s);
		Point2f line1 = contour_s[results[2]] - contour_s[results[0]];
		Point2f line2 = contour_s[results[3]] - contour_s[results[1]];
		//if (abs(cos_two_vector(line1, line2))>0.06) return true;
		//cout << line1 << endl << line2 << endl << cos_two_vector(line1, line2) << endl;
		vector<vector<Point2f>> four_place;
		four_place.push_back(contour_s);
		vector<Point2f> one_loca;
		vector<Point2f> one_loca_;
		vector<Point2f> one_loca_f;
		// 提取翻转围成的轮廓
		if (type == 0)
		{
			//实际的对称轴应该是过1，3中点且与2，4垂直的直线
			Point2f line1_cent = 0.5 * contour_s[results[2]] + 0.5 * contour_s[results[0]];
			Point2f s_axis(line2.y, -line2.x);
			Line_Seg symmetry_axis(line1_cent + 5 * s_axis, line1_cent - 5 * s_axis);
			//计算翻转后的轮廓，因为直接任意轴对称翻转比较耗时，因此先以x轴为对称轴翻转，然后旋转相应角度
			double rota_angle = acos(cos_two_vector(Point2f(0, 1), s_axis)) / PI * 180;
			if (sin_two_vector(Point2f(0, 1), s_axis) > 0) rota_angle = -rota_angle;
			//cout << rota_angle << endl;
			Point2f cross;
			if (line_intersection(Line_Seg(symmetry_axis), Line_Seg(cent - 5 * line2, cent + 5 * line2), cross) != 1) return true;  //没有交点说明出错了
			Point2f cent_shift = 2 * (cross - cent);
			one_loca = flip_only_coord(contour_s);
			Mat rot_mat = getRotationMatrix2D(cent, 2 * rota_angle, 1);
			transform(one_loca, one_loca, rot_mat);
			//将以上所得翻转的轮廓沿相对应的轴平移，计算该位置
			double cos_ax_1_3 = cos_two_vector(s_axis, line1);
			Point2f line3 = s_axis;
			if (cos_ax_1_3 < 0) line3 = -s_axis;
			//这里line3代表对称轴上与line1成锐角夹角方向的向量，长度为line1在对称轴上的投影
			line3 = line3*(abs(line1.x*line3.x + line1.y*line3.y) / (line3.x*line3.x + line3.y*line3.y));
			Point2f shift = cent_shift + line3;
			for (int i = 0; i < csize; i++)
			{
				one_loca[i] += shift;
				one_loca_.push_back(contour_s[i] + line2);
				one_loca_f.push_back(one_loca[i] + line2);
			}
			four_place.push_back(one_loca);
			four_place.push_back(one_loca_);
			four_place.push_back(one_loca_f);
		}
		else if (type == 1)
		{
			//实际的对称轴应该是过2，4中点且与1，3垂直的直线
			Point2f line2_cent = 0.5 * contour_s[results[3]] + 0.5 * contour_s[results[1]];
			Point2f s_axis(line1.y, -line1.x);
			Line_Seg symmetry_axis(line2_cent + 5 * s_axis, line2_cent - 5 * s_axis);

			double rota_angle = acos(cos_two_vector(Point2f(0, 1), s_axis)) / PI * 180;
			if (sin_two_vector(Point2f(0, 1), s_axis) > 0) rota_angle = -rota_angle;
			//cout << rota_angle << endl;
			Point2f cross;
			if (line_intersection(Line_Seg(symmetry_axis), Line_Seg(cent - 5 * line1, cent + 5 * line1), cross) != 1) return true;  //没有交点说明出错了
			Point2f cent_shift = 2 * (cross - cent);
			one_loca = flip_only_coord(contour_s);
			Mat rot_mat = getRotationMatrix2D(cent, 2 * rota_angle, 1);
			transform(one_loca, one_loca, rot_mat);

			double cos_ax_2_4 = cos_two_vector(s_axis, line2);
			Point2f line3 = s_axis;
			if (cos_ax_2_4 < 0) line3 = -s_axis;
			line3 = line3*(abs(line2.x*line3.x + line2.y*line3.y) / (line3.x*line3.x + line3.y*line3.y)); //这里line3代表对称轴上与line2成锐角夹角方向的向量，长度为line2在对称轴上的投影
			Point2f shift = cent_shift + line3;
			for (int i = 0; i < csize; i++)
			{
				one_loca[i] += shift;
				one_loca_.push_back(contour_s[i] + line1);
				one_loca_f.push_back(one_loca[i] + line1);
			}
			four_place.push_back(one_loca_);
			four_place.push_back(one_loca);
			four_place.push_back(one_loca_f);
		}
		//Mat draw1 = Mat(1600, 1600, CV_8UC3, Scalar(255, 255, 255));
		////for (int i = 1; i < 3; i++)
		//for (int j = 0; j < four_place[0].size(); j++)
		//{
		//	circle(draw1, four_place[0][j], 2, Scalar(255, 0, 0), -1);
		//}
		//imshow("dadas",draw1);
		//cout << four_place.size() << endl;
		vector<Point2f> a[2];                                                                                 // flippling(1-3)的提取规律为1的4-3, 2的1-2, 4的4-1, 3的3-2
		vector<Point2f> b[2];                                                                                 // flippling(2-4)的提取规律为1的4-3, 2的1-4, 4的2-3, 3的1-2
		for (int i = 0; i < 4; i++)
		{
			if (i == 0)                                                                                       //     沿1-3轴翻转        沿2-4轴翻转
			{																								  //       ①    ③          ①    ③          共有①②③④四个图案，
				a[0].push_back(four_place[0][results[2]]);                                                    //       /\    /\	         /\    /\		   检测①中的顶点3与②中顶点1，	
				a[1].push_back(four_place[0][results[3]]);                                                    //      /1 \  /1 \        /1 \  /3 \         以及①中顶点4与③中顶点2的角度之和
				b[0].push_back(four_place[1][results[0]]);                                                    //     /    \/    \      /    \/    \         
				b[1].push_back(four_place[2][results[1]]);                                                    //    2\   4/\2   /4    2\   4/\2   /4
			}                                                                                                 //      \  /  \  /        \  /  \  /
			else                                                                                              //      3\/    \/3        3\/    \/1
			{                                                                                                 //       /\    /\          /\    /\     
				a[0].push_back(four_place[0][(results[2] - i + csize) % csize]);                              //      /1 \  /1 \        /1 \  /3 \ 
				a[1].push_back(four_place[0][(results[3] - i + csize) % csize]);                              //     /    \/    \      /    \/    \        为保证在一定范围内可靠，
				a[0].push_back(four_place[0][(results[2] + i) % csize]);                                      //    4\   2/\4  2/     2\   4/\2  4/          比较时计算∠Pt-iPtPt+i的角度，
				a[1].push_back(four_place[0][(results[3] + i) % csize]);                                      //      \  /  \  /        \  /  \  /           i从1取到3
				b[0].push_back(four_place[1][(results[0] - i + csize) % csize]);                              //      3\/   3\/         3\/   1\/
				b[1].push_back(four_place[2][(results[1] - i + csize) % csize]);                              //       ②    ④          ②    ④
				b[0].push_back(four_place[1][(results[0] + i) % csize]);
				b[1].push_back(four_place[2][(results[1] + i) % csize]);
			}
		}
		if (vertex_angle(a[0], b[0]) || vertex_angle(a[1], b[1])) return true;  //到这里用时0.009s
		if (coll_detec_bbx(four_place[0], four_place[1], 10) || coll_detec_bbx(four_place[0], four_place[2], 10) || coll_detec_bbx(four_place[0], four_place[3], 0) || coll_detec_bbx(four_place[1], four_place[2], 0))
			return true;
		if (type == 0) return_B = extract_contour(contour_s, results, return_p, four_place, 2);
		if (type == 1) return_B = extract_contour(contour_s, results, return_p, four_place, 3);
		////visual presentation
		Point2f shift1 = Point2f(1200, 400) - (0.4*center_p(contour_s) + 0.2*line1 + 0.2*line2);
		for (int i = 0; i < 4; i++)
		{
			vector<Point2f> one_;
			for (int j = 0; j < four_place[i].size(); j++)
			{
				Point2f p = four_place[i][j] * 0.4 + shift1;
				one_.push_back(p);
			}
			draw_poly(countname, one_, center_p(one_));
		}
		return false;
	}

	vector<Point2f> Tiling_opt::extract_contour(vector<Point2f> contour_, vector<int> mark_p, vector<int> &midmark_p, vector<vector<Point2f>> &four_place, int type)
	{
		//  0:translation  1:rotation  2:flipping(1-3)   3:flipping(2-4)
		int csize = contour_.size();
		Point2f cent = center_p(contour_);
		vector<Point2f> morphed_B;
		Point2f line1 = contour_[mark_p[2]] - contour_[mark_p[0]];
		Point2f line2 = contour_[mark_p[3]] - contour_[mark_p[1]];
		if (type == 0)   // 提取translation围成的轮廓
		{
			if (four_place.empty())
			{
				vector<Point2f> shifting;
				shifting.push_back(line1);
				shifting.push_back(line2);
				shifting.push_back(line1 + line2);
				vector<Point2f> one_loca;
				four_place.push_back(contour_);
				for (int i = 0; i < 3; i++)
				{
					Point2f move = shifting[i];
					for (int j = 0; j < csize; j++)
					{
						one_loca.push_back(contour_[j] + move);
					}
					four_place.push_back(one_loca);
					one_loca.swap(vector<Point2f>());
				}
			}
			//cout << four_place.size() << endl;
			/*Mat drawing_ttt = Mat(1600, 1600, CV_8UC3, Scalar(255, 255, 255));
			for (int i = 0; i < 4; i++)
			for (int t = 0; t < four_place[i].size(); t++)
			{
			circle(drawing_ttt, four_place[i][t], 2, Scalar(0, 0, 255), -1);
			}*/
			int total_num = 0;
			midmark_p.push_back(0);
			for (int t = mark_p[3]; t > mark_p[2]; t--)                   //translation的提取规律为1的4-3,2的1-4,4的2-1,3的3-2
			{
				total_num++;
				morphed_B.push_back(four_place[0][t]);
				//circle(drawing_ttt, four_place[0][t], 2, Scalar(0, 255, 0), -1);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[0] + csize; t > mark_p[3]; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[1][t % csize]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[1]; t > mark_p[0]; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[3][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[2]; t > mark_p[1]; t--)
			{
				morphed_B.push_back(four_place[2][t]);
			}
		}
		else if (type == 1)   // 提取rotation围成的轮廓
		{
			if (four_place.empty())
			{
				vector<Point2f> one_loca = contour_;
				four_place.push_back(one_loca);
				for (int i = 0; i < 3; i++)
				{
					Point2f rota_cent = one_loca[mark_p[i]];
					Mat rot_mat = getRotationMatrix2D(rota_cent, 180, 1);
					cv::transform(one_loca, one_loca, rot_mat);
					four_place.push_back(one_loca);
				}
			}
			//Mat drawing_ttt = Mat(1600, 1600, CV_8UC3, Scalar(255, 255, 255));
			//for (int i = 0; i < 4; i++)
			//{
			//	//circle(drawing_ttt, four_place[0][mark_p[i]] + Point2f(300, 0), 15, Scalar(0, 124, 0), -1);
			//	for (int t = 0; t < four_place[i].size(); t++)
			//	{
			//		circle(drawing_ttt, four_place[i][t] + Point2f(300, 0), 2, Scalar(0, 0, 255), -1);
			//	}
			//}
			//circle(drawing_ttt, four_place[0][mark_p[0]] + Point2f(300, 0), 15, Scalar(255, 124, 0), -1);
			int total_num = 0;
			midmark_p.push_back(0);
			if (mark_p[0] < mark_p[3])
			{
				for (int t = mark_p[0] + csize; t > mark_p[3]; t--)                         //rotation的提取规律为1的1-4，4的4-3,3的3-2,2的2-1
				{
					total_num++;
					morphed_B.push_back(four_place[0][t % csize]);
					//circle(drawing_ttt, four_place[0][t % csize] + Point2f(300, 0), 2, Scalar(0, 255, 0), -1);
				}
				midmark_p.push_back(total_num);
				for (int t = mark_p[3]; t > mark_p[2]; t--)
				{
					total_num++;
					morphed_B.push_back(four_place[3][t]);
				}
				midmark_p.push_back(total_num);
			}
			else
			{
				for (int t = mark_p[0]; t > mark_p[3]; t--)                         //rotation的提取规律为1的1-4，4的4-3,3的3-2,2的2-1，1234顺序翻转
				{
					total_num++;
					morphed_B.push_back(four_place[0][t]);
				}
				midmark_p.push_back(total_num);
				for (int t = mark_p[3] + csize; t > mark_p[2]; t--)
				{
					total_num++;
					morphed_B.push_back(four_place[3][t % csize]);
				}
				midmark_p.push_back(total_num);
			}
			for (int t = mark_p[2]; t > mark_p[1]; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[2][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[1]; t > mark_p[0]; t--)
			{
				morphed_B.push_back(four_place[1][t]);
			}
			//imwrite("D:\\rotation.png ", drawing_ttt);
		}
		else if (type == 2)
		{
			if (four_place.empty())
			{
				four_place.push_back(contour_);
				vector<Point2f> one_loca;
				vector<Point2f> one_loca_;
				vector<Point2f> one_loca_f;
				Point2f line1_cent = 0.5 * contour_[mark_p[2]] + 0.5 * contour_[mark_p[0]];
				Point2f s_axis(line2.y, -line2.x);
				Line_Seg symmetry_axis(line1_cent + 5 * s_axis, line1_cent - 5 * s_axis);
				//计算翻转后的轮廓，因为直接任意轴对称翻转比较耗时，因此先以x轴为对称轴翻转，然后旋转相应角度
				double rota_angle = acos(cos_two_vector(Point2f(0, 1), s_axis)) / PI * 180;
				if (sin_two_vector(Point2f(0, 1), s_axis) > 0) rota_angle = -rota_angle;
				//cout << rota_angle << endl;
				Point2f cross;
				if (line_intersection(Line_Seg(symmetry_axis), Line_Seg(cent - 5 * line2, cent + 5 * line2), cross) != 1) cout << "Error!" << endl;  //没有交点说明出错了
				Point2f cent_shift = 2 * (cross - cent);
				one_loca = flip_only_coord(contour_);
				Mat rot_mat = getRotationMatrix2D(cent, 2 * rota_angle, 1);
				transform(one_loca, one_loca, rot_mat);
				//将以上所得翻转的轮廓沿相对应的轴平移，计算该位置
				double cos_ax_1_3 = cos_two_vector(s_axis, line1);
				Point2f line3 = s_axis;
				if (cos_ax_1_3 < 0) line3 = -s_axis;
				line3 = line3*(abs(line1.x*line3.x + line1.y*line3.y) / (line3.x*line3.x + line3.y*line3.y));
				Point2f shift = cent_shift + line3;
				for (int i = 0; i < csize; i++)
				{
					one_loca[i] += shift;
					one_loca_.push_back(contour_[i] + line2);
					one_loca_f.push_back(one_loca[i] + line2);
				}
				four_place.push_back(one_loca);
				four_place.push_back(one_loca_);
				four_place.push_back(one_loca_f);
			}
			int total_num = 0;
			midmark_p.push_back(0);                                                        //flippling(1-3)的提取规律为1的4-3,2的1-2,4的4-1,3的3-2		
			for (int t = mark_p[3]; t > mark_p[2]; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[0][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[0]; t < mark_p[1]; t++)
			{
				total_num++;
				morphed_B.push_back(four_place[1][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[3]; t <mark_p[0] + csize; t++)
			{
				total_num++;
				morphed_B.push_back(four_place[3][t%csize]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[2]; t > mark_p[1]; t--)
			{
				morphed_B.push_back(four_place[2][t]);
			}

		}
		else if (type == 3)
		{
			if (four_place.empty())
			{
				four_place.push_back(contour_);
				vector<Point2f> one_loca;
				vector<Point2f> one_loca_;
				vector<Point2f> one_loca_f;
				Point2f line2_cent = 0.5 * contour_[mark_p[3]] + 0.5 * contour_[mark_p[1]];
				Point2f s_axis(line1.y, -line1.x);
				Line_Seg symmetry_axis(line2_cent + 5 * s_axis, line2_cent - 5 * s_axis);

				double rota_angle = acos(cos_two_vector(Point2f(0, 1), s_axis)) / PI * 180;
				if (sin_two_vector(Point2f(0, 1), s_axis) > 0) rota_angle = -rota_angle;
				//cout << rota_angle << endl;
				Point2f cross;
				if (line_intersection(Line_Seg(symmetry_axis), Line_Seg(cent - 5 * line1, cent + 5 * line1), cross) != 1) cout << "Error!" << endl;  //没有交点说明出错了
				Point2f cent_shift = 2 * (cross - cent);
				one_loca = flip_only_coord(contour_);
				Mat rot_mat = getRotationMatrix2D(cent, 2 * rota_angle, 1);
				transform(one_loca, one_loca, rot_mat);

				double cos_ax_2_4 = cos_two_vector(s_axis, line2);
				Point2f line3 = s_axis;
				if (cos_ax_2_4 < 0) line3 = -s_axis;
				line3 = line3*(abs(line2.x*line3.x + line2.y*line3.y) / (line3.x*line3.x + line3.y*line3.y));
				Point2f shift = cent_shift + line3;
				for (int i = 0; i < csize; i++)
				{
					one_loca[i] += shift;
					one_loca_.push_back(contour_[i] + line1);
					one_loca_f.push_back(one_loca[i] + line1);
				}
				four_place.push_back(one_loca_);
				four_place.push_back(one_loca);
				four_place.push_back(one_loca_f);
			}
			int total_num = 0;
			midmark_p.push_back(0);                                                      //flippling(2-4)的提取规律为1的4-3,2的1-4,4的2-3,3的1-2
			for (int t = mark_p[3]; t > mark_p[2]; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[0][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[0]; t >= 0; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[1][t]);
			}
			for (int t = csize - 1; t > mark_p[3]; t--)
			{
				total_num++;
				morphed_B.push_back(four_place[1][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[1]; t < mark_p[2]; t++)
			{
				total_num++;
				morphed_B.push_back(four_place[3][t]);
			}
			midmark_p.push_back(total_num);
			for (int t = mark_p[0]; t < mark_p[1]; t++)
			{
				morphed_B.push_back(four_place[2][t]);
			}
		}
		/*Mat drawing_ttt = Mat(1600, 1600, CV_8UC3, Scalar(255, 255, 255));
		for (int i = 0; i < 4; i++)
		{
		draw_poly(drawing_ttt, four_place[i], center_p(four_place[i]));
		}
		for (int i = 0; i < morphed_B.size(); i++)
		MyLine(drawing_ttt, morphed_B[i], morphed_B[(i + 1) % morphed_B.size()], "green2");
		imshow("extracted contour",drawing_ttt);*/

		return morphed_B;
	}

	void Tiling_opt::match_candidate()
	{
		if (all_inner_conts.empty())
		{
			cout << "No correct middle patterns!" << endl;
			return;
		}
		int num_c = 1;//采样点数
		vector<pair<int, bool>> candidate_patterns;
		candidate_patterns = compare_choose_TAR(all_inner_conts[Tiling_index].in_contour);
		
		mid_inter = joint_relocate(all_inner_conts[Tiling_index].in_contour, all_inner_conts[Tiling_index].in_interval, num_c);

		vector<Point2f> contour_inner = poly_mid->contour_sample[1];
		double sc_inner = 0;
		vector<vector<double>> inner_tar = poly_mid->compute_TAR(contour_inner, sc_inner);
		candidate_contours.swap(vector<vector<Point2f>>());
		cand_paths.swap(vector<vector<pair<int, int>>>());

		for (int j = 0; j < 8; j++) //只要候选图案里的前8个
		{
			//将所有的结果保存下来
			poly_second->loadPoints(contour_dataset[candidate_patterns[j].first]);
			poly_second->contour_sampling();

			vector<Point2f> contour_cand;// = prototile_second->contour_sample[1];
			vector<vector<double>> cand_tar;
			if (candidate_patterns[j].second)
			{
				//cout << "it is flip" << endl;
				cand_tar = all_con_tars_flip[candidate_patterns[j].first];
				contour_cand = poly_second->contour_sample_flip[1];
			}
			else
			{
				//cout << "it is not flip" << endl;
				cand_tar = all_con_tars[candidate_patterns[j].first];
				contour_cand = poly_second->contour_sample[1];
			}
			vector<pair<int, int>> path;
			int shift = 0;
			int width = 6;
			double re = tar_mismatch(inner_tar, cand_tar, path, shift, width);
			
			vector<Point2f> contour_mid;
			int c2size = contour_cand.size();
			for (int i = shift; i < shift + c2size; i++)
			{
				contour_mid.push_back(contour_cand[i % c2size]);
			}

			cout << contour_inner.size() << "   " << c2size << "  c3: " << contour_mid.size() << endl;
			contour_cand = contour_mid;
			double scale = arcLength(contour_inner, true) / arcLength(contour_cand, true);
			//cout << "scale: " << scale << endl;
			for (int i = 0; i < contour_cand.size(); i++)
			{
				contour_cand[i] = contour_cand[i] * scale;
			}
			Point2f cen1 = center_p(contour_inner);
			Point2f cen2 = center_p(contour_cand);
			Point2f shift2 = cen1 - cen2;
			for (int i = 0; i < contour_cand.size(); i++)
			{
				contour_cand[i] = contour_cand[i] + shift2;
			}
			Point2f v0 = contour_inner[0] - cen1;
			Point2f v1 = contour_cand[0] - cen1;
			double angle = acos(cos_two_vector(v0, v1)) / PI * 180;
			if (sin_two_vector(v0, v1) < 0) angle = -angle;
			Mat rot_mat(2, 3, CV_32FC1);
			rot_mat = getRotationMatrix2D(cen1, angle, 1);
			transform(contour_cand, contour_mid, rot_mat);
			contour_cand = contour_mid;

			candidate_contours.push_back(contour_cand);
			cand_paths.push_back(path);

		}
	
	}

	vector<pair<int, bool>> Tiling_opt::compare_choose_TAR(vector<Point2f> inner_c)
	{
		int method = 1;
		int match_width = 4;
		vector<pair<int, bool>> all_total;

		cout << "poly_mid points: " << poly_mid->contour.size() << endl;
		poly_mid->loadPoints(inner_c);
		poly_mid->contour_sampling();

		if (method == 1) all_total = quick_choose_TAR(poly_mid->contour_sample[1]);
		else if (method == 2)
		{
			for (int i = 0; i < all_types; i++)
				all_total.push_back(make_pair(i, true));
		}
		int total_num = all_total.size();
		vector<pair<int, bool>> all_final;
		vector<double> all_result;	

		vector<Point2f> contour_mid = poly_mid->contour_sample[1];

		double shape_com_mid;
		vector<vector<double>> tar_mid = poly_mid->compute_TAR(contour_mid, shape_com_mid);
		//cout << "contour_mid: " << contour_mid.size() << "  tar_mid: " << tar_mid.size() << endl;
		for (int can_num = 0; can_num < total_num; can_num++)
		{
			int index = all_total[can_num].first;
			//cout << index << "  : ";
			vector<vector<double>> tar_sec = all_con_tars[index];
			vector<vector<double>> tar_sec_f = all_con_tars_flip[index];
			vector<pair<int, int>> path;
			int shift = 0;
			double re = tar_mismatch(tar_mid, tar_sec, path, shift, match_width);
			double re2 = tar_mismatch(tar_mid, tar_sec_f, path, shift, match_width);
			re = re / (1 + shape_com_mid + all_shape_complexity[index]);
			re2 = re2 / (1 + shape_com_mid + all_shape_complexity[index]);

			if (re < re2)
			{
				all_result.push_back(re);
				all_final.push_back(make_pair(index, false));
				//cout << re << endl;
			}
			else
			{
				all_result.push_back(re2);
				all_final.push_back(make_pair(index, true));
				//cout << re2 << endl;
			}
		}
		double temp;
		pair<int, bool> tempp;
		int all_size = all_result.size();
		for (int i = 0; i < all_size - 1; i++)
			for (int j = 0; j < all_size - 1 - i; j++)
				if (all_result[j] < all_result[j + 1])
				{
					temp = all_result[j];
					all_result[j] = all_result[j + 1];
					all_result[j + 1] = temp;
					tempp = all_final[j];
					all_final[j] = all_final[j + 1];
					all_final[j + 1] = tempp;
				}
		//cout << "the fianl order: " << endl;
		vector<pair<int, bool>> all_total_mid;
		for (int t = total_num - 1; t > total_num - 9; t--)
		{
			all_total_mid.push_back(all_final[t]);
			cout << "order: " << all_final[t].first << "  flip: " << all_final[t].second << " value: " << all_result[t] << " complxeity: " << all_shape_complexity[all_final[t].first] << endl;
		}
		all_final.swap(all_total_mid);

		return all_final;
	}

	vector<pair<int, bool>> Tiling_opt::quick_choose_TAR(vector<Point2f> contour_mid) //得到选择出的pattern的序号和是否翻转的标志
	{
		int match_width = 4;
		vector<pair<int, bool>> all_total; //未翻转:false,翻转:true
		vector<double> all_result;
		int total_num = contour_dataset.size();

		double shape_com_mid;
		vector<vector<double>> tar_mid = poly_mid->compute_TAR(contour_mid, shape_com_mid);
		vector<int> cand_points_index = most_convex_p(contour_mid, curvature_com(contour_mid), 30);
		//cout << "feature :" << cand_points_index.size()  << "total_num:  " << total_num<<endl;
		vector<vector<double>> tar_fea;
		for (int j = 0; j < cand_points_index.size(); j++)
		{
			tar_fea.push_back(tar_mid[cand_points_index[j]]);
		}

		//cout << "contour_mid: " << contour_mid.size() << "  tar_mid: " << tar_mid.size() << endl;
		for (int can_num = 0; can_num < total_num; can_num++)
		{
			//vector<vector<double>> tar_sec = all_con_tars[can_num];
			//vector<vector<double>> tar_sec_f = all_con_tars_flip[can_num];
			vector<vector<double>> tar_sec = all_fea_tars[can_num];
			vector<vector<double>> tar_sec_f = all_fea_tars_flip[can_num];
			//cout << "can_num: " << can_num <<  " tar_sec :" << tar_sec.size() << "   tar_sec_f: " << tar_sec_f.size() << endl;
			vector<pair<int, int>> path;
			int shift = 0;
			//double re = tar_mismatch(tar_mid, tar_sec, path, shift, match_width);
			//double re2 = tar_mismatch(tar_mid, tar_sec_f, path, shift, match_width);
			double re = tar_mismatch(tar_fea, tar_sec, path, shift, match_width);
			double re2 = tar_mismatch(tar_fea, tar_sec_f, path, shift, match_width);
			re = re / (1 + shape_com_mid + all_shape_complexity[can_num]);
			re2 = re2 / (1 + shape_com_mid + all_shape_complexity[can_num]);

			if (re < re2)
			{
				all_result.push_back(re);
				all_total.push_back(make_pair(can_num, false));
			}
			else
			{
				all_result.push_back(re2);
				all_total.push_back(make_pair(can_num, true));
			}
		}

		cout << "all_result.size :  " << all_result.size() << "all_total: " << all_total.size() << endl;
		double temp;
		pair<int, bool> tempp;
		int all_size = all_result.size();
		for (int i = 0; i < all_size - 1; i++)
			for (int j = 0; j < all_size - 1 - i; j++)
				if (all_result[j] < all_result[j + 1])
				{
					temp = all_result[j];
					all_result[j] = all_result[j + 1];
					all_result[j + 1] = temp;
					tempp = all_total[j];
					all_total[j] = all_total[j + 1];
					all_total[j + 1] = tempp;
				}
		vector<pair<int, bool>> all_total_mid;
		for (int t = all_total.size() - 1; t > total_num - 9; t--)
		{
			all_total_mid.push_back(all_total[t]);
			//cout << "order: " << all_total[t].first << "  flip: " << all_total[t].second << " value: " << all_result[t] << " complxeity: " << all_shape_complexity[all_total[t].first] << endl;
		}
		int midsize = all_total_mid.size();
		cout << " midsize: " << midsize << endl;
		all_total.swap(all_total_mid);
		return all_total;
	}

	double Tiling_opt::tar_mismatch(vector<vector<double>> first_arr, vector<vector<double>> second_arr, vector<pair<int, int>>& path, int &sec_shift, int width) //点对应匹配的筛选框宽度
	{
		int first_num = first_arr.size();
		int second_num = second_arr.size(); //first 作为y轴 ,second为x轴
		//cout <<  first_num << " - " << second_num << endl;
		//width = 30;
		if (first_num != second_num)
		{
			cout << "The sampling points of two contours are not equal: " << first_num << " - " << second_num << endl;
			//return 0;
		}
		if (first_arr[0].size() != second_arr[0].size())
		{
			cout << "The tar num of each point is not equal: " << first_arr[0].size() << " - " << second_arr[0].size() << endl;
			return 0;
		}
		double min_mis = 10000;
		vector<pair<int, int>> path_min;
		//double distance[202][202];
		//int step[202][202];//记录总的步数
		//cout << "first: " << first_num << "second: " << second_num << endl;
		for (int shift = 0; shift < second_num; shift++) //将first固定，分别对齐second的起点
		{
			//int ccc = 0;
			//int ddd = 0;
			for (int i = 0; i < first_num; i++)
			{
				for (int j = 0; j < second_num; j++)
				{
					//ddd++;
					dis[i][j] = 0;
					if (max(0, i - width) <= j && j <= min(second_num - 1, i + width))
					{
						//ccc++;
						distance[i][j] = 0;
					}
					else distance[i][j] = 100000;

				}
			}
			//cout << "ccc :  " << ccc <<"ddd: "<<ddd<< endl;
			//distance[0][0]记录的是对齐的第一个点
			dis[0][0] = length_two_point_tar(first_arr[0], second_arr[shift]);//
			distance[0][0] = dis[0][0];

			for (int i = 1; i < first_num; i++)
			{
				if (distance[i][0] == 100000) continue;
				dis[i][0] = length_two_point_tar(first_arr[i], second_arr[shift]);
				distance[i][0] = distance[i - 1][0] + dis[i][0];
			}
			for (int i = 1; i < second_num; i++)
			{
				if (distance[0][i] == 100000) continue;
				dis[0][i] = length_two_point_tar(first_arr[0], second_arr[(i + shift) % second_num]);
				distance[0][i] = distance[0][i - 1] + dis[0][i];
			}

			for (int i = 1; i < first_num; i++)
			{
				for (int j = 1; j < second_num; j++)
					//(int i = istart; i <= imax; i++)
				{
					if (distance[i][j] == 100000) continue;
					dis[i][j] = length_two_point_tar(first_arr[i], second_arr[(j + shift) % second_num]);
					double g1 = distance[i - 1][j] + dis[i][j];
					double g2 = distance[i - 1][j - 1] + dis[i][j];
					double g3 = distance[i][j - 1] + dis[i][j];
					if (g1 < g2)
					{
						if (g1 < g3) distance[i][j] = g1;
						else distance[i][j] = g3;
					}
					else
					{
						if (g2 < g3) distance[i][j] = g2;
						else distance[i][j] = g3;
					}
				}
			}
			//for (int i = 0; i < first_num; i++)
			//{
			//	cout << endl;
			//	for (int j = 0; j < second_num; j++)
			//		//(int i = istart; i <= imax; i++)
			//	{
			//		if (distance[i][j] == 100000) cout <<  "  0   ";
			//		else cout << distance[i][j] << "  ";
			//	}
			//}
			//cout << "shift " << shift << "  distance[first_num - 1][second_num - 1]:  " << distance[first_num - 1][second_num - 1] << endl;
			if (distance[first_num - 1][second_num - 1] < min_mis)
			{
				path_min.swap(vector<pair<int, int>>());
				min_mis = distance[first_num - 1][second_num - 1];
				print_TAR_Path(dis, distance, first_num - 1, second_num - 1, path_min);
				sec_shift = shift;
			}
		}
		path = path_min;
		return min_mis;
	}

	void Tiling_opt::print_TAR_Path(double d[][202], double dp[][202], int i, int j, vector<pair<int, int>>& path)
	{
		if (i == 0 && j == 0) {
			//cout << first_arr[i] << " - " << second_arr[j] << endl;
			path.push_back(make_pair(i, j));
			return;
		}

		if (abs(dp[i][j] - (dp[i - 1][j - 1] + dis[i][j])) < 0.001){
			print_TAR_Path(d, dp, i - 1, j - 1, path);

		}
		else if (abs(dp[i][j] - (dp[i][j - 1] + dis[i][j])) < 0.001) {
			print_TAR_Path(d, dp, i, j - 1, path);

		}
		else {
			print_TAR_Path(d, dp, i - 1, j, path);
		}
		path.push_back(make_pair(i, j));
	}

	vector<int> Tiling_opt::joint_relocate(vector<Point2f> contour_, vector<int> joint_index, int num_c) //将原始轮廓上的划分点对应到采样后的轮廓上
	{
		vector<Point2f> contour_mid = sampling(contour_, num_c + 1);//选择最少的点进行比较
		int con_size = contour_mid.size();
		vector<int> mid_in;
		for (int i = 0; i <joint_index.size(); i++)
		{
			int index_ = 0;
			double dist = 10000;
			for (int t = 0; t <con_size; t++)
			{
				if (length_two_point2f(contour_[joint_index[i]], contour_mid[t]) < dist)
				{
					dist = length_two_point2f(contour_[joint_index[i]], contour_mid[t]);
					index_ = t;
				}
			}
			mid_in.push_back(index_);
		}
		return mid_in;
	}

	vector<int> Tiling_opt::morphed_results(vector<Point2f> &morphed_A)
	{
		cout << "cand_paths num: " << cand_paths.size() << endl;
		vector<Point2f> final_pettern;
		vector<int> mid_inter_new;		
		vector<pair<int, int>> path = cand_paths[Candidate_index];

		int psize = path.size();
		cout << "path size: " << psize << endl;
		vector<Point2f> contour1 = poly_mid->contour_sample[1];
		vector<Point2f> contour2 = poly_second->contour_sample[1];
		int t = 0;
		for (int i = 0; i < psize; i++)
		{
			int first = path[i].first;
			int sec = path[i].second;
			Point2f fin = 0.5 * contour1[first] + 0.5 * contour2[sec];;
			if (t<4 && first == mid_inter[t])
			{
				//cout << t << " " << first << "  " << i << endl;
				t++;
				mid_inter_new.push_back(i);
			}
			final_pettern.push_back(fin);
		}	
		mid_inter_new = joint_relocate(final_pettern, mid_inter_new, 1);
		final_pettern = sampling(final_pettern, 2);

		if (mid_inter_new.size() != 4)
		{
			cout << "lack of tiling vertices!" << endl;
			return mid_inter_new;
		}

		int first = 0;
		int second = 0;

		vector<int> return_p;
		vector<vector<Point2f>> four_place;
		morphed_A = extract_contour(final_pettern, mid_inter_new, return_p, four_place, all_inner_conts[Tiling_index].type);   //
		
		int times = 0;
		while (self_intersect(morphed_A, first, second) && (times < 3))
		{
			++times;
			cout << "morphed_A self_intersect is repairing" << endl;
			contour_fine_tuning(morphed_A, first, second);
		}
		//cout << "times" << times << endl;
		four_place.swap(vector<vector<Point2f>>());
		mid_inter_new.swap(vector<int>());
		final_pettern = extract_contour(morphed_A, return_p, mid_inter_new, four_place, all_inner_conts[Tiling_index].type);
		cout << "lack of mid_inter_new: " << mid_inter_new.size() << endl;
		poly_tem->loadPoints(final_pettern);
		poly_tem->contour_sampling();
		return mid_inter_new;
	}

	void Tiling_opt::contour_fine_tuning(vector<Point2f> &contour_, int first, int second)
	{
		vector<Point2f> mid_con;
		int csize = contour_.size();
		vector<Point2f> pre;
		vector<Point2f> post;
		cout << csize << " " << first << " " << second << endl;
		if (second - first < 4)
		{
			Point2f tem = contour_[first + 1];
			contour_[first + 1] = contour_[second];
			contour_[second] = tem;
		}
		else
		{
			int mid = (first + second) / 2;
			for (int t = first; t < mid; t++)
			{
				pre.push_back(contour_[t]);
			}
			for (int t = mid; t <= second + 1; t++)
			{
				post.push_back(contour_[t]);
			}
			for (int t = 0; t < first; t++)
			{
				mid_con.push_back(contour_[t]);
			}
			if (mid_con.empty()) mid_con.push_back(0.5*contour_[csize - 1] + 0.5*post[post.size() - 2]);
			else mid_con.push_back(0.5*mid_con.back() + 0.5*post[post.size() - 2]);
			for (int t = post.size() - 2; t >= 0; t--)
			{
				mid_con.push_back(post[t]);
			}
			for (int t = pre.size() - 1; t > 0; t--)
			{
				mid_con.push_back(pre[t]);
			}
			mid_con.push_back(0.5*mid_con.back() + 0.5*contour_[(second + 2) % csize]);
			for (int t = second + 2; t < csize; t++)
			{
				mid_con.push_back(contour_[t]);
			}
			contour_.swap(mid_con);
		}
	}

	bool Tiling_opt::coll_detec_bbx(vector<Point2f> contour1, vector<Point2f> contour2, int threshold)
	{
		int csize = contour1.size();
		vector<Point2f> bbox1 = b_box(contour1);
		Point2f shift = Point2f(0, 0) - bbox1[1];
		for (int i = 0; i < csize; i++)
		{
			contour1[i] += shift;
			contour2[i] += shift;
		}
		Point2f cen1 = center_p(contour1);
		Point2f cen2 = center_p(contour2);
		bbox1 = b_box(contour1);
		vector<Point2f> bbox2 = b_box(contour2);
		/*Mat drawing3 = Mat(1200, 1200, CV_8UC1, Scalar(255));
		draw_poly(drawing3, contour1, cen1);
		draw_poly(drawing3, contour2, cen2);
		for (int i = 0; i < bbox1.size(); i++)
		{
		MyLine(drawing3, bbox1[i], bbox1[(i + 1) % bbox1.size()], "");
		MyLine(drawing3, bbox2[i], bbox2[(i + 1) % bbox2.size()], "");
		}
		imshow("draw3", drawing3);*/
		double margin = max(length_two_point2f(bbox1[0], bbox1[2]), length_two_point2f(bbox2[0], bbox2[2]));
		//cout << "margin "<<margin<<endl;
		vector<Point2f> search_boundary;
		vector<int> num_in1;
		vector<int> num_in2;
		for (int i = 0; i < 4; i++)
		{
			if (bbox1[i].x <= bbox2[3].x && bbox1[i].x >= bbox2[1].x)
				if (bbox1[i].y <= bbox2[3].y && bbox1[i].y >= bbox2[1].y)
				{
					num_in1.push_back(i);
				}
		}
		for (int i = 0; i < 4; i++)
		{
			if (bbox2[i].x <= bbox1[3].x && bbox2[i].x >= bbox1[1].x)
				if (bbox2[i].y <= bbox1[3].y && bbox2[i].y >= bbox1[1].y)
				{
					num_in2.push_back(i);
				}
		}
		//cout << "num_in1.size() and num_in2.size()" << num_in1.size() << "   " << num_in2.size() << endl;
		if (num_in1.size() == 0 && num_in2.size() == 0)
		{
			if (length_two_point2f(cen1, cen2) < margin)
			{
				search_boundary.push_back(Point2f(0, 0));
				search_boundary.push_back(Point2f(599, 599));
			}
			else return false;
		}
		else if (num_in1.size() == 1 && num_in2.size() == 1)
		{
			double bbx_max_x = max(bbox1[num_in1[0]].x, bbox2[num_in2[0]].x);
			double bbx_max_y = max(bbox1[num_in1[0]].y, bbox2[num_in2[0]].y);
			double bbx_min_x = min(bbox1[num_in1[0]].x, bbox2[num_in2[0]].x);
			double bbx_min_y = min(bbox1[num_in1[0]].y, bbox2[num_in2[0]].y);
			if ((abs(bbx_max_x - bbx_min_x) < 1) || (abs(bbx_max_y - bbx_min_y) < 1)) return false;
			search_boundary.push_back(Point2f(bbx_min_x, bbx_min_y));
			search_boundary.push_back(Point2f(bbx_max_x, bbx_max_y));
		}
		else if (num_in1.size() == 2 && num_in2.size() == 2)
		{
			double bbx_max_x = max(max(bbox1[num_in1[0]].x, bbox2[num_in2[0]].x), bbox1[num_in1[1]].x);
			double bbx_max_y = max(max(bbox1[num_in1[0]].y, bbox2[num_in2[0]].y), bbox1[num_in1[1]].y);
			double bbx_min_x = min(min(bbox1[num_in1[0]].x, bbox2[num_in2[0]].x), bbox1[num_in1[1]].x);
			double bbx_min_y = min(min(bbox1[num_in1[0]].y, bbox2[num_in2[0]].y), bbox1[num_in1[1]].x);
			if ((abs(bbx_max_x - bbx_min_x) < 1) || (abs(bbx_max_y - bbx_min_y) < 1)) return false;
			search_boundary.push_back(Point2f(bbx_min_x, bbx_min_y));
			search_boundary.push_back(Point2f(bbx_max_x, bbx_max_y));
		}
		else if (num_in1.size() != num_in2.size())
		{
			search_boundary.push_back(Point2f(0, 0));
			search_boundary.push_back(Point2f(599, 599));
		}
		//将第一个图画到画布上，计算搜索区域的点

		//MyLine(drawing3, search_boundary[0], search_boundary[1], "black");

		Mat drawing1 = Mat(600, 600, CV_8UC1, Scalar(255));
		Mat drawing2 = Mat(600, 600, CV_8UC1, Scalar(255));
		draw_poly(drawing1, contour1, cen1);
		draw_poly(drawing2, contour2, cen2);

		//imshow("draw1111", drawing1);
		//imshow("draw2222", drawing2);

		//imwrite("D:\\tu.png", drawing3);
		//cout << "1: " << search_boundary[0].x << "  2: " << search_boundary[1].x << "  3:" << search_boundary[0].y << "   4:" << search_boundary[1].y << endl;
		if (search_boundary[0].x < 0 || search_boundary[1].x>600 || search_boundary[0].y < 0 || search_boundary[1].y>600) cout << "search_boundary beyond!" << endl;
		int count = 0;
		for (int i = search_boundary[0].y; i <= search_boundary[1].y + 0.5; i++)
			for (int j = search_boundary[0].x; j <= search_boundary[1].x + 0.5; j++)
			{
				if ((int)drawing1.at<uchar>(i, j) == 0 && (int)drawing2.at<uchar>(i, j) == 0)
					count++;
			}
		//cout << count << endl;
		if (count > threshold) return true;
		else return false;

	}

	bool Tiling_opt::vertex_angle(vector<Point2f> angle1, vector<Point2f> angle2)
	{
		//  >360为true,<360为false
		int size = (angle1.size() - 1) / 2;
		for (int i = 0; i < size; i++)
		{
			double ang1 = 0;
			double ang2 = 0;
			double angle_cos = cos_two_vector((angle1[2 * i + 1] - angle1[0]), (angle1[2 * i + 2] - angle1[0]));
			double angle_sin = sin_two_vector((angle1[2 * i + 1] - angle1[0]), (angle1[2 * i + 2] - angle1[0]));
			if (angle_sin > 0) ang1 = acos(angle_cos) / PI * 180;
			else ang1 = 360 - acos(angle_cos) / PI * 180;

			angle_cos = cos_two_vector((angle2[2 * i + 1] - angle2[0]), (angle2[2 * i + 2] - angle2[0]));
			angle_sin = sin_two_vector((angle2[2 * i + 1] - angle2[0]), (angle2[2 * i + 2] - angle2[0]));
			if (angle_sin > 0) ang2 = acos(angle_cos) / PI * 180;
			else ang2 = 360 - acos(angle_cos) / PI * 180;
			if (ang1 + ang2 > 361)
			{
				//cout << ang1 <<"  "<<ang2<< endl;
				return true;
			}
		}

		return false;
	}
}