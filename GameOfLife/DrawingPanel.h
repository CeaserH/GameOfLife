#pragma once
#include "wx/wx.h"

class DrawingPanel : public wxPanel {

private:
	void OnPaint(wxPaintEvent& event);
	int gridSize = 15; // default grid size

public:
	DrawingPanel(wxWindow* parent);
	~DrawingPanel();

};

