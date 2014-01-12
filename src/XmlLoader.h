#pragma once

#include "ofxXmlSettings.h"
#include "Scene.h"
#include "WorldObject.h"
#include "Behaviour.h"
#include "Variable.h"
#include "Utils.h"
#include "AssetsPool.h"

class XmlLoader
{
    public:
        XmlLoader();
        virtual ~XmlLoader();

        int getWorldWidth();
        int getWorldHeight();
        float getPlayerSpeed();
        Scene * loadScene(int x, int y);
        int getObjectsLength(string _whereToLook);
        void goToRoot();
        int getGlobalVariable(string _name);
        void loadObjectAttributesIn(WorldObject * loadObject, int _index);
        void loadParametersFromThisLevel(vector<WorldObject*> * result, WorldObject * parentObj, string pool);
        vector<WorldObject*> loadObjects(string pool);
        vector<Behaviour> loadBehaviours();

        int directionStrToInt(string _dir);

    protected:

    private:

        ofxXmlSettings settings;
        int hexToInt(string hex);
        void pushTo(string _whereToLook);
        void loadParametersFromClass(WorldObject * loadObject, string className);
        void loadParameters(WorldObject * loadObject, int _index);
        AssetsPool assetsPool;

};

