
#pragma once
#include<string>

char toLower(char c) {
    if (c>='A' && c<='Z') return 'a' + c - 'A';
    return c;
}

bool stricmp(const std::string &s1, const std::string &s2) {
    if (s1.size()!=s2.size()) return false;
    int i;
    for (i=0; toLower(s1[i]) == toLower(s2[i]) && i<s1.size(); i++);
    return i==s1.size();
}