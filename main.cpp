
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <random>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>              /* low-level i/o */
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "crypto/padder_factory.h"
#include "crypto/nonce_factory.h"
#include "crypto/block_cipher_factory.h"

namespace fs =  std::filesystem;


static int xioctl(int fh, int request, void *arg)
{
    int r;
    do {
        r = ioctl(fh, request, arg);
    } while (-1 == r && EINTR == errno);
    return r;
}

int allocCamera(const char* file)
{
    struct v4l2_capability cap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;

    int camera_fd = open(file, O_RDONLY);

    if (-1 == xioctl (camera_fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf (stderr, "%s is no V4L2 device\n", file);
            exit (EXIT_FAILURE);
        } else {
            printf("\nError in ioctl VIDIOC_QUERYCAP\n\n");
            exit(0);
        }
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf (stderr, "%s is no video capture device\n", file);
        exit (EXIT_FAILURE);
    }

    if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
        fprintf (stderr, "%s does not support read i/o\n", file);
        exit (EXIT_FAILURE);
    }

    memset(&fmt, 0, sizeof(fmt));
    fmt.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 320;
    fmt.fmt.pix.height      = 240;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    if (-1 == xioctl(camera_fd, VIDIOC_S_FMT, &fmt)) {
        printf("VIDIOC_S_FMT");
    }
    return camera_fd;
}

int main(int argc, char* argv[]) {
#ifdef __arm__
    std::cout << "ARM\n";
#endif

    uint64_t random_value = 0; //Declare value to store data into
    size_t size = sizeof(random_value); //Declare size of data
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if(urandom) //Check if stream is open
    {
        urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
        if(urandom) //Check if stream is ok, read succeeded
        {
            std::cout << std::hex << "Read random value: " << random_value << std::endl;
        }
        else //Read failed
        {
            std::cerr << "Failed to read from /dev/urandom" << std::endl;
        }
        urandom.close(); //close stream
    }
    else //Open failed
    {
        std::cerr << "Failed to open /dev/urandom" << std::endl;
    }

    std::random_device rd;
    std::cout << "entropy = " << rd.entropy() << '\n';

    allocCamera("/dev/video0");

    if(argc != 3) {
        std::cout << "Usage: ./ctr input_file output_file\n";
        return 1;
    }

    fs::path input_path{argv[1]};
    fs::path output_path{argv[2]};

    if(!fs::exists(input_path)) {
        std::cout << input_path << " \033[1;31mnot found in\033[0m " << fs::current_path() << "\n";
        return 1;
    }

    std::cout << input_path << " \033[1;0m" << fs::file_size(input_path) << "\033[0m bytes\n";

    std::ifstream input_file(input_path, std::ios::in | std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(input_file), {});

    //request default padder (PKSC7) from the compile time factory
    using padder_t = crypto::padder<>;
    //request default nonce (RDSEED) from the compile time factory
    //using nonce_t = crypto::nonce<>;
    //request an AES (default) counter (CTR) block_cipher from the compile time factory
    using cipher_t = crypto::block_cipher<crypto::CTR>;
    // 256 bit key
    using key_t = std::array<cipher_t::value_type, 32>;

    key_t key = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

    // space for padding
    std::vector<padder_t::value_type> padding(padder_t::block_size());
    // a padder
    padder_t pkcs7;
    // build padding
    auto n = pkcs7.pad(buffer.begin(), buffer.end(), padding.begin());
    // the padder is deliberately agnostic to the collection - so must manipulate directly
    buffer.insert(buffer.end(), padding.begin(), padding.begin() + n);

    //std::cout << "rdseed? " << crypto::can_rdseed() << "\n";

    //nonce_t n;
    //auto nonce_block = n();


    std::cout << "padding = " << n << "\n";
    std::cout << "buffer \033[1;0m" << buffer.size() << "\033[0m bytes\n";

    std::ofstream output_file(output_path, std::ios::out | std::ios::binary);
    output_file.write(buffer.data(), buffer.size());
    output_file.close();

    std::cout << output_path << " \033[1;0m" << fs::file_size(output_path) << "\033[0m bytes\n";

}