#define FRAME_SX 200
#define FRAME_SY 200

uint32_t* resizeBuffer;

typedef struct rect_t
{
    int x;
    int y;
    int sizeX;
    int sizeY;
}
rect_t;

void DrawPixel(int x, int y, uint32_t color)
{

    if (x >= FRAME_SX - 1 || x < 0)
    {
        return;
    }

    if (y >= FRAME_SY - 1 || y < 0)
    {
        return;
    }

    resizeBuffer[y * FRAME_SX + x] = color;
}

void SetSmallerIntToOrigin(int& origin, int& end)
{
    if (origin > end)
    {
        int temp = origin;
        origin = end;
        end = temp;
    }
}

void DrawHorizontalLine(int xOrigin, int xEnd, int y, uint32_t color)
{
    //pour dessiner dans les 2 sens
    //SetSmallerIntToOrigin(xOrigin, xEnd);

    for (int i = xOrigin; i <= xEnd; i++)
    {
        DrawPixel(i, y, color);
    }
}

void DrawVerticalLine(int x, int yOrigin, int yEnd, uint32_t color)
{
    //pour dessiner dans les 2 sens
    //SetSmallerIntToOrigin(yOrigin, yEnd);

    for (int i = yOrigin; i <= yEnd; i++)
    {
        DrawPixel(x, i, color);
    }
}

void DrawOutlineRect(int xOrigin, int xEnd, int yOrigin, int yEnd, uint32_t color)
{
    DrawHorizontalLine(xOrigin, xEnd, yOrigin, color);
    DrawHorizontalLine(xOrigin, xEnd, yEnd, color);

    DrawVerticalLine(xOrigin, yOrigin, yEnd, color);
    DrawVerticalLine(xEnd, yOrigin, yEnd, color);
}

void DrawFullRect(int xOrigin, int xEnd, int yOrigin, int yEnd, uint32_t color)
{
    //pour dessiner dans les 2 sens
    //SetSmallerIntToOrigin(yOrigin, yEnd);

    for (int i = yOrigin; i <= yEnd; i++)
    {
        DrawHorizontalLine(xOrigin, xEnd, i, color);
    }
}

void DrawRect(rect_t rect, uint32_t color)
{
    //pour dessiner dans les 2 sens
    //SetSmallerIntToOrigin(yOrigin, yEnd);

    for (int i = rect.y; i <= rect.y + rect.sizeY; i++)
    {
        DrawHorizontalLine(rect.x, rect.x + rect.sizeX, i, color);
    }
}