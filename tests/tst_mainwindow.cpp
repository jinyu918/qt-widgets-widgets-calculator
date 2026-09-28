#include "mainwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>

class MainWindowInputTest final : public QObject
{
    Q_OBJECT

private slots:
    void mouseButtonsUseCalculatorEngine();
    void keyboardUsesCalculatorEngine();
    void mixedInputKeepsOneState();
    void pendingOperatorIsVisible();
};

namespace {
QLineEdit *display(MainWindow &window)
{
    return window.findChild<QLineEdit *>(QStringLiteral("displayEdit"));
}

QPushButton *button(MainWindow &window, const QString &name)
{
    return window.findChild<QPushButton *>(name);
}
}

void MainWindowInputTest::mouseButtonsUseCalculatorEngine()
{
    MainWindow window;
    button(window, QStringLiteral("digit1Button"))->click();
    button(window, QStringLiteral("addButton"))->click();
    button(window, QStringLiteral("digit2Button"))->click();
    button(window, QStringLiteral("equalsButton"))->click();

    QCOMPARE(display(window)->text(), QStringLiteral("3"));
}

void MainWindowInputTest::keyboardUsesCalculatorEngine()
{
    MainWindow window;
    QTest::keyClick(&window, Qt::Key_1);
    QTest::keyClick(&window, Qt::Key_Plus);
    QTest::keyClick(&window, Qt::Key_2);
    QTest::keyClick(&window, Qt::Key_Return);

    QCOMPARE(display(window)->text(), QStringLiteral("3"));
}

void MainWindowInputTest::mixedInputKeepsOneState()
{
    MainWindow window;
    button(window, QStringLiteral("digit6Button"))->click();
    QTest::keyClick(&window, Qt::Key_Plus);
    QTest::keyClick(&window, Qt::Key_Minus);
    QTest::keyClick(&window, Qt::Key_3);
    button(window, QStringLiteral("equalsButton"))->click();

    QCOMPARE(display(window)->text(), QStringLiteral("3"));
}

void MainWindowInputTest::pendingOperatorIsVisible()
{
    MainWindow window;
    button(window, QStringLiteral("digit8Button"))->click();
    button(window, QStringLiteral("divideButton"))->click();

    const auto *operation = window.findChild<QLabel *>(QStringLiteral("operationLabel"));
    QCOMPARE(operation->text(), QStringLiteral("8 ÷"));
    QCOMPARE(display(window)->text(), QStringLiteral("8"));
}

QTEST_MAIN(MainWindowInputTest)

#include "tst_mainwindow.moc"
