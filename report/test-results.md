# 测试结果记录

## 构建环境

- Qt 版本：Qt 5.12.11 MinGW 7.3.0
- Qt Creator：4.15.0 Community
- CMake：Visual Studio 2022 自带 CMake 3.31.6
- C++ 标准：C++17
- 测试平台：Windows，窗口级测试使用 `QT_QPA_PLATFORM=offscreen`

由于工作树路径包含中文字符，Qt 5.12.11 的 `moc` 在原路径下会把自动生成文件路径解析为乱码。验证时使用 `X:` 临时映射到同一工作树，源代码和提交内容没有复制或修改。

## 自动化测试

Debug 和 Release 均完成配置、编译和测试：

| 配置 | 构建结果 | Qt Test 结果 |
|---|---|---|
| Debug | `QtWidgetsCalculator`、`tst_calculatorengine`、`tst_mainwindow` 全部编译成功 | 2/2 通过，0.49 秒 |
| Release | `QtWidgetsCalculator`、`tst_calculatorengine`、`tst_mainwindow` 全部编译成功 | 2/2 通过，0.33 秒 |

运行方式：

```text
cmake -S X:\\lab1 -B X:\\lab1\\build-ascii -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build X:\\lab1\\build-ascii --parallel 4
ctest --test-dir X:\\lab1\\build-ascii --output-on-failure
```

测试目标和覆盖内容：

- `CalculatorEngineTest`：16 个用例，覆盖四则运算、结果格式化、重复小数点、连续运算符、一元负数、除零、错误恢复、结果继续输入/运算、连续等号、退格和清除。
- `MainWindowInputTest`：4 个用例，覆盖鼠标按钮、主键盘、鼠标与键盘混合输入，以及待处理运算符提示行。

## 手工验证矩阵

| 场景 | 验证路径 | 结果 | 证据 |
|---|---|---|---|
| `1 + 2 =` | 鼠标按钮 | `3` | `screenshots/qt5-mouse-addition.png` |
| `1 + 2 =` | 数字小键盘 | `3` | `screenshots/qt5-keypad-addition.png` |
| `1..2` | 数字小键盘 | `1.2`，第二个小数点被忽略 | Qt 5 Release 窗口观察；engine 自动化用例 `ignoresRepeatedDecimalPoint` |
| `6 ++ 3 =` | 数字小键盘 | `9` | Qt 5 Release 窗口观察；engine 自动化用例 `replacesConsecutiveOperators` |
| `6 + -3 =` | 数字小键盘 | `3` | `screenshots/qt5-unary-minus.png` |
| `8 ÷ 0 =` | 数字小键盘 | `Error` | `screenshots/qt5-error.png` |
| `Error` 后输入 `5` | 数字小键盘 | `5` | `screenshots/qt5-error-recovery.png` |
| `2 + 3 =` 后输入 `7` | engine 自动化 | `7` | `startsNewInputAfterResult` |
| `2 + 3 = × 4 =` | engine 自动化 | `20` | `continuesFromResultWithOperator` |
| `2 + 3 = =` | engine 自动化 | 保持 `5` | `ignoresRepeatedEquals` |

## 截图索引

- `screenshots/qt5-ui-initial.png`：初始界面、按钮层级和布局。
- `screenshots/qt5-mouse-addition.png`：鼠标完成加法。
- `screenshots/qt5-keypad-addition.png`：数字小键盘完成加法。
- `screenshots/qt5-unary-minus.png`：`6 + -3 = 3`。
- `screenshots/qt5-error.png`：除零后的 `Error` 状态。
- `screenshots/qt5-error-recovery.png`：错误状态输入 `5` 后恢复。

## 未执行的验证

仓库没有 `make check` 目标，因此使用 CMake 构建和 CTest 作为等价验证。当前环境没有 LibreOffice，未对原始课程文档做页面渲染检查；本仓库内的 Markdown 计划、边界记录和测试记录已完成文字核对。未执行远程推送。
