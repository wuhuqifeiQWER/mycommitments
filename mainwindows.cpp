#include "mainwindows.h"
#include "ui_mainwindows.h"
#include "mainwindow.h"


MainWindowS::MainWindowS(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowS)
    , m_scores(nullptr)
    , m_courses(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Student");
}

MainWindowS::~MainWindowS()
{
    delete ui;
}

vector<Student> readStudent(const QString &filename) {
    vector<Student> students;
    ifstream file(filename.toStdString());
    if (!file.is_open()){
        QMessageBox::information(nullptr, "info", "cannot read the file");
        return students;
    }

    string line;
    while (getline(file, line)) {
        if (line == "#END") break;
        if (line.empty() || line[0] == '#') continue;

        istringstream ss(line);
        string id, name, className, major;
        ss >> id >> name >> className >> major;

        Student student;
        student.id = a2u(id);
        student.name = a2u(name);
        student.className = a2u(className);
        student.major = a2u(major);

        students.push_back(student);
    }
    return students;
}

vector<Course> readCourse(const QString &filename) {
    vector<Course> courses;
    ifstream file(filename.toStdString());
    if (!file.is_open()) return courses;

    string line;
    while (getline(file, line)) {
        if (line == "#END") break;
        if (line.empty() || line[0] == '#') continue;

        istringstream ss(line);
        string code, name, category;
        float credit;
        int hours;
        ss >> code >> name >> credit >> hours >> category;

        Course course;
        course.code = a2u(code);
        course.name = a2u(name);
        course.credit = credit;
        course.hours = hours;
        course.category = a2u(category);

        courses.push_back(course);
    }
    return courses;
}

QString getStudentName( QString &studentId)  {
    vector<Student> stu = readStudent("student.txt");
    for (const Student &student : stu) {
        if (student.id == studentId) {
            return student.name;
        }
    }
    return QString();
}

void MainWindowS::initialize(QVector<Score>& scores, QVector<Course>& courses, QString studentId){

    m_scores=&scores;
    m_courses=&courses;
    m_studentId=studentId;
    initTable();
}

void MainWindowS::initTable(){
    // 设置表头
    QStringList headers = {"Course", "Grade", "Credit"};
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(m_scores->size()+1);

    // 填充表格
    float totalCredits = 0.0;
    for (int i = 0; i < m_scores->size(); ++i) {
        const Score& score = (*m_scores)[i];    //注意解引用
        const Course& course = (*m_courses)[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(course.name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(score.grade)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(course.credit)));

        totalCredits += course.credit;
    }

    // 在最后一行显示总学分
    ui->tableWidget->setItem(m_scores->size(), 0, new QTableWidgetItem("Total Credits"));
    ui->tableWidget->setItem(m_scores->size(), 2, new QTableWidgetItem(QString::number(totalCredits)));

    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}




void MainWindowS::on_action_Withdraw_triggered() {
    // 下拉菜单
    QStringList courseNames;
    for (const Course& course : *m_courses) {
        courseNames << course.name;
    }

    // 弹出下拉菜单
    bool ok;
    QString selectedCourse = QInputDialog::getItem(this, "Withdraw Course", "Select a course to withdraw:", courseNames, 0, false, &ok);
    if (!ok || selectedCourse.isEmpty()) {
        return;
    }

    // 从 *m_courses 和 *m_scores 中删除选中课程和分数
    for (int i = 0; i < m_scores->size(); ++i) {
        if ((*m_courses)[i].name == selectedCourse) {
            m_scores->remove(i);
            m_courses->remove(i);
            --i; // 因为删除一个元素后，后续元素会前移，所以需要将索引减1
        }
    }

    // 重新初始化表格
    initTable();
}



void MainWindowS::on_actionSelect_triggered() {
    // 获取 MainWindow 中的课程信息
    vector<Course> courses = readCourse("module.txt");

    // 创建一个字符串列表存储课程名称
    QStringList courseNames;
    for (const Course& course : courses) {
        courseNames << course.name;
    }

    // 弹出下拉菜单，让用户选择课程
    bool ok;
    QString selectedCourse = QInputDialog::getItem(this, "Select Course", "Select a course:", courseNames, 0, false, &ok);
    if (!ok || selectedCourse.isEmpty()) {
        return;
    }

    // 检查学生已有课程是否已经选择了该课程
    for (const Course& course : *m_courses) {
        if (course.name == selectedCourse) {
            QMessageBox::warning(this, "Course Selection", "You have already selected this course.");
            return;
        }
    }

    // 在 m_courses 中查找选中的课程信息
    for (const Course& course : courses) {
        if (course.name == selectedCourse) {
            // 添加选中课程信息到 *m_courses 中
            m_courses->push_back(course);

            // 创建一个新的 Score 对象并初始化
            Score newScore;
            newScore.studentId = m_studentId;
            newScore.studentName = getStudentName(m_studentId);
            newScore.courseName = course.name;
            newScore.grade = 0; // 初始化为0分

            // 添加新的 Score 对象到 *m_scores 中
            m_scores->push_back(newScore);

            // 退出循环，因为已经找到选中的课程信息并添加到 m_courses 和 m_scores 中
            break;
        }
    }

    // 重新初始化表格
    initTable();
}



void MainWindowS::on_action_Exit_triggered()
{
    auto res = QMessageBox::question(this, "Exit", "exit or not?");
    if (res == QMessageBox::Yes)
        this->close();

}

