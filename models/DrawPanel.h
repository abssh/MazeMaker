//
// Created by deboom on 11/9/24.
//

#ifndef DRAWPANEL_H
#define DRAWPANEL_H
#include <wx/wx.h>

#include "Maze.h"

enum
{
    ID_RESET = 1,
    ID_RDFS_GEN = 2,
    ID_RDFS_GEN_WALK = 3,
    ID_DFS_SEARCH = 4,
    ID_BFS_SEARCH = 5
};


class DrawPanel : public wxPanel {
private:
    bool can_generate() const;

public:
    explicit DrawPanel(wxFrame* parent);
    ~DrawPanel() override;

    Maze* maze;
    void reset_gen(wxCommandEvent& evt);
    void rdfs_gen(wxCommandEvent &evt) const;
    void rdfs_gen_walk(wxCommandEvent &evt) const;

    void dfs_search(wxCommandEvent &evt) const;
    void bfs_search(wxCommandEvent &evt) const;

    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render(wxDC& dc) const;


    DECLARE_EVENT_TABLE()

};



#endif //DRAWPANEL_H
