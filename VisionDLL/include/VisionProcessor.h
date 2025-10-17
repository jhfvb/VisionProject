#ifndef VISIONPROCESSOR_H
#define VISIONPROCESSOR_H

#include <string>
#include <mutex>
#include <functional>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
#define ALGO_EXPORT __declspec(dllexport)
#else
#define ALGO_EXPORT
#endif

class ALGO_EXPORT VisionProcessor
{
public:
	//进度回调
	using ProgressCallback = std::function<void(int)>;
	//完成回调（传递图像和结果）
	using FinishCallback = std::function<void(const std::string&, const cv::Mat&)>;

	VisionProcessor();
	~VisionProcessor() = default;

	//设置回调（供Qt前端注册）
	void setCallbacks(ProgressCallback progressFunc, FinishCallback finishFunc);

	//加载图像（支持路径和内存Mat)
	bool loadImage(const std::string& imagePath);
	bool loadImage(const cv::Mat& inputMat);

	//执行边缘检测（阻塞，需要Qt子线程中调用）
	void processEdgeDetect(int threshold1 = 50, int threshold2 = 150);

	//停止任务（线程安全）
	void stopProcessing();

private:
	bool checkStopRequested();

	ProgressCallback m_progressCallback;
	FinishCallback m_finishCallback;
	std::mutex m_mutex;
	bool m_isStopRequested = false;
	bool m_isImageLoaded = false;
	cv::Mat m_inputMat;
	cv::Mat m_resultMat;
	std::string m_lastError;
};

#endif // VISIONPROCESSOR_H