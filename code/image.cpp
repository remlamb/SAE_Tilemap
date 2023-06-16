//----Img
#pragma region Img
typedef struct img_t {
    int width, height, channels;
    unsigned char* img;
}img_t;
#pragma endregion Img

void DrawPixelAlpha(int x, int y, uint32_t color) {

    int pixel_index = y * FRAME_SX + x;
    uint32_t dest_color = resizeBuffer[pixel_index];


    if (x >= FRAME_SX - 1 || x < 0)
    {
        return;
    }

    if (y >= FRAME_SY - 1 || y < 0)
    {
        return;
    }

    float src_alpha = (uint8_t)(color >> 24) / 256.f;
    uint8_t src_r = (uint8_t)(color >> 16);
    uint8_t src_g = (uint8_t)(color >> 8);
    uint8_t src_b = (uint8_t)(color);

    uint8_t dest_r = (dest_color >> 16);
    uint8_t dest_g = (dest_color >> 8);
    uint8_t dest_b = (dest_color);


    uint8_t final_r = (uint8_t)(src_r * src_alpha + dest_r * (1.f - src_alpha));
    uint8_t final_g = (uint8_t)(src_g * src_alpha + dest_g * (1.f - src_alpha));
    uint8_t final_b = (uint8_t)(src_b * src_alpha + dest_b * (1.f - src_alpha));
    resizeBuffer[pixel_index] = MFB_ARGB(0xFF, final_r, final_g, final_b);
}

void LoadImage(img_t& img, const char* chemin) {

    img.img = stbi_load(chemin, &img.width, &img.height, &img.channels, 0);
    if (img.img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    else {
        printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", img.width, img.height, img.channels);
    }
}

void DrawImage(int posX, int posY, int& imgHeight, int& imgWidth, unsigned char* img) {
    int idx = 0;
    for (int m = posY; m < imgHeight + posY; m++)
    {
        for (int z = posX; z < imgWidth + posX; z++)
        {
            uint32_t pixelColor = (img[idx + 3] << 24 | img[idx] << 16 | img[idx + 1] << 8 | img[idx + 2]);
            DrawPixelAlpha(z, m, pixelColor);
            idx += 4;
        }
    }
}

void DrawImageAlpha(int posX, int posY, img_t img) {
    int idx = 0;
    for (int m = posY; m < img.height + posY; m++)
    {
        for (int z = posX; z < img.width + posX; z++)
        {
            uint32_t pixelColor = (img.img[idx + 3] << 24 | img.img[idx] << 16 | img.img[idx + 1] << 8 | img.img[idx + 2]);
            DrawPixelAlpha(z, m, pixelColor);
            idx += 4;
        }
    }
}

void DrawImageCentered(int posX, int posY, img_t img) {
    int idx = 0;
    for (int y = posY; y < img.height + posY; y++)
    {
        for (int x = posX; x < img.width + posX; x++)
        {
            uint32_t pixelColor = (img.img[idx + 3] << 24 | img.img[idx] << 16 | img.img[idx + 1] << 8 | img.img[idx + 2]);
            DrawPixelAlpha(x - (img.width / 2), y - (img.height / 2), pixelColor);
            idx += 4;
        }
    }
}