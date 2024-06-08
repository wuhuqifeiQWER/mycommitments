#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "mainwindows.h"
#include "functions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Student course selection and course management system");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString adminPassword = "114514";



void MainWindow::on_actionReadAll_triggered()
{
    students = readStudents("student.txt");
    teachers = readTeachers("staff.txt");
    courses = readCourses("module.txt");
    scores = readScores("score.txt");

    // 遍历所有学生
    for (Student& student : students) {
        // 清空学生的课程和分数信息容器
        student.courses.clear();
        student.scores.clear();

        // 遍历成绩表，为该学生添加相应的课程和分数信息
        for (const Score& score : scores) {
            if (score.studentId == student.id) {
                // 查找课程信息
                auto courseIt = std::find_if(courses.begin(), courses.end(), [&score](const Course& course) {
                    return course.name == score.courseName;
                });

                // 如果找到了课程信息，则添加到学生的课程容器中
                if (courseIt != courses.end()) {
                    student.courses.push_back(*courseIt);
                }

                // 将成绩添加到学生的成绩容器中
                student.scores.push_back(score);
            }
        }
    }


}




void MainWindow::on_actionStudent_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(students.size());
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"ID", "Name", "Class", "Major"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < students.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(students[i].id));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(students[i].name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(students[i].className));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(students[i].major));
    }

    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::on_actionTeacher_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(teachers.size());
    ui->tableWidget->setColumnCount(3);
    QStringList headers = {"ID", "Name", "Courses"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < teachers.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(teachers[i].id));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(teachers[i].name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(teachers[i].courses.join(", ")));
    }

    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::on_actionScore_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(scores.size());
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Student ID", "Student Name", "Course Name", "Grade"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < scores.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(scores[i].studentId));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(scores[i].studentName));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(scores[i].courseName));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scores[i].grade)));
    }

    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::on_actionCourse_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(courses.size());
    ui->tableWidget->setColumnCount(5);
    QStringList headers = {"Code", "Name", "Credit", "Hours", "Category"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < courses.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(courses[i].code));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(courses[i].name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(courses[i].credit)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(courses[i].hours)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(courses[i].category));
    }

    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::on_actionSearchStudent_triggered() {
    QString searchText = QInputDialog::getText(this, "Search Student", "Enter student name or ID:");
    if (searchText.isEmpty()) return;

    // 清空表格内容
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // 设置表格列数
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Student ID", "Student Name", "Course Name", "Grade"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 根据输入的文本查找学生信息
    bool studentFound = false;

    for (const auto& score : scores) {
        if (score.studentId == searchText || score.studentName == searchText) {
            studentFound = true;
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(score.studentId));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(score.studentName));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(score.courseName));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(score.grade)));
        }
    }

    if (!studentFound) {
        QMessageBox::information(this, "Search Result", "No student found with the given name or ID.");
        return;
    }

    // 自动调整列宽
    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}




