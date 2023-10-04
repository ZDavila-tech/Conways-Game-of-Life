#include "MainWindow.h"
#include "DrawingPanel.h"
#include "SettingsDialog.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include <fstream>
#include <iostream>
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	//Icons for tool bar
	EVT_MENU(10000, MainWindow::OnPlay)
	EVT_MENU(10001, MainWindow::OnPause)
	EVT_MENU(10002, MainWindow::OnNext)
	EVT_MENU(10003, MainWindow::OnTrash)
	EVT_TIMER(15000, MainWindow::OnTimer)
	EVT_SIZE(MainWindow::OnSizeChange)
	//Options tab
	EVT_MENU(30000, MainWindow::Options)
	EVT_MENU(30001, MainWindow::Randomize)
	EVT_MENU(30002, MainWindow::RandomizeSeed)
	EVT_MENU(30003, MainWindow::ResetSettings)
	//File tab
	EVT_MENU(wxID_NEW, MainWindow::OnNew)
	EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
	EVT_MENU(wxID_SAVE, MainWindow::OnSave)
	EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	EVT_MENU(20011, MainWindow::OnImport)
	//View tab
	EVT_MENU(20005, MainWindow::OnFiniteClick)
	EVT_MENU(20006, MainWindow::OnTorodialClick)
	EVT_MENU(20007, MainWindow::OnNeighborClick)
	EVT_MENU(20008, MainWindow::OnHUD)
	EVT_MENU(20009, MainWindow::OnShowGrid)
	EVT_MENU(20010, MainWindow::OnThickGrid)
wxEND_EVENT_TABLE()


MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(100, 100), wxSize(600, 600))
{
	//setting up menu bar
	SetMenuBar(menuBar);

	//Setting up File tab
	file->Append(wxID_NEW);
	file->Append(wxID_OPEN);
	file->Append(20011, "Import");
	file->Append(wxID_SAVE);
	file->Append(wxID_SAVEAS);
	file->Append(wxID_EXIT);
	menuBar->Append(file, "File");

	//Setting up Option tab
	menu->Append(30000, "Settings");
	menu->Append(30001, "Randomize");
	menu->Append(30002, "Randomize with Seed");
	menu->Append(30003, "Reset Settings");
	menuBar->Append(menu, "Options");

	//Setting up view tab
	_finite = new wxMenuItem(view, 20005, "Finite", "", wxITEM_CHECK);
	_finite->SetCheckable(true);
	_torodial = new wxMenuItem(view, 20006, "Torodial", "", wxITEM_CHECK);
	_torodial->SetCheckable(true);
	showNeighbor = new wxMenuItem(view, 20007, "Show Neighbor Count", "", wxITEM_CHECK);
	showNeighbor->SetCheckable(true);
	showHUD = new wxMenuItem(view, 20008, "Show HUD", "", wxITEM_CHECK);
	showHUD->SetCheckable(true);
	showGrid = new wxMenuItem(view, 20009, "Show Grid", "", wxITEM_CHECK);
	showGrid->SetCheckable(true);
	showThickGrid = new wxMenuItem(view, 20010, "Show 10 x 10 Grid", "", wxITEM_CHECK);
	showThickGrid->SetCheckable(true);
	view->Append(_finite);
	view->Append(_torodial);
	view->Append(showNeighbor);
	view->Append(showHUD);
	view->Append(showGrid);
	view->Append(showThickGrid);
	menuBar->Append(view, "View");

	//Setting up tool bar
	tool = CreateToolBar();
	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);
	tool->AddTool(10000, "Play", playIcon);
	tool->AddTool(10001, "Pause", pauseIcon);
	tool->AddTool(10002, "Next", nextIcon);
	tool->AddTool(10003, "Trash", trashIcon);
	tool->Realize();

	//Loading previously saved data
	settings.LoadData();
	RefreshMenuItems();

	//Setting Up game board
	draw = new DrawingPanel(this, _board, settings, _neighbors, generation);

	boxSize = new wxBoxSizer(wxVERTICAL);
	boxSize->Add(draw, 1, wxEXPAND | wxALL);
	this->SetSizer(boxSize);

	_timer = new wxTimer(this, 15000);

	//Created Status Bar
	status = CreateStatusBar(2);
	UpdateStatusBar();

	//setting the living cells in the drawing panel
	draw->SetLC(&livingCells);

	//initializing grid
	this->Layout();
	GridInit();
}

