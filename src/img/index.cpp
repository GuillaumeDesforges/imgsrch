#include "index.h"

#include <stdio.h>
using namespace std;

void Index::indexImage(int image_id, Image &image) {
    // Extract word list from image
    const vector<string> &words = image.getWords();
    // Add image to index (word -> image_id)
    for(const auto &word : words) {
        // Find index entry for word
        map<string, set<int>>::iterator it = index.find(word);
        if(it == index.end()) {
            // Word was not found, add this key to the index
            set<int> image_id_list;
            index.insert(pair<string, set<int>>(word, image_id_list));
        }
        // Add image id to this key
        index[word].insert(image_id);
    }
}

void Index::indexImages(map<int, Image*> &database){
    // Iterate database
    for(auto &entry : database) {
        // Extract data from database entry
        int image_id = entry.first;
        Image &image = *(entry.second);
        indexImage(image_id, image);
    }
}

const set<int> &Index::getImagesIdWithWord(string word) const {
    return index.find(word)->second;
}

map<int, int> Index::getVotes(const Image &image) const {
    map<int, int> votes;
    for(auto& word : image.getWords()) {
        for(auto& image_id : getImagesIdWithWord(word)) {
            votes[image_id]++;
        }
    }
    return votes;
}
