#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
using namespace std;

#include "image.h"

// Pour créer l'index à partir de la base de données d'images
class Index {
public:
    void indexImage(Image &image);
    void indexImages(vector<Image>& images);
    map<string, double> getScores(const Image &image);
private:
    map<string, set<string>> index;
    set<string> getImagesPathWithWord(string word);
};
