#include "safeseq.h"

SafeSeq::SafeSeq()
{
    time = 0;
}

void SafeSeq::addSeq(int uid)
{
    seq.push_back(uid);
}
void SafeSeq::delSeqBack()
{
    seq.pop_back();
}
void SafeSeq::delSeqHead()
{
    seq.erase(seq.begin());
}

void SafeSeq::setTime(int t)
{
    time = t;
}

vector<int> SafeSeq::getSeq()
{
    return seq;
}

int SafeSeq::getTime()
{
    return time;
}

void SafeSeq::clear()
{
    vector<int>().swap(seq);
    time = 0;
}

bool SafeSeq::operator<(const SafeSeq &s2) const
{
    return time < s2.time;
}