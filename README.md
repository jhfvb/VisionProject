# VisionProject
OpenCV 视觉算法动态库 + Qt 前端调用” 的完整案例，采用清晰的文件层级设计，职责分离明确（动态库负责算法实现，Qt 负责 UI 交互），包含核心视觉算法（灰度化、边缘检测）和完整调用流程。
文件目录
VisionProject/          # 项目根目录
├─ bin/                 # 所有编译产物输出目录
│  ├─ VisionDLL.dll     # 无Qt依赖的算法动态库
│  ├─ VisionDLL.lib     # 动态库导入库
│  └─ QtVisionUI.exe    # Qt前端可执行文件
├─ VisionDLL/           # 算法动态库模块（无Qt依赖）
│  ├─ include/
│  │  └─ VisionProcessor.h  # 算法类（纯C++标准库+OpenCV）
│  ├─ src/
│  │  └─ VisionProcessor.cpp # 算法实现
│  └─ CMakeLists.txt     # 编译配置（无Qt）
└─ QtVisionUI/          # Qt前端模块（保留Qt用于界面）
   ├─ MainWindow.h      # 主窗口类（Qt UI）
   ├─ MainWindow.cpp    # 界面逻辑+算法库调用
   ├─ main.cpp          # 程序入口（Qt初始化）
   ├─ VisionProcessor.h # 复制自VisionDLL/include（算法接口）
   ├─ QtVisionUI.pro    # Qt项目配置
   └─ build-xxx/        # Qt自动编译目录