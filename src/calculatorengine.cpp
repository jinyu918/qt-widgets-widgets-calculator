#include "calculatorengine.h"

#include <QtGlobal>

QString CalculatorEngine::displayText() const
{
    if (error_) {
        return QStringLiteral("Error");
    }

    if (waitingForOperand_ && pendingOperator_ && leftOperand_ && currentInput_ == QStringLiteral("0")) {
        return formatResult(*leftOperand_);
    }

    return currentInput_;
}

QString CalculatorEngine::operationText() const
{
    if (justEvaluated_ && !lastExpression_.isEmpty()) {
        return lastExpression_;
    }

    if (error_ || !leftOperand_ || !pendingOperator_) {
        return {};
    }

    QString symbol;
    switch (*pendingOperator_) {
    case Operator::Add:
        symbol = QStringLiteral("+");
        break;
    case Operator::Subtract:
        symbol = QStringLiteral("−");
        break;
    case Operator::Multiply:
        symbol = QStringLiteral("×");
        break;
    case Operator::Divide:
        symbol = QStringLiteral("÷");
        break;
    }

    return formatResult(*leftOperand_) + QLatin1Char(' ') + symbol;
}

bool CalculatorEngine::hasError() const
{
    return error_;
}

void CalculatorEngine::inputDigit(int digit)
{
    if (digit < 0 || digit > 9) {
        return;
    }

    if (error_ || (justEvaluated_ && !pendingOperator_)) {
        clear();
    }

    const QString digitText = QString::number(digit);
    if (waitingForOperand_) {
        currentInput_ = digitText;
        waitingForOperand_ = false;
    } else if (currentInput_ == QStringLiteral("0")) {
        currentInput_ = digitText;
    } else if (currentInput_ == QStringLiteral("-")) {
        currentInput_ += digitText;
    } else {
        currentInput_ += digitText;
    }

    justEvaluated_ = false;
}

void CalculatorEngine::inputDecimal()
{
    if (error_ || (justEvaluated_ && !pendingOperator_)) {
        clear();
    }

    if (currentInput_.contains(QLatin1Char('.'))) {
        return;
    }

    if (waitingForOperand_) {
        currentInput_ = QStringLiteral("0.");
        waitingForOperand_ = false;
    } else if (currentInput_ == QStringLiteral("-")) {
        currentInput_ = QStringLiteral("-0.");
    } else {
        currentInput_ += QLatin1Char('.');
    }

    justEvaluated_ = false;
}

void CalculatorEngine::inputOperator(Operator op)
{
    if (error_) {
        return;
    }

    if (justEvaluated_ && !pendingOperator_ && op == Operator::Subtract) {
        clear();
        currentInput_ = QStringLiteral("-");
        waitingForOperand_ = false;
        return;
    }

    if (justEvaluated_ && !pendingOperator_) {
        lastExpression_.clear();
    }

    const bool canStartUnaryMinus = op == Operator::Subtract
        && ((waitingForOperand_ && pendingOperator_) || (!leftOperand_ && waitingForOperand_));
    if (canStartUnaryMinus) {
        currentInput_ = QStringLiteral("-");
        waitingForOperand_ = false;
        justEvaluated_ = false;
        return;
    }

    if (!leftOperand_) {
        double value = 0.0;
        if (!parseCurrent(value)) {
            return;
        }
        leftOperand_ = value;
        pendingOperator_ = op;
        currentInput_ = QStringLiteral("0");
        waitingForOperand_ = true;
        justEvaluated_ = false;
        return;
    }

    if (waitingForOperand_) {
        pendingOperator_ = op;
        return;
    }

    if (currentInput_ == QStringLiteral("-") && pendingOperator_) {
        pendingOperator_ = op;
        currentInput_ = QStringLiteral("0");
        waitingForOperand_ = true;
        return;
    }

    if (pendingOperator_) {
        double rightOperand = 0.0;
        if (!parseCurrent(rightOperand) || !applyPending(rightOperand)) {
            return;
        }
    }

    pendingOperator_ = op;
    currentInput_ = QStringLiteral("0");
    waitingForOperand_ = true;
    justEvaluated_ = false;
}

void CalculatorEngine::inputEquals()
{
    if (error_ || justEvaluated_) {
        return;
    }

    if (!pendingOperator_ || !leftOperand_) {
        justEvaluated_ = true;
        return;
    }

    if (waitingForOperand_ || currentInput_ == QStringLiteral("-")) {
        currentInput_ = formatResult(*leftOperand_);
        leftOperand_.reset();
        pendingOperator_.reset();
        waitingForOperand_ = false;
        justEvaluated_ = true;
        return;
    }

    const QString completedExpression = operationText() + QLatin1Char(' ') + currentInput_;
    double rightOperand = 0.0;
    if (!parseCurrent(rightOperand) || !applyPending(rightOperand)) {
        return;
    }

    lastExpression_ = completedExpression;
    pendingOperator_.reset();
    waitingForOperand_ = false;
    justEvaluated_ = true;
}

void CalculatorEngine::backspace()
{
    if (error_ || (waitingForOperand_ && pendingOperator_)) {
        return;
    }

    if (justEvaluated_) {
        leftOperand_.reset();
        pendingOperator_.reset();
        lastExpression_.clear();
        justEvaluated_ = false;
        waitingForOperand_ = false;
    }

    if (currentInput_ == QStringLiteral("0")) {
        return;
    }

    currentInput_.chop(1);
    if (currentInput_.isEmpty()) {
        currentInput_ = QStringLiteral("0");
        waitingForOperand_ = true;
    } else if (currentInput_ == QStringLiteral("-")) {
        waitingForOperand_ = false;
    }
}

void CalculatorEngine::clear()
{
    currentInput_ = QStringLiteral("0");
    leftOperand_.reset();
    pendingOperator_.reset();
    waitingForOperand_ = true;
    justEvaluated_ = false;
    error_ = false;
    lastExpression_.clear();
}

QString CalculatorEngine::formatResult(double value)
{
    if (qFuzzyIsNull(value)) {
        value = 0.0;
    }

    QString result = QString::number(value, 'f', 10);
    while (result.contains(QLatin1Char('.')) && result.endsWith(QLatin1Char('0'))) {
        result.chop(1);
    }
    if (result.endsWith(QLatin1Char('.'))) {
        result.chop(1);
    }
    return result.isEmpty() || result == QStringLiteral("-0") ? QStringLiteral("0") : result;
}

bool CalculatorEngine::parseCurrent(double &value) const
{
    bool ok = false;
    value = currentInput_.toDouble(&ok);
    return ok;
}

bool CalculatorEngine::applyPending(double rightOperand)
{
    if (!leftOperand_ || !pendingOperator_) {
        return false;
    }

    const double left = *leftOperand_;
    double result = 0.0;
    switch (*pendingOperator_) {
    case Operator::Add:
        result = left + rightOperand;
        break;
    case Operator::Subtract:
        result = left - rightOperand;
        break;
    case Operator::Multiply:
        result = left * rightOperand;
        break;
    case Operator::Divide:
        if (rightOperand == 0.0) {
            error_ = true;
            return false;
        }
        result = left / rightOperand;
        break;
    }

    currentInput_ = formatResult(result);
    leftOperand_ = result;
    waitingForOperand_ = false;
    return true;
}
