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
//#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>

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

// ----------------------------------------------------------------------------------------

std::vector<matrix<rgb_pixel>> jitter_image(
    const matrix<rgb_pixel>& img
);

// ----------------------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Run this example by invoking it like this: " << endl;
        cout << "   ./dnn_face_recognition_ex your_img.jpg" << endl;
        cout << endl;
        cout << "You will also need to get the face landmarking model file as well as " << endl;
        cout << "the face recognition model file.  Download and then decompress these files from: " << endl;
        cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << "http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2" << endl;
        cout << endl;
        return 1;
    }

    // The first thing we are going to do is load all our models.  First, since we need to
    // find faces in the image we will need a face detector:
    // 定义一个人脸检测器，用来查找图片中的人脸
    frontal_face_detector detector = get_frontal_face_detector();

    // 我们也将使用人脸特征点模型将从图片中抓取的人脸对齐成一个标准的人脸
    shape_predictor sp;
    deserialize("../data/shape_predictor_68_face_landmarks.dat") >> sp;

    // 然后我们定义一个基于深度神经网络的人脸识别模型用来识别人脸.
    anet_type net;
    deserialize("../data/dlib_face_recognition_resnet_model_v1.dat") >> net;

    // 在屏幕上显示原始的图片
    matrix<rgb_pixel> img;
    load_image(img, argv[1]);
    //image_window win(img); 

    //image_window show_win;

    // 在屏幕上显示匹配好的的图片
    matrix<rgb_pixel> macthed_img;
    image_window show_macthed_win;

    try
    {
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        unsigned int count = 5;
        unsigned int isCatchedFlag = 0;

        // Grab and process frames until the main window is closed by the user.
        //while(!win.is_closed())
        while(cv::waitKey(30) != 27)    // Press 'Esc' to quit the program
        //int i = 0;
        //for (i = 0; i < 3; i++)
        {
            // Grab a frame
            cv::Mat temp;
            cap >> temp;
            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            cv_image<bgr_pixel> cimg(temp);

            // Detect show_faces 
            std::vector<rectangle> show_faces = detector(cimg);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < show_faces.size(); ++i)
            {
                shapes.push_back(sp(cimg, show_faces[i]));
                //show_win.clear_overlay();
                //show_win.set_image(cimg);
                //show_win.add_overlay(show_faces[i]);
            }

            // Display it all on the screen - landmarks
            //show_win.clear_overlay();
            //show_win.set_image(cimg);
            //show_win.add_overlay(render_face_detections(shapes));

            // Add by kevin 2017.5.6
            if (!shapes.empty()) {  
                for (int i = 0; i < 68; i++) {  
                    // 所有的需要的特征点都存储在Shapes里
                    circle(temp, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 1, cv::Scalar(246, 246, 94), -1); 

                    //每个特征点的编号
                    //putText(temp, to_string(i), cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 0, 0),1,4);
                    //  shapes[0].part(i).x();//68个  
                }  
            }  
            ///Display it all on the screen - keypoints
            imshow("Dlib特征点", temp);
            //-----------------------------------------------------------------------------------------

            if (!isCatchedFlag)
            {
                // 在从视频流中抓取的一帧图像上运行人脸检测器，每个人脸提取一个副本，其中这个副本已被归一化到150x150像素大小, 并且具备适当的旋转中心。
                std::vector<matrix<rgb_pixel>> faces;

                for (auto face : detector(cimg))
                {
                    auto shape = sp(cimg, face);
                    matrix<rgb_pixel> face_chip;
                    extract_image_chip(cimg, get_face_chip_details(shape,150,0.25), face_chip);
                    faces.push_back(move(face_chip));

                    // Also put some boxes on the faces so we can see that the detector is finding
                    // them.
                    // 还有我们给这些人脸画了一个矩形框，这样我们就可以看到探测器在找他们
                    //win.add_overlay(face);
                }

                for (auto face : detector(img))
                {
                    auto shape = sp(img, face);
                    matrix<rgb_pixel> face_chip;
                    extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
                    faces.push_back(move(face_chip));

                    // Also put some boxes on the faces so we can see that the detector is finding
                    // them.
                    // 还有我们给这些人脸画了一个矩形框，这样我们就可以看到探测器在找他们
                    //win.add_overlay(face);
                }

                if (faces.size() == 0)
                {
                    cout << "No faces found in image!" << endl;
                    return 1;
                }

                // 这个会调用深度神经网络来将人脸中的人脸图片转换成一个128D的向量
                // 在这个向量空间中，来自同一个人的图像彼此是相似的，但是来自不同的人的向量却是相距甚远.
                // 所以我们使用这些向量来识别这些图像的部分图像是来自同一个人，还是来自不同的人
                std::vector<matrix<float,0,1>> face_descriptors = net(faces);

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
                            edges.push_back(sample_pair(i,j));
                    }
                }
                std::vector<unsigned long> labels;
                const auto num_clusters = chinese_whispers(edges, labels);

                // 这个将正确地指明这张图片中有多少个人
                cout << "number of people found in the image: "<< num_clusters << endl;

                if (num_clusters > 0)   // 至少识别到了一张人脸
                {
                    isCatchedFlag = 1;
                }

                // 现在我们将这些面孔聚类结果显示到屏幕上. 你将会看到它会正确地将所有属于同一个人的面孔图像组织到一起.
                //std::vector<image_window> win_clusters(num_clusters);
                for (size_t cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
                {
                    std::vector<matrix<rgb_pixel>> temp;
                    for (size_t j = 0; j < labels.size(); ++j)
                    {
                        if (cluster_id == labels[j])
                            temp.push_back(faces[j]);
                    }
                    //win_clusters[cluster_id].set_title("face cluster " + cast_to_string(cluster_id));
                    //win_clusters[cluster_id].set_image(tile_images(temp));
                    
                    save_bmp(tile_images(temp), "kevin_macthed.bmp");
                    load_image(macthed_img, "kevin_macthed.bmp");
                    show_macthed_win.set_title("kevin_macthed");
                    show_macthed_win.set_image(macthed_img);
                }

                // 最后，我们来将所有的面孔图像描述子打印到屏幕上
                //cout << "face descriptor for one face: " << trans(face_descriptors[0]) << endl;

                // 当然这里需要注意的是，在创建人脸描述子时是否用到抖动, 是可以用来提高人脸识别精度.
                // 特别是，你需要使用jitter_image()常规计算描述符子, 才能在LFW benchmark中得到99.38%识别精度
                //matrix<float,0,1> face_descriptor = mean(mat(net(jitter_image(faces[0]))));
                //cout << "jittered face descriptor for one face: " << trans(face_descriptor) << endl;

                // 如果你没有在运用模型中使用抖动， 比如我们在区分光头佬的时候
                // 它只能在LFW benchmark中得到99.13%的识别精度. 所以抖动可以使整个识别过程更加精确，
                // 但是同时在进行计算人脸描述子时会更慢一些.
                //cout << "hit enter to terminate" << endl;
                //cin.get();
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
}

