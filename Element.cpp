#include "Element.h"

Element::Element(std::string tag_name)
    : ParentNode(NodeType::ELEMENT_NODE)
    , m_tag_name(std::move(tag_name))
{
}

Element::~Element()
{
}

