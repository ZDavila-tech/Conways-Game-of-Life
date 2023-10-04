#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_PAINT(DrawingPanel::OnPaint)
	EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& board, Settings& settings, std::vector<std::vector<int>>& neighbors, int& gen)
	: wxPanel(parent, wxID_ANY, wxPoint(100, 100), wxSize(500, 500)), _board(board), _settings(settings), _neighbors(neighbors), _gen(gen)
{
	_parent = parent;
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetDoubleBuffered(true);
	this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnMouseUp, this);

}

DrawingPanel::~DrawingPanel()
{
	
}

//Draws the game board
void DrawingPanel::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	if (!gc) { return; }

	if (_settings.showGrid)
	{
		gc->SetPen(wxColor(0,0,0,20));
	}
	else
	{
		gc->SetPen(wxTransparentColor);
	}
	
	int x = 0;
	int y = 0;
	wxSize size = GetSize();
	SetSize(size);

	int width = size.GetWidth() / _settings.GridSize;
	int height = size.GetHeight() / _settings.GridSize;

	for (int i = 0; i < _board.size(); i++)
	{
		for (int j = 0; j < _board[i].size(); j++)
		{
			if (_board[j][i] == false)
			{
				gc->SetBrush(_settings.GetDeadColor());
			}
			else
			{
				gc->SetBrush(_settings.GetLivingColor());
				
			}
			gc->DrawRectangle(x, y, width, height);
			if (_neighbors[i][j] > 0 && _settings.NeighborCount==true)	//Displays the neighbor count
			{
				if (_neighbors[i][j] == 2 || _neighbors[i][j] == 3)
				{
					gc->SetFont(wxFontInfo(10), _settings.GetNeighborLiveFont());
				}
				else
				{
					gc->SetFont(wxFontInfo(10),_settings.GetNeighborFont());
				}
				wxString cellText = std::to_string(_neighbors[i][j]);
				double textWidth, textHeight;
				gc->GetTextExtent(cellText, &textWidth, &textHeight);
				gc->DrawText(cellText, (i * width) + width / 2 - textWidth / 2, (j * height) + height / 2 - textHeight / 2);
			}
			y += height;
		}
		y = 0;
		x += width;
	}
	if (_settings.showThickGrid)	//Draws the 10x10 grid
	{
		int solidlines = _settings.GridSize / 10;
		gc->SetPen(*wxBLACK);
		for (int i = 0; i < solidlines; i++)
		{
			wxPoint vStart(10*width * (i + 1), 0);
			wxPoint vEnd(10*width * (i + 1), this->GetSize().y);

			wxPoint hStart(0, 10*height * (i + 1));
			wxPoint hEnd(this->GetSize().x, 10*height * (i + 1));

			dc.DrawLine(vStart, vEnd);
			dc.DrawLine(hStart, hEnd);
		}
	}
	if (_settings.showHUD == true)	//Draws the HUD
	{
		gc->SetFont(wxFontInfo(11), *wxRED);
		wxString genText = "Number of generations: " + std::to_string(_gen);
		wxString boundText;
		if (_settings._isFinite)
		{
			boundText = "Boundary Type: Finite";
		}
		else
		{
			boundText = "Boundary Type: Toroidal";
		}
		wxString livText = "Number of living cells: " + std::to_string((*_lc));
		wxString uniText = "Universe Size: Width = " + std::to_string(_settings.GridSize) +" Height = " + std::to_string(_settings.GridSize);
		double textWidth1, textHeight1;
		double textWidth2, textHeight2;
		double textWidth3, textHeight3;
		double textWidth4, textHeight4;
		gc->GetTextExtent(genText, &textWidth1, &textHeight1);
		gc->GetTextExtent(livText, &textWidth2, &textHeight2);
		gc->GetTextExtent(boundText, &textWidth3, &textHeight3);
		gc->GetTextExtent(uniText, &textWidth4, &textHeight4);
		gc->DrawText(uniText, size.GetWidth()/2 - textWidth4, size.GetHeight() - textHeight4);
		gc->DrawText(boundText, size.GetWidth()/2 - textWidth3, size.GetHeight() - 2*textHeight3);
		gc->DrawText(livText, size.GetWidth()/2 - textWidth2, size.GetHeight() - 3*textHeight2);
		gc->DrawText(genText,size.GetWidth()/2- textWidth1, size.GetHeight() - 4*textHeight1);
	}
	delete gc;
}
//Set the Grid Size fucntions
void DrawingPanel::SetSize(wxSize& size)
{
	wxPanel::SetSize(size);
	Refresh();
}

void DrawingPanel::SetGridSize(int _grid)
{
	_settings.GridSize = _grid;
	_board.resize(_settings.GridSize);
	for (int i = 0; i < _settings.GridSize; i++)
	{
		_board[i].resize(_settings.GridSize);
		
	}
}
//Fill in the squares
void DrawingPanel::OnMouseUp(wxMouseEvent& event)
{

	int x = event.GetX();
	int y = event.GetY();

	wxSize size = GetSize();

	int width = size.GetWidth() / _settings.GridSize;
	int height = size.GetHeight() / _settings.GridSize;

	int c = x / width;
	int r = y / height;
	
	if (_board[r][c] == false)
	{
		_board[r][c] = true;
		(*_lc)++;
	}
	else {
		_board[r][c] = false;
		(*_lc)--;
	}
	_parent->Refresh();
	
}
//Set the living cells variable
void DrawingPanel::SetLC(int* lc)
{
	_lc = lc;
}