MainWindow::~MainWindow()
{
	delete draw;
	delete _timer;
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
	wxSize size = GetSize();
	SetSize(size);
	event.Skip();
}

//Resizing the vectos for game board and neighbor counts
void MainWindow::GridInit()
{
	_board.resize(settings.GridSize);
	_neighbors.resize(settings.GridSize);
	for (int i = 0; i < settings.GridSize; i++)
	{
		_board[i].resize(settings.GridSize);
		_neighbors[i].resize(settings.GridSize);
	}
	draw->SetGridSize(settings.GridSize);
}

//Updating the numbers for generation and living cells for status bar
void MainWindow::UpdateStatusBar()
{
	std::string gen;
	gen = "Generations: " + std::to_string(generation);
	std::string living;
	living = "Living Cells: " + std::to_string(livingCells);
	status->SetStatusText(gen,0);
	status->SetStatusText(living, 1);
}

//GameBoard Functions
int MainWindow::NeighborCount(int row, int column)
{
	int count = 0;
	if (settings._isFinite)
	{
		for (int i = row - 1; i <= row + 1; i++)
		{
			for (int j = column - 1; j <= column + 1; j++)
			{
				if ((i == row && j == column) || (i < 0 || j < 0)
					|| (i >= _board.size() || j >= _board[i].size()))
				{
					continue;
				}
				if (_board[i][j] == true)
				{
					count++;
				}
			}
		}
	}
	else
	{
		for (int i = row-1; i <= row + 1; i++)
		{
			for (int j = column - 1; j <= column + 1; j++)
			{
				if (i < 0)
				{
					if (j < 0)
					{
						if (_board[settings.GridSize-1][settings.GridSize - 1] == true)
						{
							count++;
						}
					}
					else if (j > settings.GridSize - 1)
					{
						if (_board[settings.GridSize-1][0] == true)
						{
							count++;
						}
					}
					else if (_board[settings.GridSize - 1][j] == true)
					{
						count++;
					}
				}
				else if (j < 0)
				{
					if (i > settings.GridSize - 1)
					{
						if (_board[0][settings.GridSize - 1])
						{
							count++;
						}
					}
					else if (_board[i][settings.GridSize - 1] == true)
					{
						count++;
					}
				}
				else if (i > settings.GridSize-1)
				{
					if (j > settings.GridSize - 1)
					{
						if (_board[0][0] == true)
						{
							count++;
						}
					}
					else if (j < 0)
					{
						if (_board[0][settings.GridSize - 1])
						{
							count++;
						}
					}
					else if (_board[0][j] == true)
						{
							count++;
						}
				}
				else if (j > settings.GridSize - 1)
				{
					if (_board[i][0] == true)
					{
						count++;
					}
				}
				else if ((i == row && j == column))
				{
					continue;
				}
				else 
				{
					if (_board[i][j]==true)
					{
						count++;
					}
				}
			}
			
		}
	}
	return count;
}

void MainWindow::NextGeneration()
{
	std::vector<std::vector<bool>> sandbox;
	livingCells = 0;
	sandbox.resize(settings.GridSize);
	
	for (int i = 0; i < settings.GridSize; i++)
	{
		sandbox[i].resize(settings.GridSize);
	}

	for (int i = 0; i < settings.GridSize; i++)
	{
		for (int j = 0; j < settings.GridSize; j++)
		{
			int neighborCount = NeighborCount(i, j);
			if (_board[i][j] == true && (neighborCount == 2 || neighborCount == 3))
			{
				sandbox[i][j] = true;
				livingCells++;
				
			}
			else if (_board[i][j] == false && neighborCount == 3)
			{
				sandbox[i][j] = true;
				livingCells++;
	
			}
			else {
				sandbox[i][j] = false;
			}
		}
	}
	generation++;
	UpdateStatusBar();
	sandbox.swap(_board);

	Refresh();
}

//Tool Bar functions
void MainWindow::OnPlay(wxCommandEvent& event)
{
	_timer->Start(settings.interval);
}

void MainWindow::OnNext(wxCommandEvent& event)
{
	NextGeneration();
}

void MainWindow::OnPause(wxCommandEvent& event)
{
	_timer->Stop();
}

void MainWindow::OnTrash(wxCommandEvent& event)
{
	for (int i = 0; i < _board.size(); i++)
	{
		for (int j = 0; j < _board[i].size(); j++)
		{
				_board[i][j] = false;
				_neighbors[i][j] = 0;
		}
	}
	generation = 0;
	livingCells = 0;
	Refresh();
}

