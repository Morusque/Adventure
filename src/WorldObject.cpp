
#include "WorldObject.h"

WorldObject::WorldObject()
{
    m_scenePosX = 0;
    m_scenePosY = 0;
    m_screenPosX = 0;
    m_screenPosY = 0;
    m_screenPosZ = 0;
    m_maxZ = 0;
    m_currentlyStucked = false;
    m_picture = NULL;
    m_carryingPos.set(0,0);
    m_screenChanged = false;
    m_screenName = "";
    carrying = NULL;
    m_logic = true;
    m_carried = false;
    m_walkX=0;
    m_walkY=0;
    m_message="";
    m_messageCounter=0;
}

WorldObject::WorldObject(WorldObject * parentObj) {
    m_scenePosX = parentObj->getScenePosX();
    m_scenePosY = parentObj->getScenePosY();
    m_screenPosX = parentObj->getScreenPosX();
    m_screenPosY = parentObj->getScreenPosY();
    m_screenPosZ = parentObj->getScreenPosZ();
    m_maxZ = parentObj->getMaxZ();
    m_currentlyStucked = parentObj->getCurrentlyStucked();
    m_picture = parentObj->getPicture();
    m_screenName = parentObj->getScreenName();
    for (int i=0;i<parentObj->getClasses().size();i++) {
        m_classes.push_back(parentObj->getClasses().at(i));
    }
    m_id = parentObj->getId();
    carrying = parentObj->getCarrying();
    m_carryingPos.set(parentObj->getCarryingPos());
    m_screenChanged = parentObj->justChangedScreen();
    m_logic = parentObj->getLogic();
    m_carried = parentObj->getCarried();
    m_walkX = parentObj->getWalkX();
    m_walkY = parentObj->getWalkY();
    m_message = parentObj->getMessage();
    m_messageCounter = parentObj->getMessageCounter();
}

WorldObject::~WorldObject()
{
    //dtor
}

bool WorldObject::hasClass(string _class) {
    // returns true if it has the specified class (one object can have many classes)
    for (int i=0;i<m_classes.size();i++)
        if (m_classes.at(i).compare(_class)==0) return true;
    return false;
}

ofPoint WorldObject::getCarryingPos() {
    return m_carryingPos;
}

void WorldObject::setCarryingPos(float _x, float _y) {
    m_carryingPos.set(_x,_y);
}

void WorldObject::setCarryingPosX(float _x) {
    m_carryingPos.x = _x;
}

void WorldObject::setCarryingPosY(float _y) {
    m_carryingPos.y = _y;
}

void WorldObject::setMaxZ(float _maxZ) {
    m_maxZ = _maxZ;
}

float WorldObject::getMaxZ() {
    return m_maxZ;
}

bool WorldObject::isOnScene(int x, int y) {
    if (m_scenePosX == x && m_scenePosY == y) return true;
    return false;
}

void WorldObject::draw(float pX, float pY) {
    // relative
    m_picture->draw((m_screenPosX -getWidth()/2 + pX)/global->m_ratio, (m_screenPosY -getHeight()/2 + pY - m_screenPosZ)/global->m_ratio,m_picture->width/global->m_ratio,m_picture->height/global->m_ratio);
    drawMessageAt(m_screenPosX -getWidth()/2 + pX, m_screenPosY -getHeight()/2 + pY - m_screenPosZ);
}

void WorldObject::drawAt(float pX, float pY) {
    // absolute
    m_picture->draw((pX-getWidth()/2)/global->m_ratio,(pY-getHeight()/2-m_screenPosZ)/global->m_ratio,m_picture->width/global->m_ratio,m_picture->height/global->m_ratio);
    drawMessageAt(pX-getWidth()/2,pY-getHeight()/2-m_screenPosZ);
}

int WorldObject::getScenePosX() {
    return m_scenePosX;
}

int WorldObject::getScenePosY() {
    return m_scenePosY;
}

float WorldObject::getScreenPosX() {
    return m_screenPosX;
}