void MainWindow::on_actionSearchScore_triggered() {
    // 创建一个下拉菜单来选择课程
    QStringList courseNames;
    for (const auto &course : courses) {
        courseNames.append(course.name);
    }

    bool ok;
    QString selectedCourse = QInputDialog::getItem(this, "Select Course", "Choose a course:", courseNames, 0, false, &ok);
    if (!ok) return;

    // 输入分数段
    QString minGradeText = QInputDialog::getText(this, "Minimum Grade", "Enter minimum grade:", QLineEdit::Normal, "0");
    if (minGradeText.isEmpty()) return;
    float minGrade = minGradeText.toFloat();

    QString maxGradeText = QInputDialog::getText(this, "Maximum Grade", "Enter maximum grade:", QLineEdit::Normal, "100");
    if (maxGradeText.isEmpty()) return;
    float maxGrade = maxGradeText.toFloat();

    // 清空表格内容
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // 设置表格列数
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Student ID", "Student Name", "Course Name", "Grade"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 查找指定分数段的学生
    for (int i = 0; i < scores.size(); ++i) {
        if (scores[i].courseName == selectedCourse && scores[i].grade >= minGrade && scores[i].grade <= maxGrade) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(scores[i].studentId));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(scores[i].studentName));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(scores[i].courseName));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(scores[i].grade)));
        }
    }

    // 自动调整列宽
    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::on_actionEditGrade_triggered() {
    // 获取选中的单元格
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Edit Grade", "Please select a grade to edit.");
        return;
    }

    qDebug() << "Selected items count: " << selectedItems.count();
    for (const auto& item : selectedItems) {
        qDebug() << "Row: " << item->row() << ", Column: " << item->column();
    }

    // 确保选中的是成绩单元格，并且内容是数字
    if (selectedItems[0]->column() != 3 || !selectedItems[0]->text().toFloat()) {
        QMessageBox::warning(this, "Edit Grade", "Please select a valid grade cell to edit.");
        return;
    }

    // 获取选中的成绩信息
    int row = selectedItems[0]->row();
    QString studentId = ui->tableWidget->item(row, 0)->text();
    QString studentName = ui->tableWidget->item(row, 1)->text();
    QString courseName = ui->tableWidget->item(row, 2)->text();
    QString currentGrade = ui->tableWidget->item(row, 3)->text();

    // 弹出密码对话框，确认密码
    QString password = QInputDialog::getText(this, "Admin Password", "Enter admin password:", QLineEdit::Password);
    if (password != adminPassword) {
        QMessageBox::warning(this, "Admin Password", "Incorrect password. You don't have permission to edit grades.");
        return;
    }

    // 弹出对话框，输入新的成绩
    QString newGrade = QInputDialog::getText(this, "Edit Grade", "Enter new grade:", QLineEdit::Normal, currentGrade);
    if (newGrade.isEmpty()) return;

    // 更新成绩信息
    for (auto &score : scores) {
        if (score.studentId == studentId && score.studentName == studentName && score.courseName == courseName) {
            score.grade = newGrade.toFloat();
            break;
        }
    }

    // 更新表格显示
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(newGrade));
}



void MainWindow::on_actionSave_triggered() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), "Text files (*.txt)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return;
    }

    QTextStream out(&file);
    const int rowCount = ui->tableWidget->rowCount();
    const int columnCount = ui->tableWidget->columnCount();

    // 写入表头
    QStringList headers;
    for (int col = 0; col < columnCount; ++col) {
        headers << ui->tableWidget->horizontalHeaderItem(col)->text();
    }
    out << headers.join("\t") << "\n";

    // 写入表格内容
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item) {
                out << item->text();
            }
            if (col < columnCount - 1) {
                out << "\t";
            }
        }
        out << "\n";
    }

    file.close();
}



void MainWindow::on_actionSearchClass_triggered()
{
    // 使用 QSet 存储班级信息以去重
    QSet<QString> classSet;
    for (const auto &student : students) {
        classSet.insert(student.className);
    }

    // 将 QSet 转换为 QStringList
    QStringList classList;
    for (const QString &className : classSet) {
        classList.append(className);
    }

    QString selectedClass = QInputDialog::getItem(this, "Select Class", "Choose a class:", classList, 0, false);
    if (selectedClass.isEmpty()) return;


    // 清空表格内容
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // 设置表格列数
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Student ID", "Student Name", "Course Name", "Grade"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 查找选定班级的学生并显示其成绩
    for (const auto &student : students) {
        if (student.className == selectedClass) {
            for (const auto &score : scores) {
                if (score.studentId == student.id) {
                    int row = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(row);
                    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(score.studentId));
                    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(score.studentName));
                    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(score.courseName));
                    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(score.grade)));
                }
            }
        }
    }

    // 自动调整列宽
    setCentralWidget(ui->tableWidget);
    ui->tableWidget->resizeColumnsToContents();
}


void MainWindow::on_actionFilter_triggered() {
    // 获取用户选择的项
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Filter", "Please select an item in the table.");
        return;
    }

    // 获取用户选择项的列索引和内容
    int columnIndex = selectedItems[0]->column();
    QString selectedText = selectedItems[0]->text();

    // 遍历表格的特定列，删除不匹配的行
    for (int row = ui->tableWidget->rowCount() - 1; row >= 0; --row) {
        QTableWidgetItem *item = ui->tableWidget->item(row, columnIndex);
        if (item && item->text() != selectedText) {
            ui->tableWidget->removeRow(row);
        }
    }
}


