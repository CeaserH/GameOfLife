#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChange)
	EVT_MENU(ID_PLAY, MainWindow::OnPlay)
	EVT_MENU(ID_PAUSE, MainWindow::OnPause)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(400, 400)) {


	statusBar = this->CreateStatusBar();
	toolbar = CreateToolBar(wxHORIZONTAL, wxTB_DOCKABLE);

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	toolbar->AddTool(ID_PLAY, "Play", playIcon);
	toolbar->AddTool(ID_PAUSE, "Pause", pauseIcon);
	toolbar->AddTool(ID_NEXT, "Next", nextIcon);
	toolbar->AddTool(ID_CLEAR, "Clear", trashIcon);
	
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
	toolbar->Realize();

	this->Layout();

}

MainWindow::~MainWindow(){}

void MainWindow::OnPlay(wxCommandEvent& event) {
	// Implementation for the OnPlay method
}

void MainWindow::OnPause(wxCommandEvent& event) {
	// Implementation for the OnPlay method
}

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


	if (drawingPanel != nullptr) {
		drawingPanel->SetSize(newSize);
	}
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



