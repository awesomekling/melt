#include "Element.h"
#include "Parser.h"
#include "Dump.h"

int main()
{
    std::string html = "<html><head><title>my page</title></head><body><h1>Hi there</h1><p>Hello World!</p></body></html>";
    auto doc = parse(html);

    dump_tree(doc);
}

