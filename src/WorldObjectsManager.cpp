
#include "WorldObjectsManager.h"

WorldObjectsManager::WorldObjectsManager()
{
}

WorldObjectsManager::~WorldObjectsManager()
{
    // m_worldObjects = objects that actually are in the world
    for (int i=0;i<m_worldObjects.size();i++) {
        delete m_worldObjects.at(i);
    }
    // m_poolObjects = potential objects that could be in the world at some point
    for (int i=0;i<m_poolObjects.size();i++) {
        delete m_poolObjects.at(i);
    }
}

void WorldObjectsManager::loadObjects(XmlLoader * _xml) {
    m_worldObjects = _xml->loadObjects("init");
    m_poolObjects = _xml->loadObjects("pool");
    for (int i=0;i<m_worldObjects.size();i++) {// TODO put that in the xml and loads it automatically
        m_worldObjects.at(i)->setGlobal(global);
    }
    for (int i=0;i<m_poolObjects.size();i++) {// TODO put that in the xml and loads it automatically
        m_poolObjects.at(i)->setGlobal(global);
    }
}

void WorldObjectsManager::draw(int _x, int _y, WorldObject * player, Scene * _scene)
{
    // make a list of potentially visible objects from the player's POV
    vector<int> drawable;
    for (int i=0;i<getNbWorldObjects();i++) {
        if (player->screenPositionFrom(m_worldObjects.at(i)).size()>0) drawable.push_back(i);
    }
    // draws them
    for (int y=-global->m_fieldHeight;y<global->m_fieldHeight*2;y++) {
        for (int i=0;i<drawable.size();i++) {
            vector<ofPoint> thisPos = player->screenPositionFrom(m_worldObjects.at(drawable.at(i)));
            for (int j=0;j<thisPos.size();j++) {
                if (floor(thisPos.at(j).y+m_worldObjects.at(drawable.at(i))->getHeight()/2)==y) {
                    m_worldObjects.at(drawable.at(i))->drawAt(thisPos.at(j).x + global->m_txtMargWidth,
                                                              thisPos.at(j).y+ global->m_txtMargHeight);
                }
            }
        }
        if (floor(player->getScreenPosY())+player->getHeight()/2==y) {
            player->draw(global->m_txtMargWidth, global->m_txtMargHeight);
        }
    }
}

void WorldObjectsManager::setCarryingPos(Behaviour * _act, WorldObject * _obj) {
    int xP=0;
    int yP=0;
    for (int p=0;p<_act->getParams().size();p++) {
        if (_act->getParams().at(p).o().compare("pX")==0) xP = Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("pY")==0) yP = Utils::str2int(_act->getParams().at(p).v());
    }
    _obj->setCarryingPos(xP,yP);
    _obj->moveCarrying();
}

int WorldObjectsManager::getNbWorldObjects() {
    return m_worldObjects.size();
}

WorldObject * WorldObjectsManager::getWorldObject(int i) {
    return m_worldObjects.at(i);
}

void WorldObjectsManager::deleteWorldObject(int _i) {
    m_worldObjects.erase(m_worldObjects.begin()+_i);
}

void WorldObjectsManager::setGlobal(Global * _global) {
    global = _global;
}

WorldObject * WorldObjectsManager::addWorldObject(Behaviour * _act) {
    WorldObject * newObj = new WorldObject(lookForPoolParam(_act));
    newObj->setGlobal(global);
    m_worldObjects.push_back(newObj);
    return newObj;
}

void WorldObjectsManager::removeWorldObject(WorldObject * _obj) {
    for (int i=0;i<m_worldObjects.size();i++) {
        if (_obj==m_worldObjects.at(i)) {
            m_worldObjects.erase(m_worldObjects.begin()+i);
            delete _obj;
            break;
        }
    }
}

void WorldObjectsManager::replaceWorldObject(Behaviour * _act, WorldObject * _toReplace) {
    WorldObject * newObj = new WorldObject(lookForPoolParam(_act));
    newObj->setGlobal(global);
    newObj->move(_toReplace);
    *_toReplace = *newObj;
}

