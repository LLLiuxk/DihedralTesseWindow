#include "dihedraltessewindow.h"

DihedralTesseWindow::DihedralTesseWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); 
	InitialData();
	//ui.graphicsView->installEventFilter(this);
	//ui.graphicsView2->installEventFilter(this);
	//ui.graphicsView_2->installEventFilter(this);
	//ui.graphicsView->setMouseTracking(true);
	//ui.graphicsView2->setMouseTracking(true);
	//ui.graphicsView_2->setMouseTracking(true);
	//ui.graphicsView_3->setMouseTracking(true);
	//
	//
	//
	//ui.splitter1->setMouseTracking(true);
	//ui.splitter2->setMouseTracking(true);
	//ui.splitter->setMouseTracking(true);
	//ui.centralWidget->setMouseTracking(true);
	//this->setMouseTracking(true);
	//this->ui.graphicsView_3->setMouseTracking(true);
	//this->graphicsView->setAcceptDrops(false);

	
}

DihedralTesseWindow::~DihedralTesseWindow()
{

}


void
DihedralTesseWindow::on_actionLoadInput_triggered()
{
	outt << winRect[0] << winRect[1] << winRect[2] << winRect[3] << endl;
	winRectupdate();
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Polygon File"),
		"../contours/0.txt",
		tr("Polygon file (*.txt);;Image file (*.jpg *.png)"));
	if (!fileName.isEmpty()){
		openfile(fileName);
	}

	//show input
	Poly1 = new QGraphicsPolygonItem(tiling_opt->poly_first->poly);
	Poly1->setPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	Poly1->setBrush(QBrush(Qt::black));
	scene_2.clear();
	scene_2.addItem(Poly1);
	scene_2.setItemIndexMethod(QGraphicsScene::NoIndex);
	scene_2.setSceneRect(0, 0, 600, 600);

	ui.graphicsView_2->setScene(&scene_2);
	ui.graphicsView_2->fitInView(scene_2.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView_2->show();
	//ui.graphicsView_5->show();

		
}



void 
DihedralTesseWindow::on_actionCompute_triggered()
{
	QElapsedTimer mstimer;
	mstimer.start();
	//int step = 1;
	//pd = new QProgressDialog(this);
	//pd->setOrientation(Qt::Horizontal);  // 水平方向  
	//pd->setMinimum(0);  // 最小值  
	//pd->setMaximum(100);  // 最大值  
	//pd->setValue(0);  // 当前进度 
	//pd->show();

	int result = tiling_opt->tiliing_generation();
	float time = (double)mstimer.nsecsElapsed() / (double)1000000000;

	QString message;
	QMessageBox::StandardButton reply;
	if (result == 0)
	{
		message = "Please input correct pattern at first!";
		reply = QMessageBox::information(this, tr("Warning!"), message);
	}
	else if (result == 2)
	{
		message = "The tiling results had been computed!";
		reply = QMessageBox::information(this, tr("Information"), message);
	}
	else if (result == 3)
	{
		message = "No right placement!";
		reply = QMessageBox::information(this, tr("Information"), message);
	}

	else if (result == 1)
	{
		cout << "Tiling num: "<<tiling_opt->all_tiling_Mat.size() << endl;
		QString pixpath = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\gv1\\show.png";
		imwrite(pixpath.toStdString(), tiling_opt->all_tiling_Mat[0]);
		QPixmap *pixmap = new QPixmap(pixpath);
		scene.addPixmap(*pixmap);
		scene.setSceneRect(0, 0, 800, 800);
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView->setScene(&scene);
		ui.graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
		ui.graphicsView->show();

		tiling_opt->poly_mid = new PolygonTile(tiling_opt->all_inner_conts[0].in_contour);
		Poly2 = new QGraphicsPolygonItem(tiling_opt->poly_mid->poly);
		Poly2->setPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		Poly2->setBrush(QBrush(Qt::black));
		scene_3.clear();
		scene_3.addItem(Poly2);
		scene_3.setItemIndexMethod(QGraphicsScene::NoIndex);
		QRectF bbox = Q_bbox(tiling_opt->poly_mid->contour);
		QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
		scene_3.setSceneRect(cen_b);//
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView_3->setScene(&scene_3);
		ui.graphicsView_3->fitInView(scene_3.sceneRect(), Qt::KeepAspectRatio);
		//outt << "scene_3:" << scene_3.sceneRect() << endl << "graphicsView_3: " << ui.graphicsView_3->rect();
		//ui.graphicsView_3->scale(0.75, 0.75);
		//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
		//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
		ui.graphicsView_3->show();
		message = "Computing over! Time consumption: " + QString("%1").arg(time) + " s";
		reply = QMessageBox::information(this, tr("Information"), message);
		
	}
	//imshow("the 1 ", tiling_opt->all_tiling_Mat[0]);

}

