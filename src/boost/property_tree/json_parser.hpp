#pragma once
#include "ptree.hpp"
#include <istream>
#include <cctype>
namespace boost { namespace property_tree {
class json_reader {
 std::string s; size_t i=0;
 void ws(){while(i<s.size()&&std::isspace((unsigned char)s[i]))i++;}
 std::string str(){std::string o; if(s[i++]!='"')throw std::runtime_error("Expected string"); while(i<s.size()){char c=s[i++];if(c=='"')return o;if(c=='\\'&&i<s.size()){char e=s[i++];if(e=='n')o+='\n';else if(e=='r')o+='\r';else if(e=='t')o+='\t';else o+=e;}else o+=c;}throw std::runtime_error("Unclosed string");}
 ptree value(){ws();ptree n;if(i>=s.size())throw std::runtime_error("Unexpected end of JSON");if(s[i]=='{'){i++;ws();while(i<s.size()&&s[i]!='}'){ws();std::string k=str();ws();if(s[i++]!=':')throw std::runtime_error("Expected colon");n.children.push_back({k,value()});ws();if(s[i]==','){i++;continue;}break;}if(i>=s.size()||s[i++]!='}')throw std::runtime_error("Expected object end");}else if(s[i]=='['){i++;ws();while(i<s.size()&&s[i]!=']'){n.children.push_back({"",value()});ws();if(s[i]==','){i++;continue;}break;}if(i>=s.size()||s[i++]!=']')throw std::runtime_error("Expected array end");}else if(s[i]=='"'){n.data=str();}else{size_t b=i;while(i<s.size()&&s[i]!=','&&s[i]!=']'&&s[i]!='}'&&!std::isspace((unsigned char)s[i]))i++;n.data=s.substr(b,i-b);if(n.data=="null")n.data="";}return n;}
public: explicit json_reader(std::string text):s(std::move(text)){} ptree parse(){return value();}
};
inline void read_json(std::istream& in, ptree& tree){std::string s((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());tree=json_reader(s).parse();}
}}
