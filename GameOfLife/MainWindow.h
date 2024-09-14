#pragma once
#include "wx/wx.h"
#include "wx/event.h"
#include "DrawingPanel.h"
#include <vector>

class MainWindow : public wxFrame{

private:
	//panel for game grid
    wxStatusBar* statusBar;
    wxToolBar* toolbar;
    wxTimer* timer;
    std::vector<std::vector<bool>> gameBoard;
    DrawingPanel* drawingPanel;
    int generationCount;
    int livingCellsCount;
    int interval;
    const int gridSize = 20;

    void InitGameBoard();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateGame();
    int CountLivingCells();
    void UpdateStatusBar();
    int CountNeighbors(int x, int y);
    void NextGeneration();

    // Event handlers
    void OnPlay(wxCommandEvent& event) {
        timer->Start(interval);
    }
    void OnPause(wxCommandEvent& event) {
        timer->Stop();
    }
    void OnNext(wxCommandEvent& event) {
        NextGeneration();
    }
    void OnClear(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    

    // Enum for menu and toolbar IDs
    enum {
        ID_PLAY = wxID_HIGHEST + 1,
        ID_PAUSE,
        ID_NEXT,
        ID_CLEAR
    };

    wxDECLARE_EVENT_TABLE();

public:
	MainWindow();
	~MainWindow();

};
