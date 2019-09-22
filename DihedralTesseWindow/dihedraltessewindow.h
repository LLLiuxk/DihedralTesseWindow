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
	QGraphicsScene scene1;
	QGraphicsScene scene2;
	QGraphicsScene scene3;
	QPolygonF poly1;
	QPolygonF poly2;
	QPolygon poly3;
	QGraphicsPolygonItem *Poly1;
	QGraphicsPolygonItem *Poly2;

	QRect winRect[7];
	QPoint lastPoint;

	Tiling_opt *tiling_opt;



public Q_SLOTS:
	void on_actionLoadInput_triggered();
	void on_actionCompute_triggered();
	//void on_actionSaveMiddle_triggered();
	void on_actionClear_triggered();

	void openfile(QString);
	void winRectupdate();

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);
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
