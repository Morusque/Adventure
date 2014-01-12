
#include "Behaviour.h"

Behaviour::Behaviour(string _type, string _obj, string _carry, string _close, string _var, string _action, string _target, string _params)
{
    m_type = parseDescriptors(_type);
    m_obj = parseDescriptors(_obj);
    m_carry = parseDescriptors(_carry);
    m_close = parseDescriptors(_close);
    m_var = parseDescriptors(_var);
    m_action = parseDescriptors(_action);
    m_target = parseDescriptors(_target);
    m_params = parseDescriptors(_params);
}

Behaviour::~Behaviour()
{
    //dtor
}

vector<Descriptor> Behaviour::parseDescriptors(string _in) {
    vector<Descriptor> result;
    if (_in.length()==0) return result;
    vector<string> descriptors = Utils::explode(_in,';');
    for (int i=0 ; i<descriptors.size() ; i++) {
        vector<string> params = Utils::explode(descriptors.at(i),':');
        result.push_back(Descriptor(params.at(0),params.at(1)));
    }
    return result;
}

bool Behaviour::fulfilled(string _type, WorldObject * _obj, WorldObject * _close, vector<Variable> * _vars) {
    // return true if the behaviour condition is fulfilled
    WorldObject * _carry = _obj->getCarrying();
    if (_type.compare(m_type[0].o())!=0) return false;
    if (!fulfils(&m_obj, _obj, _obj)) return false;
    if (!fulfils(&m_carry, _obj, _carry)) return false;
    if (!fulfils(&m_close, _obj, _close)) return false;
    if (!fulfils(&m_var, _vars)) return false;
    return true;
}

bool Behaviour::fulfils(vector<Descriptor> * _descriptor,  vector<Variable> * _vars) {
    // return true if the variable condition is fulfilled
    int mode = 0;// 0 = && and 1 = ||
    bool totalFulfil = true;
    for (int i=0;i<_descriptor->size();i++) {
        if (_descriptor->at(i).o().compare("~")==0) {
            // 0 is the AND mode (returns true if everything is true)
            // 1 is the OR mode (returns true if at least one thing is true)
            if (_descriptor->at(i).v().compare("0")==0) mode=0;
            if (_descriptor->at(i).v().compare("1")==0) mode=1;
            if (mode==0) totalFulfil = true;
            if (mode==1) totalFulfil = false;
        } else {
            for (int j=0;j<_vars->size();j++) {
                if (_descriptor->at(i).o().compare(_vars->at(j).name)==0) {
                    bool thisFulfil = false;
                    if (_descriptor->at(i).v().substr(0,2).compare("==")==0)
                        if (_vars->at(j).value==Utils::str2int(_descriptor->at(i).v().substr(2))) thisFulfil = true;
                    if (_descriptor->at(i).v().substr(0,2).compare(">=")==0)
                        if (_vars->at(j).value>=Utils::str2int(_descriptor->at(i).v().substr(2))) thisFulfil = true;
                    if (_descriptor->at(i).v().substr(0,2).compare("<=")==0)
                        if (_vars->at(j).value<=Utils::str2int(_descriptor->at(i).v().substr(2))) thisFulfil = true;
                    if (_descriptor->at(i).v().substr(0,2).compare(">>")==0)
                        if (_vars->at(j).value>Utils::str2int(_descriptor->at(i).v().substr(2))) thisFulfil = true;
                    if (_descriptor->at(i).v().substr(0,2).compare("<<")==0)
                        if (_vars->at(j).value<Utils::str2int(_descriptor->at(i).v().substr(2))) thisFulfil = true;
                    if (_descriptor->at(i).v().substr(0,2).compare("!=")==0)
                        if (_vars->at(j).value!=Utils::str2int(_descriptor->at(i).v().substr(2))) thisFulfil = true;
                    if (mode==0) totalFulfil = totalFulfil && thisFulfil;
                    if (mode==1) totalFulfil = totalFulfil || thisFulfil;
                } else {
                    // ignore the variable if not the good name
                    // and don't do any boolean computation
                }
            }
        }
    }
    return totalFulfil;
}

