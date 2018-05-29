#define STB_IMAGE_IMPLEMENTATION

#include "error_util.h"
#include "steg_header.h"

#include <getopt.h>
#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

#define OPT_LIST "hedi:o:"
#define MAX_ARGS 4  //2 params + 1 non-optional arguments
#define MIN_ARGS 1

struct abst_image
{
    IplImage* img = 0;
    int width;
    int height;
    int step;
    int channels;
    uchar* data;
};

void init_images(struct abst_image* in_img, 
        struct abst_image* out_img, char* input);

int main(int argc, char** argv)
{
    int opt, encrypt = 0, decrypt = 0;
    char input[NAMESIZE] = "\0";
    char output[NAMESIZE] = "\0";
    char msg[BUFFER_SIZE] = "\0";

    struct abst_image in_img;
    struct abst_image out_img;

    //int index;    //if optional arguments are supported

    if (argc < MIN_ARGS + 1 || argc > MAX_ARGS + 1) {
        fprintf(stderr, "Usage: %s -i input.jpg [-o output.jpg] [-h]\n",
            argv[0]);
        exit(EXIT_F);
    }

    strlcpy(output, DEFAULT_OUTPUT, NAMESIZE);

    while ((opt = getopt(argc, argv, OPT_LIST)) != -1)
    {
        switch(opt) {
            case 'h':   //help
                printf("Writes a steganographic message to a jpeg file.\n");
                exit(0);
                break;
            case 'e':
                encrypt = 1;
                break;
            case 'd':
                decrypt = 1;
                break;
            case 'i':
                strlcpy(input, optarg, BUFFER_SIZE);
                break;
            case 'o':
                strlcpy(output, optarg, BUFFER_SIZE);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i input.jpg] [-o output.jpg] [-h]\n",
                    argv[0]);
                exit(EXIT_F);
        }
    }

    printf("Input: %s     Output: %s\n", input, output);
    //if no optional arguments are supported
    if (argv[optind] != NULL) {
        fprintf(stderr, "Usage: %s [-i input.jpg] [-o output.jpg] [-h]\n",
            argv[0]);
        exit(EXIT_F);
    } else if (encrypt == 1 && decrypt == 1) {
        fprintf(stderr, "%s can only encrypt or decrypt at a time, not both.\n",
            argv[0]);
        exit(EXIT_F);
    }


    printf("Input message:\n");
    fgets(msg, BUFFER_SIZE, stdin);
    //remove trailing newline from msg
    msg[strlen(msg) - 1] = '\0';
    printf("Msg: %s\n", msg);
    
    memset(&in_img, 0, sizeof(in_img));
    memset(&out_img, 0, sizeof(out_img));

    //init input image
    init_images(&in_img, &out_img, input);

    cvSaveImage(output, &out_img, 0);
    //imwrite(output, out_img);
    return 0;
}

void init_images(struct abst_image* in_img, 
        struct abst_image* out_img, char* input)
{
    in_img->img = cvLoadImage(input);
    if (!(in_img->img)) {
        fprintf(stderr, "Could not load the specified image file: %s\n", input);
        exit(EXIT_F);
    }
    in_img->width = in_img->img->width;
    in_img->height = in_img->img->height;
    in_img->step = in_img->img->widthStep;
    in_img->channels = in_img->img->nChannels;
    in_img->data = (uchar*)(in_img->img->imageData);

      printf("Processing a %dx%d image with %d channels\n",
        in_img->width, in_img->height, in_img->channels); 
    
    out_img->img = cvCreateImage(
        cvSize(in_img->width, in_img->height),
        in_img->img->depth, in_img->channels);
}

