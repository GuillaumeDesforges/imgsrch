#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "img/image.h"
#include "clustering/kmeanstree.h"

#include "boost/filesystem.hpp"

std::vector<std::string> get_file_list(const std::string& path)
{
    std::vector<std::string> m_file_list;
    if (!path.empty())
    {
        namespace fs = boost::filesystem;
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
