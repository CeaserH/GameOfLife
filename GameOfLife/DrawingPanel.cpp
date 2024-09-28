#include "DrawingPanel.h"
#include "Settings.h"
#include "MainWindow.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include <wx/font.h>

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_PAINT(DrawingPanel::OnPaint)
	EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoardRef, MainWindow* mainWindow)
	: wxPanel(parent), gameBoard(gameBoardRef), mainWindow(mainWindow), showNeighborCount(false)  {

	//set custome background render
	Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	settings = nullptr;
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
	if (!settings) {
		std::cerr << "Error: Settings object is null!" << std::endl;
		return;
	}

	settings->gridSize = newGridSize;

	this->Refresh();
}

void DrawingPanel::SetNeighborCounts(const std::vector<std::vector<int>>& counts) {
	if (counts.size() != settings->gridSize || counts[0].size() != settings->gridSize) {
		std::cerr << "Error: Neighbor counts size mismatch!" << std::endl;
		return;
	}
	neighborCounts = counts;
	Refresh();
}

void DrawingPanel::SetShowNeighborCount(bool show) {
	showNeighborCount = show;
	Refresh();
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

void DrawingPanel::SetShowGrid(bool show) {
	settings->ShowGrid = show;
	Refresh();
}

void DrawingPanel::SetShow10x10Grid(bool show) {
	settings->Show10x10Grid = show;
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
	//context->SetPen(*wxBLACK);

	//setting panel size
	wxSize panelSize = this->GetSize();
	int panelWidth = panelSize.GetWidth();
	int panelHeight = panelSize.GetHeight();

	//panel size and grid size
	int cellWidth = panelWidth / settings->gridSize;
	int cellHeight = panelHeight / settings->gridSize;

	if (settings->ShowGrid) {
		context->SetPen(*wxBLACK); // Set pen color for grid lines

		// Draw vertical lines
		for (int i = 1; i < settings->gridSize; ++i) {
			int x = i * cellWidth;
			context->StrokeLine(x, 0, x, panelHeight); // Vertical lines
		}

		// Draw horizontal lines
		for (int i = 1; i < settings->gridSize; ++i) {
			int y = i * cellHeight;
			context->StrokeLine(0, y, panelWidth, y); // Horizontal lines
		}
	}

	// Draw 10x10 grid if enabled
	if (settings->Show10x10Grid) {
		context->SetPen(wxPen(wxColor(0, 0, 0), 2)); // Thicker pen for 10x10 grid
		for (int i = 10; i < settings->gridSize; i += 10) {
			int x = i * cellWidth;
			int y = i * cellHeight;
			context->StrokeLine(x, 0, x, panelHeight); // Thick vertical
			context->StrokeLine(0, y, panelWidth, y);  // Thick horizontal
		}
	}

	//looping to creating grid of rectangles
	for (int row = 0; row < settings->gridSize; ++row) {
		for (int col = 0; col < settings->gridSize; ++col) {
			int x = col * cellWidth;
			int y = row * cellHeight;

			context->SetBrush(gameBoard[row][col] ? settings->GetLivingCellColor() : settings->GetDeadCellColor());
			context->DrawRectangle(x, y, cellWidth, cellHeight);

			//wxLogMessage("Cell (%d, %d): %s", row, col, gameBoard[row][col] ? "Alive" : "Dead");

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


	if (settings && settings->ShowHUD) {
		//wxLogMessage("HUD is enabled"); // Debugging log

		wxFont font(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		context->SetFont(font, *wxBLACK); // Set font color to blue

		// Get current counts without advancing generations
		int livingCellsCount = mainWindow->CountLivingCells(); // Get living cells count
		int generationCount = mainWindow->GetGenerationCount();

		wxString hudText;
		hudText += wxString::Format("Generations: %d\n", generationCount);
		hudText += wxString::Format("Living Cells: %d\n", livingCellsCount);
		hudText += wxString::Format("Boundary Type: %s\n", settings->boardType == Settings::BoardType::Toroidal ? "Toroidal" : "Finite");
		hudText += wxString::Format("Universe Size: %d x %d\n", settings->gridSize, settings->gridSize);

		//wxLogMessage("HUD Text: %s", hudText); // Log the HUD text

		double textX, textY;
		context->GetTextExtent(hudText, &textX, &textY);
		double posX = 20; // Margin from left
		double posY = panelHeight - textY - 20; // Margin from bottom

		// Ensure position is within bounds
		if (posY < 0) posY = 0;

		context->DrawText(hudText, posX, posY);
	}
	//clean up
	//delete context;

}

