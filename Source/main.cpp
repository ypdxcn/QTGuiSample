#include "QtGuiSample.h"
#include <QtWidgets/QApplication>
#include "main.h"


BaseValues* baseValues_;


void selectSystemLanguage()
{
	QString sysLocale = QLocale().name().toLower();

	if (sysLocale.startsWith("de"))
		baseValues.defaultLangFile = ":/Resources/Language/German.lng";
	else if (sysLocale.startsWith("fr"))
		baseValues.defaultLangFile = ":/Resources/Language/French.lng";
	else if (sysLocale.startsWith("zh"))
		baseValues.defaultLangFile = ":/Resources/Language/Chinese.lng";
	else if (sysLocale.startsWith("ru"))
		baseValues.defaultLangFile = ":/Resources/Language/Russian.lng";
	else if (sysLocale.startsWith("uk"))
		baseValues.defaultLangFile = ":/Resources/Language/Ukrainian.lng";
	else if (sysLocale.startsWith("pl"))
		baseValues.defaultLangFile = ":/Resources/Language/Polish.lng";
	else if (sysLocale.startsWith("nl"))
		baseValues.defaultLangFile = ":/Resources/Language/Dutch.lng";
	else if (sysLocale.startsWith("es"))
		baseValues.defaultLangFile = ":/Resources/Language/Spanish.lng";
	else if (sysLocale.startsWith("nb"))
		baseValues.defaultLangFile = ":/Resources/Language/Norwegian.lng";
	else if (sysLocale.startsWith("bg"))
		baseValues.defaultLangFile = ":/Resources/Language/Bulgarian.lng";
	else if (sysLocale.startsWith("cs"))
		baseValues.defaultLangFile = ":/Resources/Language/Czech.lng";
	else if (sysLocale.startsWith("tr"))
		baseValues.defaultLangFile = ":/Resources/Language/Turkish.lng";
	else if (sysLocale.startsWith("it"))
		baseValues.defaultLangFile = ":/Resources/Language/Italiano.lng";
	else
		baseValues.defaultLangFile = ":/Resources/Language/English.lng";

}

void BaseValues::Construct()
{
	/*
	forceDotInSpinBoxes = true;
	scriptsThatUseOrderBookCount = 0;
	trafficSpeed = 0;
	trafficTotal = 0;
	trafficTotalType = 0;
	currentExchange_ = nullptr;
	currentTheme = 0;
	gzipEnabled = true;
	appVerIsBeta = false;
	jlScriptVersion = 1.0;
	appVerStr = "1.40130";
	appVerReal = appVerStr.toDouble();

	if (appVerStr.size() > 4)
	{
		if (appVerStr.size() == 7)
			appVerStr.remove(6, 1);

		appVerStr.insert(4, ".");
	}

	appVerLastReal = appVerReal;

	logThread = nullptr;

	highResolutionDisplay = true;
	timeFormat = QLocale().timeFormat(QLocale::LongFormat).replace(" ", "").replace("t", "");
	dateTimeFormat = QLocale().dateFormat(QLocale::ShortFormat) + " " + timeFormat;
	depthCountLimit = 100;
	depthCountLimitStr = "100";
	uiUpdateInterval = 100;
	supportsUtfUI = true;
	debugLevel_ = 0;

#ifdef Q_WS_WIN

	if (QSysInfo::windowsVersion() <= QSysInfo::WV_XP)
		supportsUtfUI = false;

#endif

	upArrow = QByteArray::fromBase64("4oaR");
	downArrow = QByteArray::fromBase64("4oaT");

	if (baseValues.supportsUtfUI)
	{
		upArrowNoUtf8 = upArrow;
		downArrowNoUtf8 = downArrow;
	}
	else
	{
		upArrowNoUtf8 = ">";
		downArrowNoUtf8 = "<";
	}

	httpRequestInterval = 400;
	httpRequestTimeout = 5000;
	httpRetryCount = 5;
	apiDownCount = 0;
	groupPriceValue = 0.0;
	defaultHeightForRow_ = 22;
	*/
	selectSystemLanguage();

}

#include <openssl/hmac.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	baseValues_ = new BaseValues;
	baseValues_->Construct();

	baseValues.appThemeLight.palette = a.palette();
	baseValues.appThemeDark.palette = a.palette();
	baseValues.appThemeGray.palette = a.palette();

	baseValues.appThemeLight.loadTheme("Light");
	baseValues.appThemeDark.loadTheme("Dark");
	baseValues.appThemeGray.loadTheme("Gray");
	baseValues.appTheme = baseValues.appThemeDark;


	a.setStyleSheet(baseValues.appTheme.styleSheet);

	//baseValues.mainWindow_ = new QtGuiSample;
	//mainWindow.show();
	//baseValues.mainWindow_->setupClass();
	QtGuiSample w;
	w.show();
	return a.exec();
}
