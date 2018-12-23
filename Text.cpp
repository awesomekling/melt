#include "Text.h"

Text::Text(std::string&& data)
    : Node(NodeType::TEXT_NODE)
    , m_data(std::move(data))
{
}

Text::~Text()
{
}


