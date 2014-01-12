#pragma once

#include "ofMain.h"
#include "Global.h"

class Scene
{
    public:

        Scene();
        Scene(int _idCo, int _idLi, ofImage * _bgPicture, string _name, ofSoundPlayer * _sndLoop);
        ~Scene();
        void draw();

        string getName();

        ofPoint getLink(int _i);
        void setLink(int _i, ofPoint link);
        void setLinkX(int _i, int _x);
        void setLinkY(int _i, int _y);

        bool getStuck(int _i);
        void setStuck(int _i, bool stuck);

        bool getHide(int _i);
        void setHide(int _i, bool hide);

        ofSoundPlayer * getSndLoop();

        void setGlobal(Global * global);

    protected:

    private:

        string m_name;

        int txtMargH;
        int txtMargW;

        int m_idX;
        int m_idY;
        ofImage * m_bgPicture;

        ofSoundPlayer * m_sndLoop;

        ofPoint m_link[4];
        bool m_stuck[4];
        bool m_hide[4];

        Global * global;

};

