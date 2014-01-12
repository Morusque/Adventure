#pragma once

#include "ofMain.h"
#include "Utils.h"

class Descriptor
{

public:

    Descriptor(string _option, string _value);
    ~Descriptor();

    string o();
    string v();

protected:

private:

    string option;
	string value;

};
