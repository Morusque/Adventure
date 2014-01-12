
#include "AssetsPool.h"

// makes lists of assets
// and pointers to them is they have already been requested before

ofImage * AssetsPool::requestImg(string _url) {
    for (int i=0;i<imageFiles.size();i++) {
        if (imageFiles.at(i).compare(_url)==0) {
            return images.at(i);
        }
    }
    imageFiles.push_back(_url);
    ofImage * thisImage = new ofImage(_url);
    processImage(thisImage);
    images.push_back(thisImage);
    return images.at(images.size()-1);
}

void AssetsPool::processImage(ofImage * _im) {
    for (int y=0;y<_im->height;y++) {
        for (int x=0;x<_im->width;x++) {
          ofColor c = _im->getColor(x, y);
          float thisHue, thisSat, thisBri;
          c.getHsb(thisHue,thisSat,thisBri);
          thisHue = fmod(thisHue+rand()%10,0xFF);
          thisSat = Utils::constrain(thisSat/3+rand()%10, 0, 0xFF);
          thisBri = Utils::constrain(thisBri+rand()%10, 0, 0xFF);
          float thisAlp = Utils::constrain(c.a, 0, 0xFF);
          ofColor c2 = ofColor::fromHsb(thisHue, thisSat, thisBri);
          c2.a = thisAlp;
          _im->setColor(x, y, c2);
        }
    }
    _im->update();
    cout << ".";
}

ofSoundPlayer * AssetsPool::requestSnd(string _url) {
    for (int i=0;i<soundFiles.size();i++) {
        if (soundFiles.at(i).compare(_url)==0) {
            return sounds.at(i);
        }
    }
    soundFiles.push_back(_url);
    sounds.push_back(new ofSoundPlayer());
    sounds.at(sounds.size()-1)->loadSound(_url);
    return sounds.at(sounds.size()-1);
}

AssetsPool::~AssetsPool() {
    for (int i=0;i<images.size();i++) {
        delete images.at(i);
    }
    for (int i=0;i<sounds.size();i++) {
        delete sounds.at(i);
    }
}
