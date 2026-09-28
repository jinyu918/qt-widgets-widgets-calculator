# 测试结果记录

## 自动化测试

测试目标：`CalculatorEngineTest`

测试文件：`tests/tst_calculatorengine.cpp`

覆盖内容：

- 基础四则运算
- 小数结果格式化
- 重复小数点
- 连续普通运算符
- 一元负数
- 除零错误
- 错误恢复
- 结果后重新输入
- 结果后继续运算
- 连续等号
- 退格
- 清除

计划执行命令：

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

当前状态：本机已发现 Qt Creator 4.15.0、Visual Studio 2022、MSVC 和 CMake，但 Qt Creator 当前仅关联 Qt 5.12.11 MinGW kit，未发现 Qt 6.9.2 SDK。因此上述配置在 `find_package(Qt6 6.9.2)` 处停止，编译和测试尚未执行。代码已经完成 CMake 目标接线、测试声明/定义一致性检查和 Git diff 检查；这些静态检查不能替代 Qt Test 的实际运行结果。

## 手工验证矩阵

| 场景 | 鼠标 | 键盘 | 预期 |
|---|---|---|---|
| `12 + 3 =` | 待执行 | 待执行 | `15` |
| `1.2 + 3.4 =` | 待执行 | 待执行 | `4.6` |
| `1..2` | 待执行 | 待执行 | `1.2` |
| `6 ++ 3 =` | 待执行 | 待执行 | `9` |
| `6 + -3 =` | 待执行 | 待执行 | `3` |
| `8 ÷ 0 =` | 待执行 | 待执行 | `Error` |
| `Error` 后输入 `5` | 待执行 | 待执行 | `5` |
| `2 + 3 = 7` | 待执行 | 待执行 | `7` |
| `2 + 3 = × 4 =` | 待执行 | 待执行 | `20` |
| `2 + 3 = =` | 待执行 | 待执行 | 保持 `5` |

## 构建环境缺口

本机现有 Qt Creator 4.15.0 仅关联 Qt 5.12.11 MinGW kit。为提前检查 C++、`.ui`、QSS 资源和窗口接线，已在临时目录使用 Qt 5.12.11 做兼容性验证：

- `CalculatorEngine` 测试：16 个用例通过。
- Widgets 应用：完整编译成功。
- Qt 5 验证不作为 Qt 6.9.2 的最终验收依据。

在补齐 Qt 6.9.2 后，应重新运行 CMake 配置、Debug/Release 构建、Qt Test 和手工 UI 验证，并把实际通过数量、构建配置和截图路径写入本文件。当前 `report/screenshots/` 只保留目录，不保留临时 Qt 5 预览图。
