#include "App.h"
#include <wx/gdicmn.h>
#include "MainWindow.h"


App::App() {

}

App::~App() {

}

bool App::OnInit() {

	wxString title = "Game of Life";
	wxPoint pos(0, 0);
	wxSize size(800, 600);

	mainWindow = new MainWindow(title, pos, size, this);
	mainWindow->SetSize(size);
	mainWindow->Show();

	return true;
}

void App::UpdateSettings(const Settings& settings) {
	if (mainWindow) {
		mainWindow->UpdateBasedOnSettings(settings);
	}
}