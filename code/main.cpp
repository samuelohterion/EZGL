#include "widget.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GL2DWidget w;
	w.show();

	return a.exec();
}
