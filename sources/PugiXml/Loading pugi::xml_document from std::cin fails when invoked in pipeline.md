## [Loading pugi::xml_document from std::cin fails when invoked in pipeline](https://stackoverflow.com/questions/10419274/loading-pugixml-document-from-stdcin-fails-when-invoked-in-pipeline)
Using pugixml 1.0
Loading an XML document from std::cin works when I use shell STDIN redirection:
```shell
$ ./pugitest < sample.xml # OK
```
But when invoked in a pipeline, it fails:
```shell
$ cat sample.xml | ./pugitest # FAILS
./pugitest: Error reading from file/stream
```
This is the code of the pugitest program:
```cpp
#include "pugixml.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char *const argv[])
{
    try {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load(std::cin);
        if (!result) {
            throw std::runtime_error(result.description());
        }
    } catch (std::exception& e) {
        std::cerr << argv[0] << ": " << e.what() << '\n';
        return 1;
    }

    return 0;
}
```
I don't understand the reason.


1 Answer


pugixml 1.0 expects input stream to be seekable. Seek works if the stream is bound to a file, but fails if the stream is bound to a pipe.

Starting from pugixml 1.2 (released... yesterday :)), non-seekable streams are accepted as load() source. Your sample works when compiled using 1.2.
