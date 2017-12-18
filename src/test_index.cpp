#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "img/image.h"
#include "clustering/kmeanstree.h"

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;

// Pour creer database = (1,{bonjour, hello, ponts}) ,  (2, {allo, bonjour}) , (3,{ponts})...}

std::vector<std::string> get_file_list(const std::string& path)
{
    std::vector<std::string> m_file_list;
    if (!path.empty())
    {
        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            cout << (*i) << endl;
            const fs::path cp = (*i);
            m_file_list.push_back(cp.string());
        }
    }
    return m_file_list;
}

map<int,Image*> create_database(vector<string> file_list){
  map<int,Image*> database;
  for(int k=0; k<file_list.size();k++){
    database.insert(pair<int,Image*>(k,new Image (file_list[k])));
  }
}

int main(int argc, char** argv )
{
    auto test = get_file_list("/1.jpg");
    for(auto& s : test) {
        cout << s << endl;
        Image img(s);
        img.display();
        //click();
    }
    create_database(test);
    return 0;
}
