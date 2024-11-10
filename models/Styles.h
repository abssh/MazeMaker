//
// Created by deboom on 11/10/24.
//

#ifndef STYLES_H
#define STYLES_H
#include <wx/colour.h>


struct Styles {
    static wxColor White() { return wxColor(255,255,255); }
    // static constexpr wxColor Black() { return wxColor(0,0,0); }
    static wxColor LightRed()  { return wxColor(255,85,190); }
    // static constexpr wxColor Green() { return wxColor(0,255,0); }
    // static constexpr wxColor Blue() { return wxColor(0,0,255); }
    // static constexpr wxColor Yellow() { return wxColor(255,255,0); }
    // static constexpr wxColor Purple() { return wxColor(0,255,255); }
    // static constexpr wxColor Magenta() { return wxColor(255,0,255); }
    static wxColor Grey10() { return wxColor(230,230,230); }
    static wxColor Grey70() { return wxColor(80,80,80); }

    /*
    static const wxColor Red = wxColor(255,0,0);
    static const wxColor Green = wxColor(0,255,0);
    static const wxColor Yellow = wxColor(255,255,0);
    static const wxColor Blue = wxColor(0,0,255);
    static const wxColor Magenta = wxColor(255,0,255);
    static const wxColor Cyan = wxColor(0,255,0);
    static const wxColor Grey10 = wxColor(230,230,230);
    static const wxColor Grey70 = wxColor(80,80,80);*/
};



#endif //STYLES_H
