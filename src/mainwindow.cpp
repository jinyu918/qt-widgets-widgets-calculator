#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QPushButton>

#include <array>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int column = 0; column < 4; ++column) {
        ui->buttonGridLayout->setColumnStretch(column, 1);
    }
    for (int row = 0; row < 5; ++row) {
        ui->buttonGridLayout->setRowStretch(row, 1);
    }
    setFocusPolicy(Qt::StrongFocus);
    connectCalculatorButtons();
    refreshDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectCalculatorButtons()
{
    const std::array<QPushButton *, 10> digitButtons{
        ui->digit0Button,
        ui->digit1Button,
        ui->digit2Button,
        ui->digit3Button,
        ui->digit4Button,
        ui->digit5Button,
        ui->digit6Button,
        ui->digit7Button,
        ui->digit8Button,
        ui->digit9Button,
    };

    for (int digit = 0; digit < static_cast<int>(digitButtons.size()); ++digit) {
        connect(digitButtons[digit], &QPushButton::clicked, this, [this, digit] {
            engine_.inputDigit(digit);
            refreshDisplay();
        });
    }

    connect(ui->decimalButton, &QPushButton::clicked, this, [this] {
        engine_.inputDecimal();
        refreshDisplay();
    });
    connect(ui->clearButton, &QPushButton::clicked, this, [this] {
        engine_.clear();
        refreshDisplay();
    });
    connect(ui->backspaceButton, &QPushButton::clicked, this, [this] {
        engine_.backspace();
        refreshDisplay();
    });
    connect(ui->equalsButton, &QPushButton::clicked, this, [this] {
        engine_.inputEquals();
        refreshDisplay();
    });

    const auto bindOperator = [this](QPushButton *button, CalculatorEngine::Operator op) {
        connect(button, &QPushButton::clicked, this, [this, op] {
            engine_.inputOperator(op);
            refreshDisplay();
        });
    };
    bindOperator(ui->addButton, CalculatorEngine::Operator::Add);
    bindOperator(ui->subtractButton, CalculatorEngine::Operator::Subtract);
    bindOperator(ui->multiplyButton, CalculatorEngine::Operator::Multiply);
    bindOperator(ui->divideButton, CalculatorEngine::Operator::Divide);
}

void MainWindow::refreshDisplay()
{
    ui->operationLabel->setText(engine_.operationText());
    ui->displayEdit->setText(engine_.displayText());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const int key = event->key();
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        engine_.inputDigit(key - Qt::Key_0);
    } else if (key == Qt::Key_Period || key == Qt::Key_Comma) {
        engine_.inputDecimal();
    } else if (key == Qt::Key_Plus) {
        engine_.inputOperator(CalculatorEngine::Operator::Add);
    } else if (key == Qt::Key_Minus) {
        engine_.inputOperator(CalculatorEngine::Operator::Subtract);
    } else if (key == Qt::Key_Asterisk) {
        engine_.inputOperator(CalculatorEngine::Operator::Multiply);
    } else if (key == Qt::Key_Slash) {
        engine_.inputOperator(CalculatorEngine::Operator::Divide);
    } else if (key == Qt::Key_Enter || key == Qt::Key_Return || key == Qt::Key_Equal) {
        engine_.inputEquals();
    } else if (key == Qt::Key_Backspace) {
        engine_.backspace();
    } else if (key == Qt::Key_Escape || key == Qt::Key_C) {
        engine_.clear();
    } else {
        QMainWindow::keyPressEvent(event);
        return;
    }

    refreshDisplay();
    event->accept();
}
