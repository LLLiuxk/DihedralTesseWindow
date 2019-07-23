#include "dihedraltessewindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DihedralTesseWindow w;
	w.show();
	return a.exec();
}
