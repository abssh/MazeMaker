#include "RenderTimer.h"

RenderTimer::RenderTimer(DrawPanel *pane)
{
    RenderTimer::pane = pane;
}

void RenderTimer::Notify()
{
    pane->Refresh();
}

void RenderTimer::start()
{
    wxTimer::Start(10);
}