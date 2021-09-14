#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0 ; i < height ; i++){
        //each row
        for (int j = 0 ; j < width ; j++){
            //each item in each row
            float avg = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen);
            avg = avg/3 ;
            int out = round(avg) ;
            image[i][j].rgbtRed = out ;
            image[i][j].rgbtBlue = out ;
            image[i][j].rgbtGreen = out ;
        };
    };
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0 ; i < height ; i++){
        //each row
        for (int j = 0 ; j < width ; j++){
            int oRed = image[i][j].rgbtRed ;
            int oBlue = image[i][j].rgbtBlue ;
            int oGreen = image[i][j].rgbtGreen ;
            int newRed = round((.393 * oRed) + (.769 * oGreen) + (.189 * oBlue));
            int newGreen = round((.349 * oRed) + (.686 * oGreen) + (.168 * oBlue));
            int newBlue = round((.272 * oRed) + (.534 * oGreen) + (.131 * oBlue));
            int *arr[3] = {&newRed,&newGreen,&newBlue};
            for (int k = 0 ; k < 3 ; k++){
                if (*arr[k] > 255){
                    *arr[k] = 255;
                };
            };
            image[i][j].rgbtRed = newRed ;
            image[i][j].rgbtGreen =  newGreen ;
            image[i][j].rgbtBlue = newBlue ;

        };
    };
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0 ; i < height ; i++){
        //each row
        RGBTRIPLE newRow[width];
        for (int j = 0 ; j < width ; j++){
            newRow[j] = image[i][width-1-j] ;
        };
        for (int j = 0 ; j < width ; j++){
            image[i][j] = newRow[j] ;
        };

    };
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newPic[height][width];
    for (int i = 0 ; i < height ; i++){
        //each row
        for (int j = 0 ; j < width ; j++){
            //each item
            RGBTRIPLE near_arr[8];
            float near_pixels = 0;
            if (i == 0 && i == height-1 && j == 0 && j == width-1){
                return;
                //one pixel image
            } else if ((i == 0 || i == height-1 ) && (j == 0 || j == width-1)){
                //corner
                near_pixels = 3;
                if (i==0 && j==0){
                    //top left
                    near_arr[0] = image[i][j+1];
                    near_arr[1] = image[i+1][j+1];
                    near_arr[2] = image[i+1][j];
                };
                if (i==0 && j == width-1){
                    //top right
                    near_arr[0] = image[i][j-1];
                    near_arr[1] = image[i+1][j-1];
                    near_arr[2] = image[i+1][j];
                };
                if (i==width-1 && j==0){
                    //bottom left
                    near_arr[0] = image[i-1][j];
                    near_arr[1] = image[i-1][j+1];
                    near_arr[2] = image[i][j+1];
                };
                if (i==width-1 && j==width-1){
                    //bottom right
                    near_arr[0] = image[i-1][j];
                    near_arr[1] = image[i-1][j-1];
                    near_arr[2] = image[i][j-1];
                };
            } else if (i == 0 || i == height-1){
                //first and last row
                near_pixels = 5;
                if (i==0){
                    //first row
                    near_arr[0] = image[i][j-1];
                    near_arr[1] = image[i][j+1];
                    near_arr[2] = image[i+1][j-1];
                    near_arr[3] = image[i+1][j];
                    near_arr[4] = image[i+1][j+1];
                } else {
                    //last row
                    near_arr[0] = image[i][j-1];
                    near_arr[1] = image[i][j+1];
                    near_arr[2] = image[i-1][j-1];
                    near_arr[3] = image[i-1][j];
                    near_arr[4] = image[i-1][j+1];
                };
            } else if (j == 0 || j == width-1){
                //first and last column
                near_pixels = 5;
                if (j==0){
                    //first column
                    near_arr[0] = image[i-1][j];
                    near_arr[1] = image[i+1][j];
                    near_arr[2] = image[i+1][j+1];
                    near_arr[3] = image[i][j+1];
                    near_arr[4] = image[i-1][j+1];
                } else {
                    //last column
                    near_arr[0] = image[i-1][j];
                    near_arr[1] = image[i+1][j];
                    near_arr[2] = image[i-1][j-1];
                    near_arr[3] = image[i][j-1];
                    near_arr[4] = image[i+1][j-1];
                };
            } else {
                near_pixels = 8;
                //regular pixel
                near_arr[0] = image[i-1][j-1];
                near_arr[1] = image[i-1][j];
                near_arr[2] = image[i-1][j+1];
                near_arr[3] = image[i][j-1];
                near_arr[4] = image[i][j+1];
                near_arr[5] = image[i+1][j-1];
                near_arr[6] = image[i+1][j];
                near_arr[7] = image[i+1][j+1];
            };
            float redVal = image[i][j].rgbtRed ;
            float blueVal = image[i][j].rgbtBlue ;
            float greenVal = image[i][j].rgbtGreen ;
            for (int k = 0 ; k < near_pixels ; k++){
                //loop to get averages
                redVal += near_arr[k].rgbtRed;
                blueVal += near_arr[k].rgbtBlue;
                greenVal += near_arr[k].rgbtGreen;

            };
            redVal = redVal / (near_pixels + 1);
            blueVal = blueVal / (near_pixels + 1);
            greenVal = greenVal / (near_pixels + 1);
            int redValInt = round(redVal) ;
            int blueValInt = round(blueVal) ;
            int greenValInt = round(greenVal) ;
            newPic[i][j].rgbtRed = redValInt ;
            newPic[i][j].rgbtBlue = blueValInt ;
            newPic[i][j].rgbtGreen = greenValInt ;

        };
    };
    for (int i = 0 ; i < height ; i++){
        //each row
        for (int j = 0 ; j < width ; j++){
            //each item
            image[i][j] = newPic[i][j];
        }
    };
    return;
}
