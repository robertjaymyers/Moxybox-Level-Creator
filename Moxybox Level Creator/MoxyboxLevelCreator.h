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

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MoxyboxLevelCreator.h"
#include "CreationScreen.h"
#include <QGridLayout>
#include <QFontDatabase>

class MoxyboxLevelCreator : public QMainWindow
{
	Q_OBJECT

public:
	MoxyboxLevelCreator(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent *event);

private:
	Ui::MoxyboxLevelCreatorClass ui;

	QFontDatabase fontDatabase;

	std::unique_ptr<CreationScreen> gameplayScreen = std::make_unique<CreationScreen>(this);
	std::unique_ptr<QGridLayout> layout = std::make_unique<QGridLayout>();
};
