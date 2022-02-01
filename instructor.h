#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <QDialog>
#include <mysql/mysql.h>

namespace Ui {
class Instructor;
}

class Instructor : public QDialog
{
    Q_OBJECT

public:
    explicit Instructor(QWidget *parent = nullptr);
     explicit Instructor(QString i_id, QWidget *parent = nullptr);
    ~Instructor();



private slots:

    void on_registerButton_clicked();

    void on_EditButton_clicked();

    void on_logoutButton_clicked();

private:
    MYSQL *con;
    Ui::Instructor *ui;
    QString id;
    QStringList course;
    QStringList sec_id;
    QStringList semester;
    QStringList year;

};

#endif // INSTRUCTOR_H
