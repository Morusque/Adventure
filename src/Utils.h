
#pragma once

#include "ofMain.h"

namespace Utils
{

    std::vector<std::string> explode(const std::string & str, char separator);
    int hexToInt (string hex);
    float constrain(float orig, float min, float max);
    int str2int(const string &str);
    string& str_replace(const string &search, const string &replace, string &subject);
    ofPoint shorterFrom(vector<ofPoint> * _pt, float _x, float _y);
    string floatToStr(float f);
    string intToStr(int i);
    string intToStr(int i, int l);

};

