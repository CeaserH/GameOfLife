#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChange) // Resize event
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(400, 400)) {


	statusBar = this->CreateStatusBar();

	
	//intitalize game board
	InitGameBoard();
	
	generationCount = 0;
	livingCellsCount = 0;
	
	//passing this as parent, instantiating drawingpanel
	drawingPanel = new DrawingPanel(this, gameBoard);

	//intitiating vertical orientation
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);


	//Adding to sizer
	sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0);


	// setting sizer for mainwindow
	this->SetSizer(sizer);

	//REMOVED BECAUSE OF EVENT TABLE
	// this->Bind(wxEVT_SIZE, &MainWindow::OnSizeChange, this);

	UpdateStatusBar();

	this->Layout();

}

MainWindow::~MainWindow(){}

void MainWindow::InitGameBoard() {

	gameBoard.resize(gridSize);

	for (int i = 0; i < gridSize; ++i)
	{
		gameBoard[i].resize(gridSize, false);
	}

	drawingPanel->SetGridSize(gridSize);
}

void MainWindow::OnSizeChange(wxSizeEvent& event) {

	// getting size or new size of window
	wxSize newSize = this->GetSize();

	//setting new size for drawingpanel
	drawingPanel->SetPanelSize(newSize);

	this->Layout();

	// this allows for other events to happen
	event.Skip();

}

void MainWindow::UpdateGame() {

	generationCount++;
	livingCellsCount = CountLivingCells();

	UpdateStatusBar();
}

int MainWindow::CountLivingCells() {
	int count = 0;
	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			if (gameBoard[i][j]) {
				count++;
			}
		}
	}
	return count;
}

void MainWindow::UpdateStatusBar() {
	wxString statusText = wxString::Format("Generations: %d | Living Cells: %d", generationCount, livingCellsCount);
	statusBar->SetStatusText(statusText);
}





