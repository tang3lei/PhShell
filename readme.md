#PhShell

一个用来学习Linux和c++的练习shell,能运行常用的shell命令

<a href="#t1">已经实现的功能</a>
<a href="#t2">改进空间和想法</a>
<a href="#t3">小小的经验总结</a>

<a id="t1"/>
##已经实现的功能  

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
##改进空间和想法


<a id="t3"/>
##小小的经验总结