// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    

    This example is essentially just a version of the face_landmark_detection_ex.cpp
    example modified to use OpenCV's VideoCapture object to read from a camera instead 
    of files.


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/

#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>

#include <pthread.h>

#include <stdio.h>
#include <ctime>
#include <string>

#include <cstdlib>
#include <unistd.h>
#include <functional>
#include <fcntl.h>   
#include <ctime>

using namespace dlib;
using namespace std;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET> 
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
                            alevel0<
                            alevel1<
                            alevel2<
                            alevel3<
                            alevel4<
                            max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
                            input_rgb_image_sized<150>
                            >>>>>>>>>>>>;

typedef struct {
    string frame_name;
    string p_name;
} FACE_REC_ST;

// 给帧图像写入文件加线程互斥锁
std::mutex tmp_frame_pic_mutex; 

void *face_rec_thread_func(void* arg);

void Delay(int time); //time*1000为秒数 

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Run this example by invoking it like this: " << endl;
        cout << "   ./dnn_face_recognition_ex your_face_img" << endl;
        cout << endl;
        cout << "You will also need to get the face landmarking model file as well as " << endl;
        cout << "the face recognition model file.  Download and then decompress these files from: " << endl;
        cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << "http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2" << endl;
        cout << endl;
        return 1;
    }

    // 定义一个人脸检测器，用来查找图片中的人脸
    frontal_face_detector detector = get_frontal_face_detector();

    // 我们也将使用人脸特征点模型将从图片中抓取的人脸对齐成一个标准的人脸
    shape_predictor sp;
    deserialize("../data/shape_predictor_68_face_landmarks.dat") >> sp;

    // 在屏幕上显示原始的图片
    //matrix<rgb_pixel> img;
    //load_image(img, argv[1]);
    //image_window win(img); 

    image_window show_win;

    string macthed_name = "";

    unsigned int isCatchedFlag = 0;

    // 在屏幕上显示匹配好的的图片
    matrix<rgb_pixel> saved_macthed_img;
    image_window show_macthed_win;

    // 将每帧图像写入到一个图片文件，将路径传入线程，这样就可以对图片中的人脸进行检测和识别了.
    string frame_pic_name = "";

    string saved_matched_img_name = "";

    int ret = 0;

    try
    {
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        // 抓取并处理视频帧图像，直到用户关闭当前显示视频窗口
        while(!show_win.is_closed())
        {
            // 抓取一帧图像
            cv::Mat frame;
            cap >> frame;

            // 将Opencv‘s Mat对象转换成dlib可以处理的东西. 需要注意的是，
            // 这仅仅是包装了一下Mat对象, 并没有拷贝什么. 所以当frame变量有效的情况下，cimg才是有效的.
            // 所以不要对frame变量做什么，否则可能会内存重定向，这样会导致储存图像的内存包含空指针.
            // 基本上也就意味着，当你在使用cimg变量的时候，不应该去修改frame变量
            cv_image<bgr_pixel> cimg(frame);

            // 检测cimg中包含的人脸
            std::vector<rectangle> show_faces = detector(cimg);

            // 找到每张人脸的正面姿势
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < show_faces.size(); ++i)
            {
                shapes.push_back(sp(cimg, show_faces[i]));

                if (!shapes.empty()) 
                {  
                    for (int j = 0; j < 68; j++) 
                    {  
                        // 所有的需要的特征点都存储在Shapes里
                        circle(frame, cvPoint(shapes[0].part(j).x(), shapes[0].part(j).y()), 1, cv::Scalar(246, 246, 94), -1); 
                        
                        if (isCatchedFlag)
                        {
                            macthed_name = argv[1];
                            macthed_name = "Matched: " + macthed_name;
                        }
                        cv::Point pt(20,50);
                        cv::putText(frame, macthed_name, pt, CV_FONT_HERSHEY_DUPLEX, 1, cv::Scalar(246, 246, 94));

                        //每个特征点的编号
                        //putText(frame, to_string(j), cvPoint(shapes[0].part(j).x(), shapes[0].part(j).y()), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 0, 0),1,4);
                        //  shapes[0].part(j).x();//68个  
                    } 
                }

                show_win.clear_overlay();
                show_win.set_title("Face Recognition");
                show_win.set_image(cimg);
                // 还有我们给这些人脸画了一个矩形框，这样我们就可以看到探测器在找他们
                show_win.add_overlay(show_faces[i]);    
            }

            if (!isCatchedFlag && !shapes.empty())
            {
                frame_pic_name = "tmp_frame.bmp";
                const char *frame_file_name = frame_pic_name.c_str();
                ret = access(frame_file_name, W_OK);
                if (ret != -1)
                {  
                    cout << "frame_pic_name can be written! :)" << endl;
                    imwrite(frame_pic_name, frame);     // 摄像头抓取的一帧图像存入到图片文件
                }
                else
                {   
                    cout << "frame_pic_name can not be written! :(" << endl;
                }

                pthread_t face_rec_thread;
                FACE_REC_ST face_rec_st;

                face_rec_st.frame_name = frame_pic_name;
                face_rec_st.p_name = argv[1];
                
                pthread_create(&face_rec_thread, NULL, face_rec_thread_func, (void *)&face_rec_st);
                pthread_detach(face_rec_thread);
            }

            saved_matched_img_name = argv[1];
            saved_matched_img_name = saved_matched_img_name + "-macthed.bmp";
            const char *img_file_name = saved_matched_img_name.c_str();
            ret = access(img_file_name, F_OK);  // 文件是否已经存在
            if(ret != -1 && !isCatchedFlag)
            {
                cout << "Bingo!" << endl;
                isCatchedFlag = 1;
                load_image(saved_macthed_img, saved_matched_img_name);
                show_macthed_win.set_title(saved_matched_img_name);
                show_macthed_win.set_image(saved_macthed_img); 
            }
            else
            {
                continue;
            }
        }
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }   

    return 0;
}

