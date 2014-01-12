#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

    // loading
    cout << "Welcome to the game"  << endl;
    cout << "Arrow keys : move" << endl;
    cout << "enter/space : take, use, release" << endl;
    cout << endl;
    cout << "loading";

    ofSetFrameRate(30);

    int txtMargW = xmlLoader.getGlobalVariable("txtMargW");
    int txtMargH = xmlLoader.getGlobalVariable("txtMargH");
	m_world = new Array2D<Scene>(xmlLoader.getWorldWidth(),xmlLoader.getWorldHeight());

    ofTrueTypeFont theFont;
    float ratio = xmlLoader.getGlobalVariable("ratio");
    theFont.loadFont("ProggyClean.ttf", 24/ratio, false);
    global = Global(theFont, ofGetWidth()*ratio, ofGetHeight()*ratio, m_world->width(), m_world->height(), txtMargW, txtMargH, ratio);

    worldObjectsManager = new WorldObjectsManager();
    worldObjectsManager->setGlobal(&global);
    worldObjectsManager->loadObjects(&xmlLoader);

	player = xmlLoader.loadObjects("player").at(0);
	player->setGlobal(&global);
	playerSpeed = xmlLoader.getPlayerSpeed();

	for(int li = 0 ; li < m_world->height() ; li++) {
        for(int co = 0 ; co < m_world->width() ; co++) {
            m_world->SetElemAt(co, li, xmlLoader.loadScene(co,li));
            m_world->GetElemAt(co, li)->setGlobal(&global);
        }
    }

    for (int i=0;i<0xFFFF;i++) keys[i]=false;

    upperText = "";
    middleText = "";
    lowerText = "";
    middleTextCounter = 0;

    m_behaviours = xmlLoader.loadBehaviours();

    for (int i=0;i<m_behaviours.size();i++) {
        for (int j=0;j<m_behaviours[i].getVar().size();j++) {
            if (m_behaviours[i].getVar()[j].o().compare("name")==0) {
                bool found = false;
                for (int k=0;k<m_variables.size();k++) {
                    if (m_behaviours[i].getVar()[j].v().compare(m_variables[k].name)==0) found=true;
                }
                if (!found) m_variables.push_back(Variable(m_behaviours[i].getVar()[j].v()));
            }
        }
        for (int j=0;j<m_behaviours[i].getTarget().size();j++) {
            if (m_behaviours[i].getTarget()[j].o().compare("name")==0) {
                bool found = false;
                for (int k=0;k<m_variables.size();k++) {
                    if (m_behaviours[i].getTarget()[j].v().compare(m_variables[k].name)==0) found=true;
                }
                if (!found) m_variables.push_back(Variable(m_behaviours[i].getTarget()[j].v()));
            }
        }
    }

    checkPlayerTrigger=false;

    indicWalls[0] = ofImage("indic00.png ");
    indicWalls[1] = ofImage("indicAr00.png");
    indicWalls[2] = ofImage("indicAr01.png");
    indicWalls[3] = ofImage("indicAr02.png");
    indicWalls[4] = ofImage("indicAr03.png");
    indicWalls[5] = ofImage("indicWa00.png");
    indicWalls[6] = ofImage("indicWa01.png");
    indicWalls[7] = ofImage("indicWa02.png");
    indicWalls[8] = ofImage("indicWa03.png");

    int nbSfx = xmlLoader.getGlobalVariable("nbSfx");
    sfx = new ofSoundPlayer[nbSfx];
    for (int i=0;i<nbSfx;i++) {
        sfx[i] = ofSoundPlayer();
        sfx[i].loadSound("sfx_" + Utils::intToStr(i,2) + ".wav");
        sfx[i].setVolume(0.5);// sound volume
    }

    musicVolume = 0.35;// should probably be 0.7/1(sfx) or something

    time=0;

    ofEnableAlphaBlending();

    updateObjScenes();

    initTrigger(0);
    trigActions();
    toTrig.clear();
    trigObj.clear();

}

