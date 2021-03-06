# PhShell

一个用来学习Linux和c++的练习shell,能运行常用的shell命令

[已经实现的功能](#t1 )

<a href="#t2">改进空间和想法</a>

<a href="#t3">小小的经验总结</a>

<a id="t1"/>

## 已经实现的功能  

 - 最基本的执行程序
```
$ pwd
```
   fork出一个子进程,再调用execvp()执行新程序
 
 -  输入输出重定向
```
$ pwd > test.txt
$ echo < cmd.txt > out.txt
$ cat in.txt | grep word
```

 -  条件控制语句
```
$ if find target.txt
$ then echo yes
$ else echo no
$ fi
```

 -  更改环境,输出全局变量
```
$ set
  ...
  * SSH_AUTH_SOCK=/run/user/1000/keyring/ssh
  * XDG_SEAT=seat0
  * QT_QPA_PLATFORMTHEME=appmenu-qt5
  * BOOST_ROOT=/home/pang/Documents/boost_1_69_0
  * XDG_VTNR=7
  * XDG_RUNTIME_DIR=/run/user/1000
  * HOME=/home/pang
  * GNOME_KEYRING_PID=
  
$ test=hahaha
$ export test
$ set
  ...
  * SSH_AUTH_SOCK=/run/user/1000/keyring/ssh
  * XDG_SEAT=seat0
  * QT_QPA_PLATFORMTHEME=appmenu-qt5
  * BOOST_ROOT=/home/pang/Documents/boost_1_69_0
  * XDG_VTNR=7
  * XDG_RUNTIME_DIR=/run/user/1000
  * HOME=/home/pang
  * GNOME_KEYRING_PID=
  * test=hahaha

```

 - shell内建命令(cd ,exit)
```
/home/pang/codes/MyShell/cmake-build-debug/phsh
/home/pang/codes/MyShell/cmake-build-debug > cd ..
/home/pang/codes/MyShell > ls
cmake-build-debug  CMakeLists.txt  pch.h  practice  readme.md  src
/home/pang/codes/MyShell > cd ./practice
/home/pang/codes/MyShell/practice > exit

Process finished with exit code 0

```

<a id="t2"/>

## 改进空间和想法

#### 完善控制语句

Phshell目前不支持for,while等循环语句,还有相应的break,continue等控制语句.
这里应该利用编译原理独立出一个语法分析器,语法分析器同时应该具有分析`>``$`等特殊符号的能力

#### $变量替换

通过`var=value`语句确实能声明变量,`export`也确实能将变量输入到环境变量
但是例如
```
$ test=123
$ echo $test
>$test   并没有将test替换
应该是
>123
```
还是要借助语法分析器解决

#### tab自动补全,记录命令栈

shell中常用的tab补全命令/路径.和方向键选择命令.
在bash库中借助readline库实现.
至于readline库,具体实现还需要找时间研究一下

#### 后台运行进程

让进程在后台运行,或许要查阅 "APUE"了


<a id="t3"/>

## 小小的经验总结

不要混用c与cpp.要么使用纯c,要么将c库做底层封装.
混用后的代码扩展性和健壮性奇差.
(为了一时方便用了c+stl真是不堪设想,刷算法题那一套还是要批评一番)

