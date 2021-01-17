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
    Mat img = argv[1]





 // Read the image file
 //Mat img = imread("MyImage.jpg");

 // Check for failure
 if (img.empty())
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }


/*---------Image Segmentation with Distance Transform and Watershed Algorithm --------*/

 //Note the image is img loaded correctly

Mat src=img;
//Mat src = imread("MyImage.jpg");


/*
Any grayscale image can be viewed as a topographic surface where high intensity denotes peaks and hills while low intensity denotes valleys. You start filling every isolated valleys (local minima) with different colored water (labels). As the water rises, depending on the peaks (gradients) nearby, water from different valleys, obviously with different colors will start to merge. To avoid that, you build barriers in the locations where water merges. You continue the work of filling water and building barriers until all the peaks are under water. Then the barriers you created gives you the segmentation result. This is the "philosophy" behind the watershed. You can visit the CMM webpage on watershed to understand it with the help of some animations.

But this approach gives you oversegmented result due to noise or any other irregularities in the image. So OpenCV implemented a marker-based watershed algorithm where you specify which are all valley points are to be merged and which are not. It is an interactive image segmentation. What we do is to give different labels for our object we know. Label the region which we are sure of being the foreground or object with one color (or intensity), label the region which we are sure of being background or non-object with another color and finally the region which we are not sure of anything, label it with 0. That is our marker. Then apply watershed algorithm. Then our marker will be updated with the labels we gave, and the boundaries of objects will have a value of -1.
*/
//src is our image
//show original
Mat imS, grey, dst;
string nameimage="Original";
namedWindow(nameimage, WINDOW_NORMAL);
resize(src, imS, Size(), 0.5,0.5) ;
imshow(nameimage, imS);

//cvt to grey
cvtColor(src, grey, COLOR_RGBA2GRAY, 0);

nameimage="grey";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, grey);

//treshhold
threshold(grey, grey, 0, 255, THRESH_BINARY | THRESH_OTSU);

nameimage="Otsu/Binary";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, grey);


// get background
Mat M=Mat::ones(3, 3, CV_8U);
cout<<M<<endl;

erode(grey, grey, M);

nameimage="eroded";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, grey);

Mat coinsBg,coinsFg,opening;

dilate(grey, opening, M);
dilate(opening, coinsBg, M, Point(-1, -1), 3);


nameimage="dilated";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, opening);


// distance transform
Mat distTrans;
distanceTransform(opening, distTrans, DIST_L2, 5);
normalize(distTrans, distTrans, 1, 0, NORM_INF);

// get foreground
threshold(distTrans, coinsFg, 0.7 * 1, 255, THRESH_BINARY);

nameimage="foreground";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, distTrans);


coinsFg.convertTo(coinsFg, CV_8U, 1, 0);
Mat unknown;
subtract(coinsBg, coinsFg, unknown);


nameimage="unknown";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, unknown);
// get connected components markers

Mat markers;
connectedComponents(coinsFg, markers);
for (int i = 0; i < markers.rows; i++) {
    for (int j = 0; j < markers.cols; j++) {
        markers.at<Vec3b>(i, j)[0] = markers.at<Vec3b>(i, j)[0] + 1;
        if (unknown.at<Vec3b>(i, j)[0] == 255) {
            markers.at<Vec3b>(i, j)[0] = 0;
        }
    }
}
cvtColor(src, src, COLOR_RGBA2RGB, 0);
watershed(src, markers);
Mat mark = Mat::zeros(markers.size(), CV_8UC1);
markers.convertTo(mark, CV_8UC1);
bitwise_not(mark, mark);
nameimage="ws";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, mark);

// draw barriers
for (int i = 0; i < markers.rows; i++) {
    for (int j = 0; j < markers.cols; j++) {
        if (markers.at<Vec3b>(i, j)[0] == -1) {
            src.at<Vec3b>(i, j)[0] = 255; // R
            src.at<Vec3b>(i, j)[1] = 0; // G
            src.at<Vec3b>(i, j)[2] = 0; // B
        }
    }
}

