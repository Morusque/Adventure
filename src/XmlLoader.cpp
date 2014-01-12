
#include "XmlLoader.h"

XmlLoader::XmlLoader() {
    settings.loadFile("settings.xml");
}

XmlLoader::~XmlLoader() {
    //dtor
}

int XmlLoader::getWorldWidth() {
    goToRoot();
    return settings.getAttribute("settings:world", "width",  0, 0);
}

int XmlLoader::getWorldHeight() {
    goToRoot();
    return settings.getAttribute("settings:world", "height", 0, 0);
}

int XmlLoader::getGlobalVariable(string _name) {
    goToRoot();
    return settings.getAttribute("settings:global", _name, 0, 0);
}

float XmlLoader::getPlayerSpeed() {
    goToRoot();
    return settings.getAttribute("settings:player", "playerSpeed",  0.0, 0);
}

Scene * XmlLoader::loadScene(int co, int li) {
    int worldWidth = getWorldWidth();
    int worldHeight = getWorldHeight();
    Scene * thisScene;
    goToRoot();
    settings.pushTag("settings");
    settings.pushTag("world");
    int numberOfScenes = settings.getNumTags("scene");
    for (int i=0;i<numberOfScenes;i++) {
        int thisX = settings.getAttribute("scene", "x", 0, i);
        int thisY = settings.getAttribute("scene", "y", 0, i);
        if (co == thisX && li == thisY) {
            ofImage * thisBgPicture = assetsPool.requestImg(settings.getAttribute("scene", "bgPicture", "", i));
            ofSoundPlayer * thisSndLoop = assetsPool.requestSnd(settings.getAttribute("scene", "sndLoop", "", i));
            thisSndLoop->setVolume(0.9);
            string thisName = settings.getAttribute("scene", "name", "", i);
            thisScene = new Scene(co, li, thisBgPicture, thisName, thisSndLoop);
            thisScene->setLink(0,ofPoint((thisX+0+worldWidth)%worldWidth,(thisY-1+worldHeight)%worldHeight));
            thisScene->setLink(1,ofPoint((thisX+1+worldWidth)%worldWidth,(thisY+0+worldHeight)%worldHeight));
            thisScene->setLink(2,ofPoint((thisX+0+worldWidth)%worldWidth,(thisY+1+worldHeight)%worldHeight));
            thisScene->setLink(3,ofPoint((thisX-1+worldWidth)%worldWidth,(thisY+0+worldHeight)%worldHeight));
            thisScene->setStuck(0,false);
            thisScene->setStuck(1,false);
            thisScene->setStuck(2,false);
            thisScene->setStuck(3,false);
            settings.pushTag("scene",i);
            int numberOfBorders = settings.getNumTags("border");
            for (int border=0 ; border<numberOfBorders ; border++) {
                int thisDirection = directionStrToInt(settings.getAttribute("border", "direction", "", border));
                if (settings.attributeExists("border", "linkX", border))
                    thisScene->setLinkX(thisDirection,settings.getAttribute("border", "linkX", 0, border));
                if (settings.attributeExists("border", "linkY", border))
                    thisScene->setLinkY(thisDirection,settings.getAttribute("border", "linkY", 0, border));
                if (settings.getAttribute("border", "stuck", "false", border).compare("true")==0)
                    thisScene->setStuck(thisDirection,true);
                if (settings.getAttribute("border", "hide", "false", border).compare("true")==0)
                    thisScene->setHide(thisDirection,true);
            }
            settings.popTag();
        }
    }
    return thisScene;
}

int XmlLoader::directionStrToInt(string dir) {
    if (dir.compare("up")==0) return 0;
    if (dir.compare("right")==0) return 1;
    if (dir.compare("down")==0) return 2;
    if (dir.compare("left")==0) return 3;
    return -1;
}

void XmlLoader::goToRoot() {
    while (settings.getPushLevel()>0) settings.popTag();
}