void MainWindow::on_actionSort_triggered() {
    // 获取当前表格的选中项
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Sort", "Please select a column to sort.");
        return;
    }

    // 判断选中的列是否为 "Grade" 列
    if (selectedItems[0]->column() != 3) {
        QMessageBox::warning(this, "Sort", "Please select the 'Grade' column to sort.");
        return;
    }

    // 获取当前表格的所有行数据
    QList<QPair<QStringList, float>> rowData;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QStringList rowDataList;
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            rowDataList << ui->tableWidget->item(row, col)->text();
        }
        float grade = ui->tableWidget->item(row, 3)->text().toFloat();
        rowData.append(qMakePair(rowDataList, grade));
    }

    // 根据成绩对行数据进行排序,lambda匿名函数实现从大到小排序
    //https://blog.csdn.net/qq_44079386/article/details/124359697
    std::sort(rowData.begin(), rowData.end(), [](const QPair<QStringList, float>& a, const QPair<QStringList, float>& b) {
        return a.second > b.second;
    });

    // 清空表格
    ui->tableWidget->clearContents();

    // 将排序后的行数据重新设置到表格中
    for (int row = 0; row < rowData.size(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(rowData[row].first[col]));
        }
    }
}





void MainWindow::on_actionStatistics_triggered()
{
    // 获取当前表格的选中项
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Statistics", "Please select a column to calculate statistics.");
        return;
    }

    // 判断选中的列是否为 "Grade" 列
    if (selectedItems[0]->column() != 3) {
        QMessageBox::warning(this, "Statistics", "Please select the 'Grade' column to calculate statistics.");
        return;
    }

    // 获取 "Grade" 列的所有数据
    QVector<float> grades;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 3);
        if (item) {
            bool ok;
            float grade = item->text().toFloat(&ok);
            if (ok) {
                grades.append(grade);
            }
        }
    }

    if (grades.isEmpty()) {
        QMessageBox::warning(this, "Statistics", "No valid grades found in the selected column.");
        return;
    }

    // 计算平均成绩
    float sum = std::accumulate(grades.begin(), grades.end(), 0.0);
    float mean = sum / grades.size();

    // 计算标准差
    float variance = 0.0;
    for (float grade : grades) {
        variance += (grade - mean) * (grade - mean);
    }
    variance /= grades.size();
    float stddev = std::sqrt(variance);

    // 计算及格率（假设及格分数为60）
    int passCount = std::count_if(grades.begin(), grades.end(), [](float grade) { return grade >= 60; });
    float passRate = static_cast<float>(passCount) / grades.size() * 100;

    // 显示结果
    //https://blog.csdn.net/weixin_39609623/article/details/82987194
    QString result = QString(" Average Grade: %1\n Standard Deviation: %2\n Pass Rate: %3%").arg(mean).arg(stddev).arg(passRate);

    QMessageBox::information(this, "Statistics", result);
}


