
#include "Utils.h"

namespace Utils
{

    std::vector<std::string> explode(const std::string & str, char separator )
    {
       std::vector< std::string > result;
       size_t pos1 = 0;
       size_t pos2 = 0;
       while ( pos2 != str.npos )
       {
          pos2 = str.find(separator, pos1);
          if ( pos2 != str.npos )
          {
             if ( pos2 > pos1 )
                result.push_back( str.substr(pos1, pos2-pos1) );
             pos1 = pos2+1;
          }
       }
       result.push_back( str.substr(pos1, str.size()-pos1) );
       return result;
    }

    int hexToInt(string hex) {
        unsigned long x;
        signed long val;
        std::stringstream ss;
        ss << std::hex << hex;
        ss >> x;
        val = (signed long)x;
        return val;
    }

    float constrain(float orig, float min, float max) {
        if (orig<min) orig=min;
        if (orig>max) orig=max;
        return orig;
    }

    int str2int (const string &str) {
      stringstream ss(str);
      int num;
      if((ss >> num).fail());
      return num;
    }

    string& str_replace(const string &search, const string &replace, string &subject)
    {
        string buffer;

        int sealeng = search.length();
        int strleng = subject.length();

        if (sealeng==0)
            return subject;//no change

        for(int i=0, j=0; i<strleng; j=0 )
        {
            while (i+j<strleng && j<sealeng && subject[i+j]==search[j])
                j++;
            if (j==sealeng)//found 'search'
            {
                buffer.append(replace);
                i+=sealeng;
            }
            else
            {
                buffer.append( &subject[i++], 1);
            }
        }
        subject = buffer;
        return subject;
    }

    ofPoint shorterFrom(vector<ofPoint> * _pt, float _x, float _y) {// this is probably too specific to be in ::Utils
        if (_pt->size()==0) return NULL;
        if (_pt->size()==1) return _pt->at(0);
        float shorter = NULL;
        int shorterIndex = NULL;
        for (int i=0;i<_pt->size();i++) {
            float thisDist = ofDistSquared(_x,_y,_pt->at(i).x,_pt->at(i).y);
            if (thisDist < shorter) {
                shorterIndex = i;
                shorter = thisDist;
            }
        }
        return _pt->at(shorterIndex);
    }

    string floatToStr(float f) {
        stringstream ss (stringstream::in | stringstream::out);
        ss << f;
        string test = ss.str();
        return test;
    }

    string intToStr(int i) {
        stringstream ss (stringstream::in | stringstream::out);
        ss << i;
        string test = ss.str();
        return test;
    }

    string intToStr(int i, int l) {
        stringstream ss (stringstream::in | stringstream::out);
        ss << i;
        string test = ss.str();
        while (test.length()<l) test = "0" + test;
        return test;
    }

}

