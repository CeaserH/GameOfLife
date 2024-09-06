#pragma once
#include "wx/wx.h"

class DrawingPanel : public wxPanel {

private:
	void OnPaint(wxPaintEvent& event);

public:
	DrawingPanel(wxWindow* parent);
	~DrawingPanel();

};

