#pragma once

#include <map>
#include <vector>
#include <string>
using namespace std;

#include "image.h"

// Pour créer l'index à partir de la base de données d'images
class Index {
public:
    void indexImages(map<int, Image*> &database);
private:
    map<string, vector<int>> index;
};
