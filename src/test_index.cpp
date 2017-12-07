#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "img/image.h"
#include "clustering/kmeanstree.h"

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;


void create_database(string path_to_imagesFolder){
  //map<int, Image*> database;
  //int k=0;

  fs::path path_to_imFolder(path_to_imagesFolder);
  fs::directory_iterator end_iter;

  typedef std::multimap<std::time_t, fs::path> result_set_t;
  result_set_t result_set;

  if ( fs::exists(path_to_imagesFolder) && fs::is_directory(path_to_imagesFolder)){
    for( fs::directory_iterator dir_iter(path_to_imagesFolder) ; dir_iter != end_iter ; ++dir_iter){
      if (fs::is_regular_file(dir_iter->status()) ){
        result_set.insert(result_set_t::value_type(fs::last_write_time(dir_iter->path()), *dir_iter));
      }
    }
  }
  //Image * image(p);
  //database.insert(pair<int,Image*>(k,image));
}


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

int main(int argc, char** argv )
{
    auto test = get_file_list(".");
    for(auto& s : test) {
        cout << s << endl;
    }
    return 0;
}
