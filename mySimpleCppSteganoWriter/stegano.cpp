#define STB_IMAGE_IMPLEMENTATION

#include "error_util.h"
#include "steg_header.h"

#include <iostream>
#include <cstring>
#include <bitset>

#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define OPT_LIST "hedi:o:"
#define MAX_ARGS 5  //2 params + 1 non-optional arguments
#define MIN_ARGS 1

void imprint(Mat &input, char* msg, string& output_name);
void extract(Mat& image);

int main(int argc, char** argv)
{
    int opt, imprint_flag = 0, extract_flag = 0;
    string input;
    string output;
    char msg[BUFFER_SIZE];

    Mat in_img;

    //int index;    //if optional arguments are supported

    if (argc < MIN_ARGS + 1 || argc > MAX_ARGS + 1) {
        fprintf(stderr, "Usage: %s -d|-e -i input.png [-o output.png] [-h]\n",
            argv[0]);
        exit(EXIT_F);
    }
    output = DEFAULT_OUTPUT;

    while ((opt = getopt(argc, argv, OPT_LIST)) != -1)
    {
        switch(opt) {
            case 'h':   //help
                printf("Writes a steganographic message to a lossless image file (png).\n");
                exit(0);
                break;
            case 'e':
                imprint_flag = 1;
                break;
            case 'd':
                extract_flag = 1;
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-i input.png] [-o output.png] [-h]\n",
                    argv[0]);
                exit(EXIT_F);
        }
    }

    printf("Input: %s     Output: %s\n", input.c_str(), output.c_str());
    //if no optional arguments are supported
    if (argv[optind] != NULL) {
        fprintf(stderr, "Usage: %s [-i input.png] [-o output.png] [-h]\n",
            argv[0]);
        exit(EXIT_F);
    } else if (imprint_flag == 1 && extract_flag == 1) {
        fprintf(stderr, "%s can only imprint or extract at a time, not both.\n",
            argv[0]);
        exit(EXIT_F);
    }
    
    //init input image
    in_img = imread(input, 1);
    if (!in_img.data) {
        fprintf(stderr, "Error while getting input image data.\n");
        exit(EXIT_F);
    } else {
        cout << "Opened image: " << input << endl;
    }

    if (imprint_flag) {
        printf("Input message (up to 1024 characters):\n");
        cin.getline(msg, sizeof(msg));
        printf("Msg: %s\n", msg);

        if ((int)strlen(msg) > (in_img.rows * in_img.cols) / 4) {
            cout << "Warning: message length covers 25\% of the image size." << endl;

            if ((int)strlen(msg) >= (in_img.rows * in_img.cols)) {
                cout << "Error: message is too long, try finding a bigger image." << endl;
            }
        }
        imprint(in_img, msg, output);
    } else if (extract_flag) {
        extract(in_img);
    }

    return 0;
}

void imprint(Mat &input, char* msg, string& output_name)
{
    short int bitlen = 8;           //bitset contains 8 bits
    short int depth = 0;            //determines how many positions a mask is rotated for
    short int ctr = 0;              //used for indexing bits in an 8-bit bitset

    Mat output = input.clone();

    int counter = 0;
    vector<bitset<8>> bits;

    cout << endl;
    for (size_t i = 0; i < strlen(msg); i++)
    {
        bits.push_back(bitset<8>(msg[i]));
    }
    bits.push_back(bitset<8>(0));

    cout << "Potrebno je napraviti " << bits.size() * 8 << " supstitucije." << endl << endl;

    for (int i = 0; i < bitlen; i++) {
        for (int r = 0; r < input.rows; r++) {
            for (int c = 0; c < input.cols; c++) {
                Vec4b& in_bgra = input.at<Vec4b>(Point(r, c));
                Vec4b& out_bgra = output.at<Vec4b>(Point(r, c));

                for (int d = 0; d < 4; d++) {
                    //cout << "I:" << bitset<8>(in_bgra[d]);
                    out_bgra[d] = (in_bgra[d] & 0b11111110);   //clearing the last bit
                    //cout << "Prije: " << bitset<8>(out_bgra[d]) << " ";

                    uchar mask = bits[0][bitlen - (ctr + 1)] & (1 << depth);   //creating a mask
                    //cout << "- Maska: " << bits[0][bitlen - (ctr + 1)] << " ";

                    out_bgra[d] |= mask;                        //setting the last bit
                    //cout << "Poslije: " << bitset<8>(out_bgra[d]) << endl;
                    //cout << " - O:" << bitset<8>(out_bgra[d]) << endl;

                    if (++ctr > 7) {
                        ctr = 0;
                        bits.erase(bits.begin());
                        cout << endl;
                        if (bits.size() == 0) {
                            goto endenc;
                        }
                    }
                }
                output.at<Vec4b>(Point(r, c)) = out_bgra;
            }
        }
        depth++;
    }
endenc:
    cout << "Done" << endl;

    if (imwrite(output_name, output)) {
        cout << "Saved image to " << output_name << endl;
    } else {
        cout << "Failed to save image." << endl;
    }
}

void extract(Mat& image)
{
    short int bitlen = 8;
    short int ctr = 0;
    short int depth = 0;
    bool done = false;
    char res = 0;

    cout << "Initializing extraction..." << endl;

    for (int i = 0; i < bitlen; i++) {
        for (int r = 0; r < image.rows; r++) {
            for (int c = 0; c < image.cols; c++) {
                Vec4b& bgra = image.at<Vec4b>(Point(r, c));

                //cout << endl << "Usao ovdje." << endl;
                for (int d = 0; d < 4; d++) {
                    //cout << "Pr: " << bitset<8>(res) << " ";

                    uchar tempchar = bgra[d];   //uzmem vrijednost
                    //cout << "- U: " << bitset<8>(tempchar) << " ";

                    tempchar = tempchar & (1 << depth);
                    //cout << "- M: " << bitset<8>(tempchar)[0] << " ";

                    res |= tempchar;
                    //cout << "- N: " << bitset<8>(res) << endl;

                    //convert last bitset<8> into a character

                    if (res != '\0') {
                        done = false;
                    }
                    ctr++;
                    if (ctr > 7) {
                        if (done) {
                            goto enddec;
                        }
                        //cout << endl;
                        cout << res;
                        done = true;
                        ctr = 0;
                        res = 0;
                    }
                    res = res << 1;
                }
            }
        }
        depth++;
    }
enddec:
    cout << endl;
}
