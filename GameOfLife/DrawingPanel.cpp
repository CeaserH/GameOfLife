#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

	//set custome background render
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	//binding to onpaint
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
}

DrawingPanel::~DrawingPanel() {}

void DrawingPanel::OnPaint(wxPaintEvent& event) {

	//created to avoid flickering
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	//creating context for drawing
	wxGraphicsContext* context = wxGraphicsContext::Create(dc);

	//conditional for failing
	if (!context) {
		return;
	}

	//settings outline  color black
	context->SetPen(*wxBLACK);

	//setting fill to white
	context->SetBrush(*wxWHITE);

	//drawing a rectangle
	//context->DrawRectangle(50, 50, 100, 100);

	//grid size
	int cellSize = 10;


	//looping to creating 15x15
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			int x = col * cellSize;
			int y = row * cellSize;

			context->DrawRectangle(x, y, cellSize, cellSize);
		}
	}
	//clean up
	delete context;

}