void 
DihedralTesseWindow::on_actionMatchCandidate_triggered()
{
	outt << "The Tiling scheme you choose is:  " << tiling_opt->Tiling_index << endl;
	tiling_opt->match_candidate();

	cout << "Candidate contours num: " << tiling_opt->candidate_contours.size() << endl;
	tiling_opt->poly_second->loadPoints(tiling_opt->candidate_contours[0]);
	tiling_opt->poly_second->contour_sampling();
	Poly3 = new QGraphicsPolygonItem(tiling_opt->poly_second->poly);
	Poly3->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	Poly3->setBrush(QBrush(Qt::black));
	scene_4.clear();
	scene_4.addItem(Poly3);
	scene_4.setItemIndexMethod(QGraphicsScene::NoIndex);

	QRectF bbox = Q_bbox(tiling_opt->poly_second->contour);
	QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
	scene_4.setSceneRect(cen_b);//
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView_4->setScene(&scene_4);
	ui.graphicsView_4->fitInView(scene_4.sceneRect(), Qt::KeepAspectRatio);
	//ui.graphicsView_4->scale(0.75, 0.75);
	//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
	//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
	ui.graphicsView_4->show();
}

void 
DihedralTesseWindow::on_actionMorphing_triggered()
{
	tiling_opt->mid_inter_morphed.swap(vector<int>());
	vector<Point2f> morphed_A;
	tiling_opt->mid_inter_morphed = tiling_opt->morphed_results(morphed_A);
	if (tiling_opt->mid_inter_morphed.size() != 4)
	{
		cout << "lack of tiling vertices!" << endl;
		return;
	}

	Poly4 = new QGraphicsPolygonItem(tiling_opt->poly_tem->poly);
	Poly4->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	Poly4->setBrush(QBrush(Qt::black));
	scene_5.clear();
	scene_5.addItem(Poly4);
	scene_5.setItemIndexMethod(QGraphicsScene::NoIndex);
	QRectF bbox = Q_bbox(tiling_opt->poly_tem->contour);
	QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
	scene_5.setSceneRect(cen_b);

	ui.graphicsView_5->setScene(&scene_5);
	ui.graphicsView_5->fitInView(scene_5.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView_5->show();

	QString pixpath1 = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\gv2\\tiling_result.png";
	Mat drawing_result = Mat(2000, 2000, CV_8UC3, Scalar(0, 0, 0));
	draw_allplane(drawing_result, tiling_opt->poly_tem->contour, tiling_opt->mid_inter_morphed, 0.4, tiling_opt->all_inner_conts[tiling_opt->Tiling_index].type);
	imwrite(pixpath1.toStdString(), drawing_result);
	QPixmap *pixmap1 = new QPixmap(pixpath1);
	scene2.addPixmap(*pixmap1);
	scene2.setSceneRect(0, 0, 1600, 1600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView2->setScene(&scene2);
	ui.graphicsView2->scale(0.75, 0.75);
	//ui.graphicsView2->fitInView(scene2.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView2->show();

	poly1 = QPolygonF(vecp_cv2qt(morphed_A));
	Poly5 = new QGraphicsPolygonItem(poly1);
	Poly5->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	Poly5->setBrush(QBrush(Qt::black));
	scene_6.clear();
	scene_6.addItem(Poly5);
	scene_6.setItemIndexMethod(QGraphicsScene::NoIndex);
	bbox = Q_bbox(morphed_A);
	cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
	scene_6.setSceneRect(cen_b);

	ui.graphicsView_6->setScene(&scene_6);
	ui.graphicsView_6->fitInView(scene_6.sceneRect(), Qt::KeepAspectRatio);
	
	ui.graphicsView_6->show();
	
	
}

void 
DihedralTesseWindow::on_actionModify_triggered()
{
	tiling_opt->modify = !tiling_opt->modify;
	if (tiling_opt->modify)
	{
		scene_5.removeItem(Poly4);
		tiling_opt->modifying_c = tiling_opt->poly_tem->contour;
		int cont_size = tiling_opt->modifying_c.size();
		EllipseItem.swap(vector<QGraphicsEllipseItem *>());
		int t = 0;
		for (int i = 0; i < cont_size; i++)
		{
			QRectF rectangle;
			QGraphicsEllipseItem *circle;
			if (t < 4 && i == tiling_opt->mid_inter_morphed[t])
			{
				rectangle = QRectF(tiling_opt->modifying_c[i].x - 3, tiling_opt->modifying_c[i].y - 3, 6, 6);
				circle = new QGraphicsEllipseItem(rectangle);
				circle->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
				circle->setBrush(QBrush(Qt::black));
				t++;
			}
			else
			{
				rectangle = QRectF(tiling_opt->modifying_c[i].x - 2, tiling_opt->modifying_c[i].y - 2, 4, 4);
				circle = new QGraphicsEllipseItem(rectangle);
				circle->setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
				circle->setBrush(QBrush(Qt::red));
			}
			
			scene_5.addItem(circle);
			EllipseItem.push_back(circle);
		}
		ui.graphicsView_5->show();
	}
	else
	{
		tiling_opt->poly_tem->loadPoints(tiling_opt->modifying_c);
		Poly4 = new QGraphicsPolygonItem(tiling_opt->poly_tem->poly);
		Poly4->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		Poly4->setBrush(QBrush(Qt::black));
		scene_5.clear();
		scene_5.addItem(Poly4);
		/*scene_5.setItemIndexMethod(QGraphicsScene::NoIndex);
		QRectF bbox = Q_bbox(tiling_opt->poly_tem->contour);
		QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
		scene_5.setSceneRect(cen_b);*/
		ui.graphicsView_5->show();

		QString pixpath1 = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\gv2\\tiling_result_modified.png";
		Mat drawing_result = Mat(2000, 2000, CV_8UC3, Scalar(0, 0, 0));
		draw_allplane(drawing_result, tiling_opt->poly_tem->contour, tiling_opt->mid_inter_morphed, 0.4, tiling_opt->all_inner_conts[tiling_opt->Tiling_index].type);
		imwrite(pixpath1.toStdString(), drawing_result);
		QPixmap *pixmap1 = new QPixmap(pixpath1);
		scene2.addPixmap(*pixmap1);
		scene2.setSceneRect(0, 0, 1600, 1600);
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView2->setScene(&scene2);
		ui.graphicsView2->scale(0.75, 0.75);
		//ui.graphicsView2->fitInView(scene2.sceneRect(), Qt::KeepAspectRatio);
		ui.graphicsView2->show();

		vector<int> return_p;
		vector<vector<Point2f>> four_place;
		vector<Point2f> morphed_A = tiling_opt->extract_contour(tiling_opt->modifying_c, tiling_opt->mid_inter_morphed, return_p, four_place, tiling_opt->all_inner_conts[tiling_opt->Tiling_index].type);   //

		poly1 = QPolygonF(vecp_cv2qt(morphed_A));
		Poly5 = new QGraphicsPolygonItem(poly1);
		Poly5->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		Poly5->setBrush(QBrush(Qt::black));
		scene_6.clear();
		scene_6.addItem(Poly5);
		scene_6.setItemIndexMethod(QGraphicsScene::NoIndex);
		QRectF bbox = Q_bbox(morphed_A);
		QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
		scene_6.setSceneRect(cen_b);
		ui.graphicsView_6->setScene(&scene_6);
		ui.graphicsView_6->fitInView(scene_6.sceneRect(), Qt::KeepAspectRatio);
		ui.graphicsView_6->show();

	}

}

void
DihedralTesseWindow::on_actionSaveSVG_triggered()
{
	exportSVG(ui.graphicsView2);
	exportSVG(ui.graphicsView_5);
	exportSVG(ui.graphicsView_6);
}

void 
DihedralTesseWindow::on_actionClear_triggered()
{
	//ui.actionClear();
	poly1.clear();
	Poly2->hide();
	qDebug() << "clear!";

}

void
DihedralTesseWindow::openfile(QString fileName)
{
	tiling_opt->initial(fileName.toStdString());
	outt << "Initial over successfully";

}

void 
DihedralTesseWindow::wheelEvent(QWheelEvent *event)
{
	QPoint temp, temp1;
	temp = event->globalPos();
	temp1 = event->pos();
	double s_factor = 0.25;
	//qDebug() << "QWheelEvent globalpos(): " << temp;
	
	if (event->delta() > 0)
	{
		if (winRect[0].contains(temp))  ui.graphicsView->scale(1 - s_factor, 1 - s_factor);
		if (winRect[1].contains(temp))  ui.graphicsView2->scale(1 - s_factor, 1 - s_factor);
		if (winRect[2].contains(temp))  ui.graphicsView_2->scale(1 - s_factor, 1 - s_factor);
		if (winRect[3].contains(temp))  ui.graphicsView_3->scale(1 - s_factor, 1 - s_factor);
		if (winRect[4].contains(temp))  ui.graphicsView_4->scale(1 - s_factor, 1 - s_factor);
		if (winRect[5].contains(temp))  ui.graphicsView_5->scale(1 - s_factor, 1 - s_factor);
		if (winRect[6].contains(temp))  ui.graphicsView_6->scale(1 - s_factor, 1 - s_factor);
		//outt << "1 - s_factor:" << ui.graphicsView_6->rect();
	}
	else
	{
		if (winRect[0].contains(temp))  ui.graphicsView->scale(1 + s_factor, 1 + s_factor);
		if (winRect[1].contains(temp))  ui.graphicsView2->scale(1 + s_factor, 1 + s_factor);
		if (winRect[2].contains(temp))  ui.graphicsView_2->scale(1 + s_factor, 1 + s_factor);
		if (winRect[3].contains(temp))  ui.graphicsView_3->scale(1 + s_factor, 1 + s_factor);
		if (winRect[4].contains(temp))  ui.graphicsView_4->scale(1 + s_factor, 1 + s_factor);
		if (winRect[5].contains(temp))  ui.graphicsView_5->scale(1 + s_factor, 1 + s_factor);
		if (winRect[6].contains(temp))  ui.graphicsView_6->scale(1 + s_factor, 1 + s_factor);
	}

	// 当滚轮远离使用者时进行缩小，当滚轮向使用者方向旋转时进行放大
	
}

void
DihedralTesseWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton){

		// 我们使用鼠标指针当前的位置减去差值，就得到了窗口应该移动的位置
		QPointF temp, temp1, temp2, temp3,temp_view;
		temp_view = event->globalPos() - winRect[5].topLeft();
		temp_view = ui.graphicsView_5->mapToScene(temp_view.toPoint());
		outt << "viewPos  " << temp_view;
		if (tiling_opt->modify)
		{
			if (tiling_opt->chosen)
			{
				tiling_opt->modifying_c[tiling_opt->chosen_index] = Point2f(temp_view.x(), temp_view.y());
				QRectF rectangle(temp_view.x() - 3, temp_view.y() - 3, 6, 6);
				QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(rectangle);
				circle->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
				circle->setBrush(QBrush(Qt::green));
				scene_5.removeItem(EllipseItem[tiling_opt->chosen_index]);
				//delete(EllipseItem[tiling_opt->chosen_index]);
				EllipseItem[tiling_opt->chosen_index]->~QGraphicsEllipseItem();
				EllipseItem[tiling_opt->chosen_index] = circle;
				scene_5.addItem(EllipseItem[tiling_opt->chosen_index]);
			}
			else
			{
				Point2f temp_ = Point2f(temp_view.x(), temp_view.y());
				tiling_opt->chosen_index = locate_p(temp_, tiling_opt->modifying_c);
				//cout << "c  p:  " << tiling_opt->modifying_c[tiling_opt->chosen_index]<<endl;
				scene_5.removeItem(EllipseItem[tiling_opt->chosen_index]);
				//QRectF rect_ = EllipseItem[tiling_opt->chosen_index]->rect();
				//outt << "c  p 2 :  " << rect_ << endl;
				//EllipseItem[tiling_opt->chosen_index]->setRect(QRectF(rect_.topLeft().x() - 1, rect_.topLeft().y() - 1, 6, 6));
				EllipseItem[tiling_opt->chosen_index]->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
				EllipseItem[tiling_opt->chosen_index]->setBrush(QBrush(Qt::cyan));
				scene_5.addItem(EllipseItem[tiling_opt->chosen_index]);
			}
			tiling_opt->chosen = !tiling_opt->chosen;
		}
		//temp = event->globalPos();	
		//temp1 = event->screenPos();
		//temp2 = event->localPos();
		//temp3 = event->pos();	
		//temp2 = temp - winRect[5].topLeft();
		//outt << "globalPos  " << temp;
		//outt << "screenPos  " << temp1;
		//outt << "localPos  " << temp2;
		//outt << "Pos  " << temp3;

	}

}

