#pragma once

#include "wx/wx.h"
#include "MainWindow.h"

class App : public wxApp
{
private:
	MainWindow* mainWindow;
	

public:
	void UpdateSettings(const Settings& settings);
	App();
	~App();
	virtual bool OnInit();
};
