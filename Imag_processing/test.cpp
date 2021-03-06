#include <math.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;




//------------------------------------------------//
int main(int argc, char** argv)
{

  if (argc < 2)
  {
    cout << "Please, enter the name of your data file." << endl;
    exit(0);
  }
  Mat img = imread(argv[1]);
// Read the image file
//Mat img = imread("MyImage.jpg");


 // Read the image file
 //Mat img = imread("./TER_images/test_x100_0.07um_x100_00.png");  //test_x50_0.07um_x50_00.png //./TER_images/echantillon3/test_100x_0.03um_x100_00.png

 // Check for failure
 if (img.empty())
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }


/*---------Image Segmentation with Distance Transform and Watershed Algorithm --------*/

//see other



/* An intuitive alogo for image processing*/
double min, max;
    Point maxLoc;
    //this algorithm had to be updated, the doc i used used opencv 2.8 i use opencv3.0
    //Mat im = imread("04Bxy.jpg"); im==image
    Mat hsv; //https://en.wikipedia.org/wiki/HSL_and_HSV
    Mat channels[3]; //list of mat(image objects)
    // bgr -> hsv
    cvtColor(img, hsv, COLOR_BGR2HSV); //conversion to hsv
    split(hsv, channels); //use chanel
    // use v channel for processing
    Mat& ch = channels[2];
    /*
    */

    // namedWindow("hsv", WINDOW_NORMAL);                   1
    // imshow("hsv", ch);

    // apply Otsu thresholding (if we need to, we can implement the algo by ourselves, c'est la chose à faire mdr)
    Mat bw;
    threshold(ch, bw, 0, 255, THRESH_BINARY | THRESH_OTSU); //in bw do a binary threshholding otsu's methos
    //black an white image


    // namedWindow("bW", WINDOW_NORMAL);                     2
    // imshow("bW", bw);

    // close small gaps //https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_morphological_ops/py_morphological_ops.html in python, but definitely will help
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //elliptic filter
    Mat morph;
    morphologyEx(bw, morph, MORPH_CLOSE, kernel); //dilate and erode see below
    // namedWindow("morph", WINDOW_NORMAL);
    // imshow("morph", morph);                              3
    // imwrite("test2_morph.jpg", morph);

    // take distance transform
    Mat dist;
    distanceTransform(morph, dist, DIST_L2, DIST_MASK_PRECISE);//Use the OpenCV function cv::distanceTransform in order to obtain the derived representation of a binary image, where the value of each pixel is replaced by its distance to the nearest background pixel, this will definitely be the the most difficult thing to code manually if we choose to do so!!!!
    //show
    // namedWindow("dist", WINDOW_NORMAL);                      4
    // imshow("dist", dist);

    // add a black border to distance transformed image. to get a good match for circles in the margin, we are adding a border











    int borderSize = 70; //75-50 first saved example  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!












    Mat distborder(dist.rows + 2*borderSize, dist.cols + 2*borderSize, dist.depth());
    copyMakeBorder(dist, distborder,
            borderSize, borderSize, borderSize, borderSize,
            BORDER_CONSTANT | BORDER_ISOLATED, Scalar(0, 0, 0));
    //  from the sizes of the circles in the image,
    // a ~75 radius disk looks reasonable, so the borderSize was selected as 75 manually :-(
    Mat distTempl;
    Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(2*borderSize+1, 2*borderSize+1));
    // erode the ~75 radius disk a bit
    erode(kernel2, kernel2, kernel, Point(-1, -1), 10);
    // take its distance transform. this is the template
    distanceTransform(kernel2, distTempl, DIST_L2, DIST_MASK_PRECISE);
    // namedWindow("distTempl", WINDOW_NORMAL);                  5
    // imshow("distTempl", distTempl);
    // match template
    Mat nxcor;
    matchTemplate(distborder, distTempl, nxcor, TM_CCOEFF_NORMED);
    minMaxLoc(nxcor, &min, &max);
    // threshold the resulting image. we should be able to get peak regions.
    // we'll locate the peak of each of these peak regions, what we'll do is take the peaks and they will be the image centers annd use the transform to add distance to the mix and then tada! circles
    Mat peaks, peaks8u;
    threshold(nxcor, peaks, max*.5, 255, THRESH_BINARY);
    // namedWindow("peaks", WINDOW_NORMAL);
    // imshow("peaks", peaks);                                     6
    // imwrite("test2_peaks.jpg", peaks);



    convertScaleAbs(peaks, peaks8u);
    // find connected components. we'll use each component as a mask for distance transformed image,
    // then extract the peak location and its strength. strength corresponds to the radius of the circle
    vector<vector<Point>> contours; //point==pixel(class obhect)
    vector<Vec4i> hierarchy;
    findContours(peaks8u, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));


    //////////////--------------------------------------------------------------------------------------
    Mat src = imread("test_20x_0.17um_x20_00.png");

    Mat imS, grey, dst;
    String nameimage;

    //cvt to grey
    cvtColor(src, grey, COLOR_RGBA2GRAY, 0);

    nameimage="grey";
    // namedWindow(nameimage, WINDOW_NORMAL);                       7
    // imshow(nameimage, grey);

    //treshhold
    threshold(grey, grey, 0, 255, THRESH_BINARY | THRESH_OTSU);

    // get background
    Mat M=Mat::ones(3, 3, CV_8U);
    //cout<<M<<endl;

    erode(grey, grey, M);

    Mat imgBg,imgFg,opening;

    dilate(grey, opening, M);
    dilate(opening, imgBg, M, Point(-1, -1), 3);

    // distance transform
    Mat distTrans;
    distanceTransform(opening, distTrans, DIST_L2, 5);
    normalize(distTrans, distTrans, 1, 0, NORM_INF);

    // get foreground
    threshold(distTrans, imgFg, 0.7 * 1, 255, THRESH_BINARY);

    cvtColor(distTrans, distTrans, COLOR_GRAY2RGB);



    nameimage="foreground";
    // namedWindow(nameimage, WINDOW_NORMAL);                     8
    // imshow(nameimage, distTrans);
    ////////------------------------------------------------------------------------------------------------------------------

    //starting inquisition--------------------------------------------------------------------------------------------------
    ofstream myfile;
    myfile.open ("results.txt");
    myfile << "#information about the circles.\n";
    myfile << "#" <<" "<< "centre du cercle" << " " << "rayon" <<endl;


    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
    {
        // prepare the mask
        peaks8u.setTo(Scalar(0, 0, 0));
        drawContours(peaks8u, contours, idx, Scalar(255, 255, 0), -1);
        // find the max value and its location in distance transformed image using mask
        minMaxLoc(dist, NULL, &max, NULL, &maxLoc, peaks8u);
        // draw the circles

        circle(img, maxLoc, (int)max, Scalar(0, 255, 255), 5, 2); //add a circle each time FILLED
        myfile <<idx <<"   " << maxLoc.x <<"   "<<maxLoc.y <<"  "<< max <<endl;
    }


    myfile.close();
    String windowNameImage = "img";
    namedWindow(windowNameImage, WINDOW_NORMAL);
    imshow(windowNameImage, img);
    imwrite("test1111_result.jpg", img);

    double a,b;
    a=57.5;
    b=127.5;

    for (int x=0; x<img.rows; x++)
    {
      for (int y=0; y<img.cols; y++)
      {
        if ( img.at<Vec3b>(x, y) == Vec3b(128,128,0) )
        {
          continue;
        }
        else if( (img.at<Vec3b>(x, y)[0]<a) &&  (img.at<Vec3b>(x, y)[2]<a) && (img.at<Vec3b>(x, y)[2]<a) )  //gettng black to grey
        { //first two 127
          img.at<Vec3b>(x, y)[0] = 0; //src.at<> value of pixel in a 3 boolean vector
          img.at<Vec3b>(x, y)[1] = 0;//(0.0.0) est la couleur black af
          img.at<Vec3b>(x, y)[2] = 255;
        }
        else if((img.at<Vec3b>(x, y)[0]>a && img.at<Vec3b>(x, y)[0]<255) &&  (img.at<Vec3b>(x, y)[1]>a && img.at<Vec3b>(x, y)[1]<255) && (img.at<Vec3b>(x, y)[2]>a && img.at<Vec3b>(x, y)[2]<255) )
        {
          img.at<Vec3b>(x, y)[0] = 0; //src.at<> value of pixel in a 3 boolean vector
          img.at<Vec3b>(x, y)[1] = 255;//(0.0.0) est la couleur black af
          img.at<Vec3b>(x, y)[2] = 0;
        }
        else
        {
          // img.at<Vec3b>(x, y)[0] = 200; //src.at<> value of pixel in a 3 boolean vector
          // img.at<Vec3b>(x, y)[1] = 100;//(0.0.0) est la couleur black af
          // img.at<Vec3b>(x, y)[2] = 55;
          continue;
        }
      }
    }

    windowNameImage = "colour";
    namedWindow(windowNameImage, WINDOW_NORMAL);
    imshow(windowNameImage, img);
    imwrite("test11_result.jpg", img);



