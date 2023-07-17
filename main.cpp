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

    if (!cap.isOpened()) {
        cout << "No video stream detected..." << endl;
        system("pause");
        return -1;
    }

    while (true) {
        cap >> myImage;
        if (myImage.empty()) {
            break;
        }
        imshow("Camera", myImage);
        char c = (char)waitKey(25);
        if (c == 27) {
            break;
        }
    }
    cap.release();
    return 0;
}

