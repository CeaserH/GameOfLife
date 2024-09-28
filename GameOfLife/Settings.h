#pragma once
#include <wx/colour.h>
#include <fstream>

struct Settings {

	unsigned int livingCellRed, livingCellGreen, livingCellBlue = 128;
	unsigned int livingCellAlpha = 255;

	unsigned int deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha = 255;

	unsigned int gridSize = 15;
	unsigned int interval = 50;

	bool showNeighborCount = false;
	bool ShowGrid = true;
	bool Show10x10Grid = false;

	enum class BoardType { Finite, Toroidal };
	BoardType boardType;

	void ResetToDefault();

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

	void Load() {
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		if (file) {
			file.read((char*)this, sizeof(Settings));
			file.close();
		}
		file >> ShowGrid;
		file >> Show10x10Grid;
	}

	void Save() const {
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		if (file) {
			file.write((char*)this, sizeof(Settings));
			file.close();
		}

		file << ShowGrid << std::endl;
		file << Show10x10Grid << std::endl;
	}

};