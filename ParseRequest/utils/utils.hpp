#ifndef UTILS_HPP
# define UTILS_HPP

 # include <iostream>
 # include <vector>
 # include <string>
 #include <algorithm>
 #include <exception>

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
    while(i!=s.end())
    {
        i = std::find_if(i, s.end(), notdelim); 
        iter j = std::find_if(i, s.end(), delim);
        if(i!=s.end())
        {
            res.push_back(std::string(i, j));
            i = j;
        }
    }
    return res;
}

#endif