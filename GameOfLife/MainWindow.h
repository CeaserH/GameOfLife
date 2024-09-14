#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include <vector>

class MainWindow : public wxFrame{

private:
	//panel for game grid
	DrawingPanel* drawingPanel;
	wxBoxSizer* sizer;
	int gridSize = 15;

	void OnSizeChange(wxSizeEvent& event);

	std::vector<std::vector<bool>> gameBoard;

public:
	MainWindow();
	~MainWindow();

	void InitGameBoard();

};

