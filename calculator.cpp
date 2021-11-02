#include "calculator.h"
#include "ui_calculator.h"
double calcVal = 0.0;
double secVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool factTrigger = false;
bool isNewCalculation = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];
    for (int i = 0; i <  10; ++i){
        QString butName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }

    connect(ui->Add, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Factorial, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));


    connect(ui->Equals, SIGNAL(released()), this,
            SLOT(EqualButtonPressed()));
    connect(ui->ChangeSign, SIGNAL(released()), this,
            SLOT(ChangeNumberSign()));
    connect(ui->Clear, SIGNAL(released()), this,
            SLOT(ClearPressed()));
    connect(ui->ClearHistory, SIGNAL(released()), this,
            SLOT(ClearHistoryPressed()));


}

Calculator::~Calculator()
{
    delete ui;
}


void Calculator::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    if(isNewCalculation) {
        ui->Display->setText("0");
        isNewCalculation = false;
    }

    QString displayVal = ui->Display->text();

    if ((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){
        ui->Display->setText(butVal);
    }
    else {
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();


    if (QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
        ui->History->setPlainText(ui->History->toPlainText() + displayVal + " /");
    }
    else if (QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
        ui->History->setPlainText(ui->History->toPlainText() + displayVal + " *");
    }
    else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
        ui->History->setPlainText(ui->History->toPlainText() + displayVal + " +");
    }
    else if(QString::compare(butVal, "!", Qt::CaseInsensitive) == 0){
        QString DisplayVal = ui->Display->text();
        double dblDisplayVal = DisplayVal.toDouble();
        double solution = 1;
        if(ui->Display->text() == "INFINITY") {
            ui->History->setPlainText(ui->History->toPlainText() + DisplayVal + "!" + " = " + ui->Display->text() + "\n");
            return;
        }

        else if(dblDisplayVal > 170){
            ui->Display->setText("INFINITY");
            ui->History->setPlainText(ui->History->toPlainText() + DisplayVal + "!" + " = " + ui->Display->text() + "\n");
            return;
        }
        else {
            for(int i = 1; i <= dblDisplayVal; ++i){
                solution = solution * i;
            }

        }
        ui->Display->setText(QString::number(solution));
        ui->History->setPlainText(ui->History->toPlainText() + DisplayVal + "!" + " = " + QString::number(solution) + "\n");
        return;
    }
    else {
        subTrigger = true;
    }
    ui->Display->setText("0");
}

void Calculator::EqualButtonPressed(){
    QString displayVal = ui->Display->text();
    if (!isNewCalculation) {
        secVal = displayVal.toDouble();
    }
    else {
        calcVal = displayVal.toDouble();
    }
    double solution = 0.0;

    QString operation;
    if(addTrigger || subTrigger || multTrigger || divTrigger){
        if (addTrigger) {
            solution = calcVal + secVal;
            operation = "+";
        }
        else if (subTrigger) {
            solution = calcVal - secVal;
            operation = "-";
        }
        else if (multTrigger) {
            solution = calcVal * secVal;
            operation = "*";
        }
        else {
            solution = calcVal / secVal;
            operation = "/";
        }
    }

    ui->Display->setText(QString::number(solution));
    if(!isNewCalculation){
        ui->History->setPlainText(ui->History->toPlainText() + " " + QString::number(secVal) + " = " + QString::number(solution) + "\n");
        isNewCalculation = true;
    }
    else {
        ui->History->setPlainText(ui->History->toPlainText() + QString::number(calcVal) + " " + operation + " " + QString::number(secVal) + " = " + QString::number(solution) + "\n");
    }
}

void Calculator::ChangeNumberSign(){
    QString displayVAl = ui->Display->text();
    QRegularExpression reg("[-]?[0-9.]*?");
    QRegularExpressionMatch match = reg.match(displayVAl);
    if(match.hasMatch()){
        double dblDisplayVal = displayVAl.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->Display->setText((QString::number(dblDisplayValSign)));
    }
}

void Calculator::ClearPressed(){
    ui->Display->setText(QString::number(0));
}

void Calculator::ClearHistoryPressed(){
    ui->History->setPlainText("");
}




