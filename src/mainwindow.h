#pragma once

#include "calculatorengine.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void connectCalculatorButtons();
    void refreshDisplay();

    Ui::MainWindow *ui;
    CalculatorEngine engine_;
};
