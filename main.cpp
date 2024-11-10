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
    MyFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(800,800))
    {
        auto* sizer = new wxBoxSizer(wxHORIZONTAL);
        drawPane = new DrawPanel( this );
        sizer->Add(drawPane, 1, wxEXPAND);
        SetSizer(sizer);

        timer = new RenderTimer(drawPane);
        Show();
        timer->start();
    }
    ~MyFrame() override
    {
        delete timer;
    }
    void onClose(wxCloseEvent& evt)
    {
        timer->Stop();
        evt.Skip();
    }
    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::onClose)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
    frame = new MyFrame();
    frame->Show();

    return true;
}
