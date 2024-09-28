#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "App.h"
#include <wx/numdlg.h>
#include "wx/filedlg.h"
#include "wx/wfstream.h"
#include "wx/textfile.h"

#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "settings.xpm"

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>




wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChange)
	EVT_MENU(ID_PLAY, MainWindow::OnPlay)
	EVT_MENU(ID_PAUSE, MainWindow::OnPause)
	EVT_MENU(ID_NEXT, MainWindow::OnNext)
	EVT_MENU(ID_CLEAR, MainWindow::OnClear)
	EVT_TIMER(wxID_ANY, MainWindow::OnTimer)
	EVT_MENU(ID_NEW, MainWindow::OnNew)
	EVT_MENU(ID_OPEN, MainWindow::OnOpen)
	EVT_MENU(ID_SAVE, MainWindow::OnSave)
	EVT_MENU(ID_SAVE_AS, MainWindow::OnSaveAs)
	EVT_MENU(ID_SETTINGS, MainWindow::OnSettings)
	EVT_MENU(ID_MENU_SETTINGS, MainWindow::OnMenuSettings)
	EVT_MENU(ID_SHOW_NEIGHBOR_COUNT, MainWindow::OnShowNeighborCount)
	EVT_MENU(ID_RANDOMIZE, MainWindow::OnRandomize)
	EVT_MENU(ID_RANDOMIZE_SEED, MainWindow::OnRandomizeWithSeed)
	EVT_MENU(ID_TOROIDAL, MainWindow::OnToggleBoardType)
	EVT_MENU(ID_FINITE, MainWindow::OnToggleBoardType)
	EVT_MENU(ID_RESET_SETTINGS, MainWindow::OnResetSettings)
	EVT_MENU(ID_EXIT, MainWindow::OnExit)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, App* app)	: wxFrame(nullptr, wxID_ANY, title, pos, size), app(app) {

	settings.Load();
	statusBar = this->CreateStatusBar();
	toolbar = CreateToolBar(wxHORIZONTAL, wxTB_DOCKABLE);
	timer = new wxTimer(this, wxID_ANY);
	generationCount = 0;
	livingCellsCount = 0;

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	toolbar->AddTool(ID_PLAY, "Play", playIcon);
	toolbar->AddTool(ID_PAUSE, "Pause", pauseIcon);
	toolbar->AddTool(ID_NEXT, "Next", nextIcon);
	toolbar->AddTool(ID_CLEAR, "Clear", trashIcon);
	
	//passing this as parent, instantiating drawingpanel
	drawingPanel = new DrawingPanel(this, gameBoard);
	drawingPanel->SetSettings(&settings);

	int cellSize = 20;  // Adjust based on your design
	int windowWidth = settings.gridSize * cellSize + this->GetSize().GetWidth(); // Add toolbar/menu width
	int windowHeight = settings.gridSize * cellSize + statusBar->GetSize().GetHeight(); // Add status bar height

	this->SetSize(windowWidth, windowHeight);

	//intitiating vertical orientation
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0);
	this->SetSizer(sizer);
	this->Layout();

	UpdateStatusBar();
	toolbar->Realize();

	CreateMenuBar();

	//intitalize game board
	InitGameBoard();

}

MainWindow::~MainWindow() { delete timer; }

void MainWindow::InitGameBoard() {
	//wxLogMessage("Initializing game board with grid size: %d", settings.gridSize);
	//gameBoard.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	gameBoard.resize(settings.gridSize);

	// Resize each row to the correct number of columns
	for (auto& row : gameBoard) {
		row.resize(settings.gridSize, false); // false means 'dead cell'
	}


	if (drawingPanel) {
		drawingPanel->SetGridSize(settings.gridSize);
		drawingPanel->Refresh();
	}

}

void MainWindow::RandomizeGrid(int seed) {
	std::srand(static_cast<unsigned int>(seed));

	// Iterate over the game board and assign random values
	for (int i = 0; i < settings.gridSize; ++i) {
		for (int j = 0; j < settings.gridSize; ++j) {
			// Assign true with a 30% probability
			gameBoard[i][j] = (std::rand() % 100) < 30;
		}
	}

	// Refresh the drawing panel to display the new random game board
	drawingPanel->Refresh();
}


void MainWindow::OnRandomize(wxCommandEvent& event) {
	wxLogMessage("Randomizing grid with current time seed");
	RandomizeGrid(static_cast<unsigned int>(std::time(NULL))); // Use current time as seed
}

