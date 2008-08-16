 本软件是基于windows IME 机制编写而成，名称为：自由拼音输入法。
 本软件源代码公开，遵循GUN GPL(General Public License)。
 本软件支持win95，win98，winNT。

【安装方法】
 1. 运行register.exe，注册输入法。实际上是填充
    HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Keyboard Layouts
    HKEY_LOCAL_MACHINE\Software\freepy
 2. 把binary目录下的文件拷贝到windows的系统目录下，
    例如：c:\win98\system，c:\winnt\system32。其中的文件说明如下：
    freepy.ime 是该输入法程序本身
    freepy.hlp 是帮助文件
    freepy.tab 是输入法字典
    freepysys.dic 是系统词库
    另外，当用户使用时，系统会自造一个用户词库freepyusr.dic,并且自造词库
    和系统词库完全一样。
 3. 象其它输入法那样，从控制面板中选择该输入法：自由拼音输入法。
 4. 完成.

【注意】
 因为2.0版以后的新版本修改了freepy.tab的汉字字典，词库的结构已经改变，所以
 2.0版以前的词库无法在2.0以后的版本下使用，如果你想使用以前的词库，请在
 安装新版本以前，用tool目录下的lib2sim把以前的词库变成普通文本文件， 然后再
 安装新版本，然后再用sim2lib把普通文本文件转化为词库，这样你就可以使用以前
 的词库。对此给你造成的不便表示歉意！