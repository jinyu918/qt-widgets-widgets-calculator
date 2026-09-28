#include "calculatorengine.h"

#include <QtTest>

class CalculatorEngineTest final : public QObject
{
    Q_OBJECT

private slots:
    void performsBasicOperations();
    void exposesPendingOperation();
    void formatsDecimalResults();
    void formatsRepeatingDecimalResults();
    void ignoresRepeatedDecimalPoint();
    void replacesConsecutiveOperators();
    void supportsUnaryMinus();
    void handlesDivisionByZero();
    void recoversFromErrorWithNewInput();
    void startsNewInputAfterResult();
    void startsNegativeInputAfterResult();
    void continuesFromResultWithOperator();
    void ignoresRepeatedEquals();
    void ignoresCommandsInError();
    void removesLastInputCharacter();
    void clearsState();
};

void CalculatorEngineTest::performsBasicOperations()
{
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputDigit(3);
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("15"));
}

void CalculatorEngineTest::exposesPendingOperation()
{
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);

    QCOMPARE(engine.operationText(), QStringLiteral("12 +"));
    QCOMPARE(engine.displayText(), QStringLiteral("12"));
}

void CalculatorEngineTest::formatsDecimalResults()
{
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDecimal();
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputDigit(3);
    engine.inputDecimal();
    engine.inputDigit(4);
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("4.6"));
}

void CalculatorEngineTest::formatsRepeatingDecimalResults()
{
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputOperator(CalculatorEngine::Operator::Divide);
    engine.inputDigit(3);
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("0.3333333333"));
}

void CalculatorEngineTest::ignoresRepeatedDecimalPoint()
{
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDecimal();
    engine.inputDecimal();
    engine.inputDigit(2);

    QCOMPARE(engine.displayText(), QStringLiteral("1.2"));
}

void CalculatorEngineTest::replacesConsecutiveOperators()
{
    CalculatorEngine engine;
    engine.inputDigit(6);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputOperator(CalculatorEngine::Operator::Multiply);
    engine.inputDigit(3);
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("18"));
}

void CalculatorEngineTest::supportsUnaryMinus()
{
    CalculatorEngine engine;
    engine.inputDigit(6);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputOperator(CalculatorEngine::Operator::Subtract);
    engine.inputDigit(3);
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("3"));
}

void CalculatorEngineTest::handlesDivisionByZero()
{
    CalculatorEngine engine;
    engine.inputDigit(8);
    engine.inputOperator(CalculatorEngine::Operator::Divide);
    engine.inputDigit(0);
    engine.inputEquals();

    QVERIFY(engine.hasError());
    QCOMPARE(engine.displayText(), QStringLiteral("Error"));
}

void CalculatorEngineTest::recoversFromErrorWithNewInput()
{
    CalculatorEngine engine;
    engine.inputDigit(8);
    engine.inputOperator(CalculatorEngine::Operator::Divide);
    engine.inputDigit(0);
    engine.inputEquals();
    engine.inputDigit(5);

    QVERIFY(!engine.hasError());
    QCOMPARE(engine.displayText(), QStringLiteral("5"));
}

void CalculatorEngineTest::startsNewInputAfterResult()
{
    CalculatorEngine engine;
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputDigit(3);
    engine.inputEquals();
    engine.inputDigit(7);

    QCOMPARE(engine.displayText(), QStringLiteral("7"));
}

void CalculatorEngineTest::startsNegativeInputAfterResult()
{
    CalculatorEngine engine;
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputDigit(3);
    engine.inputEquals();
    engine.inputOperator(CalculatorEngine::Operator::Subtract);
    engine.inputDigit(4);

    QCOMPARE(engine.displayText(), QStringLiteral("-4"));
}

void CalculatorEngineTest::continuesFromResultWithOperator()
{
    CalculatorEngine engine;
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputDigit(3);
    engine.inputEquals();
    engine.inputOperator(CalculatorEngine::Operator::Multiply);
    engine.inputDigit(4);
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("20"));
}

void CalculatorEngineTest::ignoresRepeatedEquals()
{
    CalculatorEngine engine;
    engine.inputDigit(2);
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputDigit(3);
    engine.inputEquals();
    engine.inputEquals();

    QCOMPARE(engine.displayText(), QStringLiteral("5"));
}

void CalculatorEngineTest::ignoresCommandsInError()
{
    CalculatorEngine engine;
    engine.inputDigit(8);
    engine.inputOperator(CalculatorEngine::Operator::Divide);
    engine.inputDigit(0);
    engine.inputEquals();
    engine.inputOperator(CalculatorEngine::Operator::Add);
    engine.inputEquals();
    engine.backspace();
    engine.inputOperator(CalculatorEngine::Operator::Subtract);

    QVERIFY(engine.hasError());
    QCOMPARE(engine.displayText(), QStringLiteral("Error"));
}

void CalculatorEngineTest::removesLastInputCharacter()
{
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDigit(2);
    engine.backspace();

    QCOMPARE(engine.displayText(), QStringLiteral("1"));
}

void CalculatorEngineTest::clearsState()
{
    CalculatorEngine engine;
    engine.inputDigit(9);
    engine.inputOperator(CalculatorEngine::Operator::Multiply);
    engine.inputDigit(4);
    engine.clear();

    QCOMPARE(engine.displayText(), QStringLiteral("0"));
    QVERIFY(!engine.hasError());
}

QTEST_APPLESS_MAIN(CalculatorEngineTest)

#include "tst_calculatorengine.moc"
