#include "SettingsDialog.h"
#include <wx/sizer.h>

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
	EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()


SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings, DrawingPanel* drawingPanel)
    : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize), settings(settings), drawingPanel(drawingPanel) {

    // Main vertical box sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Grid size control
    wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* gridSizeLabel = new wxStaticText(this, wxID_ANY, "Grid Size:");
    gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY);
    gridSizeCtrl->SetRange(5, 100);
    gridSizeCtrl->SetValue(settings->gridSize);
    gridSizeSizer->Add(gridSizeLabel, 0, wxALL, 5);
    gridSizeSizer->Add(gridSizeCtrl, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(gridSizeSizer, 0, wxALL | wxEXPAND, 5);

    // Interval control
    wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* intervalLabel = new wxStaticText(this, wxID_ANY, "Interval:");
    intervalCtrl = new wxSpinCtrl(this, wxID_ANY);
    intervalCtrl->SetRange(10, 1000);
    intervalCtrl->SetValue(settings->interval);
    intervalSizer->Add(intervalLabel, 0, wxALL, 5);
    intervalSizer->Add(intervalCtrl, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(intervalSizer, 0, wxALL | wxEXPAND, 5);

    // Living cell color control
    wxBoxSizer* livingColorSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* livingColorLabel = new wxStaticText(this, wxID_ANY, "Living Cell Color:");
    livingCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY,
        wxColour(settings->livingCellRed, settings->livingCellGreen, settings->livingCellBlue, settings->livingCellAlpha));
    livingColorSizer->Add(livingColorLabel, 0, wxALL, 5);
    livingColorSizer->Add(livingCellColorPicker, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(livingColorSizer, 0, wxALL | wxEXPAND, 5);

    // Dead cell color control
    wxBoxSizer* deadColorSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* deadColorLabel = new wxStaticText(this, wxID_ANY, "Dead Cell Color:");
    deadCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY,
        wxColour(settings->deadCellRed, settings->deadCellGreen, settings->deadCellBlue, settings->deadCellAlpha));
    deadColorSizer->Add(deadColorLabel, 0, wxALL, 5);
    deadColorSizer->Add(deadCellColorPicker, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(deadColorSizer, 0, wxALL | wxEXPAND, 5);

    // OK and Cancel buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    // Set the main sizer
    SetSizerAndFit(mainSizer);
}

void SettingsDialog::OnOk(wxCommandEvent& event) {
    // Save the values from the controls to the settings object
    settings->gridSize = gridSizeCtrl->GetValue();
    settings->interval = intervalCtrl->GetValue();

    // Get color values from the color picker controls
    wxColour livingColor = livingCellColorPicker->GetColour();
    settings->livingCellRed = livingColor.Red();
    settings->livingCellGreen = livingColor.Green();
    settings->livingCellBlue = livingColor.Blue();
    settings->livingCellAlpha = livingColor.Alpha();

    wxColour deadColor = deadCellColorPicker->GetColour();
    settings->deadCellRed = deadColor.Red();
    settings->deadCellGreen = deadColor.Green();
    settings->deadCellBlue = deadColor.Blue();
    settings->deadCellAlpha = deadColor.Alpha();

    // Close the dialog and indicate OK was clicked
    EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& event) {
    // Close the dialog without saving changes
    EndModal(wxID_CANCEL);
}