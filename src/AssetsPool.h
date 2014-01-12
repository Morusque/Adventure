
#pragma once

#include "ofMain.h"
#include "Utils.h"
#include "math.h"

class AssetsPool
{

    public:

        ~AssetsPool();
        ofImage * requestImg(string _url);
        ofSoundPlayer * requestSnd(string _url);

    protected:

    private:

        vector<ofImage*> images;
        vector<string> imageFiles;

        vector<ofSoundPlayer*> sounds;
        vector<string> soundFiles;

        void processImage(ofImage * _im);
};
