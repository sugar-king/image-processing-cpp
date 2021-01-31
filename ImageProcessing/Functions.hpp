/*
 * Created on Thu Oct 22 2020
 *
 * Copyright (c) 2020 Peterson Yuhala, IIUN
 * Refactoring project
 */

#include <stdlib.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Image.h"
using namespace std;

/*
 *The following constant specifies the size of the kernel/mask. This value should be modified 
 *each time the kernel size needs to be changed
 */
const int SIZE = 3;
#include "Functions.h"

Image readImage(char fname[])
{
    int i, j;
    int N = 0, M = 0, Q = 0;
    unsigned char *charImage;
    char header[100], *ptr;
    bool p2 = false;

    ifstream ifs(fname, ios::in | ios::binary);

    stringstream ss;
    string inputLine = "";

    if (!ifs)
    {
        cerr << "Can't read image:" << fname << endl;
        exit(1);
    }

    getline(ifs, inputLine);  //read the first line
    if (inputLine.compare("P2") == 0)
    {
        p2 = true;
    }
    if (inputLine.compare("P5") != 0 && inputLine.compare("P2") != 0)
    {
        cerr << "Version error:" << endl;
        cerr << inputLine << inputLine.compare("P5") << ", "
             << "\n";
    }
    cerr << "Version : " << inputLine << endl;

    getline(ifs, inputLine);  // read the second line : comment
    cerr << "Comment : " << inputLine << endl;
    if (inputLine.find("#") < 0)
    {
        cerr << inputLine;
        ss << inputLine;
    }
    else
    {
        ss << ifs.rdbuf();  //read the third line : width and height
    }
    ss >> M >> N;
    cerr << M << " columns and " << N << " rows" << endl;

    ss >> Q;
    cerr << Q << endl;

    Image image(N, M, Q);

    if (p2)
    {
        image.p2 = true;
    }
    // Read and record the pixel values into the image object

    unsigned char pixel;
    int counter = 0;
    unsigned int pixel2;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            if (p2)
            {
                ss >> pixel2;
                pixel = (unsigned char)pixel2;
            }
            else
            {
                ss >> pixel;
            }
            if (counter < 10)
            {
                counter++;
            }
            image.pixelVal[i][j] = (unsigned char)pixel;
        }
    }

    ifs.close();

    return image;
}
//Logic Operations on images

