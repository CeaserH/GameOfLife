#include "MainWindow.h"

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(500, 500)) {

	//passing this as parent, instantiating drawingpanel
	drawingPanel = new DrawingPanel(this);

}

MainWindow::~MainWindow(){}