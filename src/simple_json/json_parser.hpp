#pragma once
#include "ptree.hpp"
#include <istream>
#include <iterator>
#include <cctype>
#include <utility>

namespace simple_json {
class json_reader {
    std::string text;
    size_t position = 0;

    void skipWhitespace() {
        while (position < text.size() && std::isspace(static_cast<unsigned char>(text[position]))) position++;
    }
    std::string readString() {
        std::string result;
        if (text[position++] != '"') throw std::runtime_error("Expected JSON string");
        while (position < text.size()) {
            char ch = text[position++];
            if (ch == '"') return result;
            if (ch == '\\') {
                if (position >= text.size()) break;
                char escaped = text[position++];
                switch (escaped) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    default: result += escaped; break;
                }
            } else result += ch;
        }
        throw std::runtime_error("Unclosed JSON string");
    }
    ptree readValue() {
        skipWhitespace();
        if (position >= text.size()) throw std::runtime_error("Unexpected end of JSON");
        ptree node;
        if (text[position] == '{') {
            position++; skipWhitespace();
            while (position < text.size() && text[position] != '}') {
                skipWhitespace();
                std::string key = readString(); skipWhitespace();
                if (position >= text.size() || text[position++] != ':') throw std::runtime_error("Expected colon in JSON");
                node.children.push_back({key, readValue()}); skipWhitespace();
                if (position < text.size() && text[position] == ',') { position++; continue; }
                break;
            }
            if (position >= text.size() || text[position++] != '}') throw std::runtime_error("Expected JSON object end");
        } else if (text[position] == '[') {
            position++; skipWhitespace();
            while (position < text.size() && text[position] != ']') {
                node.children.push_back({"", readValue()}); skipWhitespace();
                if (position < text.size() && text[position] == ',') { position++; continue; }
                break;
            }
            if (position >= text.size() || text[position++] != ']') throw std::runtime_error("Expected JSON array end");
        } else if (text[position] == '"') node.data = readString();
        else {
            size_t start = position;
            while (position < text.size() && text[position] != ',' && text[position] != ']' && text[position] != '}' && !std::isspace(static_cast<unsigned char>(text[position]))) position++;
            node.data = text.substr(start, position - start);
            if (node.data == "null") node.data.clear();
        }
        return node;
    }
public:
    explicit json_reader(std::string input) : text(std::move(input)) {}
    ptree parse() { return readValue(); }
};
inline void read_json(std::istream& input, ptree& tree) {
    std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    tree = json_reader(text).parse();
}
}
