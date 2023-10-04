#pragma once
#include "wx/wx.h"
#include "Settings.h"

class DrawingPanel : public wxPanel
{
private: 
	std::vector<std::vector<bool>>& _board;
	std::vector<std::vector<int>>& _neighbors;
	wxFrame* _parent = nullptr;
	int& _gen;
	int* _lc = nullptr;;
	wxPoint _point;
	Settings& _settings;
public:
	DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& board, Settings& settings, std::vector<std::vector<int>>& neighbors, int& gen);
	~DrawingPanel();
	void OnPaint(wxPaintEvent& event);
	void SetSize(wxSize& size);
	void SetGridSize(int _grid);
	void OnMouseUp(wxMouseEvent& event);
	void SetLC(int* lc);
	wxDECLARE_EVENT_TABLE();
};