float WorldObject::getScreenPosY() {
    return m_screenPosY;
}

float WorldObject::getScreenPosZ() {
    return m_screenPosZ;
}

ofImage * WorldObject::getPicture() {
    return m_picture;
}

string WorldObject::getScreenName() {
    return m_screenName;
}

string WorldObject::getId() {
    return m_id;
}

string WorldObject::getPool() {
    return m_pool;
}

vector<string> WorldObject::getClasses() {
    return m_classes;
}

void WorldObject::setScenePosX(int _scenePosX) {
    m_scenePosX = _scenePosX;
}

void WorldObject::setScenePosY(int _scenePosY) {
    m_scenePosY = _scenePosY;
}

void WorldObject::setScreenPosX(float _screenPosX) {
    m_screenPosX = _screenPosX;
}

void WorldObject::setScreenPosY(float _screenPosY) {
    m_screenPosY = _screenPosY;
}

void WorldObject::setScreenPosZ(float _screenPosZ) {
    m_screenPosZ = _screenPosZ;
}

void WorldObject::setPicture(ofImage * _picture) {
    m_picture = _picture;
}

void WorldObject::setScreenName(string _screenName) {
    m_screenName = _screenName;
}

void WorldObject::setId(string _id) {
    m_id = _id;
}

void WorldObject::setPool(string _pool) {
    m_pool = _pool;
}

void WorldObject::addClass(string _class) {
    m_classes.push_back(_class);
}

int WorldObject::getWidth() {
    return m_picture->width;
}

int WorldObject::getHeight() {
    return m_picture->height;
}

bool WorldObject::getLogic() {
    return m_logic;
}

void WorldObject::setLogic(bool _l) {
    m_logic = _l;
}

bool WorldObject::getCarried() {
    return m_carried;
}

void WorldObject::setCarried(bool _l) {
    m_carried = _l;
}

void WorldObject::setGlobal(Global * _global) {
    global = _global;
}

void WorldObject::move(WorldObject * _toReplace) {
    // move an object to the position of another object
    m_screenPosX += _toReplace->getScreenPosX();
    m_screenPosY += _toReplace->getScreenPosY();
    m_scenePosX  += _toReplace->getScenePosX();
    m_scenePosY  += _toReplace->getScenePosY();
    m_screenPosX = Utils::constrain(m_screenPosX,0,global->m_screenWidth-1);
    m_screenPosY = Utils::constrain(m_screenPosY,0,global->m_screenWidth-1);
    m_scenePosX  = Utils::constrain(m_scenePosX,0,global->m_worldWidth-1);
    m_scenePosY  = Utils::constrain(m_scenePosY,0,global->m_worldHeight-1);
}

float WorldObject::getWalkX() {
    return m_walkX;
}

float WorldObject::getWalkY() {
    return m_walkY;
}

void WorldObject::setWalkX(float _v) {
    m_walkX = _v;
}

void WorldObject::setWalkY(float _v) {
    m_walkY = _v;
}

