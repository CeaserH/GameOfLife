#pragma once
#include "wx/wx.h"
#include "wx/event.h"
#include "DrawingPanel.h"
#include <vector>
#include "Settings.h"
#include "SettingsDialog.h"

class App;

class MainWindow : public wxFrame{

private:
	//panel for game grid
    App* app;
    wxStatusBar* statusBar;
    wxToolBar* toolbar;
    wxTimer* timer;
    wxMenuBar* menuBar;
    wxMenu* viewMenu;
    wxMenuItem* finiteMenuItem;
    wxMenuItem* toroidalMenuItem;
    std::vector<std::vector<bool>> gameBoard;
    DrawingPanel* drawingPanel;
    Settings settings;
    int generationCount;
    int livingCellsCount;
    int interval;
    // const int ID_IMPORT = wxNewId();

    void InitGameBoard();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateGame();
    
    void UpdateStatusBar();
    int CountNeighbors(int x, int y);
    
    void CreateMenuBar();
    void RandomizeGrid(int seed);
    std::vector<std::vector<int>> CalculateNeighborCounts();
    void UpdateViewMenuChecks();
    void CreateOptionsMenu();
    void ResetToDefault();

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
    void OnResetSettings(wxCommandEvent& event);
    void OnToggleBoardType(wxCommandEvent& event);
    
    void SaveGameBoard(const wxString& fileName);
    void LoadGameBoard(const wxString& fileName);
    void ImportGameBoard(const wxString& fileName);
    void OnImport(wxCommandEvent& event);
    void OnToggleShowGrid(wxCommandEvent& event);
    void OnToggleShow10x10Grid(wxCommandEvent& event);
    void OnToggleShowHUD(wxCommandEvent& event);

    wxString currentFileName;

    // Enum for menu and toolbar IDs
    enum {
        ID_PLAY = wxID_HIGHEST + 1,
        ID_PAUSE,
        ID_NEXT,
        ID_CLEAR,
        ID_NEW,
        ID_OPEN,
        ID_IMPORT,
        ID_SAVE,
        ID_SAVE_AS,
        ID_SETTINGS,
        ID_MENU_SETTINGS,
        ID_SHOW_NEIGHBOR_COUNT,
        ID_RANDOMIZE,
        ID_RANDOMIZE_SEED,
        ID_TOROIDAL,
        ID_FINITE,
        ID_RESET_SETTINGS,
        ID_EXIT,
        ID_SHOW_GRID,
        ID_SHOW_10x10_GRID,
        ID_VIEW_SHOW_HUD
    };

    wxDECLARE_EVENT_TABLE();

public:
    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, App* app);
	~MainWindow();
    void UpdateBasedOnSettings(const Settings& settings);
    void NextGeneration();
    int GetGenerationCount() const { return generationCount; };
    int CountLivingCells();
    

};
