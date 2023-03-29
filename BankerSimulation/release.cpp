#include "release.h"

Release::Release(int sourceId, int number, int time)
{
    this->sourceId = sourceId;
    this->number = number;
    this->time = time;
}

bool Release::operator<(const Release &r2) const
{
    return time < r2.time;
}