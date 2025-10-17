#include"VisionProcessor.h"
#include<chrono>
#include<thread>
#include<opencv2/imgproc/imgproc.hpp>

VisionProcessor::VisionProcessor()
{
	//初始化默认空回调
	m_progressCallback = [](int) {};
	m_finishCallback = [](const std::string&, const cv::Mat&) {};

}
void VisionProcessor::setCallbacks(ProgressCallback progressFunc, FinishCallback finishFunc)
{
	if (progressFunc) m_progressCallback = progressFunc;
	if (finishFunc) m_finishCallback = finishFunc;
}

bool VisionProcessor::loadImage(const std::string& imagePath)
{
	std::lock_guard<std::mutex>lock(m_mutex);
	m_inputMat = cv::imread(imagePath);
	if (m_inputMat.empty()) {
		m_lastError = "Load failed:" + imagePath;
		m_isImageLoaded = false;
		return false;
	}
	m_isImageLoaded = true;
	return true;
}

bool VisionProcessor::loadImage(const cv::Mat& inputMat)
{
	std::lock_guard<std::mutex>lock(m_mutex);
	if (inputMat.empty()) {
		m_lastError = "Input mat is empty";
		m_isImageLoaded = false;
		return false;
	}
	m_inputMat = inputMat.clone();
	m_isImageLoaded = true;
	return true;
}

void VisionProcessor::processEdgeDetect(int threshold1, int threshold2)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_isImageLoaded) {
			m_finishCallback(m_lastError, cv::Mat());
			return;
		}
		m_isStopRequested = false;
	}

	const int stepCount = 10;
	for (int i = 0; i <= stepCount; ++i) {
		if (checkStopRequested()) {
			m_finishCallback("Canceled", cv::Mat());
			return;
		}

		m_progressCallback(i * 10);  // 0%,10%,...,100%

		if (i == stepCount) {
			cv::Mat grayMat;
			cv::cvtColor(m_inputMat, grayMat, cv::COLOR_BGR2GRAY);
			cv::GaussianBlur(grayMat, grayMat, cv::Size(3, 3), 0);
			cv::Canny(grayMat, m_resultMat, threshold1, threshold2);
			m_finishCallback("Success", m_resultMat);
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void VisionProcessor::stopProcessing()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isStopRequested = true;

}

bool VisionProcessor::checkStopRequested()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_isStopRequested;
}


