#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "settings.xpm"


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChange)
	EVT_MENU(ID_PLAY, MainWindow::OnPlay)
	EVT_MENU(ID_PAUSE, MainWindow::OnPause)
	EVT_MENU(ID_NEXT, MainWindow::OnNext)
	EVT_MENU(ID_CLEAR, MainWindow::OnClear)
	EVT_TIMER(wxID_ANY, MainWindow::OnTimer)
	EVT_MENU(ID_SETTINGS, MainWindow::OnSettings)
	EVT_MENU(ID_MENU_SETTINGS, MainWindow::OnMenuSettings)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(400, 400)) {

	settings.Load();
	statusBar = this->CreateStatusBar();
	toolbar = CreateToolBar(wxHORIZONTAL, wxTB_DOCKABLE);
	timer = new wxTimer(this, wxID_ANY);

	interval = 50;

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);
	wxBitmap settingsIcon(settings_xpm);

	toolbar->AddTool(ID_PLAY, "Play", playIcon);
	toolbar->AddTool(ID_PAUSE, "Pause", pauseIcon);
	toolbar->AddTool(ID_NEXT, "Next", nextIcon);
	toolbar->AddTool(ID_CLEAR, "Clear", trashIcon);
	toolbar->AddTool(ID_SETTINGS, "Settings", settingsIcon);
	
	//intitalize game board
	InitGameBoard();
	
	generationCount = 0;
	livingCellsCount = 0;
	
	//passing this as parent, instantiating drawingpanel
	drawingPanel = new DrawingPanel(this, gameBoard);

	drawingPanel->SetSettings(&settings);

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

	CreateMenuBar();

}

MainWindow::~MainWindow(){}

void MainWindow::CreateMenuBar() {

	wxMenuBar* menuBar = new wxMenuBar();

	wxMenu* optionsMenu = new wxMenu();
	optionsMenu->Append(ID_MENU_SETTINGS, "Settings\tCtrl+S", "Open settings dialog");

	menuBar->Append(optionsMenu, "&Options");

	this->SetMenuBar(menuBar);

}

void MainWindow::OnMenuSettings(wxCommandEvent& event) {
	SettingsDialog settingsDialog(this, &settings);
	if (settingsDialog.ShowModal() == wxID_OK) {
		InitGameBoard();
		drawingPanel->SetGridSize(settings.gridSize);
		drawingPanel->Refresh();
		settings.Save();
	}
}

void MainWindow::OnSettings(wxCommandEvent& event) {
	// Create and show the settings dialog
	OnMenuSettings(event);
}

void MainWindow::InitGameBoard() {

	gameBoard.resize(settings.gridSize);

	for (int i = 0; i < settings.gridSize; ++i)
	{
		gameBoard[i].resize(settings.gridSize, false);
	}

	drawingPanel->SetGridSize(settings.gridSize);
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
	std::vector<std::vector<bool>> newGameBoard(settings.gridSize, std::vector<bool>(settings.gridSize, false));

	for (int i = 0; i < settings.gridSize; ++i) {
		for (int j = 0; j < settings.gridSize; ++j) {
			int neighbors = CountNeighbors(i, j);
			if (gameBoard[i][j]) {
				if (neighbors == 2 || neighbors == 3) {
					newGameBoard[i][j] = true;
				}
			}
			else {
				if (neighbors == 3) {
					newGameBoard[i][j] = true;
				}
			}
		}
	}

	gameBoard = newGameBoard;

	generationCount++;
	livingCellsCount = CountLivingCells();
	UpdateStatusBar();
}

int MainWindow::CountLivingCells() {
	int count = 0;
	for (int i = 0; i < settings.gridSize; ++i) {
		for (int j = 0; j < settings.gridSize; ++j) {
			if (gameBoard[i][j]) {
				count++;
			}
		}
	}
	return count;
}

int MainWindow::CountNeighbors(int row, int col) {
	int count = 0;

	for (int i = row - 1; i <= row + 1; ++i) {
		for (int j = col - 1; j <= col + 1; ++j) {
			// Skip the current cell
			if (i == row && j == col) {
				continue;
			}
			if (i >= 0 && i < settings.gridSize && j >= 0 && j < settings.gridSize) {
				if (gameBoard[i][j]) {
					count++;
				}
			}
		}
	}

	return count;
}

void MainWindow::NextGeneration() {
	std::vector<std::vector<bool>> newGameBoard(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	int livingCount = 0;

	for (int i = 0; i < settings.gridSize; ++i) {
		for (int j = 0; j < settings.gridSize; ++j) {
			int neighbors = CountNeighbors(i, j);

			if (gameBoard[i][j]) {
				if (neighbors == 2 || neighbors == 3) {
					newGameBoard[i][j] = true;
					livingCount++;
				}
			}
			else {
				if (neighbors == 3) {
					newGameBoard[i][j] = true;
					livingCount++;
				}
			}
		}
	}

	gameBoard = newGameBoard;
	livingCellsCount = livingCount;

	generationCount++;
	UpdateStatusBar();

	drawingPanel->Refresh();
}
void MainWindow::OnPlay(wxCommandEvent& event) {
	timer->Start(interval);
}

void MainWindow::OnPause(wxCommandEvent& event) {
	timer->Stop();
}

void MainWindow::OnNext(wxCommandEvent& event) {
	NextGeneration();
}

void MainWindow::OnClear(wxCommandEvent& event) {
	// Clear the game board
	for (int i = 0; i < settings.gridSize; ++i) {
		for (int j = 0; j < settings.gridSize; ++j) {
			gameBoard[i][j] = false;
		}
	}

	// Reset counters
	generationCount = 0;
	livingCellsCount = 0;

	// Update status bar
	UpdateStatusBar();

	// Refresh drawing panel
	drawingPanel->Refresh();
}

void MainWindow::OnTimer(wxTimerEvent& event) {
	NextGeneration();
}

void MainWindow::UpdateStatusBar() {
	wxString statusText = wxString::Format("Generations: %d | Living Cells: %d", generationCount, livingCellsCount);
	statusBar->SetStatusText(statusText);
}