void WorldObject::move(int _xD, int _yD, int _sxD, int _syD, Scene*** _world) {
    m_screenPosX += _xD;
    m_screenPosY += _yD;
    m_scenePosX += _sxD;
    m_scenePosY += _syD;
    m_currentlyStucked = false;
    while (m_screenPosX >= global->m_fieldWidth) {
        if (_world[m_scenePosX][m_scenePosY]->getStuck(1)) {
            m_screenPosX -= 1;
            m_currentlyStucked = true;
        } else {
            m_screenPosX -= global->m_fieldWidth;
            int nextX = _world[m_scenePosX][m_scenePosY]->getLink(1).x;
            int nextY = _world[m_scenePosX][m_scenePosY]->getLink(1).y;
            m_scenePosX  = nextX;
            m_scenePosY  = nextY;
            m_screenChanged=true;
        }
    }
    while (m_screenPosY >= global->m_fieldHeight) {
        if (_world[m_scenePosX][m_scenePosY]->getStuck(2)) {
            m_screenPosY -= 1;
            m_currentlyStucked = true;
        } else {
            m_screenPosY -= global->m_fieldHeight;
            int nextX = _world[m_scenePosX][m_scenePosY]->getLink(2).x;
            int nextY = _world[m_scenePosX][m_scenePosY]->getLink(2).y;
            m_scenePosX  = nextX;
            m_scenePosY  = nextY;
            m_screenChanged=true;
        }
    }
    while (m_screenPosX < 0) {
        if (_world[m_scenePosX][m_scenePosY]->getStuck(3)) {
            m_screenPosX += 1;
            m_currentlyStucked = true;
        } else {
            m_screenPosX += global->m_fieldWidth;
            int nextX = _world[m_scenePosX][m_scenePosY]->getLink(3).x;
            int nextY = _world[m_scenePosX][m_scenePosY]->getLink(3).y;
            m_scenePosX  = nextX;
            m_scenePosY  = nextY;
            m_screenChanged=true;
        }
    }
    while (m_screenPosY < 0) {
        if (_world[m_scenePosX][m_scenePosY]->getStuck(0)) {
            m_screenPosY += 1;
            m_currentlyStucked = true;
        } else {
            m_screenPosY += global->m_fieldHeight;
            int nextX = _world[m_scenePosX][m_scenePosY]->getLink(0).x;
            int nextY = _world[m_scenePosX][m_scenePosY]->getLink(0).y;
            m_scenePosX  = nextX;
            m_scenePosY  = nextY;
            m_screenChanged=true;
        }
    }
    while (m_scenePosX < 0 || m_scenePosX >= global->m_worldWidth )
        m_scenePosX = (m_scenePosX + global->m_worldWidth )% global->m_worldWidth;
    while (m_scenePosY < 0 || m_scenePosY >= global->m_worldHeight)
        m_scenePosY = (m_scenePosY + global->m_worldHeight)% global->m_worldHeight;
}

bool WorldObject::getCurrentlyStucked() {
    return m_currentlyStucked;
}

void WorldObject::setCurrentlyStucked(bool _s) {
    m_currentlyStucked = _s;
}

void WorldObject::fall() {
    if (!m_carried) {
        int sens = Utils::constrain(m_maxZ-m_screenPosZ,-20,20);
        m_screenPosZ = m_screenPosZ + sens;
    }
}

bool WorldObject::justChangedScreen() {
    if (m_screenChanged) {
        m_screenChanged=false;
        return true;
    }
    return false;
}

void WorldObject::carry(WorldObject * _carrying) {
    // release current object first and inform the object itself
    if (carrying) carrying->setCarried(false);
    carrying = _carrying;
    // if the carrying is not null
    if (_carrying) {
        vector<ofPoint> thisPos = screenPositionFrom(_carrying);
        if (thisPos.size()>0) {
            ofPoint shortPos = Utils::shorterFrom(&thisPos,m_screenPosX,m_screenPosY);
            m_carryingPos.x = shortPos.x - m_screenPosX;
            m_carryingPos.y = shortPos.y - m_screenPosY;
        }
        _carrying->setCarried(true);
    } else {
        m_carryingPos.set(0,0);
    }
}

