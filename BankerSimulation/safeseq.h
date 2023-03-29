#ifndef SAFESEQ_H
#define SAFESEQ_H
#include <vector>
using std::vector;

class SafeSeq
{
private:
    vector<int> seq; // 安全序列
    int time;        // 运行时间
public:
    SafeSeq();
    void addSeq(int user);
    void delSeq();
    void setTime(int t);
    vector<int> getSeq();
    int getTime();
    void clear();
    bool operator<(const SafeSeq &s2) const;

};

#endif // SAFESEQ_H