//--------------------------------------------------------------
void testApp::update() {
    checkWindowSize();
    updateObjScenes();
    checkClosestObject();
    clockEvents();
    movePlayer();
    moveCarryings();
    act();
    checkInteractions();
    trigActions();
    toTrig.clear();
    trigObj.clear();
    checkMusicLoop();
    updateObjScenes();
}

//--------------------------------------------------------------
void testApp::draw(){
    currentScene()->draw();
    // TODO optimize the following function
    worldObjectsManager->draw(player->getScenePosX(), player->getScenePosY(), player, currentScene());
    drawText();
    drawIndicWalls();
}

void testApp::drawText() {
    // draw text margins
    ofSetColor(0,0,0);
    ofRect(0/global.m_ratio,0/global.m_ratio,global.m_screenWidth/global.m_ratio,global.m_txtMargHeight/global.m_ratio);
    ofRect(0/global.m_ratio,(global.m_screenHeight/global.m_ratio-global.m_txtMargHeight/global.m_ratio),global.m_screenWidth/global.m_ratio,global.m_txtMargHeight/global.m_ratio);
    ofRect(0/global.m_ratio,0/global.m_ratio,global.m_txtMargWidth/global.m_ratio,global.m_screenHeight/global.m_ratio);
    ofRect((global.m_screenWidth/global.m_ratio-global.m_txtMargWidth/global.m_ratio),0/global.m_ratio,global.m_txtMargWidth/global.m_ratio,global.m_screenHeight/global.m_ratio);
    ofSetColor(0xFF,0xFF,0xFF);
    global.theFont.drawString(upperText, 70/global.m_ratio, global.m_txtMargHeight/global.m_ratio - 20/global.m_ratio);
    if (middleText.size()>0) {
        ofSetColor(0xFF,0xFF,0xFF);
        ofRect(0,(global.m_screenHeight/3-35)/global.m_ratio,ofGetWidth(),50/global.m_ratio);
        ofSetColor(0,0,0);
        ofRect(0,(global.m_screenHeight/3-30)/global.m_ratio,ofGetWidth(),40/global.m_ratio);
        ofSetColor(0xFF,0xFF,0xFF);
        global.theFont.drawString(middleText, 5/global.m_ratio, (global.m_screenHeight/3)/global.m_ratio);
    }
    if (middleTextCounter>0) middleTextCounter--;
    else middleText = "";
    global.theFont.drawString(lowerText, 70/global.m_ratio, (global.m_screenHeight - global.m_txtMargHeight + 35) / global.m_ratio);
}

void testApp::checkWindowSize() {
    int w = ofGetWidth();
    int h = ofGetHeight();
    // if the window has been resized
    if (w != global.m_screenWidth/global.m_ratio || h != global.m_screenHeight/global.m_ratio) {
        // set it to a good ratio
        global.setRatio(((float)(global.m_screenWidth+global.m_screenHeight)/2)/((float)(w+h)/2));
        ofSetWindowShape(global.m_screenWidth/global.m_ratio, global.m_screenHeight/global.m_ratio);
        global.theFont.loadFont("ProggyClean.ttf", 24/global.m_ratio, false);
    }
}

// update the pointers to current scene (ugly code)
void testApp::updateObjScenes() {
    for (int o=0;o<worldObjectsManager->getNbWorldObjects();o++) {
        WorldObject * thisObj = worldObjectsManager->getWorldObject(o);
        Scene * thisScene = m_world->GetElemAt(thisObj->getScenePosX(),thisObj->getScenePosY());
        thisObj->setCurrentScene(thisScene);
    }
    player->setCurrentScene(m_world->GetElemAt(player->getScenePosX(),player->getScenePosY()));
}

void testApp::movePlayer() {
    if (keys[OF_KEY_RIGHT]) player->move( playerSpeed, 0          , 0, 0, m_world->getArray());
    if (keys[OF_KEY_LEFT])  player->move(-playerSpeed, 0          , 0, 0, m_world->getArray());
    if (keys[OF_KEY_DOWN])  player->move( 0          , playerSpeed, 0, 0, m_world->getArray());
    if (keys[OF_KEY_UP])    player->move( 0          ,-playerSpeed, 0, 0, m_world->getArray());
    if (player->justChangedScreen()) {
        lowerText = m_world->GetElemAt(player->getScenePosX(), player->getScenePosY())->getName();
    }
}