void 
DihedralTesseWindow::keyPressEvent(QKeyEvent *event)
{
	QString message;
	QMessageBox::StandardButton reply;
	if ((event->key() == Qt::Key_W) || (event->key() == Qt::Key_S))
	{
		if (tiling_opt->all_tiling_Mat.empty())
		{
			message = "No tiling results!";
			reply = QMessageBox::information(this, tr("Information"), message);
			return;
		}
		tiling_clear();
		if (event->key() == Qt::Key_W)
		{
			if (tiling_opt->Tiling_index != 0)
			{
				tiling_opt->Tiling_index--;
			}
			else
			{
				message = "It is the first tiling pattern!";
				reply = QMessageBox::information(this, tr("Information"), message);
			}
			outt << "Tiling_index:  " << tiling_opt->Tiling_index << endl;
		}
		else
		{
			if (tiling_opt->Tiling_index != tiling_opt->all_tiling_Mat.size() - 1)
			{
				tiling_opt->Tiling_index++;
			}
			else
			{
				message = "It is the last tiling pattern!";
				reply = QMessageBox::information(this, tr("Information"), message);
			}
			outt << "Tiling_index:  " << tiling_opt->Tiling_index << endl;
		}
		QString pixpath = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\gv1\\";
		pixpath = pixpath + QString::number(tiling_opt->Tiling_index) + ".png";
		imwrite(pixpath.toStdString(), tiling_opt->all_tiling_Mat[tiling_opt->Tiling_index]);
		QPixmap *pixmap = new QPixmap(pixpath);
		scene.clear();
		scene.addPixmap(*pixmap);
		scene.setSceneRect(0, 0, 800, 800);
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView->setScene(&scene);
		//ui.graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
		//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
		//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
		ui.graphicsView->show();

		tiling_opt->poly_mid = new PolygonTile(tiling_opt->all_inner_conts[tiling_opt->Tiling_index].in_contour);
		Poly2 = new QGraphicsPolygonItem(tiling_opt->poly_mid->poly);
		Poly2->setPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		Poly2->setBrush(QBrush(Qt::black));
		scene_3.clear();
		scene_3.addItem(Poly2);
		scene_3.setItemIndexMethod(QGraphicsScene::NoIndex);
		QRectF bbox = Q_bbox(tiling_opt->poly_mid->contour);
		QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
		scene_3.setSceneRect(cen_b);//
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView_3->setScene(&scene_3);
		//ui.graphicsView->fitInView(scene_3.sceneRect(), Qt::KeepAspectRatio);
		ui.graphicsView_3->show();
	}
	else if ((event->key() == Qt::Key_A) || (event->key() == Qt::Key_D))
	{
		if (tiling_opt->candidate_contours.empty())
		{
			message = "No candidate patterns!";
			reply = QMessageBox::information(this, tr("Information"), message);
			return;
		}
		candidate_clear();
		if (event->key() == Qt::Key_A)
		{
			if (tiling_opt->Candidate_index != 0)
			{
				tiling_opt->Candidate_index--;
			}
			else
			{
				message = "It is the first candidate pattern!";
				reply = QMessageBox::information(this, tr("Information"), message);
			}
			outt << "Candidate_index:  " << tiling_opt->Candidate_index << endl;
		}
		else
		{
			if (tiling_opt->Candidate_index != tiling_opt->candidate_contours.size() - 1)
			{
				tiling_opt->Candidate_index++;
			}
			else
			{
				message = "It is the last candidate pattern!";
				reply = QMessageBox::information(this, tr("Information"), message);
			}
			outt << "Candidate_index:  " << tiling_opt->Candidate_index << endl;
		}
		tiling_opt->poly_second->loadPoints(tiling_opt->candidate_contours[tiling_opt->Candidate_index]);
		tiling_opt->poly_second->contour_sampling();
		Poly3 = new QGraphicsPolygonItem(tiling_opt->poly_second->poly);
		Poly3->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		Poly3->setBrush(QBrush(Qt::black));
		scene_4.clear();
		scene_4.addItem(Poly3);
		scene_4.setItemIndexMethod(QGraphicsScene::NoIndex);
		QRectF bbox = Q_bbox(tiling_opt->poly_second->contour);
		QRectF cen_b = QRectF(bbox.topLeft() - QPointF(100, 100), bbox.bottomRight() + QPointF(100, 100));
		scene_4.setSceneRect(cen_b);//

		ui.graphicsView_4->setScene(&scene_4);
		ui.graphicsView_4->fitInView(scene_4.sceneRect(), Qt::KeepAspectRatio);
		//ui.graphicsView_4->scale(0.75, 0.75);
		ui.graphicsView_4->show();
	}
	else
	{
		message = "This is not a useful key!";
		reply = QMessageBox::information(this, tr("Information"), message);
	}

}



