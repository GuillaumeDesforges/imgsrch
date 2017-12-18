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
    void indexImage(int image_id, Image &image);
    void indexImages(map<int, Image*> &database);
    map<int, int> getVotes(const Image &image) const;
private:
    map<string, set<int>> index;
    const set<int> &getImagesIdWithWord(string word) const;
};
