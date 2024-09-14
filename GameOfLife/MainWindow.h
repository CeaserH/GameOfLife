#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"

class MainWindow : public wxFrame{

private:
	//panel for game grid
	DrawingPanel* drawingPanel;
	wxBoxSizer* sizer;

	void OnSizeChange(wxSizeEvent& event);

public:
	MainWindow();
	~MainWindow();

};

