#include "index.h"

#include <stdio.h>
using namespace std;

void Index::indexImage(Image &image) {
    // Extract word list from image
    const vector<string> &words = image.getWords();
    // Add image to index (word -> image_id)
    for(const auto &word : words) {
        // Find index entry for word
        auto it = index.find(word);
        if(it == index.end()) {
            // Word was not found, add this key to the index
            set<string> images_path_list;
            index.insert(pair<string, set<string>>(word, images_path_list));
        }
        // Add image id to this key
        index[word].insert(image.getPath());
    }
}

void Index::indexImages(vector<Image>& images){
    // Iterate database
    for(auto &image : images) {
        // Extract data from database entry
        indexImage(image);
    }
}

set<string> Index::getImagesPathWithWord(string word) {
    return index[word];
}

set<string> Index::getPossibleWords() {
    set<string> words;
    for(map<string, set<string>>::iterator it = index.begin(); it != index.end(); ++it) {
        words.insert(it->first);
    }
    return words;
}

map<string, float> Index::getScores(const Image &image) {
    map<string, float> scores;
    vector<string> words_vect = image.getWords();
    set<string> words(words_vect.begin(), words_vect.end());
    for(auto& word : words) {
        for(auto& image_path : getImagesPathWithWord(word)) {
            scores[image_path]++;
        }
    }
    for(auto& entry : scores) {
        entry.second /= words.size();
    }
    return scores;
}
