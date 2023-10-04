#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings {
	int GridSize = 15;
	int interval = 50;

	bool NeighborCount = true;
	bool showHUD = false;
	bool showGrid = true;
	bool showThickGrid = false;
	bool _isFinite = true;

	//Live Cell Color
	unsigned int LRed = 128;
	unsigned int LBlue = 128;
	unsigned int LGreen = 128;
	unsigned int LAlpha = 255;

	//Dead Cell Color
	unsigned int DRed = 255;
	unsigned int DBlue = 255;
	unsigned int DGreen = 255;
	unsigned int DAlpha = 255;

	//Dead Neighbor Font Color
	unsigned int NRed = 255;
	unsigned int NBlue = 0;
	unsigned int NGreen = 0;
	unsigned int NAlpha = 255;

	//Live Neighbor Font Color
	unsigned int NLRed = 0;
	unsigned int NLBlue = 0;
	unsigned int NLGreen = 255;
	unsigned int NLAlpha = 255;

	//Setters
	void SetLivingColor(wxColor color) {
		LRed = color.GetRed();
		LBlue = color.GetBlue();
		LGreen = color.GetGreen();
		LAlpha = color.GetAlpha();
	}
	void SetDeadColor(wxColor color) {
		DRed = color.GetRed();
		DBlue = color.GetBlue();
		DGreen = color.GetGreen();
		DAlpha = color.GetAlpha();
	}
	void SetNeighborFont(wxColor color)
	{
		NRed = color.GetRed();
		NBlue = color.GetBlue();
		NGreen = color.GetGreen();
		NAlpha = color.GetAlpha();
	}
	void SetNeighborLiveFont(wxColor color)
	{
		NLRed = color.GetRed();
		NLBlue = color.GetBlue();
		NLGreen = color.GetGreen();
		NLAlpha = color.GetAlpha();
	}

	//Getters
	wxColor GetLivingColor()
	{
		wxColor color(LRed, LGreen, LBlue, LAlpha);
		return color;
	}
	wxColor GetDeadColor()
	{
		wxColor color(DRed, DGreen, DBlue, DAlpha);
		return color;
	}
	wxColor GetNeighborFont()
	{
		wxColor color(NRed, NGreen, NBlue, NAlpha);
		return color;
	}
	wxColor GetNeighborLiveFont()
	{
		wxColor color(NLRed, NLGreen, NLBlue, NLAlpha);
		return color;
	}

	//Save the settings user customized
	void SaveData()
	{
		std::ofstream file;
		file.open("settings.bin", std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file.write((char*)this, sizeof(Settings));
			file.close();
		}
		else
		{
			wxMessageBox(wxT("Could not open Settings file to store data."));
		}
	}

	//The the users previously saved settings
	void LoadData()
	{
		std::ifstream file;
		file.open("settings.bin", std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file.read((char*)this, sizeof(Settings));
			file.close();
		}
		else
		{
			wxMessageBox(wxT("Could not find Settings file to load data."));
		}
		
		
	}

};