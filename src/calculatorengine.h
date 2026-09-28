#pragma once

#include <QString>

#include <optional>

class CalculatorEngine final
{
public:
    enum class Operator {
        Add,
        Subtract,
        Multiply,
        Divide,
    };

    void inputDigit(int digit);
    void inputDecimal();
    void inputOperator(Operator op);
    void inputEquals();
    void backspace();
    void clear();

    QString displayText() const;
    bool hasError() const;

private:
    static QString formatResult(double value);
    bool parseCurrent(double &value) const;
    bool applyPending(double rightOperand);

    QString currentInput_ = QStringLiteral("0");
    std::optional<double> leftOperand_;
    std::optional<Operator> pendingOperator_;
    bool waitingForOperand_ = true;
    bool justEvaluated_ = false;
    bool error_ = false;
};
