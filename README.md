# 简介
这是一个很简单的图书管理程序，用了mysql的API来进行数据管理，里面的很多内容都没有经过太仔细的研究，但至少，从理论上来说，它可以实现一个图书馆所需要的基本功能，现在有了基本的框架，即使需要一些其他功能也能很容易的添加。
不过话又说回来，虽然现在有了图形化界面，但其效果无疑是很差的，毕竟图形化界面是直接用Windows.h写的，所以做起什么来都特别痛苦，这一大堆恶心的函数我都是边查边用，现在写完了也没记住几个
但归根结底，这是一个大一学生的作品，注定了有很多问题存在，或许唯一的存在价值就是留在这里以博大家一笑，如有bug，还请无视。如果你愿意指出，更是万分感激
# 关于使用
这个小东西使用了Clion作为IDE，所以如果想要在别的IDE上使用，请自行研究
还有，由于使用了MYSQL数据库，请自行添加依赖项（其实我是想解决这个问题的，但发现用相对路径搞的话，构建期间会无法识别，所以，请自行解决）
# 关于中文乱码
由于代码本身以UTF-8的格式编码，而你的命令行是在使用GBK，所以造成了中文乱码，如果想要解决，直接将每个含中文的文件都改为GBK格式即可，或者将自己的电脑编码合适改为UTF-8即可
