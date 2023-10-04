#include "SettingsDialog.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_SPINCTRL(20000,SettingsDialog::SetTime)
	EVT_SPINCTRL(20001,SettingsDialog::SetGrid)
	EVT_COLOURPICKER_CHANGED(20002, SettingsDialog::SetDeadColor)
	EVT_COLOURPICKER_CHANGED(20003, SettingsDialog::SetLivingColor)
	EVT_COLOURPICKER_CHANGED(20004, SettingsDialog::SetNeighborDeadFont)
	EVT_COLOURPICKER_CHANGED(20005, SettingsDialog::SetNeighborLiveFont)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(Settings& settings) 
	: wxDialog(nullptr, wxID_ANY, "Settings", wxPoint(200,200), wxSize(400,400)), _settings(settings)
{
	boxSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(boxSizer);
	sizer = CreateButtonSizer(wxOK | wxCANCEL);
	sizer->SetMinSize(wxSize(100, 50));
	timeSetter = new wxSpinCtrl(this, 20000);
	wxStaticText* timeLabel = new wxStaticText(this, wxID_ANY, "Timer");
	gridsize = new wxSpinCtrl(this, 20001);
	wxStaticText* gridLabel = new wxStaticText(this, wxID_ANY, "Grid Size");
	deadPick = new wxColourPickerCtrl(this, 20002);
	wxStaticText* deadLabel = new wxStaticText(this, wxID_ANY, "Dead Cell Color");
	livePick = new wxColourPickerCtrl(this, 20003);
	wxStaticText* liveLabel = new wxStaticText(this, wxID_ANY, "Live Cell Color");

	deadfontPick = new wxColourPickerCtrl(this, 20004);
	wxStaticText* fontLabel = new wxStaticText(this, wxID_ANY, "Neighbor Count Dead Font Color");
	livefontPick = new wxColourPickerCtrl(this, 20005);
	wxStaticText* fontLiveLabel = new wxStaticText(this, wxID_ANY, "Neighbor Count Live Font Color");

	//Customize the Grid size
	wxBoxSizer* childSizer1 = new wxBoxSizer(wxHORIZONTAL);
	childSizer1->Add(gridLabel);
	childSizer1->Add(gridsize);
	gridLabel->SetMinSize(wxSize(100, 50));

	//Customize the timer
	wxBoxSizer* childSizer2 = new wxBoxSizer(wxHORIZONTAL);
	childSizer2->Add(timeLabel);
	childSizer2->Add(timeSetter);
	timeLabel->SetMinSize(wxSize(100, 50));

	//Customize the Dead Cell Color
	wxBoxSizer* childSizer3 = new wxBoxSizer(wxHORIZONTAL);
	childSizer3->Add(deadLabel);
	childSizer3->Add(deadPick);
	deadLabel->SetMinSize(wxSize(100, 50));

	//Customize the Live Cell Color
	wxBoxSizer* childSizer4 = new wxBoxSizer(wxHORIZONTAL);
	childSizer4->Add(liveLabel);
	childSizer4->Add(livePick);
	liveLabel->SetMinSize(wxSize(100, 50));

	//Customize the Dead Neighbor Font Color
	wxBoxSizer* childSizer5 = new wxBoxSizer(wxHORIZONTAL);
	childSizer5->Add(fontLabel);
	childSizer5->Add(deadfontPick);
	fontLabel->SetMinSize(wxSize(100, 50));

	//Customize the Live Neighbor Font Color
	wxBoxSizer* childSizer6 = new wxBoxSizer(wxHORIZONTAL);
	childSizer6->Add(fontLiveLabel);
	childSizer6->Add(livefontPick);
	fontLiveLabel->SetMinSize(wxSize(100, 50));

	boxSizer->Add(childSizer1);
	boxSizer->Add(childSizer2);
	boxSizer->Add(childSizer3);
	boxSizer->Add(childSizer4);
	boxSizer->Add(childSizer5);
	boxSizer->Add(childSizer6);
	boxSizer->Add(sizer);
	
	
}
SettingsDialog::~SettingsDialog()
{

}
//Customize the Grid size
void SettingsDialog::SetGrid(wxSpinEvent& event)
{
	_settings.GridSize = event.GetValue();
}
//Customize the timer
void SettingsDialog::SetTime(wxSpinEvent& event)
{
	_settings.interval = event.GetValue();
}
//Customize the Dead Cell Color
void SettingsDialog::SetDeadColor(wxColourPickerEvent& event)
{
	_settings.SetDeadColor(event.GetColour());
}
//Customize the Live Cell Color
void SettingsDialog::SetLivingColor(wxColourPickerEvent& event)
{
	_settings.SetLivingColor(event.GetColour());
}
//Customize the Dead Neighbor Font Color
void SettingsDialog::SetNeighborDeadFont(wxColourPickerEvent& event)
{
	_settings.SetNeighborFont(event.GetColour());
}
//Customize the Live Neighbor Font Color
void SettingsDialog::SetNeighborLiveFont(wxColourPickerEvent& event)
{
	_settings.SetNeighborLiveFont(event.GetColour());
}