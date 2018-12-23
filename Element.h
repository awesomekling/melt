#pragma once

#include "ParentNode.h"
#include <string>

class Attribute {
public:
    Attribute(std::string&& name, std::string&& value)
        : m_name(std::move(name))
        , m_value(std::move(value))
    {
    }

private:
    std::string m_name;
    std::string m_value;
};

class Element : public ParentNode {
public:
    explicit Element(std::string tag_name);
    virtual ~Element() override;

    const std::string& tag_name() const { return m_tag_name; }

private:
    std::string m_tag_name;
    std::vector<Attribute> m_attributes;
};

