#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
class MainWindow : public wxFrame
{
private:
	DrawingPanel* draw = nullptr;
	wxBoxSizer* boxSize = nullptr;
	Settings settings;
	std::vector<std::vector<bool>> _board;
	std::vector<std::vector<int>> _neighbors;
	wxTimer* _timer = nullptr;
	int generation = 0;
	int livingCells = 0;
	wxStatusBar* status;
	wxToolBar* tool;
	wxMenuBar* menuBar = new wxMenuBar;
	wxMenu* menu = new wxMenu;
	wxMenu* file = new wxMenu;
	wxMenu* view = new wxMenu;
	wxMenuItem* _finite = nullptr;
	wxMenuItem* _torodial = nullptr;
	wxMenuItem* showNeighbor = nullptr;
	wxMenuItem* showHUD = nullptr;
	wxMenuItem* showGrid = nullptr;
	wxMenuItem* showThickGrid = nullptr;
	bool _isNeighbor = true;
	std::string filename;


public:
	MainWindow();
	~MainWindow();
	void OnSizeChange(wxSizeEvent& event);
	void GridInit();
	void UpdateStatusBar();
	int NeighborCount(int row, int column);
	void NextGeneration();
	void Refresh(bool eraseBackground = true, const wxRect* rect = NULL);
	//Icons
	void OnPlay(wxCommandEvent& event);
	void OnPause(wxCommandEvent& event);
	void OnNext(wxCommandEvent& event);
	void OnTrash(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	//Options tab
	void Options(wxCommandEvent& event);
	void Randomize(wxCommandEvent& event);
	void RandomizeSeed(wxCommandEvent& event);
	void RandGrid(int seed);
	void ResetSettings(wxCommandEvent& event);
	//File tab
	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnImport(wxCommandEvent& event);
	//View Tab
	void OnFiniteClick(wxCommandEvent& event);
	void OnTorodialClick(wxCommandEvent& event);
	void RefreshMenuItems();
	void OnNeighborClick(wxCommandEvent& event);
	void OnHUD(wxCommandEvent& event);
	void OnShowGrid(wxCommandEvent& event);
	void OnThickGrid(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};

