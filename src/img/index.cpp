#include "index.h"

void Index::indexImages(map<int, Image*> &database){
    // Iterate database
    for(auto &entry : database) {
        // Extract data from database entry
        int image_id = entry.first;
        Image &image = *(entry.second);
        // Extract word list from image
        const vector<string> &words = image.getWords();
        // Add image to index (word -> image_id)
        for(const auto &word : words) {
            // Find index entry for word
            map<string, vector<int>>::iterator it = index.find(word);
            if(it == index.end()) {
                // Word was not found, add this key to the index
                vector<int> image_id_list;
                index.insert(pair<string, vector<int>>(word, image_id_list));
            }
            // Add image id to this key
            index[word].push_back(image_id);
        }
    }
}
