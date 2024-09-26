#include "App.h"



App::App() {

}

App::~App() {

}

bool App::OnInit() {

	mainWindow = new MainWindow("");
	mainWindow->Show();

	return true;
}