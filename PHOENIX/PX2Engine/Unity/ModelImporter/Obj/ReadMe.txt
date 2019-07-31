The source code in this folder is an implementation for loading .obj and .mtl
files.  Not all features of .obj are supported.  If you need any new features,
let me know.

For now, include these files in an application that needs to load .obj and
.mtl files.  The top-level call is

    #include "ObjLoader.h"
    std::string path = <path to folder containing the .obj file>;
    std::string filename = <name of .obj file>;
    ObjLoader loader(path, filename);

The interface of class ObjLoader allows you to access the information that
was loaded from the .obj file and from a .mtl file that was specified in
the .obj file.

Writing a small sample on using this code in Wild Magic 5 is an item on my
TODO list.  I have it running in a larger application, but have not yet had
the time to simplify and illustrate.  If you need a sample, let me know.