void MainWindow::OnRandomizeWithSeed(wxCommandEvent& event) {
	long seed = wxGetNumberFromUser("Enter a seed value:", "Seed:", "Randomize with Seed", 0, 0, LONG_MAX, this);
	if (seed != -1) {  // wxGetNumberFromUser returns -1 if the user cancels
		wxLogMessage("Randomizing grid with seed: %ld", seed);
		RandomizeGrid(static_cast<int>(seed));
	}
}

void MainWindow::OnShowNeighborCount(wxCommandEvent& event) {
	if (gameBoard.empty()) {
		wxLogError("Error: Game board is empty, cannot show neighbor counts.");
		return;
	}

	// Calculate and set neighbor counts if needed
	auto counts = CalculateNeighborCounts();
	drawingPanel->SetNeighborCounts(counts);
	settings.showNeighborCount = event.IsChecked();
	drawingPanel->SetShowNeighborCount(settings.showNeighborCount);
	drawingPanel->Refresh();
}

void MainWindow::OnSettings(wxCommandEvent& event) {
	// Create and show the settings dialog
	OnMenuSettings(event);
}



void MainWindow::OnSizeChange(wxSizeEvent& event) {
	
	// getting size or new size of window
	wxSize newSize = this->GetSize();


	/*if (drawingPanel != nullptr) {
		drawingPanel->SetSize(newSize);
	}*/
	//setting new size for drawingpanel
	drawingPanel->SetPanelSize(newSize);

	this->Layout();

	// this allows for other events to happen
	event.Skip();

}

int MainWindow::CountNeighbors(int row, int col) {
	int count = 0;

	//for (int i = row - 1; i <= row + 1; ++i) {
	//	for (int j = col - 1; j <= col + 1; ++j) {
	//		// Skip the current cell
	//		if (i == row && j == col) {
	//			continue;
	//		}
	//		if (i >= 0 && i < settings.gridSize && j >= 0 && j < settings.gridSize) {
	//			if (gameBoard[i][j]) {
	//				count++;
	//			}
	//		}
	//	}
	//}

	for (int i = row - 1; i <= row + 1; ++i) {
		for (int j = col - 1; j <= col + 1; ++j) {
			if (i == row && j == col) {
				continue; // Skip the current cell
			}

			// Handle toroidal wrapping
			int neighborRow = (settings.boardType == Settings::BoardType::Toroidal) ? (i + settings.gridSize) % settings.gridSize : i;
			int neighborCol = (settings.boardType == Settings::BoardType::Toroidal) ? (j + settings.gridSize) % settings.gridSize : j;

			// Check bounds for finite board
			if (neighborRow >= 0 && neighborRow < settings.gridSize && neighborCol >= 0 && neighborCol < settings.gridSize) {
				if (gameBoard[neighborRow][neighborCol]) {
					count++;
				}
			}
		}
	}

	return count;
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

std::vector<std::vector<int>> MainWindow::CalculateNeighborCounts() {
	std::vector<std::vector<int>> counts(settings.gridSize, std::vector<int>(settings.gridSize, 0));

	for (int i = 0; i < settings.gridSize; ++i) {
		for (int j = 0; j < settings.gridSize; ++j) {
			counts[i][j] = CountNeighbors(i, j);
		}
	}
	return counts;
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

void MainWindow::NextGeneration() {
	/*std::vector<std::vector<bool>> newGameBoard(settings.gridSize, std::vector<bool>(settings.gridSize, false));
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

	auto counts = CalculateNeighborCounts();
	drawingPanel->SetNeighborCounts(counts);

	drawingPanel->Refresh();*/

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

	auto counts = CalculateNeighborCounts();
	drawingPanel->SetNeighborCounts(counts);
	drawingPanel->Refresh();
}

void MainWindow::OnTimer(wxTimerEvent& event) {
	NextGeneration();
}

void MainWindow::UpdateStatusBar() {
	wxString statusText = wxString::Format("Generations: %d | Living Cells: %d", generationCount, livingCellsCount);
	statusBar->SetStatusText(statusText);
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

	std::vector<std::vector<int>> emptyCounts(settings.gridSize, std::vector<int>(settings.gridSize, 0));
	drawingPanel->SetNeighborCounts(emptyCounts);

	// Refresh drawing panel
	drawingPanel->Refresh();
}

void MainWindow::OnNew(wxCommandEvent& event) {
	// Clear the game board (20x20 default)
	gameBoard = std::vector<std::vector<bool>>(20, std::vector<bool>(20, false));

	// Clear the file name
	currentFileName.clear();

	// Refresh the panel or UI component displaying the board (not shown here)
	drawingPanel->Refresh();  // Assuming you have a method to redraw the grid
}

void MainWindow::OnOpen(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, _("Open .cells file"), "", "",
		"Cells files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return; // User canceled
	}

	// Get the file name and load the game board from it
	currentFileName = openFileDialog.GetPath();
	LoadGameBoard(currentFileName);

	// Refresh the panel or UI component displaying the board
	drawingPanel->Refresh();
}

void MainWindow::OnSave(wxCommandEvent& event) {
	if (currentFileName.IsEmpty()) {
		OnSaveAs(event);  // If no file is selected, prompt for Save As
	}
	else {
		SaveGameBoard(currentFileName);
	}
}

void MainWindow::OnSaveAs(wxCommandEvent& event) {
	wxFileDialog saveFileDialog(this, _("Save .cells file"), "", "",
		"Cells files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return; // User canceled
	}

	// Save to the new file
	currentFileName = saveFileDialog.GetPath();
	SaveGameBoard(currentFileName);
}

void MainWindow::OnExit(wxCommandEvent& event) {
	Close(true); // Close the main window
}

void MainWindow::SaveGameBoard(const wxString& fileName) {
	wxTextFile file;

	// If file doesn't exist, create a new one
	if (!wxFileExists(fileName)) {
		file.Create(fileName);
	}
	else {
		file.Open(fileName);
		file.Clear();  // Clear the file before writing
	}

	// Write the game board to the file
	for (const auto& row : gameBoard) {
		wxString line;
		for (bool cell : row) {
			line += (cell ? '*' : '.');
		}
		file.AddLine(line);
	}

	file.Write();
	file.Close();
}

void MainWindow::LoadGameBoard(const wxString& fileName) {
	wxTextFile file(fileName);

	if (!file.Exists()) {
		wxLogError("File does not exist: %s", fileName);
		return;
	}

	file.Open();

	// Clear the current game board
	gameBoard.clear();

	for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
		if (line[0] == '!') {
			continue;  // Skip comment lines starting with '!'
		}

		std::vector<bool> row;
		for (wxChar c : line) {
			if (c == '*') {
				row.push_back(true);
			}
			else if (c == '.') {
				row.push_back(false);
			}
		}
		gameBoard.push_back(row);
	}

	file.Close();

	// Resize the grid if necessary (based on the file content size)
	int newSize = gameBoard.size();
	//update the grid size in settings
	settings.gridSize = newSize;

	// Refresh the panel or UI component displaying the board
	drawingPanel->Refresh();
}

