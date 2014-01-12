#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Utils.h"
#include "Global.h"

class WorldObject
{
public:

    WorldObject();
    WorldObject(WorldObject * parentObj);
    ~WorldObject();
    void draw(float pX, float pY);
    void drawAt(float pX, float pY);

    int getScenePosX();
    int getScenePosY();
    float getScreenPosX();
    float getScreenPosY();
    float getScreenPosZ();
    ofImage * getPicture();

    string getScreenName();
    string getId();
    string getPool();
    vector<string> getClasses();

    void setScenePosX(int _scenePosX);
    void setScenePosY(int _scenePosY);
    void setScreenPosX(float _screenPosX);
    void setScreenPosY(float _screenPosY);
    void setScreenPosZ(float _screenPosZ);
    void liftCarryBy(float _z);
    void setPicture(ofImage * _picture);

    bool getCurrentlyStucked();
    void setCurrentlyStucked(bool _s);

    void setScreenName(string _name);
    void setId(string _id);
    void setPool(string _pool);
    void addClass(string _class);

    bool hasClass(string _class);

    int getWidth();
    int getHeight();
    void setWidth(int _width);
    void setHeight(int _height);

    bool isOnScene(int x, int y);
    bool isCloseTo(WorldObject * _worldObject);
    float distanceFrom(WorldObject * _worldObject);
    float sqDistanceFrom(WorldObject * _worldObject);

    void setGlobal(Global * global);

    void carry(WorldObject * _carrying);
    WorldObject * getCarrying();
    void moveCarrying();
    void fall();
    ofPoint getCarryingPos();

    void move(int _xD, int _yD, int _sxD, int syD, Scene*** _world);
    void move(WorldObject * _toReplace);
    void moveTo(int _scenePosX, int _scenePosY, int _screenPosX, int _screenPosY);
    bool justChangedScreen();

    bool getLogic();
    void setLogic(bool _l);

    bool getCarried();
    void setCarried(bool _l);
    void setCarryingPos(float _x, float _y);
    void setCarryingPosX(float _x);
    void setCarryingPosY(float _y);
    float getMaxZ();
    void setMaxZ(float _maxZ);

    float getWalkX();
    float getWalkY();
    void setWalkX(float _v);
    void setWalkY(float _v);

    void setCurrentScene(Scene * _scene);
    Scene * getCurrentScene();

    vector<ofPoint> screenPositionFrom(WorldObject * _worldObject);

    void countMessage();
    void drawMessageAt(float _pX, float _pY);
    void setMessage(string _message);
    void setMessageCounter(int _cnt);
    string getMessage();
    int getMessageCounter();

    bool occludes(WorldObject * _obj);

protected:

private:

    Scene * currentScene;

    WorldObject * carrying;
    ofPoint m_carryingPos;

    bool m_currentlyStucked;

    string m_screenName;
    string m_id;
    string m_pool;
    vector<string> m_classes;

    int m_scenePosX;
    int m_scenePosY;
    float m_screenPosX;
    float m_screenPosY;
    float m_screenPosZ;
    float m_maxZ;

    ofImage * m_picture;

    bool m_screenChanged;
    bool m_logic;

    bool m_carried;

    float m_walkX;
    float m_walkY;

    int m_messageCounter;
    string m_message;

    Global * global;

};
