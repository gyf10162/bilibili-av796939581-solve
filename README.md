# “队长杯”第6期求解代码（含思考题）

## 简介

原题：[你要如何走才能不被木乃伊吃掉？][av796939581]

题目求解用的是 C++ 代码，在 VS2017 环境下编写。因为是当时随便写的，写的比较乱，还用了很多中文变量名，**建议初学者不要去模仿**。

## 思路

本人的求解思路的灵感来自于 [A\* 算法][1]，因为本人在本科毕设里用过。A\* 算法在进行启发式搜索提高算法效率的同时，可以保证找到一条最优路径（基于评估函数）[\[1\]][1]。

本题与 A\* 算法最大的区别就在于状态的定义，A\* 算法在地图上只有一个移动的对象，但本题有多个，因此将玩家、木乃伊、蝎子及栅栏状态编码成一个字符串保存到hash表中，进行搜索时先进行查询，以避免重复搜索。

## 说明

程序分原题和思考题两个部分：

* 原题部分会在控制台显示地图和通关过程（地图比较抽象，不建议看）。

* 思考题部分，只需要把每一格分别作为起点跑一遍就行。该部分在控制台显示结果，并将结果保存到 `result.csv` 文件中。本人电脑也比较老，跑完 100 种情况基本都是在 0.5 秒以内，可见算法还是比较高效的。

`result.csv` 文件格式如下:

|   | 起点X坐标 | 起点Y坐标 | 能否通关 |   步骤   |
|--:| :-------: | :-------: | :------: | :------: |
| 1 |      1    |      1    |     0    |          |
| 2 |      1    |      2    |     1    | RUUUR... |
| 3 |      1    |      3    |     1    | URSUR... |
|...|     ...   |     ...   |    ...   |    ...   |

由于控制台显示比较简陋，本人另外用 MATLAB 进行动画演示，参见 <https://github.com/gyf10162/bilibili-av796939581-animation>。

## 参考资料
\[1\] <https://zh.wikipedia.org/zh-cn/A*%E6%90%9C%E5%B0%8B%E6%BC%94%E7%AE%97%E6%B3%95>

[1]: https://zh.wikipedia.org/zh-cn/A*%E6%90%9C%E5%B0%8B%E6%BC%94%E7%AE%97%E6%B3%95
[av796939581]: https://www.bilibili.com/video/av796939581
