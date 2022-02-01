#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseconnection.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}


void MainWindow::on_pushButton_clicked()
{
    //Mysql connection...
    con = NULL;
    MYSQL_RES *res;
    MYSQL_ROW row = NULL;
    struct connection_details mysqlD;
    con = mysql_connection_setup(mysqlD);

    //fetching the enterd ID, password and type of user...
    QString roleId = ui->userIDLogin->text();
    QString password = ui->passwordLogin->text();
    QString role = ui->roleList->currentText();

    //converting from QString to char array...
    char tableName[20], ID[20];
    QstringToCharArray(role, tableName);
    QstringToCharArray(roleId, ID);

    //starting query...
    std::ostringstream qry;
    qry << "select ID, pass from " << tableName << " where ID = " << ID;
    res = mysql_execute_query(con, qry.str().c_str());


    while((row = mysql_fetch_row(res)) != NULL){
        std::string userId = row[0];
        std::string userPass = row[1];

        //converting from std::string to QString from comparison compatibility
        QString qrow_1 = QString::fromStdString(userId);
        QString qrow_2 = QString::fromStdString(userPass);

        //validate the user id and password...
        if(qrow_1 == roleId && qrow_2 == password){
            if(role == "student"){
                this->hide();
                std = new Student(this, roleId);
                std->show();
            }
            else if(role == "instructor"){
                this ->hide();
                instr = new Instructor(roleId, this);
                instr->show();
            }

            else if(role == "faculty_coordinator"){
                this ->hide();
                factCord = new facultyCoordinator(roleId, this);
                factCord->show();
            }
        }else{
            //in case incorrect user id or password was typed in...
            QMessageBox err;
            err.setText("Wrong inputs");
            err.exec();
        }
    }
    mysql_free_result(res);
}

MainWindow::~MainWindow()
{
    mysql_close(con);
    delete ui;
}