nameimage="canvasOutput";
namedWindow(nameimage, WINDOW_NORMAL);
imshow(nameimage, src);






//Construct an histogram, at first i thought of it as to use the color background but now it could be an add on----------------------------------
  vector<Mat> bgr_planes;//Blue,red,green HSV too complicated for me
  split( src, bgr_planes );
  int histSize = 256; //BGR has 0:255
  float range[] = { 0, 256 }; //the upper boundary is exclusive
  const float* histRange = { range };
  bool uniform = true, accumulate = false;

  //calcHist calculates the hisograms, this part was copied from https://docs.opencv.org/3.4/d8/dbc/tutorial_histogram_calculation.html, not that hard to understand
  Mat b_hist, g_hist, r_hist;
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  int hist_w = 512, hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );
  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  //create the file
  string name_file="histogram.txt";
  ofstream myfile;
  myfile.open (name_file);

  for( int i = 1; i < histSize; i++ )
  {
      myfile<<b_hist.at<float>(i)<<" "<<g_hist.at<float>(i)<<" "<<r_hist.at<float>(i)<<endl;
  }
  myfile.close();

//-----------------------------------------------------------------------------------------------------------------------------

 //Turn background black af manually, all the white to black.
 // for (int x=0; x<src.rows; x++)
 // {
 //   for (int y=0; y<src.cols; y++)
 //   {
 //     if ( (src.at<Vec3b>(x, y) == Vec3b(255,255,255)) || (src.at<Vec3b>(x, y) == Vec3b(255,255,255)) )
 //     {
 //         src.at<Vec3b>(x, y)[0] = 0; //src.at<> value of pixel in a 3 boolean vector
 //         src.at<Vec3b>(x, y)[1] = 0;//(0.0.0) est la couleur black af
 //         src.at<Vec3b>(x, y)[2] = 0;
 //     }
 //   }
 // }
 // // Show output image
 // nameimage="blackaf";
 // namedWindow(nameimage, WINDOW_NORMAL);
 // // Mat imS;
 // resize(src, imS, Size(), 0.5,0.5) ;
 // imshow(nameimage, imS);
