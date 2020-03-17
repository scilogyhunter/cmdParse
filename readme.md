# cmdParse
*让命令行参数解析变得简单！*

---

## 简介
cmdParse是一个替代`getopt/getopt_long`的工具函数，用于简洁高效的解析命令行参数。

作者的工作是开发驱动程序，其中很重要的一部分内容就是编写测试工具，为了使得这些工具命令用起来灵活方便，就需要设计各种命令参数。开始也是用的`getopt/getopt_long`函数来解析的，用了一段时间发现当参数比较多比较复杂时这两个函数用起来很繁琐低效，于是就萌生了自己写一个解析工具的想法。

命令参数的目的大部分是用来获取一个新的参数值，解析这个参数值的方法无非就是常用的那几种，而getopt需要用户自己写这些重复无趣的代码来完成。cmdParse的一个很大的优点就是直接通过指定数据类型和目标地址就可以完成这些工作，简洁高效。对于一些需要特殊处理的参数或动作执行，则可以通过注册回调函数来实现。另外一个好处就是编写完选项列表的时候其实也就完成了帮助命令的设计，使用通用选项-help就会输出命令的选项列表信息。


## 特性
- 简单易用，学习成本低。
- 整个模块没有全局或静态数据，完全可重入。
- 和操作系统无关，在裸机平台上也可使用。
- 参数值可直接自动解析获取。
- 可注册回调函数来完成需要特殊处理的参数或动作执行。
- 具备通用的-help选项，统一输出命令选项列表信息。
- 参数解析本身可调试，命令行末尾加“!”，可以输出参数解析信息。
- 选项名不区分短选项或长从选项，可以是任意字符串。
- 可以设置选项强制有效，即便命令行中没有输入该选项。
- 按照选项列表的顺序执行选项。


## 用法
更多详细信息及用法说请移步作者博客。
https://blog.csdn.net/scilogyhunter/category_9735056.html




