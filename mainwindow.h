#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <vector>
#include <fstream>
#include <sstream>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <cmath>
#include <QVector>

using namespace std;


class Course;
class Score;

// 定义学生类
class Student {
public:
    QString id;
    QString name;
    QString className;
    QString major;
    QVector<Course> courses;
    QVector<Score> scores;
};

// 定义教师类
class Teacher {
public:
    QString id;
    QString name;
    QStringList courses;
};

// 定义课程类
class Course {
public:
    QString code;
    QString name;
    float credit;
    int hours;
    QString category;
};

// 定义成绩类
class Score {
public:
    QString studentId;
    QString studentName;
    QString courseName;
    float grade;
};

inline QString a2u(const std::string &a) {
    return QString::fromLocal8Bit(a.c_str());
}


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionReadAll_triggered();

    void on_actionStudent_triggered();

    void on_actionTeacher_triggered();

    void on_actionScore_triggered();

    void on_actionCourse_triggered();

    void on_actionSearchStudent_triggered();

    void on_actionSearchScore_triggered();

    void on_actionEditGrade_triggered();

    void on_actionSave_triggered();

    void on_actionSearchClass_triggered();

    void on_actionFilter_triggered();

    void on_actionSort_triggered();

    void on_actionStatistics_triggered();

    void on_actionSImport_triggered();

    void on_actionSDel_triggered();

    void on_actionTImport_triggered();

    void on_actionTDel_triggered();

    void on_actionCImport_triggered();

    void on_actionCDel_triggered();

    void on_actionInit_triggered();

private:
    Ui::MainWindow *ui;
    vector<Student> students;
    vector<Teacher> teachers;
    vector<Course> courses;
    vector<Score> scores;

};


#endif // MAINWINDOW_H
