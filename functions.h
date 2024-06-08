#ifndef FUNCTIONS_H
#define FUNCTIONS_H
vector<Student> readStudents(const QString &filename) {
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

vector<Teacher> readTeachers(const QString &filename) {
    vector<Teacher> teachers;
    ifstream file(filename.toStdString());
    if (!file.is_open()) return teachers;

    string line;
    while (getline(file, line)) {
        if (line == "#END") break;
        if (line.empty() || line[0] == '#') continue;

        istringstream ss(line);
        string id, name, course;
        ss >> id >> name;
        Teacher teacher;
        teacher.id = a2u(id);
        teacher.name = a2u(name);

        while (ss >> course) {
            teacher.courses.append(a2u(course));
        }
        teachers.push_back(teacher);
    }
    return teachers;
}

vector<Course> readCourses(const QString &filename) {
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

vector<Score> readScores(const QString &filename) {
    vector<Score> scores;
    ifstream file(filename.toStdString());
    if (!file.is_open()) return scores;

    string line;
    while (getline(file, line)) {
        if (line == "#END") break;
        if (line.empty() || line[0] == '#') continue;

        istringstream ss(line);
        string studentId, studentName, courseName;
        float grade;
        ss >> studentId >> studentName >> courseName >> grade;

        Score score;
        score.studentId = a2u(studentId);
        score.studentName = a2u(studentName);
        score.courseName = a2u(courseName);
        score.grade = grade;

        scores.push_back(score);
    }
    return scores;
}


#endif // FUNCTIONS_H
