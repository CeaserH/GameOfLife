#pragma once
#include "wx/wx.h"
#include <vector>
#include "Settings.h"

class DrawingPanel : public wxPanel {

private:
	void OnPaint(wxPaintEvent& event);
	void OnMouseUp(wxMouseEvent& event);
	Settings* settings;
	std::vector<std::vector<bool>>& gameBoard;

public:
	DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef);
	~DrawingPanel();

	void SetSettings(Settings* settings);
	void SetPanelSize(wxSize& newSize);
	void SetGridSize(int newGridSize);

	wxDECLARE_EVENT_TABLE();

};