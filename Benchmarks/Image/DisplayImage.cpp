#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat sigmoidContrast(Mat& img) {
    Mat result;
    img.convertTo(result, CV_32F);
    result = 1.5 * (result - 0.5);
    exp(result, result);
    result = 1 / (1 + result);
    result.convertTo(result, CV_8U, 255);
    return result;
}

Mat compressImage(Mat& img, int quality) {
    std::vector<uchar> buf;
    std::vector<int> params;
    params.push_back(IMWRITE_JPEG_QUALITY);
    params.push_back(quality);
    imencode(".jpg", img, buf, params);
    Mat decoded_img = imdecode(buf, IMREAD_UNCHANGED);
    return decoded_img;
}

int main() {
    int image_size = rand() % 385 + 128; 

    Mat img(image_size, image_size, CV_8UC1);
    randu(img, Scalar(0), Scalar(255));

    img.convertTo(img, CV_32F, 1.0 / 255.0);

    std::map<std::string, Mat> processed_images;

    resize(img, processed_images["resized"], Size(320, 320));

    GaussianBlur(img, processed_images["blurred"], Size(0, 0), 5, 0.5);

    processed_images["contrast_high"] = sigmoidContrast(img);

    resize(img, processed_images["resampled"], Size(256, 256));

    processed_images["compressed"] = compressImage(img, 10);

    for (const auto& pair : processed_images) {
        imshow(pair.first, pair.second);
    }
    waitKey(0);

    return 0;
}
