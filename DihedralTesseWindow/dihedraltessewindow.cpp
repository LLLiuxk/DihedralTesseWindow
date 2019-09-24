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
	winRectupdate();
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Polygon File"),
		"../contours/0.txt",
		tr("Polygon file (*.txt);;Image file (*.jpg *.png)"));
	if (!fileName.isEmpty()){
		openfile(fileName);
	}
	/*QPointF a(10, 10);
	QPointF b(10, 200);
	QPointF c(210, 200);
	QPointF d(210, 10);
	QVector<QPointF> tt;
	tt.push_back(a);
	tt.push_back(b);
	tt.push_back(c);
	tt.push_back(d);
	QPolygonF ttt = QPolygonF(tt);
	Poly1 = new QGraphicsPolygonItem(ttt);*/
	//winRectupdate();

	//show input
	Poly1 = new QGraphicsPolygonItem(tiling_opt->poly_first->poly);
	Poly1->setPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	Poly1->setBrush(QBrush(Qt::black));
	scene_2.addItem(Poly1);
	scene_2.setItemIndexMethod(QGraphicsScene::NoIndex);
	scene_2.setSceneRect(0, 0, 600, 600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView_2->setScene(&scene_2);
	//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
	//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
	ui.graphicsView_2->scale(0.5, 0.5);
	ui.graphicsView_2->show();

	outt << winRect[0] << winRect[1] << winRect[2] << winRect[3];
	//ui->setMouseTracking(true);
	
	//Poly2->polygon().swap(poly2);
	
	//Poly2 = new QGraphicsPolygonItem(Polygon1.poly);
	////qDebug() << Poly2->polygon;
	////Poly2->setScale(0.5);
	//
	//Poly2->setBrush(QBrush(Qt::cyan));



	//scene2.addItem(Poly2);
	//scene2.setItemIndexMethod(QGraphicsScene::NoIndex);
	//scene2.setSceneRect(0, 0, 600, 600);
	//
	////ui.graphicsView_2->setSceneRect(poly1.boundingRect());
	////outt << ui.graphicsView_2->sceneRect() << poly1.boundingRect();
	////outt << "scale: " << ui.graphicsView_2->sceneRect();

	//ui.graphicsView_3->setScene(&scene2);
	////ui.graphicsView_3->setMouseTracking(true);
	//ui.graphicsView_3->show();
	//ui.graphicsView2->setScene(&scene2);
	//ui.graphicsView2->show();
	//ui.graphicsView->setScene(&scene2);
	//ui.graphicsView->show();
		
}



