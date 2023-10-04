#pragma once
#include "wx/wx.h"
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"

class SettingsDialog : public wxDialog
{
private:
	Settings& _settings;
	wxBoxSizer* boxSizer = nullptr;
	wxSpinCtrl* gridsize = nullptr;
	wxSpinCtrl* timeSetter = nullptr;
	wxColourPickerCtrl* deadPick = nullptr;
	wxColourPickerCtrl* livePick = nullptr;
	wxColourPickerCtrl* deadfontPick = nullptr;
	wxColourPickerCtrl* livefontPick = nullptr;
	wxSizer* sizer = nullptr;
public:
	SettingsDialog(Settings& settings);
	~SettingsDialog();
	void SetGrid(wxSpinEvent& event);
	void SetTime(wxSpinEvent& event);
	void SetDeadColor(wxColourPickerEvent& event);
	void SetLivingColor(wxColourPickerEvent& event);
	void SetNeighborDeadFont(wxColourPickerEvent& event);
	void SetNeighborLiveFont(wxColourPickerEvent& event);
	wxDECLARE_EVENT_TABLE();
};