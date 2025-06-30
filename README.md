# S1mplePicEditor-PKUQtHomework
一个基于 Qt 框架开发的简易图像处理应用，支持常见的图像变换与处理，界面简约现代，交互直观，适合作为学习 Qt 图形处理和 UI 编程的参考项目。


本程序为北京大学2025春《程序设计实习》课程的大作业。这个应用具有设计优雅、交互直观、通用性强、基本覆盖基础需求等优点。
程序运行演示视频的北大网盘链接：
https://disk.pku.edu.cn/link/AAAC4985DB8E1947E3A3E2C4A1DA1142F3
文件名：1-演示.mp4
有效期限：永久有效
提取码：834O


在开发之初，我们受图像旋转大作业启发，曾经从bmp图片文件直接操作像素位的特性出发，希望扩展更多功能，创造一款我们自己的图像处理软件；而在开发过程中，QImage,QPixmap等类也可以完全做到像BMP一样的像素位处理，并且可以支持多种格式、通道数的文件互相转化，并支持任意格式保存。于是我们迅速把项目名从“SimpleBMPEditor”改成了“SimplePicEditor”，即适用于一般图片，极大提升了该软件的适用性。不过由于当时开发进度已经过半，qt项目更名困难，因此项目文件还是保持原来的名字。

编译环境：
Qt 6.7.3(macOS,arm64)with network
Windows 11 and macOS 15.2 were tested;Linux was not.

代码量约为2500-3000行。

关于本程序的详细介绍（具体功能，实现，分工，开发心得等）详见“1-作业报告.pdf”。

本项目采用CMake构建。
全部源代码及需要的资源文件在sourcecode/S1mpleBMPEditor文件夹内，只需把全部文件放进一个文件夹内，并用适当版本的Qt（实测6.7.3 for macOS,6.7.3 for Windows有效）打开CMakeLists.txt即可打开项目，选择"Release"后构建即可运行较稳定的版本。

以下为两个打包后可以运行的程序的网盘链接：
https://disk.pku.edu.cn/link/AAC1BC161D727F442C83ED7E859149BF93
文件夹名：boxed_application
有效期限：永久有效
提取码：vCSC

其中.exe适用于windows系统，.app适用于macOS系统，分别于windows11和macOS15.2下验证运行良好。
但注意：.app文件会因无开发者签名许可而显示“文件损坏”，需要打开任何来源解决该问题方能使用。
