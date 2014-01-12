#pragma once

#include "ofMain.h"

class Global
{
    public:

        Global();
        Global( ofTrueTypeFont _font,
                int _screenWidth,
                int _screenHeight,
                int _worldWidth,
                int _worldHeight,
                int _txtMargWidth,
                int _txtMargHeight,
                float _ratio);
        virtual ~Global();
        void setRatio(float _ratio);
        ofTrueTypeFont theFont;
        int m_screenWidth;
        int m_screenHeight;
        int m_fieldHeight;
        int m_fieldWidth;
        int m_worldWidth;
        int m_worldHeight;
        int m_txtMargWidth;
        int m_txtMargHeight;
        float m_ratio;

    protected:
    private:
};