void MainWindow::OnTimer(wxTimerEvent& event)
{
	NextGeneration();
}

//Options Tab functions
void MainWindow::Options(wxCommandEvent& event)
{
	SettingsDialog dialog(settings);
	dialog.ShowModal();
	if (dialog.ShowModal() == wxID_OK)
	{
		GridInit();
		settings.SaveData();
		Refresh();
	}
	else {
		settings.LoadData();
	}
	
}
void MainWindow::RandGrid(int seed)
{
	if (seed == NULL)
	{
		srand(time(NULL));
	}
	else
	{
		srand(seed);
	}
	for (int i = 0; i < _board.size(); i++)
	{
		for (int j = 0; j < _board[i].size(); j++)
		{	
			int num = rand();
			if (num % 2 == 0)
			{
				_board[i][j] = true;
				livingCells++;
			}
			else
			{
				_board[i][j] = false;
			}
		}
	}
	Refresh();
}

void MainWindow::Randomize(wxCommandEvent& event)
{
	RandGrid(NULL);
}

void MainWindow::RandomizeSeed(wxCommandEvent& event)
{
	int seed = 0;
	seed = wxGetNumberFromUser("What number would you like to use as the seed?", "Enter number: ", "Seed", seed, 0, 100, NULL, wxPoint(100, 100));
	RandGrid(seed);
}

void MainWindow::ResetSettings(wxCommandEvent& event)
{
	showGrid->Check(true);
	_finite->Check(true);
	_torodial->Check(false);
	showHUD->Check(false);
	showThickGrid->Check(false);
	showNeighbor->Check(true);

	settings.LRed = 128;
	settings.LBlue = 128;
	settings.LGreen = 128;
	settings.LAlpha = 255;

	settings.DRed = 255;
	settings.DBlue = 255;
	settings.DGreen = 255;
	settings.DAlpha = 255;

	settings.NRed = 255;
	settings.NBlue = 0;
	settings.NGreen = 0;
	settings.NAlpha = 255;

	settings.NLRed = 0;
	settings.NLBlue = 0;
	settings.NLGreen = 255;
	settings.NLAlpha = 255;

	settings.GridSize = 15;
	settings.interval = 50;
	settings.showThickGrid = false;
	settings.showGrid = true;
	settings.showHUD = false;
	settings.NeighborCount = true;
	settings.SaveData();
}

//File tab functions
void MainWindow::OnNew(wxCommandEvent& event)
{
	if (!filename.empty())
	{
		filename.clear();
	}
	for (int i = 0; i < _board.size(); i++)
	{
		for (int j = 0; j < _board[i].size(); j++)
		{
			_board[i][j] = false;
			_neighbors[i][j] = 0;
		}
	}
	generation = 0;
	livingCells = 0;
	Refresh();
}
void MainWindow::OnOpen(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Open Game of Life", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	_board.clear();
	_board.resize(0);
	for (int i = 0; i < _board.size(); i++)
	{
		_board[i].clear();
		_board[i].resize(0);
	}
	std::string buffer;

	std::ifstream fileStream;
	int index = 0;
	fileStream.open(std::string(fileDialog.GetPath()));
	filename = std::string(fileDialog.GetPath());
	if (fileStream.is_open())
	{
		while (!fileStream.eof())
		{
				std::getline(fileStream, buffer);
				std::string comment = "!";
				if (buffer.size() == 0)
				{
					break;
				}
				for (int i = 0; i < buffer.size(); i++)
				{
					if (buffer[i] == '!')
					{
						buffer.replace(buffer.begin(), buffer.end(),"");
					}
				}
				if (buffer.size() == 0)
				{
					continue;
				}
				if (_board.size() == 0)
				{
					_board.resize(buffer.size());
				}
				_board[index].resize(buffer.size());
				for (int i = 0; i < buffer.size(); i++)
				{
					if (buffer[i] == '*')
					{
						_board[index][i] = true;
						livingCells++;
					}
					else
					{
						_board[index][i] = false;
					}
				}
			
			index++;
		}
		
		fileStream.close();
	}
	else
	{
		wxMessageBox(wxT("Could not find file to load."));
	}
	Refresh();
	event.Skip();
}

