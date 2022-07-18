QT 版本：
	确保你的 QT 版本是 QT-5.6.1-msvc2013 版，即 qt-opensource-windows-x86-msvc2013-5.6.1.exe (下载地址：http://download.qt.io/archive/qt/5.6/5.6.1/)。由于用到了 live2d sdk vs2013 版的库和 QtWebEngine 模块，使用其他版本的 QT 可能无法编译通过。

注意：
	1、用到了网络模块，要使程序中的爬虫能够正常工作，需要拷贝 libeay32.dll 和 ssleay23.dll 这两个动态链接库到执行文件 exe 下面或到 QT 的 bin 目录下。

	2、项目中我尽量做到的路径无关，正常情况下使用正确版本的 QT，可以成功运行本项目的。

	3、如果 debug 版编译不过，先尝试编译 release 版后再编译 debug 版。

	4、项目是基于 live2d sdk 的，如果你不懂 live2d ，Live2D 和 Framework 文件夹中的源码可以跳过。


