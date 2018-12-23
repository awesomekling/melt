#include "Dump.h"
#include "Node.h"
#include "Element.h"
#include "Text.h"
#include "Document.h"

void dump_tree(Node& node)
{
    static int indent = 0;
    for (int i = 0; i < indent; ++i)
        printf("   ");
    if (node.is_document()) {
        printf("*Document*\n");
    } else if (node.is_element()) {
        printf("<%s>\n", static_cast<Element&>(node).tag_name().c_str());
    } else if (node.is_text()) {
        printf("\"%s\"\n", static_cast<Text&>(node).data().c_str());
    }
    ++indent;
    if (node.is_parent_node()) {
        static_cast<ParentNode&>(node).for_each_child([] (Node& child) {
            dump_tree(child);
        });
    }
    --indent;
}

