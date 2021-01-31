# Image-Processing-cpp
> This project was created as part of the [Parallelism and Concurrency](https://www.fer.unizg.hr/en/course/bopsaa) course on [University of Zagreb, Faculty of Electrical Engineering and Computing](https://www.fer.unizg.hr/en). 
>
> Parallelization was applied to an existing project [Image-Processing-cpp](https://github.com/Yuhala/image-processing-cpp). For more details about the project itself, go to it's [GitHub repository](https://github.com/Yuhala/image-processing-cpp). Parallelization was done using [OpenMP API](https://www.openmp.org/). 
 


## Parallelization
Due to the simple implementation of the filters, all optimizations were applied to the `for` loops which were used throughout the program. 

Since most `for` loops were working on distinctive parts of the image, the image objects were shared between threads, while other variables were private. 

In case all threads needed to acces a variable which was changing, ```#pragma omp atomic``` or ```#pragma omp critical``` directives were used. 

## Testing

All testing was performed on a laptop with a AMD Ryzen 5 4600HS CPU with 6 cores. Tests were performed on the `images/big.pgm` image.

Due to some problems and incorrect reading of compressed version (P5) of `.pgm` files in the original project, it's suggested to use uncompressed files (P2) as input.

For semi-automatic testing a custom writen script `timing.sh` was used. It's supposed to be run with the wanted filter as the first argument followed by one or two `.pgm` files and additional arguments if needed. 
> E.g.  `./timing.sh not images/big.pgm`
> `./timing.sh and images/FER_Zagreb.pgm images/big.pgm`
> 

Results of testing can be found in the folder `results` with a file for each filter. Testing was performed by running the script `timing.sh` which runs the filter 10 times and measures the average time spent on image processing. All running times and the average are writen to the file, first for serial and then for the parallelized version of the program. 

Resulting images are saved to the `filtered` folder. Images with suffix `_0` were created using the serial, while those with suffix `_1` were creating running the parallelized version.


## Results
While the time of image processing was reduced, further optimizations would make that effect even more visible. Below is the table of some of the results. All results can be found in the `results` folder. All times are in milliseconds (ms). 

|Filter         | Original  | Parallelized  |
|:---:          |   :---:   |   :---:       |
|not            |0.0379     |0.0144         |
|otsuBinarize   |0.0723     |0.0429         |
|and            |0.180      |0.109          |
|smoothingFilter|0.452      |0.137          |
|edgeDetect     |0.481      |0.151          |

As visible, time of image processing was reduced by roughly the factor of 2 on "simpler" filters, and around the factor of 3 for more complex filters. 

Even though the image processing was optimized, file reading and writing takes a bigger part of total program running time. 

Profiling analysis results using [gprof](https://sourceware.org/binutils/docs/gprof/) are visible in `analysis.txt` and `analysis_serial.txt`.

>## Below are the contents of [README.md](https://github.com/Yuhala/image-processing-cpp/blob/master/README.md) from the original repository. 
---
## Prerequisites

- To follow this tutorial, you need to install the g++ compiler on your PC.
- All examples here are based on a Linux environment but can be easily adapted for Windows based environments.

### Compiling the code
- Clone this repo into your environment and `cd` into the `ImageProcessing` folder:
```
git clone https://github.com/Yuhala/image-processing-cpp.git && cd ImageProcessing

```
Compile the program:

```
sudo g++ main.cpp Image.cpp -o app

```
## How to use the program
- The program presents 26 image processing algorithms. Logic operations, mathematical operations, convolutions and filters, and calculating an image histogram, image luminance, and contrast enhancement algorithms.
- The program can take 3, 4, or 5 arguments, including the name of the binary (`arg 0`). 
- The sample images used are in the `images` folder and are all `.pgm` format.
- The image resulting from each operation is generated in the `output.pgm`
- Below is a list of some of the operations provided by the program and how you can test them on images. 
### Logic Operations (6)
- Image binarization based on Otsu's thresholding algorithm: `./app otsuBinarize images/lena.pgm`<br/>

![alt-lena](ImageProcessing/fig/lena.png) ![alt-lena-otsu](ImageProcessing/fig/otsubin.png)

- Logic NOT: `./app not images/lena.pgm`<br/>

![alt-lena](ImageProcessing/fig/lena.png) ![alt-not-lena](ImageProcessing/fig/notlena.png)

- Logic XOR: `./app images/lena.pgm xor images/aya.pgm`<br/>

![alt-lena](ImageProcessing/fig/lena.png) ![alt-aya](ImageProcessing/fig/aya.png) ![alt-lena-xor-aya](ImageProcessing/fig/lenaXORaya.png)



### Convolution and Filters (9)
- Gauss Filter: `./app gaussFilter images/aya.pgm`<br/>

![alt-lena](ImageProcessing/fig/aya.png) ![alt-not-lena](ImageProcessing/fig/ayaGauss.png)


- Laplacien Convolution: `./app laplacienConvo images/lena.pgm`<br/>

![alt-lena](ImageProcessing/fig/lena.png) ![alt-not-lena](ImageProcessing/fig/laplacienConvoLena.png)


## Documentation
- A full list of all the implemented algorithms can be found in page 2 of [documentation](image-processing-doc.pdf).
- Enter each operation in camel case when testing, just like in the examples above.

## Some useful linux commands for images
- Resize an image: `convert img.png -resize 300x300\> output.png` (Target size in this example is 300x300).
- Identify image resolution (width x height): `identify -format "%wx%h" image.png` (Install imagemagick linux package).

## Authors

* **Igor Aradski**

* igor.aradski@fer.hr  

&nbsp;    

* **Peterson Yuhala** 
* petersonyuhala@gmail.com
* Feel free to contact me for more info or to propose fixes.
* Disclaimer: I am not an image processing expert and this code is meant solely for educational purposes. 

## License

- This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details


