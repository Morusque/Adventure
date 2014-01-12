#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Array2D.cpp"
#include "WorldObject.h"
#include "XmlLoader.h"
#include "WorldObjectsManager.h"
#include "Behaviour.h"
#include "Variable.h"
#include "Utils.h"
#include "Global.h"

class testApp : public ofBaseApp    {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    private:

        int time;

        void updateObjScenes();

        Array2D<Scene> * m_world;
        WorldObject * player;
        XmlLoader xmlLoader;

        WorldObjectsManager * worldObjectsManager;

        float playerSpeed;

        bool keys[0xFFFF];

        void checkInteractions();
        void movePlayer();

        void setTextMargins();

        void drawIndicWalls();
        ofImage indicWalls[9];

        ofSoundPlayer * sfx;
        float musicVolume;

        void checkMusicLoop();
        void checkWindowSize();

        void drawText();
        float tT;//TODO for test, remove this

        bool checkPlayerTrigger;

        WorldObject * closestObject;
        void checkClosestObject();

        Scene * currentScene();

        ofSoundPlayer * currentSndLoop;

        string lowerText;
        string upperText;
        string middleText;
        int middleTextCounter;

        string generateMessage(string in);

        string floatToStr(float _f);

        vector<Behaviour> m_behaviours;
        vector<Variable> m_variables;

        void trigActions();
        void trigAction(Behaviour * _act, WorldObject * _obj);

        void clockEvents();
        void moveCarryings();
        void act();

        void initTrigger(int trigId);

        vector<Behaviour*> toTrig;
        vector<WorldObject*> trigObj;

        Global global;

};
