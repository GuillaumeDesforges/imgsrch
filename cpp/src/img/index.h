#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
using namespace std;

#include "image.h"

#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>

// Pour créer l'index à partir de la base de données d'images
class Index {
public:
    void indexImage(Image &image);
    void indexImages(vector<Image>& images);
    map<string, float> getScores(const Image &image);
    set<string> getImagesPathWithWord(string word);
    set<string> getPossibleWords();
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(index);
    }
    map<string, set<string>> index;
};
