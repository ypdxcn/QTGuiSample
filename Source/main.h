#pragma once

#include "Global\apptheme.h"
#include "QtGuiSample.h"

#define appDataDir (baseValues_->appDataDir_)
#define mainWindow (*(baseValues_->mainWindow_))


struct BaseValues
{
	void Construct();
	//1.������
	AppTheme appThemeLight;
	AppTheme appThemeDark;
	AppTheme appThemeGray;
	AppTheme appTheme;

	//2.���л���
	QString scriptFolder;//�ű�Ŀ¼
	QString themeFolder;//����Ŀ¼
	QString appDataDir_;//����Ŀ¼


	QString defaultLangFile;//���ϵͳĬ������
	//QtGuiSample * mainWindow_;
};




#define baseValues (*baseValues_)
extern BaseValues* baseValues_;