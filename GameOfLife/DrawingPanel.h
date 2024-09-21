#pragma once
#include "wx/wx.h"
#include <vector>
#include "Settings.h"

class DrawingPanel : public wxPanel {

private:
	
	Settings* settings;
	std::vector<std::vector<bool>>& gameBoard;
	std::vector<std::vector<int>> neighborCounts;
	bool showNeighborCount;

protected:
	void OnPaint(wxPaintEvent& event);
	void OnMouseUp(wxMouseEvent& event);

public:
	DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef);
	~DrawingPanel();

	void SetSettings(Settings* settings);
	void SetPanelSize(wxSize& newSize);
	void SetGridSize(int newGridSize);
	void SetNeighborCounts(const std::vector<std::vector<int>>& counts);
	void SetShowNeighborCount(bool show);

	wxDECLARE_EVENT_TABLE();

};