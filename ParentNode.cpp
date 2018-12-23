#include "ParentNode.h"

void ParentNode::append_child(Ref<Node>&& node)
{
    if (m_last_child)
        m_last_child->set_next_sibling(node.ptr());
    m_last_child = &node.leakRef();
    if (!m_first_child)
        m_first_child = m_last_child;
}
