#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "scheduler.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

using std::vector;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int n;                    // 客户个数
    int m;                    // 资源类数
    Scheduler scheduler;      // 调度
    vector<User> user;        // 用户数组
    vector<Source> source;    // 资源数组
    vector<SafeSeq> safeSeqs; // 安全序列数组
    int chosenRow;            // 选中的安全序列行
    Ui::MainWindow *ui;
    /// @brief 显示初始化数据
    void showInitData();
    /// @brief 显示安全序列
    void showSafeSeqs();
    /// @brief 停止模拟
    void stopSimulate();

private slots:
    void randomButton();                     // “随机生成”按钮
    void bankerButton();                     // “运行银行家算法”按钮
    void safeSeqChoose(int row, int column); // 选中安全序列
    void simulateButton();                   // “开始模拟”按钮
    void stopSimulateButton();               // “关闭”按钮

public slots:
    void allocate(int);
    void release(int, int);
    void updateProgress(int);

signals:
    void stopSig();
};
#endif // MAINWINDOW_H
