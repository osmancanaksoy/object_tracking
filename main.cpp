#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
    Mat myImage;
    namedWindow("Camera");
    VideoCapture cap(0);
    //int fps = cap.get(CAP_PROP_FPS);

    if (!cap.isOpened()) {
        cout << "No video stream detected..." << endl;
        system("pause");
        return -1;
    }

    int frame_count = 0;
    stringstream file_name;

    while (frame_count < 30) {
        cap >> myImage;
        if (myImage.empty()) {
            break;
        }

        file_name.str("");
        file_name << "frame_" << frame_count << ".jpg";

        // Frameleri resim dosyası olarak kaydet
        cv::imwrite(file_name.str(), myImage);

        frame_count++;

        imshow("Camera", myImage);
        imwrite("deneme.jpg", myImage);
        //cout << fps << endl;
        char c = (char)waitKey(25);
        if (c == 27) {
            break;
        }
    }
    
    cap.release();
    return 0;
}

