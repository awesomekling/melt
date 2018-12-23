#include "Parser.h"
#include "Element.h"
#include "Text.h"

static Ref<Element> createElement(const std::string& tag_name)
{
    auto element = adoptRef(*new Element(tag_name));
    return element;
}

Ref<Document> parse(const std::string& html)
{
    auto doc = adoptRef(*new Document);

    auto head = createElement("head");
    auto title = createElement("title");
    auto title_text = adoptRef(*new Text("Page Title"));
    title->append_child(std::move(title_text));
    head->append_child(std::move(title));

    doc->append_child(std::move(head));

    auto body = createElement("body");
    auto h1 = createElement("h1");
    auto h1_text = adoptRef(*new Text("Hello World!"));

    h1->append_child(std::move(h1_text));
    body->append_child(std::move(h1));
    doc->append_child(std::move(body));

    return doc;
}