WorldObject * WorldObjectsManager::lookForPoolParam(Behaviour * _act) {
    // looks for a specific potential object based on some descriptors

    for (int p=0;p<_act->getParams().size();p++) {

        if (_act->getParams().at(p).o().compare("poolId")==0) {
            for (int i=0 ; i<m_poolObjects.size() ; i++)
                if (m_poolObjects.at(i)->getId().compare(_act->getParams().at(p).v())==0) return m_poolObjects.at(i);
        }

        if (_act->getParams().at(p).o().compare("worldId")==0) {
            for (int i=0 ; i<m_worldObjects.size() ; i++) {
                if (m_worldObjects.at(i)->getId().compare(_act->getParams().at(p).v())==0) {
                    return m_worldObjects.at(i);
                }
            }
        }

        if (_act->getParams().at(p).o().compare("poolRndClass")==0) {
            vector<WorldObject*> possibilities;
            for (int i=0 ; i<m_poolObjects.size() ; i++) {
                if (m_poolObjects.at(i)->hasClass(_act->getParams().at(p).v())) possibilities.push_back(m_poolObjects.at(i));
            }
            if (possibilities.size()>0) return possibilities.at(rand()%possibilities.size());
        }

    }

    return NULL;

}

WorldObject * WorldObjectsManager::lookForWorldTarget(Behaviour * _act) {
    // look for a target object

    for (int p=0;p<_act->getTarget().size();p++) {

        if (_act->getTarget().at(p).o().compare("id")==0) {
            for (int i=0 ; i<m_worldObjects.size() ; i++)
                if (m_worldObjects.at(i)->getId().compare(_act->getTarget().at(p).v())==0) return m_worldObjects.at(i);
        }

    }

    return NULL;

}

WorldObject * WorldObjectsManager::lookForWorldId(string _str) {
    for (int i=0 ; i<m_worldObjects.size() ; i++)
        if (m_worldObjects.at(i)->getId().compare(_str)==0) return m_worldObjects.at(i);
    return NULL;

}

WorldObject * WorldObjectsManager::lookForWorldParam(Behaviour * _act) {

    for (int p=0;p<_act->getParams().size();p++) {
        if (_act->getParams().at(p).o().compare("id")==0) {
            for (int i=0 ; i<m_worldObjects.size() ; i++)
                if (m_worldObjects.at(i)->getId().compare(_act->getParams().at(p).v())==0) return m_worldObjects.at(i);
        }
    }

    return NULL;

}

void WorldObjectsManager::moveTo(Behaviour * _act, WorldObject * _obj) {
    // moves _obj to a locations specified in _act (relative fashion)

    int pX = 0;
    int pY = 0;
    int sX = 0;
    int sY = 0;

    for (int p=0;p<_act->getParams().size();p++) {
        if (_act->getParams().at(p).o().compare("pX")==0) pX += Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("pY")==0) pY += Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("sX")==0) sX += Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("sY")==0) sY += Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("id")==0) {
            WorldObject * paramTarget = lookForWorldParam(_act);
            if (paramTarget) {
                pX = paramTarget->getScreenPosX();
                pY = paramTarget->getScreenPosY();
                sX = paramTarget->getScenePosX();
                sY = paramTarget->getScenePosY();
            }
        }
    }

    if (_obj) _obj->moveTo(sX,sY,pX,pY);

}

