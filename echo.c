/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"

#define BUFSIZEMAX 15*1024

using namespace cv;

void echo(int connfd) 
{
    // printf("1\n");
    FILE *fw, *fr; 
    time_t seconds;
    time(&seconds);
    size_t n;
    char buf[BUFSIZEMAX]; 
    char clientConnFD[10], ts[10];
    
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    // printf("1.5\n");
    sprintf(clientConnFD, "%d", connfd);
    // printf("1.6\n");
    char *rcvImgName = strcat(clientConnFD, "_");
    
    rcvImgName = strcat(rcvImgName, "_");
    
    sprintf(ts, "%d", (int)seconds);
    // printf("ts: %s\n", ts);
    // convImg = strcat(rcvImgName, "ts");
    rcvImgName = strcat(rcvImgName, ts);
    
    rcvImgName = strcat(rcvImgName, ".jpg");
    // printf("2\n");
    n = Rio_readnb(&rio,buf,BUFSIZEMAX);
    // printf("3\n");
    // printf("server received %d bytes\n", (uint8_t)n);
    fw = fopen(rcvImgName, "wb+");
    fwrite(buf, 1, BUFSIZEMAX, fw);

    fclose(fw);

    Mat rcvImg;
    rcvImg = imread(rcvImgName, 1 );
    
    if(!rcvImg.data )
    {
        printf("No image data \n " );
        // return -1;
    }

    Mat gray_image;
    cvtColor(rcvImg, gray_image, COLOR_BGR2GRAY );

    imwrite(rcvImgName, gray_image );

    fr = fopen(rcvImgName, "rb+" );
    fread(buf, 1, BUFSIZEMAX, fr);
    // printf("4\n");
    fclose(fr);
    Rio_writen(connfd, buf, BUFSIZEMAX);

}


/* $end echo */
