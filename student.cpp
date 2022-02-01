#include "student.h"
#include "ui_student.h"
#include "mainwindow.h"
#include "databaseconnection.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <QString>
#include <QLabel>
#include <QDialog>
#include <QAbstractItemModel>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QLayout>

Student::Student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
}

Student::Student(QWidget *parent, QString s_id) :
    QDialog(parent),
    ui(new Ui::Student)
{
    //set up MySQL connection...
    con = NULL;
    MYSQL_RES *res;
    MYSQL_ROW row = NULL;
    struct connection_details mysqlD;
    con = mysql_connection_setup(mysqlD);

    ui->setupUi(this);
    std::ostringstream qry;
    char std_id[50];//current(logged in) student ID...
    QstringToCharArray(s_id, std_id);

    // ======================================================== //
    //               Student Personal Details
    // ======================================================== //


    //preparing the query

    qry << "select * from student where ID = \"" << std_id << "\"";

    res = mysql_execute_query(con, qry.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            ui->studentNo->setText(row[0]);
            ui->studentName->setText(row[1]);
            ui->deptName->setText(row[2]);
            ui->faculty->setText(row[3]);
            ui->creditNum->setText(row[4]);
            ui->CGPA->setText(row[5]);
        }
    }

    mysql_free_result(res);


    // ======================================================== //
    //               Student Course Registered
    // ======================================================== //


    //preparing the query
    qry.str("");
    qry << "select course_id, sec_id, semester, year, grade from takes where ID = \"" << std_id << "\";";

    res = mysql_execute_query(con, qry.str().c_str());


    while((row = mysql_fetch_row(res)) != NULL){

        int col_num = mysql_field_count(con);

        int rowCount = 0, columnCount = 0;

        //Creating a table
        QTableWidgetItem *item;

        ui->courseInfoTable->setColumnCount(col_num);
        ui->courseInfoTable->insertRow(rowCount);

        //Adding the header of the table
        QStringList label;
        label << "Course Id" << "Group No" << "Semester" << "Year" << "Grade";
        ui->courseInfoTable->setHorizontalHeaderLabels(label);

        //Dynamically allocating...
        for(unsigned int j = 0; j < mysql_field_count(con); j++){
            item = new QTableWidgetItem;

            //chagne table's cell to be read-only
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            //the acutall inserting for data into the table
            item->setText(row[j]);

            ui->courseInfoTable->setItem(rowCount, columnCount, item);



            //Resize rows, columns, and the table to fit content
            ui->courseInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->courseInfoTable->resizeColumnsToContents();
            ui->courseInfoTable->resizeRowsToContents();

            columnCount++;
        }
        rowCount++;
    }
    mysql_free_result(res);


    // ======================================================== //
    //               Student Registered in Class
    // ======================================================== //


    //3 - queries, to fetch course_id, semester and the year in 3 separate queries
    // and store them in 3 differnet QStringList (container)
    //for converting from QStringList to QString later via a foreach (QString itm, QStringList obj)
    // they're stored as private data memebre in the .h file

    // query - 1 (course_id)
    qry.str("");
    qry << "select course_id from takes where ID = \"" << std_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());


    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                course.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);


    // query - 2 (semester)
    qry.str("");
    qry << "select semester from takes where ID = \"" << std_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                semester.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);


    // query - 3 (year)
    qry.str("");
    qry << "select year from takes where ID = \"" << std_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                year.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);


    foreach(QString itm,  course){
        //for converting from QString to char*
        //since mysql_query() accepts only char*
        char temp_course[200];
        char temp_semester[200];
        char temp_year[200];

        QstringToCharArray(itm, temp_course);

        for(QString itm_2 : semester)
            QstringToCharArray(itm_2, temp_semester);
        for(QString itm_3 : year)
            QstringToCharArray(itm_3, temp_year);

        //preparing the query...
        qry.str("");
        qry << "select student.name, takes.ID, course_id, semester, year from student, takes " <<
               "where student.ID = takes.ID and course_id = \"" << temp_course << "\""<<
               " and student.ID != \"" << std_id << "\""
               " and semester = \"" << temp_semester << "\""
               " and year = \"" << temp_year << "\";";

        //query start...
        res = mysql_execute_query(con, qry.str().c_str());

        while((row = mysql_fetch_row(res)) != NULL){
            //creating a table
            QTableWidgetItem *item;

            int col_num = mysql_field_count(con);//to set the number of columns

            int rowCount = 0, columnCount = 0;

            ui->studentRegisteredTable->setColumnCount(col_num);
            ui->studentRegisteredTable->insertRow(rowCount);

            //Adding the header of the table
            QStringList label_2;
            label_2 << "Name" << "Student No" << "Course Id" << "Semester" << "Year";
            ui->studentRegisteredTable->setHorizontalHeaderLabels(label_2);

            for(unsigned int j = 0; j < mysql_field_count(con); j++){
                item = new QTableWidgetItem;

                //chagne table's cell to be read-only
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                //the acutall inserting for data into the table
                item->setText(row[j]);
                ui->studentRegisteredTable->setItem(rowCount, columnCount, item);

                //Resize rows, columns, and the table to fit content
                ui->studentRegisteredTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                ui->studentRegisteredTable->resizeColumnsToContents();
                ui->studentRegisteredTable->resizeRowsToContents();

                columnCount++;
            }
            rowCount++;
        }
    }
    mysql_free_result(res);
}


// edit user profile...
void Student::on_EditButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/ahmad/Desktop/DBMS-project", tr("Image Files (*.png *.jpg *.bmp)"));
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "File", "File wasn't open!!");
    }

    QPixmap pic(fileName);

    ui->frame->setStyleSheet("background-color: #fff");
    ui->frame->setStyleSheet("border-color: #000");
    ui->frame->setPixmap(pic);
    ui->frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

}

//log out button...
void Student::on_logoutButton_clicked()
{
    this->close();
    MainWindow *main;
    main = new MainWindow(this);
    main->show();
}


Student::~Student()
{
    mysql_close(con);
    delete ui;
}

