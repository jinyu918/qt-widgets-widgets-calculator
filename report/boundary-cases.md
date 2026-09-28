# 计算器边界案例记录

本记录对应执行计划中的四个重点边界案例。输入路径应分别覆盖鼠标和键盘；两种路径都应进入同一个 `CalculatorEngine` 命令。

## 1. 重复小数点

| 项目 | 内容 |
|---|---|
| 输入 | `1..2` |
| 预期行为 | 第二个小数点被忽略，显示 `1.2` |
| 涉及状态 | 当前操作数文本、重复小数点校验 |
| 代码位置 | `CalculatorEngine::inputDecimal()` |
| 自动化验证 | `ignoresRepeatedDecimalPoint` |
| 运行证据 | 待 Qt Test 环境可用后补充测试输出和截图 |

## 2. 连续普通运算符

| 项目 | 内容 |
|---|---|
| 输入 | `6 ++ 3 =`，也验证 `6 + × 3 =` |
| 预期行为 | 后一个普通运算符替换前一个，不提前计算；结果分别为 `9` 和 `18` |
| 涉及状态 | 左操作数、待处理运算符、等待第二操作数状态 |
| 代码位置 | `CalculatorEngine::inputOperator()` |
| 自动化验证 | `replacesConsecutiveOperators` |
| 运行证据 | 待 Qt Test 环境可用后补充测试输出和截图 |

## 3. 一元负数

| 项目 | 内容 |
|---|---|
| 输入 | `6 + -3 =` |
| 预期行为 | 运算符后的 `-` 被识别为第二操作数的一元负号，结果为 `3` |
| 涉及状态 | 运算符后等待操作数、负号文本、二元运算 |
| 代码位置 | `CalculatorEngine::inputOperator()`、`CalculatorEngine::inputDigit()` |
| 自动化验证 | `supportsUnaryMinus` |
| 运行证据 | 待 Qt Test 环境可用后补充测试输出和截图 |

## 4. 除零和错误恢复

| 项目 | 内容 |
|---|---|
| 输入 | `8 ÷ 0 =`，随后输入 `5` |
| 预期行为 | 第一步显示 `Error`；数字输入清除错误状态并开始新的计算，显示 `5` |
| 涉及状态 | 除零校验、错误状态、错误恢复 |
| 代码位置 | `CalculatorEngine::applyPending()`、`inputDigit()`、`inputDecimal()` |
| 自动化验证 | `handlesDivisionByZero`、`recoversFromErrorWithNewInput` |
| 运行证据 | 待 Qt Test 环境可用后补充测试输出和截图 |

## 记录格式

在 Qt 环境可用后，每个案例补充以下证据：

1. 鼠标操作截图或录屏帧。
2. 键盘操作截图或录屏帧。
3. Qt Test 对应用例的通过输出。
4. 若发现问题，记录原始现象、原因、修复提交和修复后结果。