void MainWindow::on_actionSImport_triggered() {
    // 获取学生信息
    bool ok;
    QString id = QInputDialog::getText(this, "Add Student", "Enter student ID:", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty()) return;

    QString name = QInputDialog::getText(this, "Add Student", "Enter student name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString className = QInputDialog::getText(this, "Add Student", "Enter class name:", QLineEdit::Normal, "", &ok);
    if (!ok || className.isEmpty()) return;

    QString major = QInputDialog::getText(this, "Add Student", "Enter major:", QLineEdit::Normal, "", &ok);
    if (!ok || major.isEmpty()) return;

    // 检查学号是否已存在
    for (const Student& student : students) {
        if (student.id == id) {
            QMessageBox::warning(this, "Add Student", "Student ID already exists.");
            return;
        }
    }

    // 添加学生信息
    Student newStudent = {id, name, className, major};
    students.push_back(newStudent);
    on_actionStudent_triggered();   //刷新学生表格

    QMessageBox::information(this, "Add Student", "Student added successfully.");
}


void MainWindow::on_actionSDel_triggered()
{
    // 获取学生ID
    bool ok;
    QString id = QInputDialog::getText(this, "Remove Student", "Enter student ID:", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty()) return;

    // 使用 Lambda 表达式查找学生
    auto it = std::find_if(students.begin(), students.end(), [&id](const Student& student) {
        return student.id == id;
    });

    if (it != students.end()) {
        students.erase(it);
        QMessageBox::information(this, "Remove Student", "Student removed successfully.");
    } else {
        QMessageBox::warning(this, "Remove Student", "Student ID not found.");
    }

    on_actionStudent_triggered();   //刷新学生表格

}


void MainWindow::on_actionTImport_triggered()
{
    // 获取教师信息
    bool ok;
    QString id = QInputDialog::getText(this, "Add Teacher", "Enter teacher ID:", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty()) return;

    QString name = QInputDialog::getText(this, "Add Teacher", "Enter teacher name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString subjectsStr = QInputDialog::getText(this, "Add Teacher", "Enter subjects (comma-separated):", QLineEdit::Normal, "", &ok);
    if (!ok || subjectsStr.isEmpty()) return;

    // 将逗号分隔的字符串转换为 QStringList
    QStringList subjects = subjectsStr.split(",", Qt::SkipEmptyParts);

    // 检查工号是否已存在
    for (const Teacher& teacher : teachers) {
        if (teacher.id == id) {
            QMessageBox::warning(this, "Add Teacher", "Teacher ID already exists.");
            return;
        }
    }

    // 添加教师信息
    Teacher newTeacher = {id, name, subjects};
    teachers.push_back(newTeacher);
    on_actionTeacher_triggered();   //刷新教师表格

    QMessageBox::information(this, "Add Teacher", "Teacher added successfully.");
}


void MainWindow::on_actionTDel_triggered()
{
    // 获取教师ID
    bool ok;
    QString id = QInputDialog::getText(this, "Remove Teacher", "Enter teacher ID:", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty()) return;

    auto it = std::find_if(teachers.begin(), teachers.end(), [&id](const Teacher& teacher) {
        return teacher.id == id;
    });

    if (it != teachers.end()) {
        teachers.erase(it);
        QMessageBox::information(this, "Remove Teacher", "Teacher removed successfully.");
    } else {
        QMessageBox::warning(this, "Remove Teacher", "Teacher ID not found.");
    }

    on_actionTeacher_triggered();   //刷新教师表格

}


void MainWindow::on_actionCImport_triggered() {
    // 获取课程信息
    bool ok;
    QString code = QInputDialog::getText(this, "Add Course", "Enter course code:", QLineEdit::Normal, "", &ok);
    if (!ok || code.isEmpty()) return;

    QString name = QInputDialog::getText(this, "Add Course", "Enter course name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString creditsStr = QInputDialog::getText(this, "Add Course", "Enter course credits:", QLineEdit::Normal, "", &ok);
    if (!ok || creditsStr.isEmpty()) return;
    float credits = creditsStr.toFloat();

    QString hoursStr = QInputDialog::getText(this, "Add Course", "Enter course hours:", QLineEdit::Normal, "", &ok);
    if (!ok || hoursStr.isEmpty()) return;
    float hours = hoursStr.toFloat();

    QString category = QInputDialog::getText(this, "Add Course", "Enter course category:", QLineEdit::Normal, "", &ok);
    if (!ok || category.isEmpty()) return;

    // 检查课程code是否已存在
    for (const Course& course : courses) {
        if (course.code == code) {
            QMessageBox::warning(this, "Add Course", "Course ID already exists.");
            return;
        }
    }

    // 添加课程信息
    Course newCourse = {code, name, credits, hours, category};
    courses.push_back(newCourse);
    on_actionCourse_triggered();    // 刷新课程表格

    QMessageBox::information(this, "Add Course", "Course added successfully.");

}


void MainWindow::on_actionCDel_triggered() {
    // 获取课程code
    bool ok;
    QString code = QInputDialog::getText(this, "Remove Course", "Enter course code:", QLineEdit::Normal, "", &ok);
    if (!ok || code.isEmpty()) return;

    auto it = std::find_if(courses.begin(), courses.end(), [&code](const Course& course) {
        return course.code == code;
    });

    if (it != courses.end()) {
        courses.erase(it);  // 找到课程后删除
        QMessageBox::information(this, "Remove Course", "Course removed successfully.");
    } else {
        QMessageBox::warning(this, "Remove Course", "Course code not found.");
    }

    on_actionCourse_triggered();    // 刷新课程表格

}

void MainWindow::on_actionInit_triggered() {


    // 获取学生学号
    bool ok;
    QString studentId = QInputDialog::getText(this, "Initialize Student", "Enter student ID:", QLineEdit::Normal, "", &ok);
    if (!ok || studentId.isEmpty()) return;

    // 查找学生
    auto it = std::find_if(students.begin(), students.end(), [&studentId](const Student& student) {
        return student.id == studentId;
    });

    if (it != students.end()) {
        // 创建新的 MainWindowS
        MainWindowS *sWindow = new MainWindowS(this);
        sWindow->initialize(it->scores, it->courses, studentId);
        sWindow->show();
    } else {
        QMessageBox::warning(this, "Initialize Student", "Student ID not found.");
    }
}



