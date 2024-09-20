#pragma once
#include <wx/colour.h>

struct Settings {

	unsigned int livingCellRed, livingCellGreen, livingCellBlue = 128;
	unsigned int livingCellAlpha = 255;

	unsigned int deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha = 255;

	unsigned int gridSize = 15;
	unsigned int interval = 50;

	wxColour GetLivingCellColor() const {
		return wxColour(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
	}

	wxColour GetDeadCellColor() const {
		return wxColour(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
	}

	void SetLivingCellColor(const wxColour& color) {
		livingCellRed = color.Red();
		livingCellGreen = color.Green();
		livingCellBlue = color.Blue();
		livingCellAlpha = color.Alpha();
	}

	void SetDeadCellColor(const wxColour& color) {
		deadCellRed = color.Red();
		deadCellGreen = color.Green();
		deadCellBlue = color.Blue();
		deadCellAlpha = color.Alpha();
	}
};