// //create hsv-optional
// Mat hsv;
// Mat channels[3];
// cvtColor(src, hsv, COLOR_BGR2HSV);
// split(hsv, channels);
// Mat& ch = channels[2];
// nameimage="channels";
// namedWindow(nameimage, WINDOW_NORMAL);
// resize(ch, imS, Size(), 0.5,0.5) ;
// imshow(nameimage, imS);
//
// // Create a kernel that we will use for accuting/sharpening our image
// Mat kernel = (Mat_<float>(3,3) <<
//         1,  1, 1,
//         1, -8, 1,
//         1,  1, 1); // an approximation of second derivative, a quite strong kernel
// // do the laplacian filtering as it is
// //https://homepages.inf.ed.ac.uk/rbf/HIPR2/log.htm laplacian filters
// Mat imgLaplacian;
// Mat sharp = src; // copy source image to another temporary one
// filter2D(sharp, imgLaplacian, CV_32F, kernel);//CV_32F
// src.convertTo(sharp, CV_32F);
// Mat imgResult = sharp - imgLaplacian;
// // convert back to 8bits gray scale
// imgResult.convertTo(imgResult, CV_8UC3);
// imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
// // imshow( "Laplace Filtered Image", imgLaplacian );
// nameimage="LaplaceF";
// namedWindow(nameimage, WINDOW_NORMAL);
// resize(imgResult, imS, Size(), 0.5,0.5) ;
// imshow(nameimage, imS);
// // Create binary image from source image
// Mat bw;
// cvtColor(src, bw, COLOR_BGR2GRAY);
// threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
// nameimage="Treshholded";
// namedWindow(nameimage, WINDOW_NORMAL);
// resize(bw, imS, Size(), 0.5,0.5) ;
// imshow(nameimage, imS);
// imshow("treshholded", imS);
// // Perform the distance transform algorithm
// Mat dist;
// distanceTransform(bw, dist, DIST_L2, 3);
// // Normalize the distance image for range = {0.0, 1.0}
// // so we can visualize and threshold it
// normalize(dist, dist, 0, 1., NORM_MINMAX);
// nameimage="Distance";
// namedWindow(nameimage, WINDOW_NORMAL);
// resize(dist, imS, Size(), 0.5,0.5) ;
// imshow(nameimage, imS);
//
//
// // Threshold to obtain the peaks
// // This will be the markers for the foreground objects
// threshold(dist, dist, .4, 1., THRESH_BINARY);
// // Dilate a bit the dist image
// Mat kernel1 = Mat::ones(3, 3, CV_8UC1);//matrice de Dilatation, 3eme arg facultatif
// dilate(dist, dist, kernel1);
// nameimage="Peaks";
// namedWindow(nameimage, WINDOW_NORMAL);
// resize(dist, imS, Size(), 0.5,0.5) ;
// imshow(nameimage, imS);
//
//
// // Create the CV_8U version of the distance image
// // It is needed for findContours()
// Mat dist_8u;
// dist.convertTo(dist_8u, CV_8U);
// // Find total markers
// vector<vector<Point> > contours;
// findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
// // Create the marker image for the watershed algorithm
// Mat markers = Mat::zeros(dist_8u.size(), CV_32SC1); //un core dumped after execution 32F
// // Draw the foreground markers
// for (size_t i = 0; i < contours.size(); i++){
//     drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);}
// // Draw the background marker
// circle(markers, Point(5,5), 3, CV_RGB(255,255,255), -1);
// // namedWindow("Markers", WINDOW_NORMAL);
// // imshow("Markers", markers*10000); //BuG relative to format
//
// cout<<dist.type()<<" "<<src.type()<<endl;
//
//
// // Perform the watershed algorithm
// watershed(src, markers);
// Mat mark = Mat::zeros(markers.size(), CV_8UC1);
// markers.convertTo(mark, CV_8UC1);
// bitwise_not(mark, mark);
// //    imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
//                               // image looks like at that point
// // Generate random colors
// vector<Vec3b> colors;
// for (size_t i = 0; i < contours.size(); i++)
// {
//     int b = theRNG().uniform(0, 255);
//     int g = theRNG().uniform(0, 255);
//     int r = theRNG().uniform(0, 255);
//     colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
// }
// // Create the result image
// Mat dst = Mat::zeros(markers.size(), CV_8UC3);
// // Fill labeled objects with random colors
// for (int i = 0; i < markers.rows; i++)
// {
//     for (int j = 0; j < markers.cols; j++)
//     {
//         int index = markers.at<int>(i,j);
//         if (index > 0 && index <= static_cast<int>(contours.size()))
//             dst.at<Vec3b>(i,j) = colors[index-1];
//         else
//             dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
//     }
// }
// // Visualize the final image
// imshow("Final Result", dst);
//
//
//
//








 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(nameimage); //destroy the created window



//














