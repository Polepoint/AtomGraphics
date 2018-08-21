# AtomGraphics
 
## This is a Cross-Platform Graphics SDK project
 
作者开发AtomGraphics的初衷是基于C/C++开发一套能跨平台绘图的API，能充分利用各个平台的特性，实现高流畅度、低内存消耗的的画图SDK

工程的雏形(prototype)参考了cocos2d，有的代码来自其开源代码，后边会逐一声明

**已支持的图形库**

|   | iOS | Android |
| --- | --- | --- |
| OpenGL | ✔️ | ✔️ |
| CoreGraphics | ✔️  |  |
| Vulkan |  |  |
| Metal |  |  |
| Skia |  |  |


## Getting Started

### 工程代码准备
克隆工程代码及其子模块
 
### iOS
* 环境准备
    * Install [iOS Environment](https://developer.apple.com/library/ios/documentation/IDEs/Conceptual/AppStoreDistributionTutorial/Setup/Setup.html)
    * Install [CocoaPods](https://guides.cocoapods.org/using/getting-started.html)
* 启动
    * `cd [ProjectRoot]/test/ios`
    * `pod update`
    * Open AtomGraphics.xcworkspace in Xcode
    * Build & Run
 
### Android
 * 环境准备
    * Install [Android Studio](https://developer.android.com/studio/)
    * Install JDK(<=1.8) & SDK(>=19) & NDK
 * 启动
    * Open [ProjectRoot]/test/Android with Android Studio
    * 如果没有文件 `[ProjectRoot]/test/Android/local.properties` 则新建
    * 修改文件 `local.properties` 中的SDK、NDK路径为你本地路径
        <pre>
        <code>ndk.dir=[Your NDK Path]
        sdk.dir=[Your SDK Path]</code></pre>
    * Build & Run

## 动画用原生实现效果
EChart的[ZRender](https://github.com/ecomfe/zrender)封装了Canvas动画，并且支持Node环境，下边是其几个demo
https://ecomfe.github.io/zrender-doc/public/examples.html
AtomGraphics默认加载了zrender.js，所以可以直接使用ZRender的api实现Canvas动画


## 关于GCanvas的一系列问题
GCanvas在Android上的问题比较多，主要是在FreeType上，总结一下集中在下面几个方面：

* canvas shadow没有实现，包括iOS和Android（阴影效果做了一部分，但是还有问题，暂时没时间修复，后边有时间再看）
* ~~没有实现measureText()~~（已经解决，但是没有适配RN或者Weex）
* ~~FT_Face没有做缓存，导致每个文字的渲染都要加载一遍字体文件，文字渲染的时间明显远长于其他命令执行的时间~~（已解决）
* Android上的GCanvas的OpenGL放在了子线程上渲染，但是只是简单地给每个GTextureView创建了一个线程，让其在自己的子线程上渲染。很明显，GCanvas只考虑了当前界面只有一个Canvas的情况。这么做有个问题，如果创建了多个GTextureView并且同时执行动画，很可能有多个GTextureView同时执行自己的渲染命令，结果很可能将命令渲染到其他surface上

