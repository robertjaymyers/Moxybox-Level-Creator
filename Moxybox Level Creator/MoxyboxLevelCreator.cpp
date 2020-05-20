/*
This file is part of Moxybox Level Creator.
	Moxybox Level Creator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Moxybox Level Creator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with Moxybox Level Creator.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "MoxyboxLevelCreator.h"

MoxyboxLevelCreator::MoxyboxLevelCreator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	fontDatabase.addApplicationFont(":/MoxyboxLevelCreator/Fonts/Pixellari.ttf");

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