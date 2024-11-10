//
// Created by deboom on 11/9/24.
//

#ifndef DRAWPANEL_H
#define DRAWPANEL_H
#include <wx/wx.h>

#include "Maze.h"


class DrawPanel : public wxPanel {

public:
    explicit DrawPanel(wxFrame* parent);
    ~DrawPanel() override;

    Maze* maze;
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render(wxDC& dc) const;

    DECLARE_EVENT_TABLE()

};



#endif //DRAWPANEL_H
