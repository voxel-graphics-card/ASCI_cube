#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h> // For Sleep()

// --- Constants ---
constexpr int WIDTH = 80;
constexpr int HEIGHT = 22;
constexpr int AREA = WIDTH * HEIGHT;
constexpr char BACKGROUND_ASCII_CODE = ' ';

constexpr float CUBE_WIDTH = 10.0f;
constexpr float INCREMENT_SPEED = 0.9f; // Try decreasing this significantly, e.g., 0.1f or 0.05f

constexpr float K1 = 50.0f;
constexpr float DISTANCE_FROM_CAM = 100.0f;

constexpr float INCREMENT_ANGLE = 0.05f;
constexpr int SLEEP_TIME_MS = 5;

// --- Global Variables ---
char buffer[AREA];
float z_buffer[AREA];

float A = 0.0f, B = 0.0f, C = 0.0f;
float x, y, z;
float ooz;
int xp, yp;
int idx_buffer;

// --- Function Prototypes ---
float calculateX(float i, float j, float k);
float calculateY(float i, float j, float k);
float calculateZ(float i, float j, float k);
void calculateForSurface(float cube_x, float cube_y, float cube_z, char ch);

int main()
{
    std::cout << "\x1b[2J";

    while (true)
    {
        std::fill(buffer, buffer + AREA, BACKGROUND_ASCII_CODE);
        std::fill(z_buffer, z_buffer + AREA, 0.0f);

        // --- REVISED CUBE SURFACE DRAWING LOOP ---
        // Iterate through two dimensions to draw each face as a filled plane
        for (float val1 = -CUBE_WIDTH; val1 <= CUBE_WIDTH; val1 += INCREMENT_SPEED)
        {
            for (float val2 = -CUBE_WIDTH; val2 <= CUBE_WIDTH; val2 += INCREMENT_SPEED)
            {
                // Front face (Z = -CUBE_WIDTH)
                calculateForSurface(val1, val2, -CUBE_WIDTH, '@');
                // Back face (Z = CUBE_WIDTH)
                calculateForSurface(val1, val2, CUBE_WIDTH, '#');

                // Left face (X = -CUBE_WIDTH)
                // Note: For X-faces, val1 can be Y, val2 can be Z
                calculateForSurface(-CUBE_WIDTH, val1, val2, '$');
                // Right face (X = CUBE_WIDTH)
                calculateForSurface(CUBE_WIDTH, val1, val2, '~');

                // Top face (Y = CUBE_WIDTH)
                // Note: For Y-faces, val1 can be X, val2 can be Z
                calculateForSurface(val1, CUBE_WIDTH, val2, ';');
                // Bottom face (Y = -CUBE_WIDTH)
                calculateForSurface(val1, -CUBE_WIDTH, val2, '+');
            }
        }
        // --- END REVISED CUBE SURFACE DRAWING LOOP ---


        std::cout << "\x1b[H";
        for (int k = 0; k < AREA; ++k)
        {
            std::cout << (k % WIDTH ? buffer[k] : '\n');
        }

        A += INCREMENT_ANGLE;
        B += INCREMENT_ANGLE;
        // C += INCREMENT_ANGLE;

        Sleep(SLEEP_TIME_MS);
    }

    return 0;
}

// Function implementations (unchanged from previous version)
float calculateX(float i, float j, float k)
{
    return j * std::sin(A) * std::sin(B) * std::cos(C) - k * std::cos(A) * std::sin(B) * std::cos(C) +
           j * std::cos(A) * std::sin(C) + k * std::sin(A) * std::sin(C) + i * std::cos(B) * std::cos(C);
}

float calculateY(float i, float j, float k)
{
    return j * std::cos(A) * std::cos(C) + k * std::sin(A) * std::cos(C) -
           j * std::sin(A) * std::sin(B) * std::sin(C) + k * std::cos(A) * std::sin(B) * std::sin(C) -
           i * std::cos(B) * std::sin(C);
}

float calculateZ(float i, float j, float k)
{
    return k * std::cos(A) * std::cos(B) - j * std::sin(A) * std::cos(B) + i * std::sin(B);
}

void calculateForSurface(float cube_x, float cube_y, float cube_z, char ch)
{
    x = calculateX(cube_x, cube_y, cube_z);
    y = calculateY(cube_x, cube_y, cube_z);
    z = calculateZ(cube_x, cube_y, cube_z) + DISTANCE_FROM_CAM;

    ooz = 1.0f / z;

    xp = static_cast<int>(WIDTH / 2.0f + K1 * ooz * x * 2.0f);
    yp = static_cast<int>(HEIGHT / 2.0f + K1 * ooz * y);

    idx_buffer = xp + yp * WIDTH;

    if (idx_buffer >= 0 && idx_buffer < AREA)
    {
        if (ooz > z_buffer[idx_buffer])
        {
            z_buffer[idx_buffer] = ooz;
            buffer[idx_buffer] = ch;
        }
    }
}