String windowNameCircleImage;




// for (int x=0; x<img.rows; x++)
// {
//   for (int y=0; y<img.cols; y++)
//   {
//     if ( img.at<Vec3b>(x, y) == Vec3b(255,255,0) )
//     {
//       continue;
//     }
//     else if(img.at<Vec3b>(x, y) == Vec3b(255,255,255)  )
//     {
//       img.at<Vec3b>(x, y)[0] = 0; //src.at<> value of pixel in a 3 boolean vector
//       img.at<Vec3b>(x, y)[1] = 0;//(0.0.0) est la couleur black af
//       img.at<Vec3b>(x, y)[2] = 255;
//     }
//     else if(img.at<Vec3b>(x, y) == Vec3b(0,0,0)  )
//     {
//       img.at<Vec3b>(x, y)[0] = 0; //src.at<> value of pixel in a 3 boolean vector
//       img.at<Vec3b>(x, y)[1] = 255;//(0.0.0) est la couleur black af
//       img.at<Vec3b>(x, y)[2] = 255;
//     }
//     else
//     {
//       img.at<Vec3b>(x, y)[0] = 200; //src.at<> value of pixel in a 3 boolean vector
//       img.at<Vec3b>(x, y)[1] = 100;//(0.0.0) est la couleur black af
//       img.at<Vec3b>(x, y)[2] = 55;
//     }
//   }
// }