void 
DihedralTesseWindow::resizeEvent(QResizeEvent *event)
{
	winRectupdate();
	/*winRect[0] = QRect(ui.graphicsView->mapToGlobal(QPoint(0,0)), ui.graphicsView->size());
	winRect[1] = QRect(ui.graphicsView2->mapToGlobal(QPoint(0, 0)), ui.graphicsView2->size());
	winRect[2] = QRect(ui.graphicsView_2->mapToGlobal(QPoint(0, 0)), ui.graphicsView_2->size());
	winRect[3] = QRect(ui.graphicsView_3->mapToGlobal(QPoint(0, 0)), ui.graphicsView_3->size());
	winRect[4] = QRect(ui.graphicsView_4->mapToGlobal(QPoint(0, 0)), ui.graphicsView_4->size());
	winRect[5] = QRect(ui.graphicsView_5->mapToGlobal(QPoint(0, 0)), ui.graphicsView_5->size());
	winRect[6] = QRect(ui.graphicsView_6->mapToGlobal(QPoint(0, 0)), ui.graphicsView_6->size());*/
	//outt << winRect[0] << winRect[1] << winRect[2] << winRect[3];
	
}

void 
DihedralTesseWindow::moveEvent(QMoveEvent *event)
{
	winRectupdate();
}

