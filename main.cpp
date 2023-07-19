#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{

    VideoCapture cap(0);
    //int fps = cap.get(CAP_PROP_FPS);

    if (!cap.isOpened()) {
        cout << "No video stream detected..." << endl;
        system("pause");
        return -1;
    }

    
    namedWindow("Camera", WINDOW_AUTOSIZE);
    Mat myImage;
    
    cap.read(myImage);
    Rect tracking_box = selectROI("Select Object", myImage);

    destroyAllWindows();

    Mat selected_region_HSV;
    cvtColor(myImage(tracking_box), selected_region_HSV, COLOR_BGR2HSV);

    Mat selected_histogram;
    int histogram_size = 256;
    float range[] = { 0, 256 };
    const float* histogram_range = { range };
    int channels[] = { 0 };
    calcHist(&selected_region_HSV, 1, channels, Mat(), selected_histogram, 1, &histogram_size, &histogram_range);
    normalize(selected_histogram, selected_histogram, 0, 255, NORM_MINMAX);

    int frame_count = 0;
    bool recording = false;
    vector<string> frame_paths;

    while (true) {
        cap >> myImage;
        if (myImage.empty()) {
            break;
        }      
        
        imshow("Camera", myImage);

        int key = waitKey(1);

        if (key == 'a' || key == 'A') {
            recording = true;

            if (recording) {
                cout << "Recording started." << endl;
            }
            else {
                cout << "Recording end." << endl;
            }
        }

        if (recording && frame_count < 10) {
            frame_paths.push_back("frame" + to_string(frame_count) + ".jpg");
            imwrite(frame_paths.back(), myImage);
            cout << "Frame" << frame_count << "recorded." << endl;
            frame_count++;
        }

        if (frame_count >= 10)
            break;
    }
    
    cap.release();

    cv::namedWindow("Object Tracking");


    for (int i = 0; i < frame_paths.size(); i++) {
        Rect temp_rect = tracking_box;
        
        myImage = imread(frame_paths[i]);

        Mat current_frame_HSV;
        cvtColor(myImage, current_frame_HSV, COLOR_BGR2HSV);
        
        Mat current_region_HSV = current_frame_HSV(temp_rect);
        Mat current_histogram;
        calcHist(&current_region_HSV, 1, channels, Mat(), current_histogram, 1, &histogram_size, &histogram_range);
        normalize(current_histogram, current_histogram, 0, 255, NORM_MINMAX);

        double similarity = compareHist(selected_histogram, current_histogram, HISTCMP_BHATTACHARYYA);

        vector<string> frame_paths;
        if (similarity < 0.5) {
            temp_rect.x += 10;
            tracking_box = temp_rect;
            rectangle(myImage, tracking_box, Scalar(0, 255, 0), 2);
            frame_paths.push_back("detected_frames" + to_string(i) + ".jpg");
            imwrite(frame_paths.back(), myImage);
        }
    }

    return 0;
}

