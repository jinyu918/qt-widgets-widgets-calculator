#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

#include <array>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->displayEdit->setText(engine_.displayText());
}
