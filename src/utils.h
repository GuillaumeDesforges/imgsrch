#pragma once

std::vector<std::string> get_file_list(const std::string& path)
{
    std::vector<std::string> m_file_list;
    if (!path.empty())
    {
        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            const fs::path cp = (*i);
            m_file_list.push_back(cp.string());
        }
    }
    return m_file_list;
}
