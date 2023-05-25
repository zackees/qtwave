# qtwave

This is an implementation of a 1 dimensional differential wave. There is a QT app that allows you to run the wave simulation with different values.

![out_alt_alt](https://github.com/zackees/qtwave/assets/6856673/7c44c43e-080f-4d4d-8740-01d183ff346c)

The algorithm is seperate from the main UI app and can be copied and pasted into your own app. Note that it's called `grid` because in the future we want to make this a 2D simulation.

```C++
#ifndef GRID_HPP
#define GRID_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>

template <size_t N>
class Grid {
public:
    Grid() = default;
    ~Grid() = default;

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

    // value => {-1,1}
    void perturb(int x, int y, float value) {
        if (x >= N) {
            printf("warning X value too high\n");
        }
        if (x < 0) {
            printf("warning X value is negative");
        }
        if (y != 0) {
            printf("Warning y value is not == 0\n");
        }
        x = std::max<int>(0, std::min<int>(x, N-1));
        y = 0;
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
        curr[0] = 0;
        curr[N + 1] = 0;
        const float dampening_factor = pow(2.0, dampening);
        for (size_t i = 1; i < N + 1; i++) {
            float f = -next[i] + 2.0f*curr[i] +
                      courantSq_*(curr[i + 1] - 2.0f*curr[i] + curr[i - 1]);
            f = f - (f / dampening_factor);
            f = std::max<float>(-1.0f, std::min<float>(1.0f, f));
            next[i] = f;
        }
    }

    float getGridValue(size_t x, size_t y) const {
        if (x >= N || y >= 1) {
            throw std::out_of_range("Grid indices out of range");
        }
        //return grid[y][x];
        return grid[whichGrid_][x+1];
    }

private:
    size_t whichGrid_ = 0;
    float grid[2][N + 2]= {{0.0f}, {0.0f}};  // Two extra for the boundary condition.
    float curr_grid[N+2] = {0.0f};
    float courantSq_ = 0.16f;
    float dampening = 6.0f;
};

#endif // GRID_HPP
```