void MainWindow::OnResetSettings(wxCommandEvent& event) {
	// Reset settings to default
	settings.ResetToDefault();

	// Update settings file
	app->UpdateSettings(settings);

	// Apply the updated settings to the game
	UpdateBasedOnSettings(settings);

	wxLogMessage("Settings have been reset to default.");
}

void MainWindow::OnToggleBoardType(wxCommandEvent& event) {
	wxMenuBar* menuBar = GetMenuBar();
	wxMenu* optionsMenu = menuBar->GetMenu(0);
	int id = event.GetId();
	if (id == ID_TOROIDAL) {
		settings.boardType = Settings::BoardType::Toroidal;
		// Uncheck the finite option if toroidal is checked
		optionsMenu->Check(ID_FINITE, false);
		optionsMenu->Check(ID_TOROIDAL, true);
	}
	else if (id == ID_FINITE) {
		settings.boardType = Settings::BoardType::Finite;
		// Uncheck the toroidal option if finite is checked
		optionsMenu->Check(ID_TOROIDAL, false);
		optionsMenu->Check(ID_FINITE, true);
	}

	// Apply updated settings
	UpdateBasedOnSettings(settings);
}

void MainWindow::UpdateBasedOnSettings(const Settings& settings) {
	gameBoard.resize(settings.gridSize, std::vector<bool>(settings.gridSize, false));

	// Update the drawing panel with the new grid size
	drawingPanel->SetGridSize(settings.gridSize);
	drawingPanel->Refresh();

	// Optionally, reset generation and living cell counts or other state
	generationCount = 0;
	livingCellsCount = 0;
	UpdateStatusBar();
}

