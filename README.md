# README

由于 `LVGL`、`TFT_eSPI`这两个库需要进行裁剪、以及对部分功能进行修改

而默认的pio工程中，这两个库是通过`lib_deps`的方式引入放在了`~/.pio` 目录下
所以**gitignore**文件中是不能忽略 `.pio` 目录的

## 1. 项目结构

```shell
.
├── .gitignore
├── .pio
...
├── README.md
```

## 2. [项目文档 ✨](document/schedule.md)
