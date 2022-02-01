#ifndef STUDENT_H
#define STUDENT_H
#include <QtCore>
#include <QtGui>
#include <QStringList>
#include <QDialog>
#include <mysql/mysql.h>

namespace Ui {
class Student;
}

class Student : public QDialog
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = nullptr);
    explicit Student(QWidget *parent, QString);
    void courseAssined(QString);

    ~Student();
    Ui::Student *ui;

private slots:
    void on_EditButton_clicked();

    void on_logoutButton_clicked();

private:
    QString id;
    QStringList course;
    QStringList semester;
    QStringList year;
    MYSQL *con;
};

#endif // STUDENT_H