void MainWindow::OnImport(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Open Game of Life", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	std::string buffer;
	std::ifstream fileStream;
	int index = 0;
	fileStream.open(std::string(fileDialog.GetPath()));
	filename = fileDialog.GetFilename();
	if (fileStream.is_open())
	{
		while (!fileStream.eof())
		{
			std::getline(fileStream, buffer);
			if (buffer.size() == 0)
			{
				break;
			}
			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*')
				{
					_board[index][i] = true;
					livingCells++;
				}
				else {
					_board[index][i] = false;
				}
			}
			index++;
		}
		
		fileStream.close();
	}
	else
	{
		wxMessageBox(wxT("Could not import the pattern."));
	}
	Refresh();
	event.Skip();
}

void MainWindow::OnSave(wxCommandEvent& event)
{
	std::ofstream fileStream;
	if (filename.empty())
	{
		wxFileDialog fileDialog(this, "Save Game of Life", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (fileDialog.ShowModal() == wxID_CANCEL)
		{
			return;
		}
		
		fileStream.open(std::string(fileDialog.GetPath()));
	}
	else
	{
		fileStream.open(filename);
	}
	if (fileStream.is_open())
	{
		for (int i = 0; i < _board.size(); i++)
		{
			for (int j = 0; j < _board[i].size(); j++)
			{
				if (_board[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}

		fileStream.close();
	}
	else
	{
		wxMessageBox(wxT("Could not save."));
	}
	event.Skip();
}
void MainWindow::OnSaveAs(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Save Game of Life", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	
	std::ofstream fileStream;
	fileStream.open(std::string(fileDialog.GetPath()));
	if (fileStream.is_open())
	{
		for (int i = 0; i < _board.size(); i++)
		{
			for (int j = 0; j < _board[i].size(); j++)
			{
				if (_board[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}

		fileStream.close();
	}
	else
	{
		wxMessageBox(wxT("Could not save."));
	}
	event.Skip();
}
void MainWindow::OnExit(wxCommandEvent& event)
{
	this->Close();
}

//Event Handlers for View fucntions
void MainWindow::OnFiniteClick(wxCommandEvent& event)
{
	settings._isFinite = true;
	RefreshMenuItems();
	settings.SaveData();
	event.Skip();
}

void MainWindow::OnTorodialClick(wxCommandEvent& event)
{
	settings._isFinite = false;
	RefreshMenuItems();
	settings.SaveData();
	event.Skip();
}

void MainWindow::OnNeighborClick(wxCommandEvent& event)
{
	if (showNeighbor->IsChecked())
	{
		settings.NeighborCount = true;
		settings.SaveData();
	}
	else
	{
		settings.NeighborCount = false;
		settings.SaveData();
	}
	RefreshMenuItems();
	
}

void MainWindow::OnHUD(wxCommandEvent& event)
{
	if (showHUD->IsChecked())
	{
		settings.showHUD = true;
		settings.SaveData();
	}
	else
	{
		settings.showHUD = false;
		settings.SaveData();
	}
	RefreshMenuItems();
}

void MainWindow::OnShowGrid(wxCommandEvent& event)
{
	if (showGrid->IsChecked())
	{
		settings.showGrid = true;
		settings.SaveData();
	}
	else
	{
		settings.showGrid = false;
		settings.SaveData();
	}
	RefreshMenuItems();
}

void MainWindow::OnThickGrid(wxCommandEvent& event)
{
	if (showThickGrid->IsChecked())
	{
		settings.showThickGrid = true;
		settings.SaveData();
	}
	else
	{
		settings.showThickGrid = false;
		settings.SaveData();
	}
	RefreshMenuItems();
}

//Keep checked items updated with values in settings
void MainWindow::RefreshMenuItems()
{
	_finite->Check(settings._isFinite);
	_torodial->Check(!settings._isFinite);
	showHUD->Check(settings.showHUD);
	showGrid->Check(settings.showGrid);
	showThickGrid->Check(settings.showThickGrid);
	showNeighbor->Check(settings.NeighborCount);
}

//Updating the status bar and neighbor counts in real time
void MainWindow::Refresh(bool eraseBackground, const wxRect* rect)
{
	wxFrame::Refresh();
	UpdateStatusBar();
	for (int i = 0; i < settings.GridSize; i++)
	{
		for (int j = 0; j < settings.GridSize; j++)
		{
			int neighborCount = NeighborCount(j, i);
			_neighbors[i][j] = neighborCount;
		}
	}
}