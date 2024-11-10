#include <complex.h>
#include <wx/sizer.h>
#include <wx/wx.h>
#include <wx/timer.h>

#include "models/RenderTimer.h"


class MyFrame;

class MyApp: public wxApp
{
    bool OnInit() override;

    MyFrame* frame = nullptr;
public:

};

IMPLEMENT_APP(MyApp)

class MyFrame : public wxFrame
{
    RenderTimer* timer;
    DrawPanel* drawPane;

public:
    MyFrame() : wxFrame(nullptr, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(800,1000))
    {
        auto *menuGen = new wxMenu;
        menuGen->Append(ID_RESET, "&Reset","Resets the Maze");
        menuGen->AppendSeparator();
        menuGen->Append(ID_RDFS_GEN, "&RDFS fast", "RDFS algorithm fast");
        menuGen->AppendSeparator();
        menuGen->Append(ID_RDFS_GEN_WALK, "&RDFS walk", "RDFS algorithm walk");

        auto *menuSolve = new wxMenu;
        menuSolve->Append(ID_DFS_SEARCH, "&DFS", "DFS algorithm solve");
        menuSolve->AppendSeparator();
        menuSolve->Append(ID_BFS_SEARCH, "&BFS", "BFS algorithm solve");

        auto *menuBar = new wxMenuBar;
        menuBar->Append(menuGen, "&Gen");
        menuBar->Append(menuSolve, "&Solve");

        wxFrameBase::SetMenuBar(menuBar);

        auto* sizer = new wxBoxSizer(wxHORIZONTAL);
        drawPane = new DrawPanel( this );
        sizer->Add(drawPane, 1, wxEXPAND);
        SetSizer(sizer);

        timer = new RenderTimer(drawPane);
        wxTopLevelWindowGTK::Show();
        timer->start();
    }
    ~MyFrame() override
    {
        delete timer;
    }
    void onClose(wxCloseEvent& evt) {
        timer->Stop();
        evt.Skip();
    }
    void onRESET(wxCommandEvent& evt) {
        drawPane->reset_gen(evt);
    }
    void onRDFS_GEN(wxCommandEvent& evt) {
        drawPane->rdfs_gen(evt);
    }
    void onRDFS_GEN_WALK(wxCommandEvent& evt) {
        drawPane->rdfs_gen_walk(evt);
    }
    void onDFS_SEARCH(wxCommandEvent& evt) {
        drawPane->dfs_search(evt);
    }
    void onBFS_SEARCH(wxCommandEvent& evt) {
        drawPane->bfs_search(evt);
    }

    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::onClose)

EVT_MENU(ID_RESET, MyFrame::onRESET)
EVT_MENU(ID_RDFS_GEN, MyFrame::onRDFS_GEN)
EVT_MENU(ID_RDFS_GEN_WALK, MyFrame::onRDFS_GEN_WALK)
EVT_MENU(ID_DFS_SEARCH, MyFrame::onDFS_SEARCH)
EVT_MENU(ID_BFS_SEARCH, MyFrame::onBFS_SEARCH)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
    frame = new MyFrame();
    frame->Show();

    return true;
}
