#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiSample.h"

class QtGuiSample : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiSample(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiSampleClass ui;
};
