#include "Node.h"
#include "Ref.h"

Node::Node(NodeType type)
    : m_type(type)
{
}

Node::~Node()
{
}

void Node::ref()
{
    ASSERT(m_ref_count);
    ++m_ref_count;
}

void Node::deref()
{
    ASSERT(m_ref_count);
    if (!--m_ref_count)
        delete this;
}
