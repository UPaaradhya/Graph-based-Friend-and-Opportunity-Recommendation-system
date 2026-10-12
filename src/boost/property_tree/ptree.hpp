#pragma once
#include <string>
#include <list>
#include <stdexcept>
#include <sstream>
#include <type_traits>
namespace boost {
template<class T> class optional { using base_type = typename std::remove_reference<T>::type; base_type* p; public: optional():p(0){} optional(base_type* v):p(v){} explicit operator bool() const{return p!=0;} base_type* operator->(){return p;} const base_type* operator->()const{return p;} T get(){return *p;} const base_type& get()const{return *p;} };
namespace property_tree {
class ptree {
public:
 using value_type=std::pair<std::string,ptree>; using container_type=std::list<value_type>; using iterator=container_type::iterator; using const_iterator=container_type::const_iterator;
 container_type children; std::string data;
 iterator begin(){return children.begin();} iterator end(){return children.end();} const_iterator begin()const{return children.begin();} const_iterator end()const{return children.end();}
 bool empty()const{return children.empty();} value_type& front(){return children.front();} const value_type& front()const{return children.front();}
 template<class T> T get_value()const { std::stringstream s(data); T v{}; if constexpr(std::is_same<T,std::string>::value) return data; else { if(!(s>>v)) throw std::runtime_error("Invalid JSON value"); return v; } }
 ptree& get_child(const std::string& key){for(auto& x:children)if(x.first==key)return x.second;throw std::runtime_error("Missing JSON field: "+key);}
 const ptree& get_child(const std::string& key)const{for(auto& x:children)if(x.first==key)return x.second;throw std::runtime_error("Missing JSON field: "+key);}
 boost::optional<ptree&> get_child_optional(const std::string& key){for(auto& x:children)if(x.first==key)return boost::optional<ptree&>(&x.second);return boost::optional<ptree&>();}
 template<class T> T get(const std::string& key,const T& fallback)const {try{return get_child(key).template get_value<T>();}catch(...){return fallback;}}
};
}
}