// windowNameImage = "distarnsnew";
// namedWindow(windowNameImage, WINDOW_NORMAL);
// imshow(windowNameImage, distTrans);
// String windowNameImage = "Couloured";
// namedWindow(windowNameImage, WINDOW_NORMAL);                           9
// imshow(windowNameImage, img);
//imwrite("test2_result.jpg", img);
//------------Detect coulours, wow what a douzy hh

// Read the image file

// Check for failure
if (src.empty())
{
 cout << "Could not open or find the image" << endl;
 cin.get(); //wait for any key press
 return -1;
}


Mat hsvFull;
// this returns hue within the range 0..255
cvtColor(src, hsvFull, COLOR_BGR2HSV_FULL);
vector<uchar> colorsCount(256,0);
Scalar pixel;
uchar color;
int maxColor=0, minColor=INT16_MAX;
// Scan image to count colors
// for alternative/efficient way to scan images
for (int r = 0; r < hsvFull.rows; r++)
{
    for (int c = 0; c < hsvFull.cols; c++)
    {
        pixel = hsvFull.at<Vec3b>(Point(c, r));
        color = pixel(0);  //channel 0 de couleur
        colorsCount[color] +=1;
        if (color < minColor) minColor = color;
        if (color > maxColor) maxColor = color;
    }
}
int colorRange = maxColor - minColor;
int minCount, maxCount;
minCount = colorsCount[minColor];
maxCount = colorsCount[maxColor];
cout<<minCount<<" "<<maxCount<<" "<<colorRange<<endl;

Mat img2(src.rows, src.cols, src.type(), colorsCount.data());
//cout<<img2<<endl;

myfile.close();



waitKey(0); // Wait for any keystroke in the window

//destroyWindow(windowNameCircleImage); //destroy the created window

/*------------end of intuitive algorithm*/






