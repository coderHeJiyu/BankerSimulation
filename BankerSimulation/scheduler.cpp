#include "scheduler.h"
#include <ctime>
#include <random>
#include <algorithm>
std::default_random_engine e(time(0));

#define RANDOM_NEED e() % 5
#define RANDOM_ALLOCATION e() % 4 + 1
#define RANDOM_TIME e() % 4 + 2
#define RANDOM_AVAILABLE e() % 7;

Scheduler::Scheduler()
{
    n = 0;
    m = 0;
}

void Scheduler::setNum(int n, int m)
{
    this->n = n;
    this->m = m;
}

void Scheduler::initData(vector<User> &user, vector<Source> &source)
{
    if (n == 0 || m == 0)
    {
        return;
    }
    // 初始化user
    for (auto it = user.begin(); it != user.end(); it++)
    {
        it->clearAll();
    }
    vector<User>().swap(user);
    for (int i = 0; i < n; i++)
    {
        user.push_back(User());
        for (int j = 0; j < m; j++)
        {
            user[i].need.push_back(RANDOM_NEED);
            user[i].allocation.push_back(RANDOM_ALLOCATION);
            user[i].time.push_back(RANDOM_TIME);
        }
    }
    // 初始化source
    vector<Source>().swap(source);
    for (int j = 0; j < m; j++)
    {
        int cnt = 0;
        for (int i = 0; i < n; i++)
        {
            cnt += user[i].allocation[j];
        };
        int available = RANDOM_AVAILABLE;
        int total = cnt + available;
        source.push_back(Source(total, available));
    }

    // debug1
    // vector<User>().swap(user);
    // vector<Source>().swap(source);
    // for (int i = 0; i < 3; i++)
    // {
    //     user.push_back(User());
    // }
    // user[0].need.push_back(0);
    // user[0].allocation.push_back(1);
    // user[0].time.push_back(3);
    // user[1].need.push_back(0);
    // user[1].allocation.push_back(4);
    // user[1].time.push_back(4);
    // user[2].need.push_back(3);
    // user[2].allocation.push_back(2);
    // user[2].time.push_back(2);
    // source.push_back(Source(13, 6));

    // debug2
    // vector<User>().swap(user);
    // vector<Source>().swap(source);
    // for (int i = 0; i < 3; i++)
    // {
    //     user.push_back(User());
    // }
    // user[0].need.push_back(1);
    // user[0].allocation.push_back(4);
    // user[0].time.push_back(4);
    // user[0].need.push_back(3);
    // user[0].allocation.push_back(3);
    // user[0].time.push_back(3);
    // user[1].need.push_back(4);
    // user[1].allocation.push_back(1);
    // user[1].time.push_back(4);
    // user[1].need.push_back(2);
    // user[1].allocation.push_back(1);
    // user[1].time.push_back(4);
    // user[2].need.push_back(0);
    // user[2].allocation.push_back(1);
    // user[2].time.push_back(2);
    // user[2].need.push_back(1);
    // user[2].allocation.push_back(4);
    // user[2].time.push_back(4);
    // source.push_back(Source(7, 1));
    // source.push_back(Source(9, 1));
}

void Scheduler::banker(vector<User> user, vector<Source> source, vector<SafeSeq> &safeSeqs)
{
    if (n == 0 || m == 0)
    {
        return;
    }
    // 初始化工作数据结构
    lever = 0;
    for (auto s : source)
    {
        work.push_back(s.available);
    }
    for (int i = 0; i < n; i++)
    {
        finished.push_back(false);
    }
    this->user = user;
    this->source = source;
    vector<SafeSeq>().swap(safeSeqs);
    safeSeq.clear();
    // 开始算法
    for (int i = 0; i < n; i++)
    {
        if (isSatisfied(i))
        {
            dfs(i, safeSeqs);
        }
    }
    // 排序
    sort(safeSeqs.begin(), safeSeqs.end());
    // 释放内存
    vector<int>().swap(work);
    vector<Release>().swap(release);
    vector<bool>().swap(finished);
}

void Scheduler::simulate(vector<User> user, vector<Source> source, SafeSeq safeSeq)
{
    // 初始化工作数据结构
    this->user = user;
    this->source = source;
    for (auto s : source)
    {
        work.push_back(s.available);
    }
    this->safeSeq = safeSeq;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(simulate1Sec()));
    // 开始模拟
    runTime = 0;
    simulate1Sec();
    timer->start(1000);
}

void Scheduler::dfs(int uid, vector<SafeSeq> &safeSeqs)
{
    set<int> visitedUserSet;
    // 保护节点数据
    SafeSeq safeSeqTemp(safeSeq);
    vector<Release> releaseTemp(release);
    vector<int> workTemp(work);
    // 修改状态
    lever++;
    safeSeq.addSeq(uid);
    finished[uid] = true;
    for (int i = 0; i < m; i++)
    {
        work[i] -= user[uid].need[i];
        int number = user[uid].allocation[i] + user[uid].need[i];
        if (number > 0)
        {
            Release r(i, number, user[uid].time[i] + safeSeq.getTime());
            release.insert(std::upper_bound(release.begin(), release.end(), r), r);
        }
    }
    // 判断是否到达最后一层
    if (lever == n)
    {
        safeSeq.setTime((release.end() - 1)->time);
        safeSeqs.push_back(SafeSeq(safeSeq));
    }
    else // 继续访问下一层
    {
        while (visitedUserSet.size() < n - lever)
        {
            for (int i = 0; i < n; i++)
            {
                if (finished[i])
                {
                    continue;
                }
                if (visitedUserSet.find(i) != visitedUserSet.end())
                {
                    continue;
                }
                if (isSatisfied(i))
                {
                    visitedUserSet.insert(i);
                    dfs(i, safeSeqs);
                }
            }
            if (release.size() > 0)
            {
                auto r = release.begin();
                work[r->sourceId] += r->number;
                safeSeq.setTime(r->time);
                release.erase(r);
            }
            else
            {
                break;
            }
        }
    }
    // 回溯
    finished[uid] = false;
    lever--;
    work = workTemp;
    release = releaseTemp;
    safeSeq = safeSeqTemp;
    set<int>().swap(visitedUserSet);
    vector<Release>().swap(releaseTemp);
    vector<int>().swap(workTemp);
}

bool Scheduler::isSatisfied(int uid)
{
    for (int i = 0; i < m; i++)
    {
        if (user[uid].need[i] > work[i])
        {
            return false;
        }
    }
    return true;
}

void Scheduler::simulate1Sec()
{
    // 出口
    if (runTime > safeSeq.getTime())
    {
        timer->stop();
        delete timer;
        // 释放内存
        safeSeq.clear();
        vector<int>().swap(work);
        vector<Release>().swap(release);
        set<int>().swap(runSet);
        return;
    }
    // 发送时钟信号
    if (runTime > 0)
    {
        emit progressSig(-1);
        for (auto &&i : runSet)
        {
            emit progressSig(i);
        }
    }
    // 模拟该runTime下的释放过程
    while (release.begin() != release.end() && release.begin()->time == runTime)
    {
        auto r = release.begin();
        work[r->sourceId] += r->number;
        emit releaseSig(r->uid, r->sourceId);
        release.erase(r);
    }
    // 模拟该runTime下的分配过程
    auto uidIt = safeSeq.getSeq().begin();
    while (uidIt != safeSeq.getSeq().end() && isSatisfied(*uidIt))
    {
        for (int i = 0; i < m; i++)
        {
            work[i] -= user[*uidIt].need[i];
            int number = user[*uidIt].allocation[i] + user[*uidIt].need[i];
            if (number > 0)
            {
                Release r(i, number, user[*uidIt].time[i] + runTime);
                r.uid = *uidIt;
                release.insert(std::upper_bound(release.begin(), release.end(), r), r);
            }
        }
        runSet.insert(*uidIt);
        emit allocateSig(*uidIt);
        safeSeq.delSeqHead();
        uidIt = safeSeq.getSeq().begin();
    }
    runTime++;
}

void Scheduler::stopSimulate()
{
    // 重置simulate1Sec()
    safeSeq.setTime(-1);
}