void testApp::checkMusicLoop() {
    // if the current music loop is not the one that is supposed to be played
    if (currentScene()->getSndLoop() != currentSndLoop) {
        int targetPosition=0;
        currentSndLoop = m_world->GetElemAt(player->getScenePosX(),player->getScenePosY())->getSndLoop();
        // stop other sound loops
        for(int li = 0 ; li < m_world->height() ; li++) {
            for(int co = 0 ; co < m_world->width() ; co++) {
                if (m_world->GetElemAt(co,li)->getSndLoop()->getIsPlaying()) {
                    targetPosition = m_world->GetElemAt(co,li)->getSndLoop()->getPositionMS();
                    m_world->GetElemAt(co,li)->getSndLoop()->stop();
                }
            }
        }
        currentSndLoop->setVolume(musicVolume);
        currentSndLoop->play();
        // sync it
        currentSndLoop->setPositionMS(targetPosition);
    }
}

void testApp::moveCarryings() {
    // move the objects that are being carried
    for (int o=0;o<worldObjectsManager->getNbWorldObjects();o++) {
        if (worldObjectsManager->getWorldObject(o)->getLogic()) {
            worldObjectsManager->getWorldObject(o)->moveCarrying();
        }
    }
    player->moveCarrying();
}

void testApp::act() {
    for (int o=0;o<worldObjectsManager->getNbWorldObjects();o++) {
        worldObjectsManager->getWorldObject(o)->fall();
        worldObjectsManager->getWorldObject(o)->countMessage();
    }
    player->fall();
    player->countMessage();
}

void testApp::initTrigger(int trigId) {
    // looks for all the events that are going to be triggered at the beginning of the game
    for (int i=0;i<m_behaviours.size();i++) {
        if (m_behaviours.at(i).getType().at(0).o().compare("init")==0) {
            if (Utils::str2int(m_behaviours.at(i).getType().at(0).v())==trigId) {
                if (m_behaviours.at(i).getObj().at(0).o().compare("#")==0 &&
                    m_behaviours.at(i).getObj().at(0).v().compare("unused")==0) {
                    toTrig.push_back(&m_behaviours.at(i));
                    trigObj.push_back(NULL);
                } else {
                    WorldObject * obj;
                    obj = worldObjectsManager->lookForWorldId(m_behaviours.at(i).getObj().at(0).v());
                    // push the event into these lists
                    toTrig.push_back(&m_behaviours.at(i));
                    // and the object the event is about
                    trigObj.push_back(obj);
                }
            }
        }
    }
}

void testApp::clockEvents() {
    // looks for all the clock events that are going to be triggered this frame
    for (int i=0;i<m_behaviours.size();i++) {
        if (m_behaviours.at(i).getType().at(0).o().compare("clock")==0) {
            if (time % Utils::str2int(m_behaviours.at(i).getType().at(0).v()) == 0) {
                for (int o=0;o<worldObjectsManager->getNbWorldObjects();o++) {
                    if (worldObjectsManager->getWorldObject(o)->getLogic()) {
                        if (m_behaviours.at(i).fulfilled("clock", worldObjectsManager->getWorldObject(o), closestObject, &m_variables)) {
                            // push the event into these lists
                            toTrig.push_back(&m_behaviours.at(i));
                            // and the object the event is about
                            trigObj.push_back(worldObjectsManager->getWorldObject(o));
                        }
                    }
                }
                if (m_behaviours.at(i).fulfilled("clock", player, closestObject, &m_variables)) {
                    toTrig.push_back(&m_behaviours.at(i));
                    trigObj.push_back(player);
                }
            }
        }
    }
    time++;
}

