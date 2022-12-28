

1. 创建引擎
    XYCGWindowsEngine *m_pBeautyEngine = new CG::XYCGWindowsEngine();

2. 权限验证
    m_pBeautyEngine->initWindowsEngine(license, userid);

3. 加载AI模型，用于人脸检测
    m_pBeautyEngine->loadAIModel(path);

4. 设置美颜资源路径
    m_pBeautyEngine->setBeautyResourcePath(path);

5. 美颜设置：磨皮、美白、瘦脸等
    5.1  m_pBeautyEngine->enableBeauty(bool);
    5.2  m_pBeautyEngine->enableBeautyType(FaceBeautyType, bool);
    5.3  m_pBeautyEngine->setBeautyTypeIntensity(FaceBeautyType, float);

6. 滤镜设置
    6.1 m_pBeautyEngine->enableFilter(bool);
    6.2 m_pBeautyEngine->setFilterResourcePath(path);
    6.3 m_pBeautyEngine->setFilterIntensity(float);

7. 颜色调整设置
    7.1  m_pBeautyEngine->enableColorfulAdjustment(bool)
    7.2  m_pBeautyEngine->setColorfulTypeIntensity(xhs_colorful_type, float);


8. 美颜、滤镜处理
    // 处理: processYUV()内部就是把y、u、v组装成nv12再调用process()
    m_pBeautyEngine->processYUV() 或 m_pBeautyEngine->process() 
    // 获取美颜、滤镜等处理后的数据(YUV-I420格式)用于上屏
    m_pBeautyEngine->getOutputYUVData()

9. 销毁资源
    m_pBeautyEngine->destroyWindowsEngine();