void MainWindow::OnMenuSettings(wxCommandEvent& event) {
	SettingsDialog settingsDialog(this, &settings, drawingPanel);
	if (settingsDialog.ShowModal() == wxID_OK) {
		InitGameBoard();
		drawingPanel->SetGridSize(settings.gridSize);
		drawingPanel->Refresh();
		settings.Save();
		
	}
}

void MainWindow::ImportGameBoard(const wxString& fileName) {
	wxTextFile file(fileName);

	if (!file.Exists()) {
		wxLogError("File does not exist: %s", fileName);
		return;
	}

	file.Open();
	std::vector<std::vector<bool>> pattern; // Temporary storage for the imported pattern

	for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
		if (line[0] == '!') {
			continue; // Skip comment lines
		}

		std::vector<bool> row;
		for (wxChar c : line) {
			row.push_back(c == '*');
		}
		pattern.push_back(row);
	}

	file.Close();

	// Centering the pattern
	int patternRows = pattern.size();
	int patternCols = pattern.empty() ? 0 : pattern[0].size();

	// Determine starting positions to center the pattern
	int startRow = (settings.gridSize - patternRows) / 2;
	int startCol = (settings.gridSize - patternCols) / 2;

	// Import the pattern into the game board
	for (int i = 0; i < patternRows; ++i) {
		for (int j = 0; j < patternCols; ++j) {
			if (startRow + i >= 0 && startRow + i < settings.gridSize &&
				startCol + j >= 0 && startCol + j < settings.gridSize) {
				gameBoard[startRow + i][startCol + j] = pattern[i][j];
			}
		}
	}

	// Update generation and living cell counts
	livingCellsCount = CountLivingCells();
	UpdateStatusBar();
}

void MainWindow::OnImport(wxCommandEvent& event) {
	wxFileDialog importFileDialog(this, _("Import .cells file"), "", "",
		"Cells files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (importFileDialog.ShowModal() == wxID_CANCEL) {
		return; // User canceled
	}

	wxString fileName = importFileDialog.GetPath();
	ImportGameBoard(fileName);
	drawingPanel->Refresh(); // Refresh to display the updated board
}

void MainWindow::CreateMenuBar() {

	wxMenuBar* menuBar = new wxMenuBar();
	wxMenu* optionsMenu = new wxMenu();

	optionsMenu->Append(ID_NEW, "New\tCtrl+N", "Clear the game board and start fresh");
	optionsMenu->Append(ID_OPEN, "Open\tCtrl+O", "Open an existing .cells file");
	optionsMenu->Append(ID_IMPORT, "Import\tCtrl+I", "Import an existing game board");
	optionsMenu->Append(ID_SAVE, "Save\tCtrl+S", "Save the current game board");
	optionsMenu->Append(ID_SAVE_AS, "Save As", "Save the current game board to a new file");
	optionsMenu->Append(ID_MENU_SETTINGS, "Settings\tCtrl+S", "Open settings dialog");
	optionsMenu->Append(ID_RANDOMIZE, "Randomize", "Randomize the game board");
	optionsMenu->Append(ID_RANDOMIZE_SEED, "Randomize with Seed", "Randomize the game board with a seed");
	optionsMenu->AppendCheckItem(ID_SHOW_NEIGHBOR_COUNT, "Show Neighbor Count", "Display the number of living neighbors");
	optionsMenu->Check(ID_SHOW_NEIGHBOR_COUNT, settings.showNeighborCount);
	optionsMenu->AppendSeparator();
	optionsMenu->AppendCheckItem(ID_TOROIDAL, "Toroidal", "Use a toroidal board");
	optionsMenu->Check(ID_TOROIDAL, settings.boardType == Settings::BoardType::Toroidal);
	optionsMenu->AppendCheckItem(ID_FINITE, "Finite", "Use a finite board");
	optionsMenu->Check(ID_FINITE, settings.boardType == Settings::BoardType::Finite);
	optionsMenu->AppendSeparator();
	optionsMenu->Append(ID_RESET_SETTINGS, "Reset Settings");
	optionsMenu->Append(ID_EXIT, "Exit", "Exit the application");

	menuBar->Append(optionsMenu, "&Menu");



	this->SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &MainWindow::OnToggleBoardType, this, ID_FINITE);
	Bind(wxEVT_MENU, &MainWindow::OnToggleBoardType, this, ID_TOROIDAL);
	Bind(wxEVT_MENU, &MainWindow::OnImport, this, ID_IMPORT);
}