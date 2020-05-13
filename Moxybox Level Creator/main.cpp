#include "MoxyboxLevelCreator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MoxyboxLevelCreator w;
	a.setWindowIcon(QIcon(":/MoxyboxLevelCreator/Icon/moxybox_level_creator_program_icon.ico"));
	w.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
	w.show();
	return a.exec();
}
