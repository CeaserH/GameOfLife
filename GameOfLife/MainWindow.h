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

	int generationCount;
	int livingCellsCount;
	wxStatusBar* statusBar;

	void UpdateStatusBar();
	void UpdateGame();
	int CountLivingCells();

public:
	MainWindow();
	~MainWindow();

	void InitGameBoard();

	wxDECLARE_EVENT_TABLE();
};
