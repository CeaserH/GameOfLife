#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard) : wxPanel(parent), gameBoard(gameBoard) {

	//set custome background render
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	//binding to onpaint
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
	this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnMouseUp, this);
}

DrawingPanel::~DrawingPanel() {}

void DrawingPanel::SetPanelSize(wxSize& newSize) {

	//setting size of panel
	this->SetSize(newSize);
	//gridSize = static_cast<int>(std::ceil(std::min(newSize.GetWidth(), newSize.GetHeight()) / 10.0));

	//refreshing panel
	this->Refresh();
}

void DrawingPanel::SetGridSize(int newGridSize) {
	if (!this) {
		std::cerr << "Error: DrawingPanel object is null!" << std::endl;
		return;
	}

	gridSize = newGridSize;

	this->Refresh();
}

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
	context->SetBrush(*wxBLUE);

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

			if (gameBoard[row][col])
			{
				context->SetBrush(*wxBLUE);  // Cell is alive (black)
			}
			else
			{
				context->SetBrush(*wxWHITE);  // Cell is dead (white)
			}

			context->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
	//clean up
	delete context;

}

void DrawingPanel::OnMouseUp(wxMouseEvent& event) {

	//get click position
	int mouseX = event.GetX();
	int mouseY = event.GetY();

	//get size of panel
	wxSize panelSize = this->GetSize();
	int panelWidth = panelSize.GetWidth();
	int panelHeight = panelSize.GetHeight();

	//calculate cell size based on panel and grid sizes
	int cellWidth = panelWidth / gridSize;
	int cellHeight = panelHeight / gridSize;

	//calulate row and column clicked
	int col = mouseX / cellWidth;
	int row = mouseY / cellHeight;

	//toggle if clicked cell
	if (row >= 0 && row < gridSize && col >= 0 && col < gridSize) {
		gameBoard[row][col] = !gameBoard[row][col];
	}

	this->Refresh();

	event.Skip();
}