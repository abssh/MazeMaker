//
// Created by deboom on 11/9/24.
//

#ifndef RENDERTIMER_H
#define RENDERTIMER_H
#include <wx/timer.h>

#include "DrawPanel.h"


class RenderTimer : public wxTimer
{
    DrawPanel* pane;
public:
    explicit RenderTimer(DrawPanel* pane);
    void Notify() override;
    void start();
};



#endif //RENDERTIMER_H