bool Behaviour::fulfils(vector<Descriptor> * _descriptor,  WorldObject * _baseObj, WorldObject * _obj) {
    // return true if the behaviour condition is fulfilled
    int mode = 0;// 0 = && and 1 = ||
    bool totalFulfil = true;
    for (int i=0;i<_descriptor->size();i++) {
        if (_descriptor->at(i).o().compare("~")==0) {
            if (_descriptor->at(i).v().compare("0")==0) mode=0;
            if (_descriptor->at(i).v().compare("1")==0) mode=1;
            if (mode==0) totalFulfil = true;
            if (mode==1) totalFulfil = false;
            i++;
        }
        bool thisFulfil = false;
        if (_descriptor->at(i).o().compare("stucked")==0 &&
            _descriptor->at(i).v().compare("1")==0)
                if (_obj->getCurrentlyStucked()) thisFulfil = true;
        if (_descriptor->at(i).o().compare("#")==0 &&
            _descriptor->at(i).v().compare("else")==0)
                if (_baseObj != _obj) thisFulfil = true;
        if (_descriptor->at(i).o().compare("#")==0 &&
            _descriptor->at(i).v().compare("this")==0)
                if (_baseObj == _obj) thisFulfil = true;
        if (_descriptor->at(i).o().compare("#")==0 &&
            _descriptor->at(i).v().compare("any")==0) thisFulfil = true;
        if (_descriptor->at(i).o().compare("#")==0 &&
            _descriptor->at(i).v().compare("unused")==0) thisFulfil = true;
        if (_descriptor->at(i).o().compare("#")==0 &&
            _descriptor->at(i).v().compare("something")==0)
            if (_obj) thisFulfil = true;
        if (_descriptor->at(i).o().compare("#")==0 &&
            _descriptor->at(i).v().compare("null")==0)
            if (!_obj) thisFulfil = true;
        if (_descriptor->at(i).o().compare("class")==0)
            if (_obj)
                if (_obj->hasClass(_descriptor->at(i).v())) thisFulfil = true;
        if (_descriptor->at(i).o().compare("!class")==0)
            if (!_obj) thisFulfil = true;
            else if (!_obj->hasClass(_descriptor->at(i).v())) thisFulfil = true;
        if (_descriptor->at(i).o().compare("id")==0)
            if (_obj)
                if (_obj->getId().compare(_descriptor->at(i).v())==0) thisFulfil = true;
        if (_descriptor->at(i).o().compare("!id")==0)
            if (_obj)
                if (_obj->getId().compare(_descriptor->at(i).v())!=0) thisFulfil = true;
        if (_descriptor->at(i).o().compare("carried")==0)
            if (_obj)
                if((_descriptor->at(i).v().compare("0")==0 && !_obj->getCarried()) ||
                   (_descriptor->at(i).v().compare("1")==0 &&  _obj->getCarried()) ) thisFulfil = true;
        if (_descriptor->at(i).o().compare("carryClass")==0)
            if (_obj)
                if (_obj->getCarrying())
                    if(_obj->getCarrying()->hasClass(_descriptor->at(i).v())) thisFulfil = true;
        if (_descriptor->at(i).o().compare("carry")==0) {
            if (_obj) {
                if ( _obj->getCarrying() && _descriptor->at(i).v().compare("1")==0) thisFulfil = true;
                if (!_obj->getCarrying() && _descriptor->at(i).v().compare("0")==0) thisFulfil = true;
            }
        }
        if (mode==0) totalFulfil = totalFulfil && thisFulfil;
        if (mode==1) totalFulfil = totalFulfil || thisFulfil;
    }
    return totalFulfil;
}

vector<Descriptor> Behaviour::getType() {
    return m_type;
}

vector<Descriptor> Behaviour::getObj() {
    return m_obj;
}

vector<Descriptor> Behaviour::getCarry() {
    return m_carry;
}

vector<Descriptor> Behaviour::getClose() {
    return m_close;
}

vector<Descriptor> Behaviour::getVar() {
    return m_var;
}

vector<Descriptor> Behaviour::getAction() {
    return m_action;
}

vector<Descriptor> Behaviour::getTarget() {
    return m_target;
}

vector<Descriptor> Behaviour::getParams() {
    return m_params;
}
