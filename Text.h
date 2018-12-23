#pragma once

#include "Node.h"
#include <string>

class Text final : public Node {
public:
    explicit Text(std::string&&);
    virtual ~Text() override;

    const std::string& data() const { return m_data; }

private:
    std::string m_data;
};