void testApp::drawIndicWalls() {
    float xTD,yTD;
    float spriteSize = 50;// TODO this shouldn't be hardcoded
    for (int i=0;i<4;i++) {
        if (i==0) {
            xTD = (float)global.m_screenWidth/2 - spriteSize/2;
            yTD = 0;
        }
        if (i==1) {
            xTD = (float)global.m_screenWidth - spriteSize;
            yTD = (float)global.m_screenHeight/2 - spriteSize/2;
        }
        if (i==2) {
            xTD = (float)global.m_screenWidth/2 - spriteSize/2;
            yTD = (float)global.m_screenHeight - spriteSize;
        }
        if (i==3) {
            xTD = 0;
            yTD = (float)global.m_screenHeight/2 - spriteSize/2;
        }
        if (currentScene()->getStuck(i)) indicWalls[i+5].draw(xTD/global.m_ratio,yTD/global.m_ratio,indicWalls[i+5].width/global.m_ratio,indicWalls[i+5].height/global.m_ratio);
        else indicWalls[i+1].draw(xTD/global.m_ratio,yTD/global.m_ratio,indicWalls[i+1].width/global.m_ratio,indicWalls[i+1].height/global.m_ratio);
    }
}

Scene * testApp::currentScene() {
    return player->getCurrentScene();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (!keys[OF_KEY_RETURN]) {
        if (key==OF_KEY_RETURN) {
            checkPlayerTrigger = true;
        }
    }
    if (!keys[' ']) {
        if (key==' ') {
            checkPlayerTrigger = true;
        }
    }
    keys[key]=true;
}

void testApp::checkInteractions(){
    // looks for all the clock events that are going to be triggered this frame
    if (checkPlayerTrigger) {
        int foundInteractions = 0;
        for (int i=0;i<m_behaviours.size();i++) {
            if (m_behaviours.at(i).fulfilled("trigger", player, closestObject, &m_variables)) {
                // push the event into these lists
                toTrig.push_back(&m_behaviours.at(i));
                // and the object the event is about
                trigObj.push_back(player);
                foundInteractions++;
            }
        }
        if (foundInteractions == 0) {
            for (int i=0;i<m_behaviours.size();i++) {
                if (m_behaviours.at(i).fulfilled("trigger fail", player, closestObject, &m_variables)) {
                    toTrig.push_back(&m_behaviours.at(i));
                    trigObj.push_back(player);
                }
            }
        }
        checkPlayerTrigger = false;
    }
}

void testApp::trigActions() {
    for (int i=0;i<toTrig.size();i++) {
        trigAction(toTrig.at(i),trigObj.at(i));
    }
}

