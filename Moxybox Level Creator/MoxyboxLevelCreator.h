#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MoxyboxLevelCreator.h"
#include "CreationScreen.h"
#include <QGridLayout>

class MoxyboxLevelCreator : public QMainWindow
{
	Q_OBJECT

public:
	MoxyboxLevelCreator(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent *event);

private:
	Ui::MoxyboxLevelCreatorClass ui;

	std::unique_ptr<CreationScreen> gameplayScreen = std::make_unique<CreationScreen>(this);
	std::unique_ptr<QGridLayout> layout = std::make_unique<QGridLayout>();
};
