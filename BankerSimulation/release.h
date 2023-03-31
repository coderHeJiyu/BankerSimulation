#ifndef RELEASE_H
#define RELEASE_H

class Release
{
public:
    int uid;      // 用户号
    int sourceId; // 资源号
    int number;   // 资源数量
    int time;     // 释放时间
    Release(int sourceId, int number, int time);
    bool operator<(const Release &r2) const;
};

#endif // RELEASE_H
