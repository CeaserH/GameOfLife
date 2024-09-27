#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include "Settings.h"
#include "DrawingPanel.h"

class SettingsDialog : public wxDialog {

private:
	wxSpinCtrl* gridSizeCtrl;
	wxSpinCtrl* intervalCtrl;
	wxColourPickerCtrl* livingCellColorPicker;
	wxColourPickerCtrl* deadCellColorPicker;

	Settings* settings;
	DrawingPanel* drawingPanel;

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	
	

	wxDECLARE_EVENT_TABLE();

public:
	SettingsDialog(wxWindow* parent, Settings* settings, DrawingPanel* drawingPanel);


};