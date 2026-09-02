# industrial-hmi-template

基于 **Qt5** 的工业上位机（HMI）图形界面工程模板，面向嵌入式 ARM 触摸屏 / 工控一体机，
内置登录、设置、实时 / 历史数据、图表、通信（串口 / 网络 / PLC / 云端）等常用界面与业务模块，
可作为新项目裁剪复用的起步框架。

> 本仓库仅归档**源码与资源**，不含编译产物、`*.db` 运行时数据库和 Qt Creator 个人配置
> （`.pro.user`），也不含大型可执行文件——这些都见 [.gitignore](.gitignore)。

## 目录结构

```
industrial-hmi-template/
├── industrial-hmi-template.pro  # qmake 主工程文件
├── *.cpp / *.h / *.ui           # 主界面/对话框源码（顶层：main/mainwindow/form*/frame*/dialogsetup*）
├── Sources/                     # 业务实现
│   ├── BLL/                     # 业务逻辑层
│   ├── DAL/                     # 数据访问层
│   ├── DT/                      # 数据类型/设备：PLC、串口(uart)、云端(Cloud*)、子机(SubWorker*)、SqliteTools…
│   └── Tools/                   # 工具：软键盘 keyBoard、CScreenSaver、ping、JsonParse、sysutil…
├── Headers/                     # 对应头文件（BLL/DAL/Model/Tools）
├── Images/ + Images.qrc         # 界面图片资源
└── industrial-hmi-template_zh_CN.ts   # Qt 翻译源文件
```

> 说明：`Sources/DT/` 下还有一个独立的 `CloudDataServer.pro` 子工程，其 `.pro.user`、运行期
> `TalkerHistoryDB` 等文件未纳入本仓库。

## 依赖

- Qt 5：`core gui sql network widgets`
- **qwt**（图表库）：编译需系统装有 qwt6，头文件在 `/usr/include/qwt`，链接 `-lqwt-qt5`
  （见 `industrial-hmi-template.pro` 末尾）

## 构建

```bash
qmake industrial-hmi-template.pro
make
```

- `UI_DIR=./UI`，`ui_*.h` 由 `uic` 在构建时自动生成到 `./UI`，不入库。
- 运行时 SQLite 数据库（默认 `industrialhmi.db`，见 `Sources/DAL/dal.cpp`）不入库；
  恢复出厂设置会以 `industrialhmi_bak.db` 备份还原，相关表结构请按业务调整。
- 本目录是干净源码快照；日常开发 / 修改仍在原工程目录进行，改完把变更文件同步进来再
  `git add -A && git commit && git push` 即可。

## 授权说明

私有归档仓库，版权归作者所有，未选择开源许可证。
