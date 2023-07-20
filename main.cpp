#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{

    VideoCapture cap("deneme_2.mp4");
    //VideoCapture cap(0);
    //int fps = cap.get(CAP_PROP_FPS);

    if (!cap.isOpened()) {
        cout << "No video stream detected..." << endl;
        system("pause");
        return -1;
    }

    
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
        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Display the resulting frame
        imshow("Frame", frame);

        if (frame_count < 30) {
            frame_paths.push_back("frame" + to_string(frame_count) + ".jpg");
            imwrite(frame_paths.back(), frame);
            cout << "Frame" << frame_count << "recorded." << endl;
            frame_count++;
        }


        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if (c == 27)
            break;


    }
    
    cap.release();

    Rect current_region = tracking_box;
    Rect next_region = current_region;
    next_region.x += 5;
    for (int i = 0; i < frame_paths.size(); i++) {
        
        myImage = imread(frame_paths[i]);
        

        Mat current_region_frame = myImage(tracking_box);
        cvtColor(current_region_frame, current_region_frame, COLOR_BGR2GRAY);
        Mat current_histogram;

        calcHist(&current_region_frame, 1, channels, Mat(), current_histogram, 1, &histogram_size, &histogram_range);
        normalize(current_histogram, current_histogram, 0, 255, NORM_MINMAX);


        Mat next_region_frame = myImage(next_region);
        cvtColor(next_region_frame, next_region_frame, COLOR_BGR2GRAY);
        Mat next_histogram;

        calcHist(&next_region_frame, 1, channels, Mat(), next_histogram, 1, &histogram_size, &histogram_range);
        normalize(next_histogram, next_histogram, 0, 255, NORM_MINMAX);       


        double similarity = compareHist(current_histogram, next_histogram, HISTCMP_BHATTACHARYYA);
        cout << similarity << endl;

        vector<string> frame_paths;
        if (similarity < 0.1) {
            //current_region = next_region;
            next_region.x += 5;

            rectangle(myImage, next_region, Scalar(0, 255, 0), 2);
            frame_paths.push_back("detected_frames" + to_string(i) + ".jpg");
            imwrite(frame_paths.back(), myImage);
        }
        else {
            rectangle(myImage, next_region, Scalar(0, 255, 0), 2);
            frame_paths.push_back("detected_frames" + to_string(i) + ".jpg");
            imwrite(frame_paths.back(), myImage);
        }
        
    }

    return 0;
}

