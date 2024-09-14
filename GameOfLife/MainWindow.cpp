#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500)) {

	//passing this as parent, instantiating drawingpanel
	drawingPanel = new DrawingPanel(this);

	//intitiating vertical orientation
	sizer = new wxBoxSizer(wxVERTICAL);


	//Adding to sizer
	sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0);


	// setting sizer for mainwindow
	this->SetSizer(sizer);

	InitGameBoard();

	this->Bind(wxEVT_SIZE, &MainWindow::OnSizeChange, this);

}

MainWindow::~MainWindow(){}

void MainWindow::InitGameBoard() {

	gameBoard.resize(gridSize);

	for (int i = 0; i < gridSize; i++) {
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