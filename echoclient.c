/*
gcc -g echoclient.c ../csapp.h ../csapp.c -lpthread -o client.out

gcc -g echoclient.c csapp.h csapp.c -lpthread -o client.out

-----------------
g++ -c -I /usr/local/include/opencv4 echoclient.c csapp.h csapp.c
g++ echoclient.o csapp.o  -o echoclient -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui  -L/usr/local/lib -lopencv_imgcodecs -L/usr/lib

*/

#include <stdlib.h>
#include <stdio.h>
#include "csapp.h"
#include "opencv2/opencv.hpp"

#define MAXBUFSIZE 15*1024
using namespace cv;
int main(int argc, char **argv)
{
    FILE *fr, *fw;
    int clientfd;
    char *host, buf[MAXBUFSIZE];
    // int port;
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    // port = atoi(argv[2]);
    fr = fopen("godzillajpg.jpg", "rb+");
    fread(buf, MAXBUFSIZE, 1, fr);
    //fprintf("length: %d\n", strlen(buf));
    clientfd = Open_clientfd(host, argv[2]);
    
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, MAXBUFSIZE);
    
    Rio_readnb(&rio, buf, MAXBUFSIZE);
    fw = fopen("godzillagrayjpgclient.jpg" , "wb+" );
    fwrite(buf , 1 , MAXBUFSIZE, fw);
    fclose(fw);

    fclose(fr);
    Close(clientfd);
    Mat image, gray_image;
    image = imread("godzillajpg.jpg" , 1);
    gray_image = imread("godzillagrayjpgclient.jpg" , 0);

    namedWindow("Original", WINDOW_AUTOSIZE );
    namedWindow("Gray Image", WINDOW_AUTOSIZE );

    imshow("Original", image );
    imshow("Gray image", gray_image );

    waitKey(0);
    destroyAllWindows();
    exit(0);

}

