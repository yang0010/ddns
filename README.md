# 基于阿里云sdk c++ ddns

#### 介绍
本程序用于动态ip解析到域名的ddns程序，采用c++编写，可在不同架构处理器上工作。支持ipv4,ipv6解析。
如有任何问题或者建议可联系作者，qq:1261424813

#### 软件架构
软件架构说明:
本程序支持arm,arm64,x64架构，请根据设备自行修改CMakeLists.txt对应动态库链接目录编译


#### 安装教程

1.  编译之前请先打开CMakeLists.txt文件修改link_directories(${catkin_LIB_DIRS} arm64_lib)
对应系统的库文件目录，默认为arm64_lib，如果你系统是arm就输入arm_lib，x64就输入x64_lib。占时不支持windows。
2.  请使用命令cmake .构建编译环境，然后输入命令make即可编译

#### 使用说明

1.  首次使用ddns程序会在当前运行目录生成setting.dat配置文件，请根据提示操作并修改适合你自己的配置文件，你需要有阿里云的顶级域名和生成了AccessKeyID和AccessKeySecret
2.  程序参数:(如何使用:比如 ./ddns ipv4 即可返回本机ipv4信息)
2.	ipv4 返回ipv4地址
3.	ipv6 返回ipv6地址
4.	d AccessKeyID AccessKeySecret 你的顶级域名 解析记录类型AAAA或者A 你的子域名
此参数为临时单次解析，可不需要设置配置文件，可配合crontab之内的配合使用
如: ./ddns d xxx xxx baidu.com AAAA www

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
