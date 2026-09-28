# 计算器边界案例记录

本记录对应执行计划中的四个重点边界案例。鼠标和键盘按钮最终都调用同一个 `CalculatorEngine` 命令接口；窗口级测试验证了这条复用路径，以下记录补充了 Qt 5 Release 窗口中的实际结果。

## 1. 重复小数点

| 项目 | 内容 |
|---|---|
| 输入 | `1..2` |
| 预期行为 | 第二个小数点被忽略，显示 `1.2` |
| 涉及状态 | 当前操作数文本、重复小数点校验 |
| 代码位置 | `CalculatorEngine::inputDecimal()` |
| 自动化验证 | `ignoresRepeatedDecimalPoint` 通过 |
| 手工验证 | Qt 5 Release 数字小键盘输入显示 `1.2` |

## 2. 连续普通运算符

| 项目 | 内容 |
|---|---|
| 输入 | `6 ++ 3 =`，另有 `6 + × 3 =` 自动化验证 |
| 预期行为 | 后一个普通运算符替换前一个，不提前计算；结果分别为 `9` 和 `18` |
| 涉及状态 | 左操作数、待处理运算符、等待第二操作数状态 |
| 代码位置 | `CalculatorEngine::inputOperator()` |
| 自动化验证 | `replacesConsecutiveOperators` 通过 |
| 手工验证 | Qt 5 Release 数字小键盘输入 `6 ++ 3 =` 显示 `9` |

## 3. 一元负数

| 项目 | 内容 |
|---|---|
| 输入 | `6 + -3 =` |
| 预期行为 | 运算符后的 `-` 被识别为第二操作数的一元负号，结果为 `3` |
| 涉及状态 | 运算符后等待操作数、负号文本、二元运算 |
| 代码位置 | `CalculatorEngine::inputOperator()`、`CalculatorEngine::inputDigit()` |
| 自动化验证 | `supportsUnaryMinus` 通过；窗口级混合输入测试通过 |
| 手工验证 | Qt 5 Release 数字小键盘输入结果为 `3`，截图见 `screenshots/qt5-unary-minus.png` |

## 4. 除零和错误恢复

| 项目 | 内容 |
|---|---|
| 输入 | `8 ÷ 0 =`，随后输入 `5` |
| 预期行为 | 第一步显示 `Error`；数字输入清除错误状态并开始新的计算，显示 `5` |
| 涉及状态 | 除零校验、错误状态、错误恢复 |
| 代码位置 | `CalculatorEngine::applyPending()`、`inputDigit()`、`inputDecimal()` |
| 自动化验证 | `handlesDivisionByZero`、`recoversFromErrorWithNewInput` 通过 |
| 手工验证 | Qt 5 Release 窗口先显示 `Error`，再显示 `5`；截图见 `screenshots/qt5-error.png` 和 `screenshots/qt5-error-recovery.png` |

## 结果

四个案例均有对应的自动化覆盖；重复小数点、连续运算符、一元负数和除零恢复均在 Qt 5 Release 窗口中复核。未发现需要追加修复的问题。
