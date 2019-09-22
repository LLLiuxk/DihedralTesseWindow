/********************************************************************************
** Form generated from reading UI file 'dihedraltessewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIHEDRALTESSEWINDOW_H
#define UI_DIHEDRALTESSEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DihedralTesseWindowClass
{
public:
    QAction *actionLoadInput;
	QAction *actionCompute;
    QAction *actionSaveMiddle;
    QAction *actionClear;
    QWidget *centralWidget;
    QHBoxLayout *hboxLayout;
    QSplitter *splitter;
    QSplitter *splitter1;
    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView2;
    QSplitter *splitter2;
    QGraphicsView *graphicsView_2;
    QGraphicsView *graphicsView_3;
    QGraphicsView *graphicsView_4;
    QGraphicsView *graphicsView_5;
    QGraphicsView *graphicsView_6;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DihedralTesseWindowClass)
    {
        if (DihedralTesseWindowClass->objectName().isEmpty())
            DihedralTesseWindowClass->setObjectName(QStringLiteral("DihedralTesseWindowClass"));
        DihedralTesseWindowClass->resize(1600, 1000);
        actionLoadInput = new QAction(DihedralTesseWindowClass);
        actionLoadInput->setObjectName(QStringLiteral("actionLoadInput"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/fileNew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadInput->setIcon(icon);
		actionCompute = new QAction(DihedralTesseWindowClass);
		actionCompute->setObjectName(QStringLiteral("actionCompute"));
		actionCompute->setIcon(icon);
        actionSaveMiddle = new QAction(DihedralTesseWindowClass);
        actionSaveMiddle->setObjectName(QStringLiteral("actionSaveMiddle"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/fileOpen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveMiddle->setIcon(icon1);
        actionClear = new QAction(DihedralTesseWindowClass);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        actionClear->setIcon(icon);
        centralWidget = new QWidget(DihedralTesseWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        hboxLayout = new QHBoxLayout(centralWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setFocusPolicy(Qt::StrongFocus);
        splitter->setOrientation(Qt::Vertical);
        splitter1 = new QSplitter(splitter);
        splitter1->setObjectName(QStringLiteral("splitter1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(13);
        sizePolicy1.setHeightForWidth(splitter1->sizePolicy().hasHeightForWidth());
        splitter1->setSizePolicy(sizePolicy1);
        splitter1->setOrientation(Qt::Horizontal);
        graphicsView = new QGraphicsView(splitter1);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(2);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy2);
        graphicsView->setFocusPolicy(Qt::StrongFocus);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
        splitter1->addWidget(graphicsView);
        graphicsView2 = new QGraphicsView(splitter1);
        graphicsView2->setObjectName(QStringLiteral("graphicsView2"));
        graphicsView2->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(3);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(graphicsView2->sizePolicy().hasHeightForWidth());
        graphicsView2->setSizePolicy(sizePolicy3);
        graphicsView2->setFocusPolicy(Qt::StrongFocus);
        graphicsView2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView2->setTransformationAnchor(QGraphicsView::NoAnchor);
        splitter1->addWidget(graphicsView2);
        splitter->addWidget(splitter1);
        splitter2 = new QSplitter(splitter);
        splitter2->setObjectName(QStringLiteral("splitter2"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(7);
        sizePolicy4.setHeightForWidth(splitter2->sizePolicy().hasHeightForWidth());
        splitter2->setSizePolicy(sizePolicy4);
        splitter2->setOrientation(Qt::Horizontal);
        graphicsView_2 = new QGraphicsView(splitter2);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        splitter2->addWidget(graphicsView_2);
        graphicsView_3 = new QGraphicsView(splitter2);
        graphicsView_3->setObjectName(QStringLiteral("graphicsView_3"));
        graphicsView_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        splitter2->addWidget(graphicsView_3);
        graphicsView_4 = new QGraphicsView(splitter2);
        graphicsView_4->setObjectName(QStringLiteral("graphicsView_4"));
        splitter2->addWidget(graphicsView_4);
        graphicsView_5 = new QGraphicsView(splitter2);
        graphicsView_5->setObjectName(QStringLiteral("graphicsView_5"));
        splitter2->addWidget(graphicsView_5);
        graphicsView_6 = new QGraphicsView(splitter2);
        graphicsView_6->setObjectName(QStringLiteral("graphicsView_6"));
        splitter2->addWidget(graphicsView_6);
        splitter->addWidget(splitter2);

        hboxLayout->addWidget(splitter);

        DihedralTesseWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DihedralTesseWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        DihedralTesseWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DihedralTesseWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DihedralTesseWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DihedralTesseWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DihedralTesseWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionLoadInput);
		menu->addAction(actionCompute);
        menu->addAction(actionSaveMiddle);
        menu->addAction(actionClear);
        mainToolBar->addAction(actionLoadInput);
		mainToolBar->addAction(actionCompute);
        mainToolBar->addAction(actionSaveMiddle);
        mainToolBar->addAction(actionClear);

        retranslateUi(DihedralTesseWindowClass);

        QMetaObject::connectSlotsByName(DihedralTesseWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *DihedralTesseWindowClass)
    {
        DihedralTesseWindowClass->setWindowTitle(QApplication::translate("DihedralTesseWindowClass", "DihedralTesseWindow", 0));
        actionLoadInput->setText(QApplication::translate("DihedralTesseWindowClass", "Load Input", 0));
		actionCompute->setText(QApplication::translate("DihedralTesseWindowClass", "Compute", 0));
        actionSaveMiddle->setText(QApplication::translate("DihedralTesseWindowClass", "Save Middle", 0));
        actionClear->setText(QApplication::translate("DihedralTesseWindowClass", "Clear", 0));
        menu->setTitle(QApplication::translate("DihedralTesseWindowClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class DihedralTesseWindowClass: public Ui_DihedralTesseWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIHEDRALTESSEWINDOW_H
