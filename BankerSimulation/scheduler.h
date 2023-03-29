#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QTimer>
#include "user.h"
#include "source.h"
#include "safeseq.h"
#include "release.h"
#include <set>

using std::set;
class Scheduler : public QObject
{
    Q_OBJECT
private:
    int n;                   // 客户个数
    int m;                   // 资源类数
    int lever;               // 搜索层数
    vector<int> work;        // 工作向量
    vector<Release> release; // 待释放的资源
    vector<bool> finished;   // 完成状态向量
    vector<User> user;
    vector<Source> source;
    SafeSeq safeSeq; // 安全序列
    QTimer *timer;
    void dfs(int uid, vector<SafeSeq> &safeSeqs); // 深度优先遍历解空间
    bool isSatisfied(int uid);

public:
    /// @brief 构造函数
    Scheduler();

    /// @brief 设置客户个数和资源类数
    /// @param n 客户个数
    /// @param m 资源类数
    void setNum(int n, int m);

    /// @brief 产生初始化数据
    /// @param user 用户数组
    /// @param source 资源数组
    void initData(vector<User> &user, vector<Source> &source);

    /// @brief 生成安全序列
    /// @param user 用户数组
    /// @param source 资源数组
    /// @param safeSeqs 安全序列数组
    void banker(vector<User> user, vector<Source> source, vector<SafeSeq> &safeSeqs);

    /// @brief 生成模拟过程
    /// @param user 用户数组
    /// @param source 资源数组
    /// @param safeSeqs 安全序列
    void simulate(vector<User> user, vector<Source> source, SafeSeq safeSeqs);

signals:
    void mysig();
    void allocateSig(int);
    void clockSig(int);
private slots:
    void simulate1Sec();
public slots:
    void stopSimulate();
};

#endif // SCHEDULER_H