void 
DihedralTesseWindow::on_actionCompute_triggered()
{
	int step = 1;
	pd = new QProgressDialog(this);
	pd->setOrientation(Qt::Horizontal);  // 水平方向  
	pd->setMinimum(0);  // 最小值  
	pd->setMaximum(100);  // 最大值  
	pd->setValue(0);  // 当前进度 
	pd->show();

	int result = tiling_opt->tiliing_generation();
	QString message;
	QMessageBox::StandardButton reply;
	if (result == 0)
	{
		message = "Please input correct pattern at first!";
		reply = QMessageBox::information(this, tr("Warning!"), message);
	}
	else if (result == 2)
	{
		message = "The  file/dir had been Exisit!";
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
		scene_3.addItem(Poly2);
		scene_3.setItemIndexMethod(QGraphicsScene::NoIndex);
		Point2f cen = tiling_opt->poly_mid->poly_c;
		scene_3.setSceneRect(cen.x - 300, cen.y-300, 600, 600);
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView_3->setScene(&scene_3);
		ui.graphicsView_3->fitInView(scene_3.sceneRect(), Qt::KeepAspectRatio);
		ui.graphicsView_3->scale(0.75, 0.75);
		//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
		//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
		ui.graphicsView_3->show();
		message = "Computing over!";
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
	scene_4.addItem(Poly3);
	scene_4.setItemIndexMethod(QGraphicsScene::NoIndex);
	Point2f cen = tiling_opt->poly_second->poly_c;
	scene_4.setSceneRect(cen.x - 300, cen.y - 300, 600, 600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView_4->setScene(&scene_4);
	ui.graphicsView_4->fitInView(scene_4.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView_4->scale(0.75, 0.75);
	//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
	//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
	ui.graphicsView_4->show();
}

void 
DihedralTesseWindow::on_actionMorphing_triggered()
{
	vector<int> mid_inter_new; 
	vector<Point2f> morphed_A;
	mid_inter_new = tiling_opt->morphed_results(morphed_A);
	if (mid_inter_new.size() != 4)
	{
		cout << "lack of tiling vertices!" << endl;
		return;
	}

	Poly4 = new QGraphicsPolygonItem(tiling_opt->poly_tem->poly);
	Poly4->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	Poly4->setBrush(QBrush(Qt::black));
	scene_5.addItem(Poly4);
	scene_5.setItemIndexMethod(QGraphicsScene::NoIndex);
	Point2f cen = tiling_opt->poly_second->poly_c;
	scene_5.setSceneRect(cen.x - 300, cen.y - 300, 600, 600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView_5->setScene(&scene_5);
	ui.graphicsView_5->fitInView(scene_5.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView_5->scale(0.75, 0.75);
	//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
	//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
	ui.graphicsView_5->show();

	QString pixpath1 = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\gv2\\tiling_result.png";
	QString pixpath2 = "D:\\VisualStudioProjects\\DihedralTesseWindow\\result\\gv2\\morphedA.png";
	Mat drawing_result = Mat(2000, 2000, CV_8UC3, Scalar(0, 0, 0));
	Mat drawing_mA = Mat(600, 600, CV_8UC3, Scalar(255, 255, 255));
	draw_allplane(drawing_result, tiling_opt->poly_tem->contour_sample[1], mid_inter_new, 0.4, tiling_opt->all_inner_conts[tiling_opt->Tiling_index].type);
	draw_poly(drawing_mA, morphed_A, center_p(morphed_A),0);
	imwrite(pixpath1.toStdString(), drawing_result);
	imwrite(pixpath2.toStdString(), drawing_mA);

	QPixmap *pixmap1 = new QPixmap(pixpath1);
	QPixmap *pixmap2 = new QPixmap(pixpath2);
	scene2.addPixmap(*pixmap1);
	scene2.setSceneRect(0, 0, 1600, 1600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView2->setScene(&scene2);
	ui.graphicsView2->fitInView(scene2.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView2->show();

	scene_6.addPixmap(*pixmap2);
	scene_6.setSceneRect(0, 0, 1600, 1600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView_6->setScene(&scene_6);
	ui.graphicsView_6->fitInView(scene_6.sceneRect(), Qt::KeepAspectRatio);
	ui.graphicsView_6->show();
	
	
}

void 
DihedralTesseWindow::on_actionClear_triggered()
{
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
		temp = event->globalPos();
		
		temp1 = event->screenPos();
		temp2 = event->localPos();
		temp3 = event->pos();
		temp_view = temp - winRect[2].topLeft();
		/*outt << "globalPos  " << temp;
		outt << "screenPos  " << temp1;
		outt << "localPos  " << temp2;
		outt << "Pos  " << temp3;*/
		temp_view = ui.graphicsView_2->mapToScene(temp_view.toPoint());
		outt << "viewPos  " << temp_view;
		//lastPoint = temp.toPoint();
		//qDebug() << "QMouseEvent globalpos(): " << lastPoint;
		
		//Poly2->rotation();
		//QRectF t = ui.graphicsView_2->sceneRect();
		//ui.graphicsView_2->setSceneRect(t.x()+20,t.y()+20,t.width(),t.height());
		//move(temp);
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
		Point2f cen = tiling_opt->poly_mid->poly_c;
		scene_3.setSceneRect(cen.x - 300, cen.y - 300, 600, 600);
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
		Point2f cen = tiling_opt->poly_second->poly_c;
		scene_4.setSceneRect(cen.x - 300, cen.y - 300, 600, 600);
		//scene1.setSceneRect(0, 0, 400, 400);
		ui.graphicsView_4->setScene(&scene_4);
		ui.graphicsView_4->fitInView(scene_4.sceneRect(), Qt::KeepAspectRatio);
		ui.graphicsView_4->scale(0.75, 0.75);
		//ui.graphicsView_2->setSceneRect(10, 10, 600, 600);
		//ui.graphicsView_2->setSceneRect(Polygon1.poly_c.x - 300, Polygon1.poly_c.y - 300, 600, 600);
		ui.graphicsView_4->show();
	}
	else
	{
		message = "This is not a useful key!";
		reply = QMessageBox::information(this, tr("Information"), message);
	}

}

//
//void
//DihedralTesseWindow::mouseMoveEvent(QMouseEvent *event)
//{
//}

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


