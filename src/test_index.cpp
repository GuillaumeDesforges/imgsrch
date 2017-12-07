#include <stdio.h>
#include <experimental/filesystem>
namespace fs = filesystem;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "img/image.h"
#include "clustering/kmeanstree.h"


namespace fs = boost::filesystem;


void create_database(string path_to_imagesFolder){
  //map<int, Image*> database;
  //int k=0;

  fs::path path_to_imFolder(path_to_imagesFolder);
  fs::directory_iterator end_iter;

  typedef std::multimap<std::time_t, fs::path> result_set_t;
  result_set_t result_set;

  if ( fs::exists(someDir) && fs::is_directory(someDir)){
    for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter){
      if (fs::is_regular_file(dir_iter->status()) ){
        result_set.insert(result_set_t::value_type(fs::last_write_time(dir_iter->path()), *dir_iter));
      }
    }
  }
  //Image * image(p);
  //database.insert(pair<int,Image*>(k,image));
}



int main(int argc, char** argv )
{

    return 0;
}
