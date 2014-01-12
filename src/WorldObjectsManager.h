
#pragma once

#include "ofMain.h"
#include "WorldObject.h"
#include "XmlLoader.h"
#include "Behaviour.h"
#include "Scene.h"
#include "Global.h"

class WorldObjectsManager
{

    public:
        WorldObjectsManager();
        ~WorldObjectsManager();
        void loadObjects(XmlLoader * _xml);
        void draw(int _x, int _y, WorldObject * player, Scene * _scene);
        int getNbWorldObjects();
        WorldObject * getWorldObject(int i);
        void deleteWorldObject(int _i);
        WorldObject * addWorldObject(Behaviour * _act);
        void replaceWorldObject(Behaviour * _act, WorldObject * _toReplace);
        void setCarryingPos(Behaviour * _act, WorldObject * _obj);
        WorldObject * lookForPoolParam(Behaviour * _act);
        WorldObject * lookForWorldParam(Behaviour * _act);
        WorldObject * lookForWorldTarget(Behaviour * _act);
        WorldObject * lookForWorldId(string _str);
        void setGlobal(Global * _global);
        void move(Behaviour * _act, WorldObject * _obj, Scene*** _world);
        void fill(Behaviour * _act);
        void setSomething(WorldObject * _obj, Behaviour * _act);
        void moveTo(Behaviour * _act, WorldObject * _obj);
        void direct(WorldObject * _obj, Behaviour * _act, WorldObject * _player);
        void removeWorldObject(WorldObject * _obj);

    protected:

    private:

    vector<WorldObject*> m_worldObjects;
    vector<WorldObject*> m_poolObjects;

    Global * global;

};
