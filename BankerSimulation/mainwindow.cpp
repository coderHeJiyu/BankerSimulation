#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置资源表宽度
    ui->sourceTable->setColumnWidth(0, 50);
    ui->sourceTable->setColumnWidth(1, 80);
    ui->sourceTable->setColumnWidth(2, 110);
    // 设置模拟面板
    ui->simulateFrame->hide();
    // 设置信号与槽
    connect(&scheduler, SIGNAL(mysig()), this, SLOT(slot2()));
    connect(&scheduler, SIGNAL(allocateSig(int)), this, SLOT(allocate(int)));
    connect(&scheduler, SIGNAL(clockSig(int)), this, SLOT(timeStep(int)));
    connect(this, SIGNAL(stopSig()), &scheduler, SLOT(stopSimulate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInitData()
{
    int n = user.size();
    int m = source.size();
    // 显示资源表
    auto table = ui->sourceTable;
    table->clearContents();
    table->setColumnCount(m);
    for (int i = 0; i < m; i++)
    {
        int col = i;
        QString name = QStringLiteral("S") + QString::number(i);
        table->setItem(0, col, new QTableWidgetItem(name));
        table->setItem(1, col, new QTableWidgetItem(QString::number(source[i].total)));
        table->setItem(2, col, new QTableWidgetItem(QString::number(source[i].available)));
        table->item(0, col)->setTextAlignment(Qt::AlignCenter);
        table->item(1, col)->setTextAlignment(Qt::AlignCenter);
        table->item(2, col)->setTextAlignment(Qt::AlignCenter);
    }
    // 显示用户表
    table = ui->userTable;
    table->clearContents();
    table->setColumnCount(1);
    table->setRowCount(n + 2);
    table->setColumnCount(m * 3 + 1);
    table->setSpan(0, 0, 2, 1);
    table->setItem(0, 0, new QTableWidgetItem("客户"));
    table->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    if (m <= 2)
    {
        for (int i = 1; i <= m * 3; i++)
        {
            table->setColumnWidth(i, 45);
        }
        if (m == 1)
        {
            table->setColumnWidth(2, 80);
        }
    }
    else
    {
        for (int i = 1; i <= 6; i++)
        {
            table->setColumnWidth(i, 30);
        }
    }
    QString str[3] = {"need", "allocation", "time"};
    for (int i = 0; i < 3; i++)
    {
        table->setSpan(0, m * i + 1, 1, m);
        table->setItem(0, m * i + 1, new QTableWidgetItem(str[i]));
        table->item(0, m * i + 1)->setTextAlignment(Qt::AlignCenter);
    }
    for (int j = 0; j < m; j++)
    {
        int row = 1;
        QString name = QStringLiteral("S") + QString::number(j);
        table->setItem(row, j + 1, new QTableWidgetItem(name));
        table->setItem(row, m + j + 1, new QTableWidgetItem(name));
        table->setItem(row, 2 * m + j + 1, new QTableWidgetItem(name));
    }
    for (int i = 0; i < n; i++)
    {
        int row = i + 2;
        QString name = QStringLiteral("U") + QString::number(i);
        table->setItem(row, 0, new QTableWidgetItem(name));
        for (int j = 0; j < m; j++)
        {
            table->setItem(row, j + 1, new QTableWidgetItem(QString::number(user[i].need[j])));
            table->setItem(row, m + j + 1, new QTableWidgetItem(QString::number(user[i].allocation[j])));
            table->setItem(row, 2 * m + j + 1, new QTableWidgetItem(QString::number(user[i].time[j])));
        }
    }
    for (int j = 1; j < table->columnCount(); j++)
    {
        table->item(1, j)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i = 2; i < table->rowCount(); i++)
    {
        for (int j = 0; j < table->columnCount(); j++)
        {
            table->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void MainWindow::showSafeSeqs()
{
    ui->safeSeqTable->setRowCount(safeSeqs.size());
    int row = 0;
    for (auto &&i : safeSeqs)
    {
        QString strSeq;
        auto seq = i.getSeq();
        strSeq += QStringLiteral("U") + QString::number(seq[0]);
        for (auto j = seq.begin() + 1; j != seq.end(); j++)
        {
            strSeq += QStringLiteral("->U") + QString::number(*j);
        }
        QString strTime;
        strTime = QString::number(i.getTime()) + QStringLiteral("s");
        ui->safeSeqTable->setItem(row, 0, new QTableWidgetItem(strSeq));
        ui->safeSeqTable->setItem(row, 1, new QTableWidgetItem(strTime));
        ui->safeSeqTable->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        row++;
    }
    ui->safeSeqTable->resizeColumnsToContents();
}

void MainWindow::randomButton()
{
    scheduler.setNum(ui->nSpinBox->value(), ui->mSpinBox->value());
    scheduler.initData(user, source);
    showInitData();
    ui->safeSeqTable->setRowCount(0);
    ui->safeSeqTable->clearContents();
    ui->bankerButton->setEnabled(true);
}

void MainWindow::bankerButton()
{
    ui->bankerButton->setEnabled(false);
    scheduler.banker(user, source, safeSeqs);
    showSafeSeqs();
}
void MainWindow::safeSeqChoose(int row, int column)
{
    //中止模拟
    stopSimulate();
    // 初始化模拟面板
    ui->mainProgressBar->setValue(0);
    ui->mainProgressBar->setMaximum(safeSeqs[row].getTime());
    // 初始化模拟表
    int n = user.size();
    ui->simulateTable->setRowCount(n);
    ui->simulateTable->setColumnCount(2);
    // 设置每个客户的进度条
    auto progressBars = this->findChildren<QProgressBar *>();
    qDebug("%d", progressBars.size());
    for (int i = 0; i < n; i++)
    {
        QString name = QStringLiteral("U") + QString::number(i);
        QProgressBar *progressBar = new QProgressBar(this);
        int maxTime = *std::max_element(user[i].time.begin(), user[i].time.end());
        progressBar->setMaximum(maxTime);
        progressBar->setValue(0);
        progressBar->setFormat("%vs");
        progressBar->setObjectName(name);
        ui->simulateTable->setItem(i, 0, new QTableWidgetItem(name));
        ui->simulateTable->setCellWidget(i, 1, progressBar);
    }
    ui->simulateFrame->show();
    chosenRow = row;
}

void MainWindow::simulateButton()
{
    qDebug("simu");
    // 禁用按钮
    ui->randomButton->setEnabled(false);
    ui->simulateButton->setEnabled(false);
    //开始模拟
    scheduler.simulate(user, source, safeSeqs[chosenRow]);
}
void MainWindow::slot2()
{
    std::cout << "slot2";
}

void MainWindow::allocate(int uid)
{
    qDebug("all");
    runSet.insert(uid);
    int m = source.size();
    auto table = ui->userTable;
    int row = uid + 2;
    for (int i = 0; i < m; i++)
    {
        auto need = table->item(row, i + 1);
        int needVal = need->text().toInt();
        auto allocation = table->item(row, m + i + 1);
        int allocationVal = allocation->text().toInt();
        auto work = ui->sourceTable->item(2, i);
        int workVal = work->text().toInt();
        qDebug("need:%p\nitem:(%d,%d)", need, row, i + 1);
        need->setText(QString::number(0));
        allocation->setText(QString::number(needVal + allocationVal));
        work->setText(QString::number(workVal - needVal));
    }
}

void MainWindow::timeStep(int t)
{
    // 更新主进度条
    ui->mainProgressBar->setValue(t);
    int m = source.size();
    // 更新资源表、客户表和客户进度条
    for (auto &&uid : runSet)
    {
        int row = uid + 2;
        int endFlag = true;
        for (int i = 0; i < m; i++)
        {
            // 修改客户表time栏
            auto time = ui->userTable->item(row, 2 * m + i + 1);
            int timeVal = time->text().toInt();
            timeVal = std::max(timeVal - 1, 0);
            time->setText(QString::number(timeVal));

            if (timeVal == 0)
            {
                // 释放资源，修改资源表和客户allocation表
                auto allocation = ui->userTable->item(row, m + i + 1);
                int allocationVal = allocation->text().toInt();
                auto work = ui->sourceTable->item(2, i);
                int workVal = work->text().toInt();
                allocation->setText(QString::number(0));
                work->setText(QString::number(workVal + allocationVal));
            }
            else
            {
                // 客户进程没有结束
                endFlag = false;
            }
        }
        // 更新客户进度条
        // auto cellWidget = ui->simulateTable->cellWidget(uid, 1);
        QString name = QStringLiteral("U") + QString::number(uid);
        auto progressBar = this->findChild<QProgressBar *>(name);
        // qDebug("pbsize:%d",progressBar.size());
        progressBar->setValue(progressBar->value() + 1);
        if (endFlag)
        {
            runSet.erase(uid);
        }
    }
    // if (t == safeSeqs[chosenRow].getTime())
    // {
    //     stopSimulate();
    // }
}

void MainWindow::stopSimulateButton()
{
    stopSimulate();
    ui->simulateFrame->hide();
    // 恢复按钮
    ui->randomButton->setEnabled(true);
}

void MainWindow::stopSimulate()
{
    // 发送信号
    emit stopSig();
    showInitData();
    // 恢复按钮
    ui->simulateButton->setEnabled(true);
    // 释放内存
    set<int>().swap(runSet);
}