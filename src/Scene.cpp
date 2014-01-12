
#include "Scene.h"

Scene::Scene()
{
    m_idY = 0;
    m_idX = 0;
    m_bgPicture = NULL;
    m_sndLoop = NULL;
    m_name = "";
}

Scene::Scene(int _idX, int _idY, ofImage * _bgPicture, string _name, ofSoundPlayer * _sndLoop)
{
    m_idY = _idY;
    m_idX = _idX;
    m_bgPicture = _bgPicture;
    m_sndLoop = _sndLoop;
    m_sndLoop->setLoop(true);
    m_name = _name;
}

Scene::~Scene()
{
    //dtor
}

ofSoundPlayer * Scene::getSndLoop() {
    return m_sndLoop;
}

void Scene::draw() {
    ofSetColor(0xFF,0xFF,0xFF);
    m_bgPicture->draw(global->m_txtMargWidth/global->m_ratio,global->m_txtMargHeight/global->m_ratio,m_bgPicture->width/global->m_ratio,m_bgPicture->height/global->m_ratio);
}

void Scene::setGlobal(Global * _global) {
    global = _global;
}

string Scene::getName() {
    return m_name;
}

// links are the links to the neighbour scenes
// 0 = up
// 1 = right
// 2 = down
// 3 = left
ofPoint Scene::getLink(int _i) {
    return m_link[_i];
}

void Scene::setLink(int _i, ofPoint _linkInd) {
    m_link[_i] = _linkInd;
}

void Scene::setLinkX(int _i, int _x) {
    m_link[_i].x = _x;
}

void Scene::setLinkY(int _i, int _y) {
    m_link[_i].y = _y;
}

void Scene::setStuck(int _i, bool _s) {
    m_stuck[_i] = _s;
}

bool Scene::getStuck(int _i) {
    return m_stuck[_i];
}

void Scene::setHide(int _i, bool _h) {
    m_hide[_i] = _h;
}

bool Scene::getHide(int _i) {
    return m_hide[_i];
}
