#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_PAINT(DrawingPanel::OnPaint)
	EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard) : wxPanel(parent), gameBoard(gameBoard) {

	//set custome background render
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel() {}

void DrawingPanel::SetSettings(Settings* settings) {
	this->settings = settings;
}

void DrawingPanel::SetPanelSize(wxSize& newSize) {

	//setting size of panel
	this->SetSize(newSize);

	//refreshing panel
	this->Refresh();
}

void DrawingPanel::SetGridSize(int newGridSize) {
	if (!this) {
		std::cerr << "Error: DrawingPanel object is null!" << std::endl;
		return;
	}

	settings->gridSize = newGridSize;

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

	//setting panel size
	wxSize panelSize = this->GetSize();
	int panelWidth = panelSize.GetWidth();
	int panelHeight = panelSize.GetHeight();

	//panel size and grid size
	int cellWidth = panelWidth / settings->gridSize;
	int cellHeight = panelHeight / settings->gridSize;


	//looping to creating grid of rectangles
	for (int row = 0; row < settings->gridSize; ++row) {
		for (int col = 0; col < settings->gridSize; ++col) {
			int x = col * cellWidth;
			int y = row * cellHeight;

			context->SetBrush(gameBoard[row][col] ? settings->GetLivingCellColor() : settings->GetDeadCellColor());
			context->DrawRectangle(x, y, cellWidth, cellHeight);

			// If showing neighbor counts, draw them
			if (showNeighborCount && neighborCounts[row][col] > 0) {
				wxString countText = wxString::Format("%d", neighborCounts[row][col]);
				context->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxRED);
				double textX, textY;
				context->GetTextExtent(countText, &textX, &textY);
				context->DrawText(countText, x + (cellWidth - textX) / 2, y + (cellHeight - textY) / 2);
			}
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
	int cellWidth = panelWidth / settings->gridSize;
	int cellHeight = panelHeight / settings->gridSize;

	//calulate row and column clicked
	int col = mouseX / cellWidth;
	int row = mouseY / cellHeight;

	//toggle if clicked cell
	if (row >= 0 && row < settings->gridSize && col >= 0 && col < settings->gridSize) {
		gameBoard[row][col] = !gameBoard[row][col];
	}

	this->Refresh();

	event.Skip();
}

void DrawingPanel::SetNeighborCounts(const std::vector<std::vector<int>>& counts) {
	neighborCounts = counts;
	Refresh();
}

void DrawingPanel::SetShowNeighborCount(bool show) {
	showNeighborCount = show;
	Refresh();
}