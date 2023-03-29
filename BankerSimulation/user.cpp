#include "user.h"

User::User()
{
}

void User::clearAll()
{
    vector<int>().swap(need);
    vector<int>().swap(allocation);
    vector<int>().swap(time);
}