void testApp::trigAction(Behaviour * _act, WorldObject * _obj) {
    // execute les actions listées pour cette frame
    // TODO remplacer les "if" par une méthode de testApp qui va chercher le bon objet selon un descriptor quel qu'il soit
    // TODO aussi remplacer les comparaisons de string par des choses moins gourmandes en cpu
    // avec des DEFINE ou enum peut-être ?
    WorldObject * thisTarget = NULL;
    if (_act->getTarget().at(0).o().compare("#")==0 &&
        _act->getTarget().at(0).v().compare("this")==0)
        thisTarget=_obj;
    else if (_act->getTarget().at(0).o().compare("#")==0 &&
        _act->getTarget().at(0).v().compare("carry")==0)
        thisTarget=player->getCarrying();
    else if (_act->getTarget().at(0).o().compare("#")==0 &&
        _act->getTarget().at(0).v().compare("close")==0)
        thisTarget=closestObject;
    else thisTarget = worldObjectsManager->lookForWorldTarget(_act);
    WorldObject * thisParam = NULL;
    if (_act->getParams().at(0).o().compare("#")==0 &&
        _act->getParams().at(0).v().compare("this")==0)
        thisParam=_obj;
    else if (_act->getParams().at(0).o().compare("#")==0 &&
        _act->getParams().at(0).v().compare("carry")==0)
        thisParam=player->getCarrying();
    else if (_act->getParams().at(0).o().compare("#")==0 &&
        _act->getParams().at(0).v().compare("close")==0)
        thisParam=closestObject;
    else thisParam = worldObjectsManager->lookForPoolParam(_act);
    if (_act->getAction().at(0).o().compare("sound")==0) {
        sfx[Utils::str2int(_act->getTarget().at(0).v())].play();
    }
    if (_act->getAction().at(0).o().compare("release")==0) {
        for (int i=0;i<_act->getParams().size();i++) {
            if (_act->getParams().at(i).o().compare("pZ")==0)
                thisTarget->getCarrying()->setScreenPosZ(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("maxPZ")==0)
                thisTarget->getCarrying()->setMaxZ(Utils::str2int(_act->getParams().at(i).v()));
        }
        thisTarget->carry(NULL);
    }
    if (_act->getAction().at(0).o().compare("carry")==0) {
        thisTarget->carry(thisParam);
        for (int i=0;i<_act->getParams().size();i++) {
            if (_act->getParams().at(i).o().compare("cX")==0)
                thisTarget->setCarryingPosX(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("cY")==0)
                thisTarget->setCarryingPosY(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("pZ")==0)
                thisTarget->getCarrying()->setScreenPosZ(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("maxPZ")==0)
                thisTarget->getCarrying()->setMaxZ(Utils::str2int(_act->getParams().at(i).v()));
        }
    }
    if (_act->getAction().at(0).o().compare("set")==0) {
        worldObjectsManager->setSomething(thisTarget,_act);
    }
    if (_act->getAction().at(0).o().compare("direct")==0) {
        worldObjectsManager->direct(thisTarget,_act,player);
    }
    if (_act->getAction().at(0).o().compare("give")==0) {
        WorldObject * toPass = thisTarget->getCarrying();
        if (thisTarget) thisTarget->carry(NULL);
        if (closestObject) {
            closestObject->carry(toPass);
            worldObjectsManager->setCarryingPos(_act, closestObject);
        }
        for (int i=0;i<_act->getParams().size();i++) {
            if (_act->getParams().at(i).o().compare("cX")==0)
                closestObject->setCarryingPosX(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("cY")==0)
                closestObject->setCarryingPosY(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("pZ")==0)
                closestObject->getCarrying()->setScreenPosZ(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("maxPZ")==0)
                closestObject->getCarrying()->setMaxZ(Utils::str2int(_act->getParams().at(i).v()));
        }
    }
    if (_act->getAction().at(0).o().compare("take")==0) {
        WorldObject * toPass = thisTarget->getCarrying();
        if (thisTarget) thisTarget->carry(NULL);
        if (_obj) {
            _obj->carry(toPass);
            worldObjectsManager->setCarryingPos(_act, _obj);
        }
        for (int i=0;i<_act->getParams().size();i++) {
            if (_act->getParams().at(i).o().compare("cX")==0)
                thisTarget->setCarryingPosX(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("cY")==0)
                thisTarget->setCarryingPosY(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("pZ")==0)
                thisTarget->getCarrying()->setScreenPosZ(Utils::str2int(_act->getParams().at(i).v()));
            if (_act->getParams().at(i).o().compare("maxPZ")==0)
                thisTarget->getCarrying()->setMaxZ(Utils::str2int(_act->getParams().at(i).v()));
        }
    }
    if (_act->getAction().at(0).o().compare("replace")==0) {
        if (thisTarget) worldObjectsManager->replaceWorldObject(_act, thisTarget);
    }
    if (_act->getAction().at(0).o().compare("add")==0) {
        worldObjectsManager->addWorldObject(_act);
    }
    if (_act->getAction().at(0).o().compare("remove")==0) {
        worldObjectsManager->removeWorldObject(thisTarget);// TODO automatically check for releases
    }
    if (_act->getAction().at(0).o().compare("move")==0) {
        worldObjectsManager->move(_act,thisTarget,m_world->getArray());
    }
    if (_act->getAction().at(0).o().compare("moveTo")==0) {
        worldObjectsManager->moveTo(_act,thisTarget);
    }
    if (_act->getAction().at(0).o().compare("stuck")==0) {
        int sX=0;
        int sY=0;
        for (int i=0;i<_act->getTarget().size();i++) {
            if (_act->getTarget().at(i).o().compare("sX")==0) sX = Utils::str2int(_act->getTarget().at(i).v());
            if (_act->getTarget().at(i).o().compare("sY")==0) sY = Utils::str2int(_act->getTarget().at(i).v());
        }
        for (int i=0;i<_act->getParams().size();i++) {
            m_world->GetElemAt(sX,sY)->setStuck(xmlLoader.directionStrToInt(_act->getParams().at(i).o()),_act->getParams().at(i).v()=="0"?0:1);
        }
    }
    if (_act->getAction().at(0).o().compare("message")==0) {
        string txt = "";
        int counter = 0;
        for (int i=0;i<_act->getParams().size();i++) {
            if (_act->getParams().at(i).o().compare("txt")==0)
                txt = generateMessage(_act->getParams().at(0).v());
            if (_act->getParams().at(i).o().compare("counter")==0)
                counter = Utils::str2int(_act->getParams().at(i).v());
        }
        if (_act->getTarget().at(0).o().compare("#")==0 &&
            _act->getTarget().at(0).v().compare("upper")==0 )
            upperText = txt;
        else if (_act->getTarget().at(0).o().compare("#")==0 &&
            _act->getTarget().at(0).v().compare("lower")==0 )
            lowerText = txt;
        else if (_act->getTarget().at(0).o().compare("#")==0 &&
            _act->getTarget().at(0).v().compare("middle")==0 ) {
            middleText = txt;
            middleTextCounter = 100;
        } else {
            thisTarget->setMessage(txt);
            thisTarget->setMessageCounter(counter);
        }
    }
    if (_act->getAction().at(0).o().compare("fill")==0) {
        worldObjectsManager->fill(_act);
    }
    if (_act->getAction().at(0).o().compare("console")==0) {
        cout << _act->getParams().at(0).v() << endl;
    }
    if (_act->getAction().at(0).o().compare("var")==0) {
        for (int i=0;i<m_variables.size();i++) {
            if (_act->getTarget().at(0).o().compare("name")==0 &&
                _act->getTarget().at(0).v().compare(m_variables[i].name)==0) {
                if (_act->getParams().at(0).o().compare("add")==0)
                    m_variables[i].add(Utils::str2int(_act->getParams().at(0).v()));
                if (_act->getParams().at(0).o().compare("set")==0)
                    m_variables[i].set(Utils::str2int(_act->getParams().at(0).v()));
                if (_act->getParams().at(0).o().compare("sub")==0)
                    m_variables[i].set(Utils::str2int(_act->getParams().at(0).v()));
             }
        }
    }
}

