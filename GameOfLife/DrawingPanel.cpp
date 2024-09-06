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

	//setting panel size
	wxSize panelSize = this->GetSize();
	int panelWidth = panelSize.GetWidth();
	int panelHeight = panelSize.GetHeight();

	//panel size and grid size
	int cellWidth = panelWidth / gridSize;
	int cellHeight = panelHeight / gridSize;


	//looping to creating grid of rectangles
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			int x = col * cellWidth;
			int y = row * cellHeight;

			context->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
	//clean up
	delete context;

}