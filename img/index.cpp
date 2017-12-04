#include "index.h"

// Pour créer l'index à partir de la base de données d'images


// Create the index
// de la forme index={ (bonjour, {1,4,7}) , (ponts, {4,8,10}) ... }
map<string, vector<int>> create_index(map<int,Image> data_base){
    //data_base est une liste de doublets (numero_image, image) [ -> à construire]

    map<string, vector<int>> index;
    map<string, vector<int>>::iterator it; // (defini plus bas)

    // On parcourt la base de donées
    for (int k=0;k<data_base.size(); k++){
        pair<int,Image> image=data_base[k];
        vector<string> words_list= kmeanstree(image.second);

        for (int p=0;p<words_list.size();p++){
            string word=words_list[p];
            it=index.find(word); // =v.end() si le mot n'a pas été rencontré avant dans aucune image
            if (it!= v.end()){
                it->second.push_back(image.first);
            }
            else{ //si c'est un nouveau mot
                vector<string> liste_images;
                liste_images.push_back(image[0]);
                index.insert(pair<string, vector<int>>(word,liste_images))
            }
        }
    }
    return index;
}
