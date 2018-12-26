# THUCAL
## 一、简介

本程序是一个可以通过命令行操作、以图形化界面显示，具有日程记录，TODO记录、导入、导出功能；可以导入清华大学课程表生成日程的日历。

## 二、安装

要求：linux系统（测试系统为ubuntu/debian）cmake版本为3.5及以上，有c++编译环境

1. 安装依赖

   Ubuntu/Debian:

   安装ncurses5

   ```bash
   sudo apt-get install libncurses5-dev
   ```

   安装cdk5

   ```bash
   wget https://invisible-island.net/datafiles/release/cdk.tar.gz
   tar -xvzf cdk.tar.gz
   cd cdk-5.0-20180306/
   ./configure
   sudo make
   sudo make install
   ```

   下载libxl([下载地址](http://www.libxl.com/download.html))

   ```bash
   wget http://www.libxl.com/download/libxl-lin-3.8.4.tar.gz
   ```

   安装libxl

   ```
   tar -xvzf libxl-lin-3.8.4.tar.gz
   cd libxl-3.8.4/
   sudo cp include_cpp/* /usr/include
   sudo cp lib64/* /usr/lib/
   cd ..
   ```

   下载源码，安装

   ```bash
   git clone https://git.tsinghua.edu.cn/hukq16/thucal.git
   cd thucal
   cmake .
   make
   ```

   使用

   ```
   ./thucal
   ```

## 使用说明

### 开始

启动后，显示帮助信息，按任意键进入界面，后按下h获取帮助