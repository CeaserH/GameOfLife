#pragma once
#include "wx/wx.h"
#include <vector>

class DrawingPanel : public wxPanel {

private:
	void OnPaint(wxPaintEvent& event);
	void OnMouseUp(wxMouseEvent& event);

	int gridSize = 15; // default grid size

	std::vector<std::vector<bool>>& gameBoard;

public:
	DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef);
	~DrawingPanel();

	void SetPanelSize(wxSize& newSize);

	void SetGridSize(int newGridSize);

};

