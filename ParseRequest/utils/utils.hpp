#ifndef UTILS_HPP
# define UTILS_HPP

 # include <iostream>
 # include <vector>
 # include <string>
 # include <algorithm>
 # include <exception>

bool space(char c)
{
    return std::isspace(c);
}
 
bool notspace(char c)
{
    return !std::isspace(c);
}

int del(int c)
{
    if(c == '\r' || c == '\n')
        return(1);
    return(0);
}

bool delim(char c)
{
    return del(c);
}
bool notdelim(char c)
{
    return !del(c);
}

int checkColon(int c)
{
    if(c == ':')
        return(1);
    return(0);
}

bool colon(char c)
{
    return checkColon(c);
}
bool notColon(char c)
{
    return !checkColon(c);
}

std::vector<std::string> split(const std::string& s)
{
    typedef std::string::const_iterator iter;
    std::vector<std::string> res;
    iter i = s.begin();
    while(i!=s.end())
    {
        i = std::find_if(i, s.end(), notspace); 
        iter j = std::find_if(i, s.end(), space);
        if(i!=s.end())
        {
            res.push_back(std::string(i, j));
            i = j;
        }
    }
    return res;
}

std::vector<std::string> catLine(const std::string& s)
{
    typedef std::string::const_iterator iter;
    std::vector<std::string> res;
    iter i = s.begin();
    int r = 0;
    while(i!=s.end())
    {
        i = std::find_if(i, s.end(), notdelim); 
        iter j = std::find_if(i, s.end(), delim);
        if(i!=s.end())
        {
            res.push_back(std::string(i, j));
            //std::cout << res[r] << '\n';
            i = j;
            r++;
        }
    }
    return res;
}
std::vector<std::string> splitByColon(const std::string& s)
{
    typedef std::string::const_iterator iter;
    std::vector<std::string> res;
    iter i = s.begin();
    int r = 0;
    while(i!=s.end())
    {
        i = std::find_if(i, s.end(), notColon); 
        iter j = std::find_if(i, s.end(), colon);
        if(i!=s.end())
        {
            res.push_back(std::string(i, j));
            //std::cout << res[r] << '\n';
            i = j;
            r++;
        }
    }
    return res;
}


std::string& trim_left_in_place(std::string& str) {
    size_t i = 0;
    while(i < str.size() && isspace(str[i])) { ++i; };
    return str.erase(0, i);
}

std::string& trim_right_in_place(std::string& str) {
    size_t i = str.size();
    while(i > 0 && isspace(str[i - 1])) { --i; };
    return str.erase(i, str.size());
}

std::string& trim_in_place(std::string& str) {
    return trim_left_in_place(trim_right_in_place(str));
}

std::string trim_right(std::string str) {
    return trim_right_in_place(str);
}

std::string trim_left(std::string str) {
    return trim_left_in_place(str);
}

std::string trim(std::string str) {
    return trim_left_in_place(trim_right_in_place(str));
}

std::string getLine(std::string &request)
{
  size_t CRPos = request.find('\r');
  size_t LFPos = request.find('\n');
  if (LFPos != std::string::npos) {
    if (CRPos != std::string::npos && LFPos - CRPos == 1)  
    {
      std::string requestLine = request.substr(0, CRPos);
      request.erase(0, LFPos + 1);
      return requestLine;
    }
    else
    {
      std::string requestLine = request.substr(0, LFPos);
      request.erase(0, LFPos + 1);
      return requestLine;
    }
  }
  return "";
}      
#endif