vector<ofPoint> WorldObject::screenPositionFrom(WorldObject * _worldObject) {
    vector<ofPoint> result;
    if (_worldObject) {
        if (currentScene && _worldObject->getCurrentScene()) {
            for (int direction=-1 ; direction<4 ; direction++) {
                if (direction == -1) {
                    if (getScenePosX() == _worldObject->getScenePosX() && getScenePosY() == _worldObject->getScenePosY()) {
                        result.push_back(ofPoint(_worldObject->getScreenPosX(),_worldObject->getScreenPosY()));
                    }
                } else {
                    ofPoint thisPos = ofPoint(currentScene->getLink(direction).x,currentScene->getLink(direction).y);
                    if (_worldObject->getScenePosX()==thisPos.x && _worldObject->getScenePosY()==thisPos.y) {
                        if (!currentScene->getStuck(direction) && !currentScene->getHide(direction)) {
                            float realX = _worldObject->getScreenPosX();
                            float realY = _worldObject->getScreenPosY();
                            if (direction==0) realY -= global->m_fieldHeight;
                            if (direction==1) realX += global->m_fieldWidth;
                            if (direction==2) realY += global->m_fieldHeight;
                            if (direction==3) realX -= global->m_fieldWidth;
                            result.push_back(ofPoint(realX,realY));
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool WorldObject::isCloseTo(WorldObject * _worldObject) {
    if (_worldObject) {
        vector<ofPoint> positions = screenPositionFrom(_worldObject);
        for (int i=0;i<positions.size();i++) {
            if (abs(m_screenPosX - positions.at(i).x) < (getWidth() + _worldObject->getWidth()) /2 &&
                abs((m_screenPosY-m_screenPosZ) - (positions.at(i).y-_worldObject->m_screenPosZ)) < (getHeight() + _worldObject->getHeight()) /2 ) {
                for (int x=0 ; x<m_picture->getWidth() ; x++) {
                    for (int y=0 ; y<m_picture->getHeight() ; y++) {
                        if ((int)m_picture->getColor(x,y).a > 0) {
                            int collX = ((m_screenPosX-(float)getWidth()/2)+x) -
                                        (positions.at(i).x-(float)_worldObject->getWidth()/2);
                            int collY = ((m_screenPosY-m_screenPosZ-(float)getHeight()/2)+y) -
                                        (positions.at(i).y-_worldObject->m_screenPosZ-(float)_worldObject->getHeight()/2);
                            if (collX>=0 && collY>=0 &&
                                collX<_worldObject->getWidth() && collY<_worldObject->getHeight()) {
                                if ((int)_worldObject->getPicture()->getColor(collX,collY).a > 0) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

float WorldObject::distanceFrom(WorldObject * _worldObject) {
    if (_worldObject) {
        vector<ofPoint> positions = screenPositionFrom(_worldObject);
        ofPoint pos = Utils::shorterFrom(&positions,m_screenPosX,m_screenPosY);
        float result = ofDist(m_screenPosX, m_screenPosY, pos.x, pos.y);
        return result;
    }
    return NULL;
}

float WorldObject::sqDistanceFrom(WorldObject * _worldObject) {
    // square distance (for optimized collision check)
    if (_worldObject) {
        vector<ofPoint> positions = screenPositionFrom(_worldObject);
        ofPoint pos = Utils::shorterFrom(&positions,m_screenPosX,m_screenPosY);
        float result = ofDistSquared(m_screenPosX, m_screenPosY, pos.x, pos.y);
        return result;
    }
    return NULL;
}

WorldObject * WorldObject::getCarrying() {
    return carrying;
}

void WorldObject::moveCarrying() {
    // moves the object that is being currently carried along
    if (carrying) {
        carrying->moveTo(m_scenePosX,m_scenePosY,m_screenPosX+m_carryingPos.x,m_screenPosY+m_carryingPos.y);
        int sens = Utils::constrain(carrying->getMaxZ()-carrying->getScreenPosZ(),-20,20);
        carrying->setScreenPosZ(carrying->getScreenPosZ()+sens);
        if (!isCloseTo(carrying)) carrying->setScreenPosZ(carrying->getScreenPosZ()-sens);
    }
}

bool WorldObject::occludes(WorldObject * _worldObject) {
    // returns true if _worldObject is hidden behind
    if (_worldObject) {
        vector<ofPoint> positions = screenPositionFrom(_worldObject);
        for (int i=0;i<positions.size();i++) {
            if (_worldObject->getScreenPosY()+(float)_worldObject->getHeight()/2>m_screenPosY+(float)getHeight()/2) {
                return false;
            }
            for (int x=0;x<_worldObject->getWidth() ;x++) {
                for (int y=0;y<_worldObject->getHeight() ;y++) {
                    if ((int)_worldObject->getPicture()->getColor(x,y).a > 0) {
                        int collX = ((positions.at(i).x-(float)_worldObject->getWidth()/2)+x) -
                                    (m_screenPosX-(float)getWidth()/2);
                        int collY = ((positions.at(i).y-_worldObject->m_screenPosZ-(float)_worldObject->getHeight()/2)+y) -
                                    (m_screenPosY-m_screenPosZ-(float)getHeight()/2);
                        if (collX<0 || collY<0 || collY>=getWidth() || collY>=getHeight()) {
                            return false;
                        } else if ((int)m_picture->getColor(collX,collY).a == 0) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void WorldObject::moveTo(int _scenePosX,int _scenePosY,int _screenPosX,int _screenPosY) {
    // absolute
    m_scenePosX =_scenePosX;
    m_scenePosY =_scenePosY;
    if (m_screenPosX!=_screenPosX||
        m_screenPosY!=_screenPosY)
        m_screenChanged=true;
    m_screenPosX=_screenPosX;
    m_screenPosY=_screenPosY;
}

void WorldObject::setCurrentScene(Scene * _scene) {
    currentScene = _scene;
}

Scene * WorldObject::getCurrentScene() {
    return currentScene;
}

void WorldObject::countMessage() {
    if (m_messageCounter > 0) m_messageCounter--;
    if (m_messageCounter == 0) m_message = "";
}

void WorldObject::setMessageCounter(int _cnt) {
    m_messageCounter = _cnt;
}

void WorldObject::setMessage(string _message) {
    m_message = _message;
}

void WorldObject::drawMessageAt(float _pX, float _pY) {
    if (m_message.length()>0) {
        int maxCharsX = 20;
        float fontSizeX = 13;
        float fontSizeY = 25;
        float margin = 5;
        int nbLines = ceil((float)m_message.length()/maxCharsX);
        string finalMessage[nbLines];
        for (int i=0 ; i<nbLines ; i++) {
                finalMessage[i] = m_message.substr(i*maxCharsX,maxCharsX);
        }
        _pY -= fontSizeY * nbLines;
        float pxL = min((int)m_message.length(),maxCharsX) * fontSizeX + 3;
        float thisPx = floor(_pX - pxL/2);
        float pyL = nbLines * fontSizeY;
        float xEnd = thisPx + (fontSizeX * maxCharsX);
        float shiftX = 0;
        if (thisPx < 0) shiftX = -thisPx;
        if (xEnd - global->m_fieldWidth > 0) shiftX = global->m_fieldWidth - xEnd;
        thisPx += shiftX;
        if (_pY-fontSizeY-margin-global->m_txtMargHeight<0) _pY = fontSizeY+margin+global->m_txtMargHeight;
        ofSetColor(0xFF,0xFF,0xFF);
        ofRect((global->m_txtMargWidth+thisPx-margin*2)/global->m_ratio,(_pY-fontSizeY-margin*2)/global->m_ratio,(pxL+margin*4)/global->m_ratio,(pyL+margin*4)/global->m_ratio);
        ofSetColor(0,0,0);
        ofRect((global->m_txtMargWidth+thisPx-margin)/global->m_ratio,(_pY-fontSizeY-margin)/global->m_ratio,(pxL+margin*2)/global->m_ratio,(pyL+margin*2)/global->m_ratio);
        ofSetColor(0xFF,0xFF,0xFF);
        for (int i=0 ; i<nbLines ; i++) {
            global->theFont.drawString(finalMessage[i] , (global->m_txtMargWidth+thisPx - 1)/global->m_ratio, (_pY - margin + 0 + fontSizeY * i)/global->m_ratio);
        }
    }
}

string WorldObject::getMessage() {
    return m_message;
}

int WorldObject::getMessageCounter() {
    return m_messageCounter;
}

