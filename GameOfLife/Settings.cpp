#include "Settings.h"

void Settings::ResetToDefault() {
    // Reset each setting to its default value
    //isToroidal = false;
    showNeighborCount = false;
    gridSize = 15;
    interval = 50;  // default interval between generations in milliseconds

    // Add any other settings that should be reset to default

    // Save the default settings to the file
    Save();
}