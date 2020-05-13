#include "MoxyboxLevelCreator.h"

MoxyboxLevelCreator::MoxyboxLevelCreator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.centralWidget->setLayout(layout.get());

	layout.get()->setMargin(0);
	layout.get()->addWidget(gameplayScreen.get(), 0, 0);

	gameplayScreen.get()->show();
}

void MoxyboxLevelCreator::closeEvent(QCloseEvent *event)
{
	if (gameplayScreen.get()->fileSaveInterruptPromptPassed())
	{
		gameplayScreen.get()->prefSave();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}