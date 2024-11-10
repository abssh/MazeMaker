//
// Created by deboom on 11/9/24.
//

#include "DrawPanel.h"

#include <wx/wx.h>

BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
    EVT_PAINT(DrawPanel::paintEvent)
END_EVENT_TABLE()

DrawPanel::DrawPanel(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
    this->maze = new Maze(100, 100, 20, 20);
    maze->generateMaze_DFS();
}

void DrawPanel::paintEvent(wxPaintEvent& evt) {
    wxPaintDC dc(this);
    render(dc);
}

void DrawPanel::paintNow() {
    wxPaintDC dc(this);
    render(dc);
}

void DrawPanel::render(wxDC &dc) {

    static int y = 0;
    static int y_speed = 2;

    y += y_speed;
    if(y<0) y_speed = 2;
    if(y>200) y_speed = -2;

    dc.SetBackground( *wxWHITE_BRUSH );
    dc.Clear();
    maze->drawMaze(dc);
}
