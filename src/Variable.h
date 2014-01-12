#pragma once

#include "ofMain.h"

class Variable
{
    public:

        Variable(string name);
        ~Variable();

        int value;
        string name;

        void add(int _i);
        void sub(int _i);
        void set(int _i);

    protected:
    private:

};
