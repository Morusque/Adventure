#pragma once

#include "ofMain.h"
#include "WorldObject.h"
#include "Utils.h"
#include "Descriptor.h"
#include "Variable.h"

class Behaviour
{

public:

    Behaviour(string _type, string _obj, string _carry, string _close, string _var, string _action, string _target, string _params);
    ~Behaviour();

    // TODO place each vector inside the descriptors and mimic the vector methods
    // also write methods like isTag("close")
    vector<Descriptor> getType();
    vector<Descriptor> getObj();
    vector<Descriptor> getCarry();
    vector<Descriptor> getClose();
    vector<Descriptor> getVar();

    vector<Descriptor> getAction();
    vector<Descriptor> getTarget();
    vector<Descriptor> getParams();

    vector<Descriptor> parseDescriptors(string _in);

    bool fulfilled(string _type, WorldObject * _obj, WorldObject * _close, vector<Variable> * _vars);
    bool fulfils(vector<Descriptor> * _descriptor, WorldObject * _baseObj, WorldObject * _obj);
    bool fulfils(vector<Descriptor> * _descriptor,  vector<Variable> * _vars);

protected:

private:

    // conditions
    vector<Descriptor> m_type;
    vector<Descriptor> m_obj;
    vector<Descriptor> m_carry;
    vector<Descriptor> m_close;
    vector<Descriptor> m_var;

    // action
    vector<Descriptor> m_action;
    vector<Descriptor> m_target;
    vector<Descriptor> m_params;


};
