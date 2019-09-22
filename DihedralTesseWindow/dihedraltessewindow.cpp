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
	scene1.addItem(Poly1);
	scene1.setItemIndexMethod(QGraphicsScene::NoIndex);
	scene1.setSceneRect(0, 0, 600, 600);
	//scene1.setSceneRect(0, 0, 400, 400);
	ui.graphicsView_2->setScene(&scene1);
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
DihedralTesseWindow::openfile(QString fileName)
{
	tiling_opt->initial(fileName.toStdString());
	outt << "initial ok";

}

void 
DihedralTesseWindow::on_actionCompute_triggered()
{

}


void 
DihedralTesseWindow::on_actionClear_triggered()
{
	poly1.clear();
	Poly2->hide();
	qDebug() << "clear!";

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
	}
	else
	{
		if (winRect[0].contains(temp))  ui.graphicsView->scale(1 + s_factor, 1 + s_factor);
		if (winRect[1].contains(temp))  ui.graphicsView2->scale(1 + s_factor, 1 + s_factor);
		if (winRect[2].contains(temp))  ui.graphicsView_2->scale(1 + s_factor, 1 + s_factor);
		if (winRect[3].contains(temp))  ui.graphicsView_3->scale(1 + s_factor, 1 + s_factor);
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


