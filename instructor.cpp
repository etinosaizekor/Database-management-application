#include "instructor.h"
#include "mainwindow.h"
#include "ui_instructor.h"
#include "databaseconnection.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <QString>
#include <QtGui>
#include <mysql/mysql.h>
#include <QAbstractItemModel>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>


Instructor::Instructor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instructor)
{
    ui->setupUi(this);
}

Instructor::Instructor(QString i_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instructor)
{
    ui->setupUi(this);

    //database connection
    con = NULL;
    MYSQL_RES *res;
    MYSQL_ROW row = NULL;
    struct connection_details mysqlD;
    con = mysql_connection_setup(mysqlD);


    char inst_id[50];//current(logged in) instructor ID...
    QstringToCharArray(i_id, inst_id);
    std::ostringstream qry;

    // ======================================================== //
    //               Student Personal Details
    // ======================================================== //

    qry << "select ID, name, dept_name, faculty from instructor where ID = " << inst_id;
    res = mysql_execute_query(con, qry.str().c_str());
    row = mysql_fetch_row(res);

    //Inserting to the UI the perosnal data of instructor
    ui->instructorID ->setText(row[0]);
    ui->instructorName->setText(row[1]);
    ui->deptName->setText(row[2]);
    ui->faculty->setText(row[3]);
    mysql_free_result(res);


    // ======================================================== //
    //                      Assinged Courses
    // ======================================================== //




    //4 - queries, to fetch course_id, section id semester and the year in 4 separate queries
    // and store them in 3 differnet QStringList (container)
    //for converting from QStringList to QString later via a foreach (QString itm, QStringList obj)
    // they're stored as private data memebre in the .h file

    // query - 1 (course_id)
    qry.str("");
    qry.clear();
    qry << "select course_id from teaches where ID = \"" << inst_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());


    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                course.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);


    // query - 2 (sec_id)
    qry.str("");
    qry.clear();
    qry << "select sec_id from teaches where ID = \"" << inst_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                sec_id.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);

    // query - 3 (semester)
    qry.str("");
    qry.clear();
    qry << "select semester from teaches where ID = \"" << inst_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                semester.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);


    // query - 4 (year)
    qry.str("");
    qry.clear();
    qry << "select year from teaches where ID = \"" << inst_id << "\";";
    res = mysql_execute_query(con, qry.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i = 0; i < mysql_field_count(con); i++){
            for(unsigned int j = 0; j < mysql_num_rows(res); j++){
                year.insert(j, row[j]);
            }
        }
    }
    mysql_free_result(res);


    //preparing the query
    std::ostringstream qry_2;
    qry_2.str() = "";
    qry.clear();
    qry_2 << "select course_id, sec_id, semester, year from teaches where ID = " << inst_id;
    res = mysql_execute_query(con, qry_2.str().c_str());

    while((row = mysql_fetch_row(res)) != NULL){
        QTableWidgetItem *item;

        //int row_num = mysql_num_rows(res);
        int col_num = mysql_field_count(con);

        int rowCount = 0, columnCount = 0;

        ui->courseAssignedTable->setColumnCount(col_num);
        ui->courseAssignedTable->insertRow(rowCount);

        //Adding the header of the table
        QStringList label;
        label << "Course Id" << "Group No" << "Semester" << "Year" << "Grade";
        ui->courseAssignedTable->setHorizontalHeaderLabels(label);
        //Dynamically allocating...
        for(unsigned int j = 0; j < mysql_field_count(con); j++){
            item = new QTableWidgetItem;

            //chagne table's cell to be read-only
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            //the acutall inserting for data into the table
            item->setText(row[j]);

            ui->courseAssignedTable->setItem(rowCount, columnCount, item);

            //Resize rows, columns, and the table to fit content
            ui->courseAssignedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->courseAssignedTable->resizeColumnsToContents();
            ui->courseAssignedTable->resizeRowsToContents();

            columnCount++;
        }
        rowCount++;
    }
    mysql_free_result(res);




    // ======================================================== //
    //                      Courses Destails
    // ======================================================== //


    foreach(QString itm, course ){
        //for converting from QString to char*
        //since mysql_query() accepts only char*
        char temp_course[200];
        char temp_sec[200];
        char temp_semester[200];
        char temp_year[200];

        QstringToCharArray(itm, temp_course);

        foreach(QString itm_2, semester)
            QstringToCharArray(itm_2, temp_semester);
        foreach(QString itm_3, year)
            QstringToCharArray(itm_3, temp_year);
        foreach (QString itm_4, sec_id)
            QstringToCharArray(itm_4, temp_sec);

        //preparing a query...
        std::ostringstream qry_3;
        qry_3 << "select student.name, student.ID, course_id, sec_id semester, year from takes, student "
              << "where student.ID = takes.ID and course_id = \"" << temp_course
              << "\" and sec_id = " << temp_sec
              << " and semester = \"" << temp_semester << "\" and year = \"" << temp_year << "\";";

        res = mysql_execute_query(con, qry_3.str().c_str());

        while((row = mysql_fetch_row(res)) != NULL){
            //creating a table
            QTableWidgetItem *item;

            int col_num = mysql_field_count(con);
            int rowCount = 0, columnCount = 0;

            //assign number of rows and columns dynamically
            ui->courseInfoTable->setColumnCount(col_num);
            ui->courseInfoTable->insertRow(rowCount);

            //Adding the header of the table
            QStringList label_2;
            label_2 << "Name" << "Student No" << "Course Id" << "Section"<< "Semester" << "Year";
            ui->courseInfoTable->setHorizontalHeaderLabels(label_2);

            //inserting into the QTableWidget automatically...
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
    }
    mysql_free_result(res);
    mysql_close(con);
}

