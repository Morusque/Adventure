
#include "Descriptor.h"

Descriptor::Descriptor(string _option, string _value)
{
    option = _option;// part of the xml parameters before the colon
    value = _value;// part of the xml parameters after the colon
}

Descriptor::~Descriptor()
{
    //dtor
}

string Descriptor::o()
{
    return option;
}

string Descriptor::v()
{
    return value;
}