/*the following are pretested methods, in commentary not to bother you while building, use them directly a sort of toolbox

/*---------------------------------------*/
// Read the image file
//
// Mat image = imread("test_100x_0.03um_x100_00.png");
//
// // Check for failure
// if (image.empty())
// {
//  cout << "Could not open or find the image" << endl;
//  cin.get(); //wait for any key press
//  return -1;
// }
//
//  /*
//  Make changes to the image as necessary
//  e.g.
//   1. Change brightness/contrast of the image
//   */
//
//       Mat imageContrastHigh2;
//       image.convertTo(imageContrastHigh2, -1, 2, 0); //increase the contrast by 2
//
//       Mat imageContrastHigh4;
//       image.convertTo(imageContrastHigh4, -1, 4, 0); //increase the contrast by 4
//
//       Mat imageContrastLow0_5;
//       image.convertTo(imageContrastLow0_5, -1, 0.5, 0); //decrease the contrast by 0.5
//
//       Mat imageContrastLow0_25;
//       image.convertTo(imageContrastLow0_25, -1, 0.25, 0); //decrease the contrast by 0.25
//
//
//       //Defining window names for above images
//       String windowNameOriginalImage = "Original Image";
//       String windowNameContrastHigh2 = "Contrast Increased by 2";
//       String windowNameContrastHigh4 = "Contrast Increased by 4";
//       String windowNameContrastLow0_5 = "Contrast Decreased by 0.5";
//       String windowNameContrastLow0_25 = "Contrast Decreased by 0.25";
//
//       //Create and open windows for above images
//       namedWindow(windowNameOriginalImage, WINDOW_NORMAL);
//       namedWindow(windowNameContrastHigh2, WINDOW_NORMAL);
//       namedWindow(windowNameContrastHigh4, WINDOW_NORMAL);
//       namedWindow(windowNameContrastLow0_5, WINDOW_NORMAL);
//       namedWindow(windowNameContrastLow0_25, WINDOW_NORMAL);
//
//       //Show above images inside the created windows.
//       imshow(windowNameOriginalImage, image);
//       imshow(windowNameContrastHigh2, imageContrastHigh2);
//       imshow(windowNameContrastHigh4, imageContrastHigh4);
//       imshow(windowNameContrastLow0_5, imageContrastLow0_5);
//       imshow(windowNameContrastLow0_25, imageContrastLow0_25);
//
//       waitKey(0); // Wait for any key stroke
//
//       destroyAllWindows(); //destroy all open windows
//   /*
//   2. Smooth/Blur image====erode more important in project
//   3. Crop the image
//   4. Rotate the image
//   //
//   5. Draw shapes on the image
//   6.Histogram equalization
//   */
//   //change the color image to grayscale image
//     cvtColor(image, image, COLOR_BGR2GRAY);
//
//     //equalize the histogram
//     Mat hist_equalized_image;
//     equalizeHist(image, hist_equalized_image);
//
//     //Define names of windows
//     String windowNameOfOriginalImage = "Original Image";
//     String windowNameOfHistogramEqualized = "Histogram Equalized Image";
//
//     // Create windows with the above names
//     namedWindow(windowNameOfOriginalImage, WINDOW_NORMAL);
//     namedWindow(windowNameOfHistogramEqualized, WINDOW_NORMAL);
//
//     // Show images inside created windows.
//     imshow(windowNameOfOriginalImage, image);
//     imshow(windowNameOfHistogramEqualized, hist_equalized_image);
//   /*
//   7.Erosion of images, better edges
//   /*
//   */
//
//     //Erode the image with 3x3 kernel
//     Mat image_eroded_with_3x3_kernel;
//     erode(image, image_eroded_with_3x3_kernel, getStructuringElement(MORPH_RECT, Size(3, 3)));
//
//     //Erode the image with 5x5 kernel
//     Mat image_eroded_with_5x5_kernel;
//     erode(image, image_eroded_with_5x5_kernel, getStructuringElement(MORPH_RECT, Size(5, 5)));
//
//     //Define names of the windows
//     String window_name = "Original";
//     String window_name_eroded_with_3x3_kernel = "Original eroded with 3 x 3 kernel";
//     String window_name_eroded_with_5x5_kernel = "Original eroded with 5 x 5 kernel";
//
//     // Create windows with above names
//     namedWindow(window_name);
//     namedWindow(window_name_eroded_with_3x3_kernel);
//     namedWindow(window_name_eroded_with_5x5_kernel);
//
//     // Show our images inside the created windows.
//     imshow(window_name, image);
//     imshow(window_name_eroded_with_3x3_kernel, image_eroded_with_3x3_kernel);
//     imshow(window_name_eroded_with_5x5_kernel, image_eroded_with_5x5_kernel);
//   /*
//   8.Dilatation of images
//   */
//   //dilate and display the dilated image
//           Mat img1;
//           dilate(image, img1, getStructuringElement(MORPH_RECT,Size(3,3)));
//           namedWindow("Dilated");
//           imshow("Dilated", img1);
//
//           waitKey(0);
//
//           //cleaning up
//           destroyWindow("MyWindow");
//           destroyWindow("Dilated");
//   /*
//  */
//
//  bool isSuccess = imwrite("MyImage.jpg", image); //write the image to a file as JPEG
//  //bool isSuccess = imwrite("D:/MyImage.png", image); //write the image to a file as PNG
//  if (isSuccess == false)
//  {
//   cout << "Failed to save the image" << endl;
//   cin.get(); //wait for a key press
//   return -1;
//  }
//
//  cout << "Image is succusfully saved to a file" << endl;
//
//
//  String windowName = "My image saved"; //Name of the window
//
//  namedWindow(windowName); // Create a window
//
//  imshow(windowName, image); // Show our image inside the created window.
//
//  waitKey(0); // Wait for any keystroke in the window
//
//  destroyWindow(windowName); //destroy the created window

 return 0;
}
