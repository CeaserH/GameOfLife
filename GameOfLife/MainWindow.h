#pragma once
#include "wx/wx.h"
#include "wx/event.h"
#include "DrawingPanel.h"
#include <vector>
#include "Settings.h"
#include "SettingsDialog.h"

class MainWindow : public wxFrame{

private:
	//panel for game grid
    wxStatusBar* statusBar;
    wxToolBar* toolbar;
    wxTimer* timer;
    wxMenuBar* menuBar;
    std::vector<std::vector<bool>> gameBoard;
    DrawingPanel* drawingPanel;
    Settings settings;
    int generationCount;
    int livingCellsCount;
    int interval;

    void InitGameBoard();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateGame();
    int CountLivingCells();
    void UpdateStatusBar();
    int CountNeighbors(int x, int y);
    void NextGeneration();
    void CreateMenuBar();
    void RandomizeGrid(int seed);
    std::vector<std::vector<int>> CalculateNeighborCounts();

    // Event handlers
    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnMenuSettings(wxCommandEvent& event);
    void OnShowNeighborCount(wxCommandEvent& event);
    void OnRandomize(wxCommandEvent& event);
    void OnRandomizeWithSeed(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    void SaveGameBoard(const wxString& fileName);
    void LoadGameBoard(const wxString& fileName);

    wxString currentFileName;

    // Enum for menu and toolbar IDs
    enum {
        ID_PLAY = wxID_HIGHEST + 1,
        ID_PAUSE,
        ID_NEXT,
        ID_CLEAR,
        ID_NEW,
        ID_OPEN,
        ID_SAVE,
        ID_SAVE_AS,
        ID_SETTINGS,
        ID_MENU_SETTINGS,
        ID_SHOW_NEIGHBOR_COUNT,
        ID_RANDOMIZE,
        ID_RANDOMIZE_SEED,
        ID_EXIT
    };

    wxDECLARE_EVENT_TABLE();

public:
	MainWindow(const wxString& title);
	~MainWindow();

};