void WorldObjectsManager::direct(WorldObject * _obj, Behaviour * _act, WorldObject * _player) {

    // updates the current direction of _obj based on __act
    // it might occasionaly refer to the _player (i.e. follow him)

    float angle=0;
    float speed=0;

    for (int p=0;p<_act->getParams().size();p++) {
        if (_act->getParams().at(p).o().compare("keepAngle")==0)
            angle += Utils::str2int(_act->getParams().at(p).v()) * atan2(_obj->getWalkY(),_obj->getWalkX());
        if (_act->getParams().at(p).o().compare("keepSpeed")==0)
            speed += Utils::str2int(_act->getParams().at(p).v()) * ofDist(0,0,_obj->getWalkY(),_obj->getWalkX());
        if (_act->getParams().at(p).o().compare("angle")==0)
            angle += (float)Utils::str2int(_act->getParams().at(p).v())*TWO_PI/360;
        if (_act->getParams().at(p).o().compare("rndAngle")==0)
            angle += ((float)(rand()%Utils::str2int(_act->getParams().at(p).v())))*TWO_PI/360;
        if (_act->getParams().at(p).o().compare("speed")==0)
            speed += Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("rndSpeed")==0)
            speed += rand()%Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("follow")==0) {
            if (_act->getParams().at(p).v().compare("player")==0) {
                if (_player->getScenePosX()==_obj->getScenePosX() && _player->getScenePosY()==_obj->getScenePosY())
                    angle = atan2(_player->getScreenPosY()-_obj->getScreenPosY(),_player->getScreenPosX()-_obj->getScreenPosX());
                else angle = atan2(_player->getScenePosY()-_obj->getScenePosY(),_player->getScenePosX()-_obj->getScenePosX());
            }
        }
    }

    _obj->setWalkX(cos(angle)*speed);
    _obj->setWalkY(sin(angle)*speed);

}

void WorldObjectsManager::move(Behaviour * _act, WorldObject * _obj, Scene*** _world) {

    // move _obj, absolute fashion

    int pX=0;
    int pY=0;
    int sX=0;
    int sY=0;

    for (int p=0;p<_act->getParams().size();p++) {
        if (_act->getParams().at(p).o().compare("pX")==0) pX = Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("pY")==0) pY = Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("sX")==0) sX = Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("sY")==0) sY = Utils::str2int(_act->getParams().at(p).v());
        if (_act->getParams().at(p).o().compare("#")==0) {
            if (_act->getParams().at(p).v().compare("self")==0) {
                pX += _obj->getWalkX();
                pY += _obj->getWalkY();
            }
        }
    }

    if (_obj) _obj->move(pX,pY,sX,sY, _world);

}

void WorldObjectsManager::fill(Behaviour * _act) {
    // fille the world with a quantity of objects
    int iterations = Utils::str2int(_act->getAction().at(0).v());// TOSO place the str2int in the accessor
    for (int i=0 ; i < iterations ; i++) {
        WorldObject * newObj = addWorldObject(_act);
        // TODO replace this by "getValueOf" parameter in Descriptor, once the whole vector is in it
        int sX=0;
        int sY=0;
        int sXL=0;
        int sYL=0;
        for (int p=0 ; p<_act->getTarget().size() ; p++) {
            if (_act->getTarget().at(p).o().compare("sX")==0)  sX  = Utils::str2int(_act->getTarget().at(p).v());
            if (_act->getTarget().at(p).o().compare("sY")==0)  sY  = Utils::str2int(_act->getTarget().at(p).v());
            if (_act->getTarget().at(p).o().compare("sXL")==0) sXL = Utils::str2int(_act->getTarget().at(p).v());
            if (_act->getTarget().at(p).o().compare("sYL")==0) sYL = Utils::str2int(_act->getTarget().at(p).v());
        }
        newObj->setScenePosX(sX+rand()%sXL);
        newObj->setScenePosY(sY+rand()%sYL);
        newObj->setScreenPosX(rand()%global->m_fieldWidth);
        newObj->setScreenPosY(rand()%global->m_fieldHeight);
    }
}

void WorldObjectsManager::setSomething(WorldObject * _obj, Behaviour * _act) {
    // set the value of some parameter
    for (int p=0 ; p<_act->getParams().size() ; p++) {
        if (_act->getParams().at(p).o().compare("logic")==0) {
            if (_act->getParams().at(p).v().compare("0")==0) _obj->setLogic(false);
            else _obj->setLogic(true);
        }
        if (_act->getParams().at(p).o().compare("stucked")==0) {
            if (_act->getParams().at(p).v().compare("0")==0) _obj->setCurrentlyStucked(false);
            else _obj->setCurrentlyStucked(true);
        }
    }
}