// ======================================================== //
//                Assign Students to Course
// ======================================================== //


void Instructor::on_registerButton_clicked()
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char s_id[10];
    char i_id[10];
    char studentFaculty[50];
    char instrFaculty[50];
    int flag = 0;
    std::ostringstream query;

    QstringToCharArray(ui->studentID->text(), s_id);
    res = mysql_execute_query(con, "select ID from student");

    //Loop through all ID in student and checks
    //if the ID entered is found in the Student Database
    char temp[10];
    while((row = mysql_fetch_row(res)) != NULL){
        QstringToCharArray(row[0], temp);
        if(strcmp(s_id, temp) == 0){
            flag = 1;
            break;
        }
    }
    mysql_free_result(res);
    if(flag == 0){
        QMessageBox::information(this, "Check entry", "Student not in database");
    }
    else{
        flag = 0;
        if(ui->courseIDBox->currentIndex() == 0){
            QMessageBox::information(this, "Select course_id", "No course ID was selected");
        }

        QstringToCharArray(id, i_id);

        //Querying for faculty for both instructor and student to ensure that
        //the instructor can only register student in the same faculty

        query<<"select faculty from student where ID = '" <<s_id <<"'";
        res = mysql_execute_query(con, query.str().c_str());
        while((row = mysql_fetch_row(res)) != NULL){
            strcpy(studentFaculty, row[0]);
        }
        mysql_free_result(res);

        query.str("");
        query.clear();
        query<<"select faculty from instructor where ID = " <<i_id;
        res = mysql_execute_query(con, query.str().c_str());
        while((row = mysql_fetch_row(res)) != NULL){
            strcpy(instrFaculty, row[0]);
        }
        mysql_free_result(res);

        //check to ensure student and instructor are in the same faculty;
        if((strcmp(studentFaculty, instrFaculty)) == 0){
            QStringList groupList, semesterList, yearList;

            char course_id[10];
            QstringToCharArray(ui->courseIDBox->currentText(), course_id);

            query.str("");
            query.clear();

            query<<"select sec_id, semester, year "
                     "from teaches where ID = '" <<i_id <<"' and course_id ='" <<course_id <<"'";

            res = mysql_execute_query(con, query.str().c_str());

            //Fetching the group, semester and year of the choosen course_id assigned to instructor
            while((row = mysql_fetch_row(res)) != NULL){
                groupList<<row[0];
                semesterList<<row[1];
                yearList<<row[2];
            }
            mysql_free_result(res);
            //Check if the group entered is a valid group in that course
            if(ui->groupLineEdit->text() != NULL){
                for(auto &i : groupList){
                    if(i == ui->groupLineEdit->text()){
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0){
                    QMessageBox::information(this, "Invalid group entry", "Check course assigned tab "
                                                                          "for valid group entry");
                }
                else{
                    flag = 0;
                    //Check to ensure the instructor has been assigned to take the course for the selected semester
                    if(ui->semesterLineEdit->text() != NULL){
                        for(auto &i : semesterList){
                            if(i == ui->semesterLineEdit->text()){
                                flag = 1;
                                break;
                            }
                        }
                        if(flag == 0){
                            QMessageBox::information(this, "Invalid semester entry", "Check course assigned tab "
                                                                                     "for valid Semester entry");
                        }
                        else{
                            //Check to ensure the instructor has been assigned to take the course for the selected year
                            flag = 0;
                            if(ui->yearLineEdit->text() != NULL){
                                for(auto &i : yearList){
                                    if(i == ui->yearLineEdit->text()){
                                        flag = 1;
                                        break;
                                    }
                                }
                                if(flag == 0){
                                    QMessageBox::information(this, "Invalid year entry", "Check course assigned tab "
                                                                                         "for valid year entry");
                                }
                                else{
                                    char semester[8], year[10];
                                    int group;
                                    QstringToCharArray(ui->semesterLineEdit->text(), semester);
                                    QstringToCharArray(ui->yearLineEdit->text(), year);
                                    group = ui->groupLineEdit->text().toInt();

                                    query.str("");
                                    query.clear();
                                    query<<"insert ignore into takes(ID, course_id, sec_id, semester, year, grade)"
                                             "values('"<<s_id <<"', '"<<course_id<<"', '" <<group <<"', '" <<semester
                                          <<"', '" <<year <<"', '" <<0 <<"')";

                                    int status = mysql_query(con, query.str().c_str());
                                    if(status == 0){
                                        QMessageBox::information(this, "Success", "Registration successful");
                                    }
                                    else{
                                        QMessageBox::information(this, "Unsuccessful", "Make sure the entries are valid");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            QMessageBox::information(this, "Not valid", "Cannot register student from another faculty");
        }
    }
}


//Edit profile picture
void Instructor::on_EditButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
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


void Instructor::on_logoutButton_clicked()
{
    this->close();
    MainWindow *main;
    main = new MainWindow(this);
    main->show();
}


Instructor::~Instructor()
{
    delete ui;
}
