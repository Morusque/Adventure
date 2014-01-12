
#include "Global.h"

Global::Global()
{
}

Global::Global( ofTrueTypeFont _font,
                int _screenWidth,
                int _screenHeight,
                int _worldWidth,
                int _worldHeight,
                int _txtMargWidth,
                int _txtMargHeight,
                float _ratio)
{
    theFont = _font;
    m_screenWidth = _screenWidth;
    m_screenHeight = _screenHeight;
    m_txtMargHeight = _txtMargHeight;
    m_txtMargWidth = _txtMargWidth;
    m_fieldHeight = _screenHeight - (_txtMargHeight*2);
    m_fieldWidth = _screenWidth - (_txtMargWidth*2);
    m_worldWidth = _worldWidth;
    m_worldHeight = _worldHeight;
    m_ratio = _ratio;
}

Global::~Global()
{
    //dtor
}

void Global::setRatio(float _ratio) {
    m_ratio=_ratio;
}
