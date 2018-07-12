#pragma once

#include "Global\apptheme.h"
#include "QtGuiSample.h"

#define appDataDir (baseValues_->appDataDir_)
#define mainWindow (*(baseValues_->mainWindow_))


struct BaseValues
{
	void Construct();
	//1.多主题
	AppTheme appThemeLight;
	AppTheme appThemeDark;
	AppTheme appThemeGray;
	AppTheme appTheme;

	//2.运行环境
	QString scriptFolder;//脚本目录
	QString themeFolder;//主题目录
	QString appDataDir_;//数据目录


	QString defaultLangFile;//软件系统默认语言
	//QtGuiSample * mainWindow_;
};




#define baseValues (*baseValues_)
extern BaseValues* baseValues_;