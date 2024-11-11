//
// Created by deboom on 11/9/24.
//

#include "DrawPanel.h"

#include <wx/wx.h>

BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
    EVT_PAINT(DrawPanel::paintEvent)
END_EVENT_TABLE()

bool DrawPanel::can_generate() const {
    bool flag = true;
    flag = flag & !maze->bfsState.generate;
    flag = flag & !maze->dfsState.generate;
    flag = flag & !maze->randomDFSState.generate;
    flag = flag & !maze->randomDFSStateWalking.generate;

    return flag;
}

DrawPanel::DrawPanel(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
    this->maze = new Maze(50, 100, 28, 28);
    // maze->generateMazeRandomDFS();
}

DrawPanel::~DrawPanel() {
    delete maze;
}

void DrawPanel::reset_gen(wxCommandEvent &evt) {
    auto temp = maze;
    maze = new Maze(50, 100, 28, 28);
    delete temp;
}

void DrawPanel::rdfs_gen(wxCommandEvent &evt) const {
    maze->dfsState.message = "";
    maze->bfsState.message = "";
    if (can_generate()) {
        maze->randomDFSState.generate = true;
    } else {
        wxMessageBox("can't preform RDFS generate fast. already generating?",
                     "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawPanel::rdfs_gen_walk(wxCommandEvent &evt) const {
    maze->dfsState.message = "";
    maze->bfsState.message = "";
    if (can_generate()) {
        maze->randomDFSStateWalking.generate = true;
    } else {
        wxMessageBox("can't preform RDFS generate walk. already generating?",
                     "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawPanel::dfs_search(wxCommandEvent &evt) const {
    if (can_generate()) {
        maze->dfsState.generate = true;
    } else {
        wxMessageBox("can't preform DFS search. already generating?",
                     "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawPanel::bfs_search(wxCommandEvent &evt) const {
    if (can_generate()) {
        maze->bfsState.generate = true;
    } else {
        wxMessageBox("can't preform BFS search. already generating?",
                     "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawPanel::paintEvent(wxPaintEvent &evt) {
    wxPaintDC dc(this);
    render(dc);
}

void DrawPanel::paintNow() {
    wxPaintDC dc(this);
    render(dc);
}

void DrawPanel::render(wxDC &dc) const {
    static int y = 0;
    static int y_speed = 2;

    y += y_speed;
    if (y < 0) y_speed = 2;
    if (y > 200) y_speed = -2;

    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    if (maze->randomDFSState.generate) {
        maze->generateMazeRandomDFS();
    } else if (maze->randomDFSStateWalking.generate) {
        maze->generateMazeRandomDFSWalking();
    } else if (maze->dfsState.generate) {
        maze->generateDFSAnswer();
    } else if (maze->bfsState.generate) {
        maze->generateBFSAnswer();
    }

    dc.DrawText(maze->dfsState.message + "  " + maze->bfsState.message, 10, 10);


    maze->drawMaze(dc);
}
