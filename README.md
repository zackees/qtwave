# qtwave

This is an implementation of a 1 and 2 dimensional differential wave. There is a QT app that allows you to run the wave simulation with different values.

![out_alt_alt](https://github.com/zackees/qtwave/assets/6856673/7c44c43e-080f-4d4d-8740-01d183ff346c)

The algorithm is seperate from the main UI app and can be copied and pasted into your own app.

https://hplgit.github.io/num-methods-for-PDEs/doc/pub/wave/sphinx/._main_wave001.html#formulating-a-recursive-algorithm

```C++
#ifndef WAVE_SIMULATION_H
#define WAVE_SIMULATION_H

#include <stdio.h>
#include <cmath>
#include <cstddef>

// One dimensional wave simulation.
template <size_t N>
class WaveSimulation1D {
public:
    WaveSimulation1D() = default;
    ~WaveSimulation1D() = default;

    void setSpeed(float something) {
        courantSq_ = something;
    }

    void setDampenening(float damp) {
        dampening = damp;
    }

    float getDampening() const {
        return dampening;
    }

    float getSpeed() const {
        return courantSq_;
    }

    float get(size_t x) const {
        if (x >= N) {
            printf("Out of range.\n");
            return 0.0f;
        }
        //return grid[y][x];
        return grid[whichGrid_][x+1];
    }

    bool has(size_t x) const {
        return x < N;
    }

    // value => {-1,1}
    void set(int x, float value) {
        if (x >= N) {
            printf("warning X value too high\n");
            return;
        }
        if (x < 0) {
            printf("warning X value is negative");
            return;
        }
        x = std::max<int>(0, std::min<int>(x, N-1));
        float *curr = grid[whichGrid_];
        curr[x+1] = value;
    }

    void update() {
        float *curr = grid[whichGrid_];
        float *next = grid[whichGrid_ ^= 1];  // also toggles whichGrid.
        // // Set the first derivative of the boundaries to zero:
        curr[0] = curr[1];
        curr[N + 1] = curr[N];
        // // Ensure the boundaries are zero:
        //curr[0] = 0;
        //curr[N + 1] = 0;
        const float dampening_factor = pow(2.0, dampening);
        for (size_t i = 1; i < N + 1; i++) {
            float f = -next[i] + 2.0f*curr[i] +
                      courantSq_*(curr[i + 1] - 2.0f*curr[i] + curr[i - 1]);
            f = f - (f / dampening_factor);
            f = std::max<float>(-1.0f, std::min<float>(1.0f, f));
            next[i] = f;
        }
    }


private:
    size_t whichGrid_ = 0;
    float grid[2][N + 2]= {{0.0f}, {0.0f}};  // Two extra for the boundary condition.
    float curr_grid[N+2] = {0.0f};
    float courantSq_ = 0.16f;
    float dampening = 6.0f;
};


// Two dimensional wave simulation.
template <size_t W, size_t H>
class WaveSimulation2D {
public:
    WaveSimulation2D() = default;
    ~WaveSimulation2D() = default;

    void setSpeed(float something) {
        courantSq_ = something;
    }

    void setDampenening(float damp) {
        dampening = damp;
    }

    float getDampening() const {
        return dampening;
    }

    float getSpeed() const {
        return courantSq_;
    }

    float get(size_t x, size_t y) const {
        if (x >= W || y >= H) {
            printf("Out of range.\n");
            return 0.0f;
        }
        return grid[whichGrid_][y+1][x+1];
    }

    bool has(size_t x, size_t y) const {
        if (x >= W || y >= H) {
            return false;
        }
        return true;
    }

    void set(size_t x, size_t y, float value) {
        if (x >= W || y >= H) {
            printf("Out of range.\n");
            return;
        }
        grid[whichGrid_][y+1][x+1] = value;
    }

    void update() {
        float (*curr)[W+2] = grid[whichGrid_];
        float (*next)[W+2] = grid[whichGrid_ ^= 1];  // also toggles whichGrid.

        for (size_t i = 0; i < H+2; i++) {
            curr[i][0] = curr[i][1];
            curr[i][W + 1] = curr[i][W];
        }
        for (size_t i = 0; i < W+2; i++) {
            curr[0][i] = curr[1][i];
            curr[H + 1][i] = curr[H][i];
        }

        const float dampening_factor = pow(2.0, dampening);
        for (size_t j = 1; j < H; j++) {
            for (size_t i = 1; i < W; i++) {
                float f = -next[j][i] + 2.0f*curr[j][i] +
                          courantSq_*(curr[j][i + 1] + curr[j][i - 1] + curr[j + 1][i] + curr[j - 1][i] - 4.0f*curr[j][i]);
                f = f - (f / dampening_factor);
                f = std::max<float>(-1.0f, std::min<float>(1.0f, f));
                next[j][i] = f;
            }
        }
    }


private:
    size_t whichGrid_ = 0;
    float grid[2][H+2][W+2] = {{{0.0f}}};  // Two extra for the boundary condition.
    float courantSq_ = 0.16f;
    float dampening = 6.0f;
};


#endif // WAVE_SIMULATION_H

```
