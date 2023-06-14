#define COLOR_WHITE 0xFFFFFFFF

void DrawLetter(unsigned char* img, int posX, int posY, int letterPosX, int letterPosY, int& imgWidth) {
    for (int y = letterPosY * 12; y < letterPosY * 12 + 9; y++) {
        for (int x = letterPosX * 7; x < letterPosX * 7 + 7; x++) {
            int idx = (y * imgWidth + x) * 4;
            uint32_t pixelColor = MFB_ARGB(img[idx + 3], img[idx], img[idx + 1], img[idx + 2]);
            DrawPixel(x + posX, y + posY, pixelColor);
        }
    }
    //x .7 - y .9;   
}

void DrawLetterCropped(unsigned char* img, int posX, int posY, int letterPosX, int letterPosY, int& imgWidth) {
    for (int y = letterPosY * 12; y < letterPosY * 12 + 9; y++) {
        for (int x = letterPosX * 7; x < letterPosX * 7 + 7; x++) {
            int idx = (y * imgWidth + x) * 4;
            uint32_t pixelColor = MFB_ARGB(img[idx + 3], img[idx], img[idx + 1], img[idx + 2]);
            DrawPixelAlpha(x + (posX - letterPosX * 7), y + (posY - letterPosY * 12), pixelColor);
        }
    }
    //x .7 - y .9;   
}

void DrawLetterCropped(unsigned char* img, int posX, int posY, int letterPosX, int letterPosY, int& imgWidth, uint32_t color) {
    for (int y = letterPosY * 12; y < letterPosY * 12 + 9; y++) {
        for (int x = letterPosX * 7; x < letterPosX * 7 + 7; x++) {
            int idx = (y * imgWidth + x) * 4;
            uint32_t pixelColor = MFB_ARGB(img[idx + 3], img[idx], img[idx + 1], img[idx + 2]);
            if (pixelColor == COLOR_WHITE) {
                pixelColor = color;
            }
            DrawPixelAlpha(x + (posX - letterPosX * 7), y + (posY - letterPosY * 12), pixelColor);
        }
    }
}

void DrawText(img_t font, std::string textToDraw, int textPosX, int textPosY) {
    int startposX = textPosX;
    for (int i = 0; textToDraw[i] != 0; i++) {
        char character = textToDraw[i];
        if (character >= 64 && character <= 90) {
            DrawLetterCropped(font.img, textPosX, textPosY, character - 65, 0, font.width);
            //espace entre les lettres
            textPosX += 5;
        }
        if (character >= 97 && character <= 122) {
            DrawLetterCropped(font.img, textPosX, textPosY, character - 97, 1, font.width);
            textPosX += 5;
        }
        if (character >= 33 && character <= 63) {
            DrawLetterCropped(font.img, textPosX, textPosY, character - 33, 2, font.width);
            textPosX += 5;
        }
        if (character == 32) {
            textPosX += 5;
        }

        if (textPosX >= font.width - 20) {
            textPosX = startposX;
            textPosY += 10;
        }
    }
}

void DrawText(img_t font, std::string textToDraw, int textPosX, int textPosY, uint32_t color) {
    int startposX = textPosX;
    for (int i = 0; textToDraw[i] != 0; i++) {
        char character = textToDraw[i];
        if (character >= 64 && character <= 90) {
            DrawLetterCropped(font.img, textPosX, textPosY, character - 65, 0, font.width, color);
            //espace entre les lettres
            textPosX += 5;
        }
        if (character >= 97 && character <= 122) {
            DrawLetterCropped(font.img, textPosX, textPosY, character - 97, 1, font.width, color);
            textPosX += 5;
        }
        if (character >= 33 && character <= 63) {
            DrawLetterCropped(font.img, textPosX, textPosY, character - 33, 2, font.width, color);
            textPosX += 5;
        }
        if (character == 32) {
            textPosX += 5;
        }

        if (textPosX >= font.width - 20) {
            textPosX = startposX;
            textPosY += 10;
        }
    }
}

void DrawTextCentered(img_t font, std::string textToDraw, int textPosX, int textPosY) {
    DrawText(font, textToDraw, textPosX + FRAME_SX / 2 - textToDraw.size() * 4, textPosY + FRAME_SY / 2);
}

void DrawTextCentered(img_t font, std::string textToDraw, int textPosX, int textPosY, uint32_t color) {
    DrawText(font, textToDraw, textPosX + FRAME_SX / 2 - textToDraw.size() * 4, textPosY + FRAME_SY / 2, color);
}