// /* An intuitive alogo for image processing*/
// double min, max;
//     Point maxLoc;
//     //this algorithm had to be updated, the doc i used used opencv 2.8 i use opencv3.0
//     //Mat im = imread("04Bxy.jpg"); im==image
//     Mat hsv; //https://en.wikipedia.org/wiki/HSL_and_HSV
//     Mat channels[3]; //list of mat(image objects)
//     // bgr -> hsv
//     cvtColor(img, hsv, COLOR_BGR2HSV); //conversion to hsv
//     split(hsv, channels); //use chanel
//     // use v channel for processing
//     Mat& ch = channels[2];
//     /*
//     */
//
//     namedWindow("hsv", WINDOW_NORMAL);
//     imshow("hsv", img);
//
//     // apply Otsu thresholding (if we need to, we can implement the algo by ourselves, c'est la chose à faire mdr)
//     Mat bw;
//     threshold(ch, bw, 0, 255, THRESH_BINARY | THRESH_OTSU); //in bw do a binary threshholding otsu's methos
//     //black an white image
//     namedWindow("bW", WINDOW_NORMAL);
//     imshow("bW", bw);
//
//     // close small gaps //https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_morphological_ops/py_morphological_ops.html in python, but definitely will help
//     Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //elliptic filter, why elliptiC???
//     Mat morph;
//     morphologyEx(bw, morph, MORPH_CLOSE, kernel); //dilate and erode see below
//     namedWindow("morph", WINDOW_NORMAL);
//     imshow("morph", morph);
//     // take distance transform
//     Mat dist;
//     distanceTransform(morph, dist, DIST_L2, DIST_MASK_PRECISE);//Use the OpenCV function cv::distanceTransform in order to obtain the derived representation of a binary image, where the value of each pixel is replaced by its distance to the nearest background pixel, this will definitely be the the most difficult thing to code manually!!!!
//     //show
//     namedWindow("dist", WINDOW_NORMAL);
//     imshow("dist", dist);
//     // add a black border to distance transformed image. to get a good match for circles in the margin, we are adding a border
//     int borderSize = 75;
//     Mat distborder(dist.rows + 2*borderSize, dist.cols + 2*borderSize, dist.depth());
//     copyMakeBorder(dist, distborder,
//             borderSize, borderSize, borderSize, borderSize,
//             BORDER_CONSTANT | BORDER_ISOLATED, Scalar(0, 0, 0));
//     //  from the sizes of the circles in the image,
//     // a ~75 radius disk looks reasonable, so the borderSize was selected as 75,w hy though???
//     Mat distTempl;
//     Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(2*borderSize+1, 2*borderSize+1));
//     // erode the ~75 radius disk a bit
//     erode(kernel2, kernel2, kernel, Point(-1, -1), 10);
//     // take its distance transform. this is the template
//     distanceTransform(kernel2, distTempl, DIST_L2, DIST_MASK_PRECISE);
//     namedWindow("distTempl", WINDOW_NORMAL);
//     imshow("distTempl", distTempl);
//     // match template
//     Mat nxcor;
//     matchTemplate(distborder, distTempl, nxcor, TM_CCOEFF_NORMED);
//     minMaxLoc(nxcor, &min, &max);
//     // threshold the resulting image. we should be able to get peak regions.
//     // we'll locate the peak of each of these peak regions, what we'll do is take the peaks and they will be the image centers annd use the transform to add distance to the mix and then tada! circles
//     Mat peaks, peaks8u;
//     threshold(nxcor, peaks, max*.5, 255, THRESH_BINARY);
//     namedWindow("peaks", WINDOW_NORMAL);
//     imshow("peaks", peaks);
//     convertScaleAbs(peaks, peaks8u);
//     // find connected components. we'll use each component as a mask for distance transformed image,
//     // then extract the peak location and its strength. strength corresponds to the radius of the circle
//     vector<vector<Point>> contours; //point==pixel(class obhect)
//     vector<Vec4i> hierarchy;
//     findContours(peaks8u, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));
//     for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
//     {
//         // prepare the mask
//         peaks8u.setTo(Scalar(0, 0, 0));
//         drawContours(peaks8u, contours, idx, Scalar(255, 255, 255), -1);
//         // find the max value and its location in distance transformed image using mask
//         minMaxLoc(dist, NULL, &max, NULL, &maxLoc, peaks8u);
//         // draw the circles
//         circle(img, maxLoc, (int)max, Scalar(0, 0, 255), 2); //add a circle each time
//     }
//
// String windowNameCircleImage = "The result";
// namedWindow(windowNameCircleImage, WINDOW_NORMAL);
// imshow(windowNameCircleImage, img);
//
// waitKey(0); // Wait for any keystroke in the window
//
// destroyWindow(windowNameCircleImage); //destroy the created window
//
// /*------------end of intuitive algorithm*/
//
//




/*the following are pretested methods, in commentary not to bother you while building, use them directly a sort of toolbox

/*---------------------------------------*/
// Read the image file

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
