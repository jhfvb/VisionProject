#include "../include/VisionProcessor.h"
#include <iostream>
#include <thread>   // 用于创建处理线程（避免阻塞主线程）
#include <opencv2/opencv.hpp>  // OpenCV主头文件（包含 core, imgcodecs, highgui 等）

// 示例程序：演示如何调用VisionProcessor动态库
int main(int argc, char* argv[])
{
    // 1. 创建算法处理器对象（构造函数已修复，无返回值）
    VisionProcessor processor;

    // 2. 设置回调函数（接收进度和结果）
    processor.setCallbacks(
        // 进度回调：打印进度到控制台
        [](int progress) {
            std::cout << "[Progress] " << progress << "% completed" << std::endl;
        },
        // 完成回调：处理结果（打印信息+显示图像）
            [](const std::string& result, const cv::Mat& resultMat) {
            std::cout << "[Result] " << result << std::endl;
            // 若结果图像非空，弹出窗口显示
            if (!resultMat.empty()) {
                cv::namedWindow("Edge Detection Result", cv::WINDOW_NORMAL);
                cv::imshow("Edge Detection Result", resultMat);
                cv::waitKey(0);  // 等待用户按键关闭窗口
                cv::destroyAllWindows();
            }
        }
        );

    // 3. 加载测试图像（替换为你的图像路径，需放在bin目录）
    std::string imagePath = "test.jpg";
    bool loadSuccess = processor.loadImage(imagePath);
    if (!loadSuccess) {
        std::cerr << "[Error] Failed to load image: " << imagePath << std::endl;
        return -1;  // 加载失败，退出程序
    }

    // 4. 在新线程中执行边缘检测（避免阻塞主线程）
    std::thread processThread(
        &VisionProcessor::processEdgeDetect,  // 成员函数地址
        &processor,                           // 对象指针
        50,                                   // Canny低阈值
        150                                   // Canny高阈值
    );

    // 5. 模拟用户交互：按Enter键取消任务
    std::cout << "Press Enter to cancel processing..." << std::endl;
    std::cin.get();  // 等待用户输入

    // 6. 发送停止请求（线程安全）
    processor.stopProcessing();
    std::cout << "[Info] Stop request sent, waiting for task to exit..." << std::endl;

    // 7. 等待处理线程结束（避免程序提前退出）
    if (processThread.joinable()) {
        processThread.join();
    }

    std::cout << "[Info] Program exited normally" << std::endl;
    return 0;
}