#ifndef DIHEDRALTESSEWINDOW_H
#define DIHEDRALTESSEWINDOW_H

#include <cstdio>
#include <fstream>
#include <iostream>

#include <QtWidgets>
#include <QtGui>
#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog> 
#include <QDebug>
#include <QGraphicsItem>

#include "ui_dihedraltessewindow.h"
#include "TilingOpt.h"

using namespace std;
using namespace Tiling_tiles;

#define outt qDebug()

class DihedralTesseWindow : public QMainWindow
{
	Q_OBJECT

public:
	DihedralTesseWindow(QWidget *parent = 0);
	~DihedralTesseWindow();

private:
	QGraphicsScene scene;
	QGraphicsScene scene2;
	QGraphicsScene scene_2;
	QGraphicsScene scene_3;
	QGraphicsScene scene_4;
	QGraphicsScene scene_5;
	QGraphicsScene scene_6;
	QPolygonF poly1;
	QPolygonF poly2;

	QGraphicsPolygonItem *Poly1;
	QGraphicsPolygonItem *Poly2;
	QGraphicsPolygonItem *Poly3;
	QGraphicsPolygonItem *Poly4;
	QGraphicsPolygonItem *Poly5;

	QRect winRect[7];
	QPoint lastPoint;

	Tiling_opt *tiling_opt;

	QProgressDialog *pd;
	QTimer *t;

public Q_SLOTS:
	void on_actionLoadInput_triggered();
	void on_actionCompute_triggered();
	void on_actionMatchCandidate_triggered();
	void on_actionMorphing_triggered();
	//void on_actionSaveMiddle_triggered();
	void on_actionClear_triggered();

	void openfile(QString);
	void winRectupdate();

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	void resizeEvent(QResizeEvent *event);
	void moveEvent(QMoveEvent *event);
	//bool eventFilter(QObject *o, QEvent *e);


	void InitialData();


Q_SIGNALS:
	void changed();

private:
	Ui::DihedralTesseWindowClass ui;

};



#endif // DIHEDRALTESSEWINDOW_H
