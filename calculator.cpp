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
    SetLightModeStyle();    // set default mode as light

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

    connect(ui->actionDark_Mode, SIGNAL(triggered()), this,
            SLOT(SetDarkModeStyle()));
    connect(ui->actionLight_Mode, SIGNAL(triggered()), this,
            SLOT(SetLightModeStyle()));
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

void Calculator::SetDarkModeStyle()
{
    ui->actionLight_Mode->setEnabled(true);
    ui->actionDark_Mode->setEnabled(false);

    // Styles for widgets
    ui->centralwidget->setStyleSheet("background: #010206");
    ui->scrollAreaWidgetContents->setStyleSheet("background: #292929; "
                                                "border-style: outset; "
                                                "border-width: 0; ");
    // Styles for display screens
    ui->History->setStyleSheet("color: #CCCCCC; ");
    ui->Display->setStyleSheet("color: #CCCCCC; "
                               "background: #292929");

    // Styles for Number buttons
    ui->Button0->setStyleSheet("background: #2400FF");
    ui->Button1->setStyleSheet("background: #2400FF");
    ui->Button2->setStyleSheet("background: #2400FF");
    ui->Button3->setStyleSheet("background: #2400FF");
    ui->Button4->setStyleSheet("background: #2400FF");
    ui->Button5->setStyleSheet("background: #2400FF");
    ui->Button6->setStyleSheet("background: #2400FF");
    ui->Button7->setStyleSheet("background: #2400FF");
    ui->Button8->setStyleSheet("background: #2400FF");
    ui->Button9->setStyleSheet("background: #2400FF");

    // Styles for Sign buttons
    ui->Add->setStyleSheet("background: #2400FF");
    ui->ChangeSign->setStyleSheet("background: #2400FF");
    ui->Clear->setStyleSheet("background: #2400FF");
    ui->ClearHistory->setStyleSheet("background: #2400FF");
    ui->Divide->setStyleSheet("background: #2400FF");
    ui->Equals->setStyleSheet("background: #2400FF");
    ui->Factorial->setStyleSheet("background: #2400FF");
    ui->Multiply->setStyleSheet("background: #2400FF");
    ui->Subtract->setStyleSheet("background: #2400FF");
}

void Calculator::SetLightModeStyle()
{
    ui->actionLight_Mode->setEnabled(false);
    ui->actionDark_Mode->setEnabled(true);

    // Styles for widgets
    ui->centralwidget->setStyleSheet("background: white");
    ui->scrollAreaWidgetContents->setStyleSheet("background: white; "
                                                "border-style: outset; "
                                                "border-width: 0; ");

    // Styles for display screens
    ui->History->setStyleSheet("color: black");
    ui->Display->setStyleSheet("color: black; "
                               "background: white");

    // Styles for Number buttons
    ui->Button0->setStyleSheet("background: white");
    ui->Button1->setStyleSheet("background: white");
    ui->Button2->setStyleSheet("background: white");
    ui->Button3->setStyleSheet("background: white");
    ui->Button4->setStyleSheet("background: white");
    ui->Button5->setStyleSheet("background: white");
    ui->Button6->setStyleSheet("background: white");
    ui->Button7->setStyleSheet("background: white");
    ui->Button8->setStyleSheet("background: white");
    ui->Button9->setStyleSheet("background: white");

    // Styles for Sign buttons
    ui->Add->setStyleSheet("background: white");
    ui->ChangeSign->setStyleSheet("background: white");
    ui->Clear->setStyleSheet("background: white");
    ui->ClearHistory->setStyleSheet("background: white");
    ui->Display->setStyleSheet("background: white");
    ui->Divide->setStyleSheet("background: white");
    ui->Equals->setStyleSheet("background: white");
    ui->Factorial->setStyleSheet("background: white");
    ui->Multiply->setStyleSheet("background: white");
    ui->Subtract->setStyleSheet("background: white");
}
