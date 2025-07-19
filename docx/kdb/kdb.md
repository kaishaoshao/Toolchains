# kdb

## 引言

项目主要是学习《Building a Debugger》作者 Sy Brand

作者github : https://github.com/TartanLlama

作者相关博客：https://tartanllama.xyz/posts/writing-a-linux-debugger

项目代码仓库：主要是想写一个自己的调试器，如果可以我希望能为在作者的基础上支持riscv架构

作者推荐将调试器设置为一个库，更好的编写自动化测试，并且能开发与调试器交互的应用程序（单线程）

## 常见知识点

### 缩写

| 名称 | 作用 |
| ---- | ---- |
| ABI  |      |
| ASLR |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |
|      |      |

## Chapter 1 : Project Setup

项目设置 引导你完成设置构建调试器的环境

使用cmake

文件结构

```bash
kdb
├── CMakeLists.txt
├── include        对外头文件
│   └── libsdb
├── src
│   ├── CMakeLists.txt
│   └── include    对内头文件
├── test
│   └── CMakeLists.txt
└── tools
|   └── CMakeLists.txt
└── CMakeLists.txt
```
