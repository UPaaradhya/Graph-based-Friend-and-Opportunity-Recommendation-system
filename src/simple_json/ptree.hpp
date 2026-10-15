#pragma once
#include <string>
#include <list>
#include <stdexcept>
#include <sstream>
#include <type_traits>
#include <utility>

namespace simple_json {

template<class T>
class optional {
    using base_type = typename std::remove_reference<T>::type;
    base_type* value;
public:
    optional() : value(nullptr) {}
    optional(base_type* item) : value(item) {}
    explicit operator bool() const { return value != nullptr; }
    base_type* operator->() { return value; }
    const base_type* operator->() const { return value; }
    T get() { return *value; }
    const base_type& get() const { return *value; }
};

class ptree {
public:
    using value_type = std::pair<std::string, ptree>;
    using container_type = std::list<value_type>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;

    container_type children;
    std::string data;

    iterator begin() { return children.begin(); }
    iterator end() { return children.end(); }
    const_iterator begin() const { return children.begin(); }
    const_iterator end() const { return children.end(); }
    bool empty() const { return children.empty(); }
    value_type& front() { return children.front(); }
    const value_type& front() const { return children.front(); }

    template<class T>
    T get_value() const {
        if constexpr (std::is_same<T, std::string>::value) {
            return data;
        } else {
            std::stringstream stream(data);
            T result{};
            if (!(stream >> result)) throw std::runtime_error("Invalid JSON value");
            return result;
        }
    }

    ptree& get_child(const std::string& key) {
        for (auto& item : children) if (item.first == key) return item.second;
        throw std::runtime_error("Missing JSON field: " + key);
    }
    const ptree& get_child(const std::string& key) const {
        for (const auto& item : children) if (item.first == key) return item.second;
        throw std::runtime_error("Missing JSON field: " + key);
    }
    optional<ptree&> get_child_optional(const std::string& key) {
        for (auto& item : children) if (item.first == key) return optional<ptree&>(&item.second);
        return optional<ptree&>();
    }
    template<class T>
    T get(const std::string& key, const T& fallback) const {
        try { return get_child(key).template get_value<T>(); }
        catch (...) { return fallback; }
    }
};
}
