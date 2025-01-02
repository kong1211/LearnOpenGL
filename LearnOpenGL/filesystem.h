#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>
#include "root_directory.h" // This is a configuration file generated by CMake.

class FileSystem
{
private:
    typedef std::string(*Builder) (const std::string& path);

public:
    static std::string getPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    static std::string const& getRoot()
    {
        //static char const* envRoot = _dupenv_s("LOGL_ROOT_PATH");
        //static char const* givenRoot = (envRoot != nullptr ? envRoot : logl_root);
        //static std::string root = (givenRoot != nullptr ? givenRoot : "");
        static std::string root = "";
        return root;
    }

    //static std::string(*foo (std::string const &)) getPathBuilder()
    static Builder getPathBuilder()
    {
        if (getRoot() != "")
            return &FileSystem::getPathRelativeRoot;
        else
            return &FileSystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("/") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }


};

// FILESYSTEM_H
#endif
