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



Tips：

在编写代码时，为了能代码补全相关的库函数，运行

```
xmake project -k compile_commands
```

然后右键该`compile_commands.json`文件选择`load ...`，就会像python一样导包了，享受到和之前一样的代码提示、自动补全，好耶！！



参考相关项目

基于xmake的创建imgui示例页面 [xmake-examples/imgui-scaffold: A minimal imgui project template (github.com)](https://github.com/xmake-examples/imgui-scaffold)

读取obj文件 [xianzhongfang/topocut: Open source code for SIGGRAPH 2022 journal paper TopoCut. (github.com)](https://github.com/xianzhongfang/topocut)

参考资料

[OPENFILENAMEW (commdlg.h) - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/api/commdlg/ns-commdlg-openfilenamew)