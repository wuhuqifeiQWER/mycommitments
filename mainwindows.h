#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class MainWindowS;
}

class MainWindowS : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowS(QWidget *parent = nullptr);
    ~MainWindowS();

    // 声明 initialize 函数
    void initialize(QVector<Score> &scores, QVector<Course> &courses, QString studentId);
    void initTable();


private slots:
    void on_action_Withdraw_triggered();

    void on_actionSelect_triggered();

    void on_action_Exit_triggered();

private:
    Ui::MainWindowS *ui;
    QVector<Score>* m_scores;
    QVector<Course>* m_courses;
    QString m_studentId;
};

#endif // MAINWINDOWS_H