void Delay(int time)  //time*1000为秒数 
{ 
    clock_t now = clock(); 

    while(clock() - now < time); 
}

void *face_rec_thread_func(void* arg)
{
    Delay(10);

    int ret = 0;
    string tmp_frame_pic_name = "";
    string pic_name = "";

    FACE_REC_ST *face_rec_st = (FACE_REC_ST *)arg;

    tmp_frame_pic_name = face_rec_st->frame_name;
    pic_name =  face_rec_st->p_name;

    std::cout << "tmp_frame_pic_name: " << tmp_frame_pic_name << std::endl;
    std::cout << "pic_name: " << pic_name << std::endl;

    // 将截取视频流生成的图片转换成matrix对象
    matrix<rgb_pixel> cap_img;

    tmp_frame_pic_mutex.lock();     // 帧图像加互斥锁

    const char *tmp_frame_file_name = tmp_frame_pic_name.c_str();
    ret = access(tmp_frame_file_name, R_OK);    // 如果已经保存的帧图像不能读，则结束当前线程
    if (ret == -1)
    {  
        cout << "frame_pic_name can not be written! :(" << endl;
        tmp_frame_pic_mutex.unlock();   // 帧图像解互斥锁
        pthread_exit(NULL);
    }
    
    load_image(cap_img, tmp_frame_pic_name);

    // 定义一个人脸检测器，用来查找图片中的人脸
    frontal_face_detector detector = get_frontal_face_detector();

    // 我们也将使用人脸特征点模型将从图片中抓取的人脸对齐成一个标准的人脸
    shape_predictor sp;
    deserialize("../data/shape_predictor_68_face_landmarks.dat") >> sp;

    // 然后我们定义一个基于深度神经网络的人脸识别模型用来识别人脸.
    anet_type net;
    deserialize("../data/dlib_face_recognition_resnet_model_v1.dat") >> net;

    clock_t _detect_face_start, _detect_face_end;
    clock_t _DNN_net_start, _DNN_net_end;
    clock_t _separate_face_start, _separate_face_end;
    clock_t _cluster_start, _cluster_end;
    clock_t _chinese_whispers_start, _chinese_whispers_end;

    // 将待识别的人脸图片转换成matrix对象
    matrix<rgb_pixel> img;
    load_image(img, pic_name);

    _detect_face_start = clock(); // 记录起始时间

    // 在从视频流中抓取的一帧图像上运行人脸检测器，每个人脸提取一个副本，其中这个副本已被归一化到150x150像素大小, 并且具备适当的旋转中心。
    std::vector<matrix<rgb_pixel>> faces;

    for (auto face : detector(cap_img))
    {
        auto shape = sp(cap_img, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(cap_img, get_face_chip_details(shape,150,0.25), face_chip);
        faces.push_back(move(face_chip));
    }

    for (auto face : detector(img))
    {
        auto shape = sp(img, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
        faces.push_back(move(face_chip));
    }

    if (faces.size() == 0)
    {
        cout << "No faces found in image!" << endl;
        tmp_frame_pic_mutex.unlock();   // 帧图像解互斥锁
        pthread_exit(NULL);
    }

    _detect_face_end = clock();           // 记录结束时间
    {
        double seconds  =(double)(_detect_face_end - _detect_face_start)/CLOCKS_PER_SEC;
        fprintf(stderr, "Detect faces use time is: %.8f\n", seconds);
    }

    _DNN_net_start = clock();

    // 这个会调用深度神经网络来将人脸中的人脸图片转换成一个128D的向量
    // 在这个向量空间中，来自同一个人的图像彼此是相似的，但是来自不同的人的向量却是相距甚远.
    // 所以我们使用这些向量来识别这些图像的部分图像是来自同一个人，还是来自不同的人
    std::vector<matrix<float,0,1>> face_descriptors = net(faces);

    cout << "face_descriptors size = " << face_descriptors.size() << endl;

    _DNN_net_end = clock();
    {
        double seconds  =(double)(_DNN_net_end - _DNN_net_start)/CLOCKS_PER_SEC;
        fprintf(stderr, "DNN net use time is: %.8f\n", seconds);
    }

    _separate_face_start = clock();
    // 值得注意的是，我们可以做的一件简单的事情是做人脸聚类。
    // 下一步的代码创建一个图的连接面，然后使用中国耳语图聚类算法，以确定有多少人以及有哪些面孔属于谁
    std::vector<sample_pair> edges;
    for (size_t i = 0; i < face_descriptors.size(); ++i)
    {
        for (size_t j = i+1; j < face_descriptors.size(); ++j)
        {
            // 如果图中的面孔足够接近，那么在图上它们就是连接在一起的.
            // 然后我们检查这些面孔的描述子之间的距离是否小于0.6, 0.6是我们训练深度神经网络的确定阀值
            // 当然你可以使用其他任意你觉得有用的阀值
            if (length(face_descriptors[i] - face_descriptors[j]) < 0.6)
            {
                edges.push_back(sample_pair(i,j));
            }
        }
    }
    _separate_face_end = clock();
    {
        double seconds  =(double)(_separate_face_end - _separate_face_start)/CLOCKS_PER_SEC;
        fprintf(stderr, "Separate face use time is: %.8f\n", seconds);
    }

    _chinese_whispers_start = clock();
    std::vector<unsigned long> labels;
    const auto num_clusters = chinese_whispers(edges, labels);

    // 这个将正确地指明这张图片中有多少个人
    cout << "number of people found in the image: "<< num_clusters << endl;

    cout << "num_clusters = " << num_clusters << ", labels size = " << labels.size() << endl;

    _chinese_whispers_end = clock();
    {
        double seconds  =(double)(_chinese_whispers_end - _chinese_whispers_start)/CLOCKS_PER_SEC;
        fprintf(stderr, "Separate face use time is: %.8f\n", seconds);
    }

    if (num_clusters != 0)
    {
        _cluster_start = clock();
        // 现在我们将这些面孔聚类结果显示到屏幕上. 你将会看到它会正确地将所有属于同一个人的面孔图像组织到一起.
        //std::vector<image_window> win_clusters(num_clusters);
        for (size_t cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
        {
            std::vector<matrix<rgb_pixel>> cluster_temp;
            for (size_t j = 0; j < labels.size(); ++j)
            {
                if (cluster_id == labels[j])
                    cluster_temp.push_back(faces[j]);
            }
            //win_clusters[cluster_id].set_title("face cluster " + cast_to_string(cluster_id));
            //win_clusters[cluster_id].set_image(tile_images(cluster_temp));
            
            string save_img_name = pic_name;
            save_img_name = save_img_name + "-macthed.bmp";
            save_bmp(tile_images(cluster_temp), save_img_name);
            cout << "Saved match!" << endl;
            //load_image(macthed_img, save_img_name);
            //show_macthed_win.set_title(save_img_name);
            //show_macthed_win.set_image(macthed_img);
        }
        _cluster_end = clock();
        {
            double seconds  =(double)(_cluster_end - _cluster_start)/CLOCKS_PER_SEC;
            fprintf(stderr, "Cluster use time is: %.8f\n", seconds);
        }
    }
    else
    {
        printf("Does not recognize anyone :(\n");
    }

    tmp_frame_pic_mutex.unlock();   // 帧图像解互斥锁

    Delay(10);  // 让face_rec_thread线程有足够时间去解锁

    pthread_exit(NULL);
}