int XmlLoader::getObjectsLength(string _whereToLook) {
    pushTo(_whereToLook);
    return(settings.getNumTags("object"));
}

vector<WorldObject*> XmlLoader::loadObjects(string _pool) {
    pushTo("objects");
    WorldObject* thisObj = new WorldObject();
    vector<WorldObject*> result;
    loadParametersFromThisLevel(&result, thisObj, _pool);
    return result;
}

void XmlLoader::loadParametersFromThisLevel(vector<WorldObject*> * result, WorldObject * parentObj, string pool) {
    int numberOfObjects = settings.getNumTags("object");
    for (int i=0;i<numberOfObjects;i++) {
        WorldObject * thisObj = new WorldObject(parentObj);
        loadObjectAttributesIn(thisObj, i);
        settings.pushTag("object",i);
        loadParametersFromThisLevel(result,thisObj,pool);
        settings.popTag();
        if (thisObj->getPool().compare(pool)==0) result->push_back(thisObj);
        else delete thisObj;
    }
}

void XmlLoader::loadObjectAttributesIn(WorldObject * loadObject, int _index) {
    if (settings.attributeExists("object", "sceneX", _index))
        loadObject->setScenePosX(settings.getAttribute("object", "sceneX", 0, _index));
    if (settings.attributeExists("object", "sceneY", _index))
        loadObject->setScenePosY(settings.getAttribute("object", "sceneY", 0, _index));
    if (settings.attributeExists("object", "posX", _index))
        loadObject->setScreenPosX(settings.getAttribute("object", "posX",  0, _index));
    if (settings.attributeExists("object", "posY", _index))
        loadObject->setScreenPosY(settings.getAttribute("object", "posY",  0, _index));
    if (settings.attributeExists("object", "picture", _index))
        loadObject->setPicture(assetsPool.requestImg(settings.getAttribute("object", "picture", "", _index)));
    if (settings.attributeExists("object", "name", _index))
        loadObject->setScreenName(settings.getAttribute("object", "name", "", _index));
    if (settings.attributeExists("object", "id", _index))
        loadObject->setId(settings.getAttribute("object", "id", "", _index));
    if (settings.attributeExists("object", "class", _index))
        loadObject->addClass(settings.getAttribute("object", "class", "", _index));
    if (settings.attributeExists("object", "pool", _index))
        loadObject->setPool(settings.getAttribute("object", "pool", "", _index));
    if (settings.attributeExists("object", "logic", _index))
        loadObject->setLogic(settings.getAttribute("object", "logic", true, _index));
}

void XmlLoader::pushTo(string _whereToLook) {
    goToRoot();
    settings.pushTag("settings");
    vector<string> subs = Utils::explode(_whereToLook, '/');
    for (int i=0;i<subs.size();i++) {
        settings.pushTag(subs.at(i));
    }
}

vector<Behaviour> XmlLoader::loadBehaviours() {
    pushTo("behaviours");
    vector<Behaviour> result;
    int nbBehaviours = settings.getNumTags("behaviour");
    for (int i=0;i<nbBehaviours;i++) {
        string thisType = settings.getAttribute("behaviour", "type", "", i);
        string thisObj = settings.getAttribute("behaviour", "obj", "", i);
        string thisCarry = settings.getAttribute("behaviour", "carry", "", i);
        string thisClose = settings.getAttribute("behaviour", "close", "", i);
        string thisVar = settings.getAttribute("behaviour", "var", "", i);
        settings.pushTag("behaviour",i);
        int nbActions = settings.getNumTags("action");
        for (int j=0;j<nbActions;j++) {
            result.push_back(Behaviour(thisType, thisObj, thisCarry, thisClose, thisVar,
                settings.getAttribute("action", "action", "", j),
                settings.getAttribute("action", "target", "", j),
                settings.getAttribute("action", "params", "", j) ));
        }
        settings.popTag();
    }
    return result;
}