void
DihedralTesseWindow::winRectupdate()
{
	winRect[0] = QRect(ui.graphicsView->mapToGlobal(QPoint(0, 0)), ui.graphicsView->size());
	winRect[1] = QRect(ui.graphicsView2->mapToGlobal(QPoint(0, 0)), ui.graphicsView2->size());
	winRect[2] = QRect(ui.graphicsView_2->mapToGlobal(QPoint(0, 0)), ui.graphicsView_2->size());
	winRect[3] = QRect(ui.graphicsView_3->mapToGlobal(QPoint(0, 0)), ui.graphicsView_3->size());
	winRect[4] = QRect(ui.graphicsView_4->mapToGlobal(QPoint(0, 0)), ui.graphicsView_4->size());
	winRect[5] = QRect(ui.graphicsView_5->mapToGlobal(QPoint(0, 0)), ui.graphicsView_5->size());
	winRect[6] = QRect(ui.graphicsView_6->mapToGlobal(QPoint(0, 0)), ui.graphicsView_6->size());

}

void 
DihedralTesseWindow::InitialData()
{
	cout << "所有涉及坐标的计算使用的都是全局坐标"<<endl;
	tiling_opt = new Tiling_tiles::Tiling_opt();

}

void 
DihedralTesseWindow::exportSVG(QGraphicsView* view)
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Export to SVG"),
		"../result/compare/",
		tr("SVG (*.svg)\n"));

	QSvgGenerator svg;
	svg.setFileName(fileName);

	svg.setSize(view->size());
	svg.setViewBox(view->sceneRect());
	svg.setTitle(tr("%1 drawing").arg(qApp->applicationName()));
	svg.setDescription(tr("Generated using %1").arg(qApp->applicationName()));

	QPainter painter;
	painter.begin(&svg);
	view->render(&painter);
	painter.end();
}

void 
DihedralTesseWindow::tiling_clear()
{
	scene2.clear();
	scene_4.clear();
	scene_5.clear();
	scene_6.clear();
	tiling_opt->Candidate_index = 0;
	tiling_opt->candidate_contours.swap(vector<vector<Point2f>>());
}

void 
DihedralTesseWindow::candidate_clear()
{
	scene2.clear();
	scene_5.clear();
	scene_6.clear();
}

//bool
//DihedralTesseWindow::eventFilter(QObject *o, QEvent *e)
//{
//	if ((o == ui.graphicsView) && (e->type() == QEvent::MouseMove))
//	{
//		outt << "111111";
//	}
//	else if ((o == ui.graphicsView_2) && (e->type() == QEvent::MouseMove))
//	{
//		outt << "222222";
//	}
//	else if ((o == ui.graphicsView_3) && (e->type() == QEvent::MouseMove))
//	{
//		outt << "333333333";
//	}
//	return false;
//}


