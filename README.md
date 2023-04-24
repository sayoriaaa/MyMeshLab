之前使用USTC的框架来进行Mesh相关算法的实现，虽然因为课程的关系比较熟悉，但是在写参数化之外的算法时，相比如GCAL、libigl等常见算法库，面临手册信息没有、调bug痛苦的问题。

正好自己最近也在学习开源项目，因此计划自己写一个，计划利用CGAL作为主体，使用opengl+imgui作为前端，利用xmake构建（超赞！！人生苦短，我选xmake）



在这里[Releases · xmake](https://github.com/xmake-io/xmake/releases)下载xmake

目前暂时不支持Linux（虽然跨平台实现是挺方便的，但是时间...）

构建

```
xmake
```

运行

```
xmake run MyLab
```



参考相关项目

基于xmake的创建imgui示例页面 

读取obj文件 