string testApp::generateMessage(string in) {
    // remplace les #tags des messages par les objets auquels ils font référence
    string closeName = "";
    string carryName = "";
    if (closestObject) closeName = closestObject->getScreenName();
    if (player->getCarrying()) carryName = player->getCarrying()->getScreenName();
    Utils::str_replace("#close",closeName,in);
    Utils::str_replace("#carry",carryName,in);
    for (int j=0;j<m_variables.size();j++) {
        string code = "#var$" + m_variables[j].name + "$";
        Utils::str_replace(code,Utils::intToStr(m_variables[j].value),in);
    }
    return in;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    keys[key]=false;
}

void testApp::checkClosestObject() {
    // return the object that is closest and collides with the player
    closestObject = NULL;
    float closestDist = NULL;
    for (int i=0 ; i<worldObjectsManager->getNbWorldObjects() ; i++) {
        if (worldObjectsManager->getWorldObject(i)->getLogic()) {
            if (worldObjectsManager->getWorldObject(i) != player->getCarrying()) {
                if (player->isCloseTo(worldObjectsManager->getWorldObject(i))) {
                    bool goodDistance = true;
                    float thisSqDist = worldObjectsManager->getWorldObject(i)->sqDistanceFrom(player);
                    if (closestDist) {
                        if (thisSqDist >= closestDist) goodDistance=false;
                    }
                    if (i==0 || goodDistance){
                        closestObject = worldObjectsManager->getWorldObject(i);
                        closestDist   = thisSqDist;
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
