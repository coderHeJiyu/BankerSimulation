#ifndef USER_H
#define USER_H

#include<vector>
using std::vector;
class User
{
public:
    vector<int> need;
    vector<int> allocation;
    vector<int> time;
    User();
    void clearAll();
};

#endif // USER_H