Image logicAND(Image &im1, Image &im2)
{
    if (!im1.gray == im2.gray)
    {
        cerr << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    unsigned char pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im1.rows; i++)
    {
        for (int j = 0; j < im1.cols; j++)
        {
            pixel = newIm1.pixelVal[i][j] && newIm2.pixelVal[i][j];
            val = (pixel == 1) ? 255 : 0;  //NB: 255 && 255=1, 255 && 0=0
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

Image logicNAND(Image &im1, Image &im2)
{  //NAND=NOT(AND)
    if (!im1.gray == im2.gray)
    {
        cerr << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    unsigned char pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im1.rows; i++)
    {
        for (int j = 0; j < im1.cols; j++)
        {
            pixel = !(newIm1.pixelVal[i][j] && newIm2.pixelVal[i][j]);  // im1 NAND im2
            val = (pixel == 1) ? 255 : 0;                               //NB: 255 && 255=1, 255 && 0=0
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

Image logicOR(Image &im1, Image &im2)
{
    if (!im1.gray == im2.gray)
    {
        cerr << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im1.rows; i++)
    {
        for (int j = 0; j < im1.cols; j++)
        {
            pixel = newIm1.pixelVal[i][j] || newIm2.pixelVal[i][j];
            val = (pixel == 1) ? 255 : 0;  //NB: 255 && 255=1, 255 && 0=0
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

Image logicXOR(Image &im1, Image &im2)
{  // A XOR B=(A AND NOT B)||(NOT A AND B)
    if (!im1.gray == im2.gray)
    {
        cerr << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int pixel = 0, val = 0;
    Image newIm1 = im1.otsuBinarize();
    Image newIm2 = im2.otsuBinarize();
    Image newImage = Image(im1.rows, im1.cols, im1.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im1.rows; i++)
    {
        for (int j = 0; j < im1.cols; j++)
        {
            pixel = (newIm1.pixelVal[i][j] && !(newIm2.pixelVal[i][j])) || (newIm2.pixelVal[i][j] && !(newIm1.pixelVal[i][j]));
            val = (pixel == 1) ? 255 : 0;  //NB: 255 && 255=1, 255 && 0=0
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

//Mathematical Operations on images

Image addition(Image &im1, Image &im2)
{
    if (!im1.gray == im2.gray)
    {
        cerr << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int pixel = 0, val = 0;
    int Max = im1.gray;
    Image newImage = Image(im1.rows, im1.cols, im1.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im1.rows; i++)
    {
        for (int j = 0; j < im1.cols; j++)
        {
            pixel = im1.pixelVal[i][j] + im2.pixelVal[i][j];
            val = (int)min(pixel, Max);
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

Image subtraction(Image &im1, Image &im2)
{  //im1-im2
    if (!im1.gray == im2.gray)
    {
        cerr << "Images must have same maximum gray values" << endl;
        exit(1);
    }
    int pixel = 0, val = 0;
    Image newImage = Image(im1.rows, im1.cols, im1.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im1.rows; i++)
    {
        for (int j = 0; j < im1.cols; j++)
        {
            pixel = im1.pixelVal[i][j] - im2.pixelVal[i][j];
            val = (int)max(pixel, 0);
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

Image multiplication(Image &im, double ratio)
{
    double pixel = 0;
    double val = 0;
    int Max = im.gray;
    Image newImage = Image(im.rows, im.cols, im.gray);

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j] * ratio;
            val = min(pixel, Max);
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

//Image convolution algorithm

/**
 * All kernels used here are matrices
 *
 *
 * +-+-+-+-
 * |1|2|3|
 * +-+-+-+
 * |0|4|5|
 * +-+-+-+
 * |2|3|7|
 * +-+-+-+
 * 
 * @param im The image object passed on which mean filtering is performed.
 * @param kSize - The size of the mask is an odd integer like 3, 5, 7 … etc.
 * @param kernel- The mask itself
 */
Image convolution(Image &im, double kernel[SIZE][SIZE], int kSize, int norm)
{
    Image newImage = Image(im.rows, im.cols, im.gray);

    // find center position of kernel (half of kernel size)
    int kCenter = kSize / 2;
    //int mm = 0, nn = 0, ii = 0, jj = 0;
    double sum = 0;

//#pragma omp parallel for private(sum)
    for (int i = 0; i < im.rows; ++i)  // rows
    {
        for (int j = 0; j < im.cols; ++j)  // columns
        {
            for (int m = 0; m < kSize; ++m)  // kernel rows
            {
                int mm = kSize - 1 - m;

                for (int n = 0; n < kSize; ++n)  // kernel columns
                {
                    int nn = kSize - 1 - n;
                    // ignore input samples which are out of bound
                    int ii = i + (m - kCenter);
                    int jj = j + (n - kCenter);
                    if (ii < 0)
                        ii = ii + 1;
                    if (jj < 0)
                        jj = jj + 1;
                    if (ii >= im.rows)
                        ii = ii - 1;
                    if (jj >= im.cols)
                        jj = jj - 1;
                    if (ii >= 0 && ii < im.rows && jj >= 0 && jj < im.cols)
                        sum += im.pixelVal[ii][jj] * kernel[mm][nn];
                }
            }

            double val = sum / norm;

            val = val < 0 ? 0 : min(val, im.gray);
            newImage.pixelVal[i][j] = (int)val;
            sum = 0;
        }
    }

    return newImage;
}

Image convo2D(Image &im, double kernel[2][2], int kSize, int norm)
{
    Image newImage = Image(im.rows, im.cols, im.gray);

    // find center position of kernel (half of kernel size)
    int kCenter = kSize / 2;
    //int mm = 0, nn = 0, ii = 0, jj = 0;
    double sum = 0;

//#pragma omp parallel for private(sum)
    for (int i = 0; i < im.rows; ++i)  // rows
    {
        for (int j = 0; j < im.cols; ++j)  // columns
        {
            for (int m = 0; m < kSize; ++m)  // kernel rows
            {
                int mm = kSize - 1 - m;

                for (int n = 0; n < kSize; ++n)  // kernel columns
                {
                    int nn = kSize - 1 - n;
                    // ignore input samples which are out of bound
                    int ii = i + (m - kCenter);
                    int jj = j + (n - kCenter);
                    if (ii < 0)
                        ii = ii + 1;
                    if (jj < 0)
                        jj = jj + 1;
                    if (ii >= im.rows)
                        ii = ii - 1;
                    if (jj >= im.cols)
                        jj = jj - 1;
                    if (ii >= 0 && ii < im.rows && jj >= 0 && jj < im.cols)
                        sum += im.pixelVal[ii][jj] * kernel[mm][nn];
                }
            }

            double val = sum / norm;

            val = val < 0 ? 0 : min(val, im.gray);
            newImage.pixelVal[i][j] = (int)val;
            sum = 0;
        }
    }

    return newImage;
}

Image robertFilter(Image &im)
{
    double kern1[2][2] = {
        {-1, 0},
        {0, -1}

    };
    double kern2[2][2] = {
        {0, 1},
        {-1, 0}

    };

    Image temp = Image(convo2D(im, kern1, 2, 1));
    return (convo2D(temp, kern2, 2, 1));
}
Image edgeDetect(Image &im)
{
    double kern[3][3] = {
        {0, 1, 0},
        {1, -4, 1},
        {0, 1, 0}};
    return (convolution(im, kern, 3, 1));
}  //Detects the edges of an image

Image gaussFilter(Image &im)
{
    double gaussBlur[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}};
    return (convolution(im, gaussBlur, 3, 9));
}

Image prewittFilter(Image &im)
{
    double kern1[3][3] = {
        {-1, -1, -1},
        {0, 0, 0},
        {1, 1, 1}};
    double kern2[3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}};

    Image temp = Image(convolution(im, kern1, 3, 1));
    return (convolution(temp, kern2, 3, 1));
}

Image sobelFilter(Image &im)
{
    double kern1[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};
    double kern2[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    Image temp = Image(convolution(im, kern1, 3, 1));
    return (convolution(temp, kern2, 3, 1));
}

Image smoothingFilter(Image &im)
{
    double kern[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}};
    return (convolution(im, kern, 3, 9));
}

//Histogram and Contrast algorithms

void printHistogram(Image &im)
{
    int histogram[im.gray + 1] = {0};
    int sum = 0;
    //Calculate image histogram
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            int pixel = im.pixelVal[i][j];
            histogram[pixel]++;
        }
    }
    //Print histogram
    for (int i = 0; i < im.gray + 1; i++)
    {
        cerr << i << ": " << histogram[i] << endl;
        sum += histogram[i];
    }
    cerr << "Sum of pixels= " << sum << endl;
}

int calculateContrast(Image &im)
{
    //Calculates contrast using min-max variation

    double contrast = 0;
    int mini = minPixel(im);
    int maxi = maxPixel(im);
    int val = 0;
    contrast = (maxi - mini) / (maxi + mini);
    return contrast;
}

double luminance(Image &im)
{
    double lum = 0, sum = 0;
    int total = im.cols * im.rows, pixel = 0;
    ;
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            sum += im.pixelVal[i][j];
        }
    }
    lum = sum / total;
    return lum;
}

Image linearContrast(Image &im)
{  //Amelioration de contrast lineaire
    Image newImage = Image(im.rows, im.cols, im.gray);
    int mini = minPixel(im);
    int maxi = maxPixel(im), pixel = 0;
    int val = 0;

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j];
            val = im.gray * (pixel - mini) / (maxi - mini);
            newImage.pixelVal[i][j] = val;
        }
    }
    return newImage;
}

Image linearContrastSaturation(Image &im, double sMin, double sMax)
{
    //Amelioration de contrast par transformation lineaire avec saturation

    if ((sMin > sMax) || (sMin < minPixel(im)) || (sMax > maxPixel(im)))
    {
        //Test simples pour niveaux de saturation
        cerr << "Valeurs de saturation invalides" << endl;
        exit(1);
    }
    Image newImage = Image(im.rows, im.cols, im.gray);
    int val = 0, pixel = 0;

//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j];
            val = im.gray * (pixel - sMin) / (sMax - sMin);
            val = val < 0 ? 0 : val;
            val = val > im.gray ? im.gray : val;
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

Image histogramEquilization(Image &im)
{
    Image newImage = Image(im.rows, im.cols, im.gray);
    int pixel = 0, val = 0;
    //On utilise les doubles ici pour eviter la perte de l'information pendant la division!
    double histogram[im.gray + 1] = {0};
    double hn[im.gray + 1] = {0};
    double ci[im.gray + 1] = {0};
    double nbp = im.cols * im.rows;
    int sum = 0;
    //Calculate image histogram

//#pragma omp parallel for default(shared) private(pixel)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j];
//#pragma omp atomic
            histogram[pixel]++;
        }
    }
//Normalise the histogram
//#pragma omp parallel for default(shared)
    for (int i = 0; i < im.gray + 1; i++)
    {
        hn[i] = histogram[i] / nbp;
    }
//Densite de probabilité normalisé
//#pragma omp parallel for default(shared)
    for (int i = 0; i < im.gray + 1; i++)
    {
        for (int j = 0; j < i; j++)
        {
            ci[i] += hn[j];
        }
    }
//Transformation des niveau de gris de l'image
//#pragma omp parallel for default(shared) private(pixel, val)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j];
//#pragma omp critical
            val = ci[pixel] * im.gray;
            newImage.pixelVal[i][j] = val;
        }
    }

    return newImage;
}

//Derivation of images

Image laplacienConvo(Image &im)
{
    //Calculate the laplacian approximation using 2D convolution
    double kern[3][3] = {
        {1, 1, 1},
        {1, -8, 1},
        {1, 1, 1}};
    return (convolution(im, kern, 3, 1));
}

//Interpolation of images

Image sharpen(Image &im)
{
    double kern[3][3] = {
        {1.0 / 4, 1.0 / 2, 1.0 / 4},
        {1.0 / 2, 1, 1.0 / 2},
        {1.0 / 4, 1.0 / 2, 1.0 / 4}};
    return (convolution(im, kern, 3, 4));
}

Image scalingNN(Image &im, double scale)
{
    int pixels[im.cols * im.rows] = {0};
    //Put image pixels in 1D array
//#pragma omp parallel for default(shared)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixels[j * im.rows + i] = im.pixelVal[i][j];
        }
    }

    int destW = (int)im.cols * scale;
    int destH = (int)im.rows * scale;

    int temp[destH * destW] = {0};
    int px = 0, py = 0, pixel = 0;
    Image newImage = Image(destH, destW, im.gray);

//#pragma omp parallel for default(shared) private(py, px)
    for (int i = 0; i < destH; i++)
    {
        for (int j = 0; j < destW; j++)
        {
            py = (int)i * scale;
            px = (int)j * scale;

            temp[(j * destH) + i] = pixels[(int)(px * im.rows + py)];
        }
    }
    //Put temp pixels in final image
//#pragma omp parallel for default(shared)
    for (int i = 0; i < destH; i++)
    {
        for (int j = 0; j < destW; j++)
        {
            newImage.pixelVal[i][j] = temp[j * destH + i];
        }
    }

    return newImage;
}
//NB: 0<=scale<=1

Image mirror(Image &im)
{
    //Calculates the mirror of the input image
    Image newImage = Image(im.rows, im.cols, im.gray);
    int pixel = 0;

//#pragma omp parallel for default(shared) private(pixel)
    for (int i = 0; i < im.rows; i++)
    {
        for (int lx = 0, rx = im.cols - 1; lx < im.cols; lx++, rx--)
        {
            pixel = im.pixelVal[i][lx];
            newImage.pixelVal[i][rx] = pixel;
        }
    }

    return newImage;
}
//Morphological Operations

Image erosion(Image &im)
{
    Image binImage = Image(im.otsuBinarize());
    double kern[3][3] = {
        {1, -1, 1},
        {1, -1, 1},
        {1, -1, 1}};
    return (convolution(binImage, kern, 3, 3));
}

double max(double a, double b)
{
    return ((a >= b) ? a : b);
}

double min(double a, double b)
{
    return ((a <= b) ? a : b);
}

int maxPixel(Image &im)
{
    int maxi = 0, pixel = 0;
    int val = 0;

//#pragma omp parallel for private(pixel)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j];
            if (pixel > maxi)
//#pragma omp critical
                maxi = pixel;
        }
    }
    return maxi;
}

int minPixel(Image &im)
{
    int mini = im.gray, pixel = 0;
    int val = 0;
//#pragma omp parallel for private(pixel)
    for (int i = 0; i < im.rows; i++)
    {
        for (int j = 0; j < im.cols; j++)
        {
            pixel = im.pixelVal[i][j];
            if (pixel < mini)
//#pragma omp critical
                mini = pixel;
        }
    }
    return mini;
}
