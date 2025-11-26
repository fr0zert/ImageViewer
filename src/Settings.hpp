#pragma once

// TODO:
// 1. Have init values for settings
// 2. Load settings from file
// 3. Change settings inside program
// 4. Save settings to file

class Settings {
public:
  Settings();
  ~Settings();

  bool LoadSettingsFromFile();
  bool SaveSettingsToFile();

private:
};
