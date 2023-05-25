#ifndef GRID_HPP
#define GRID_HPP

#include <assert.h>
#include <cmath>
#include <cstddef>
#include <stdexcept>

#define SOMETHING 0.5f

template <size_t N>
class Grid {
public:
    Grid() = default;
    ~Grid() = default;

    void setSpeed(float something) {
        courantSq_ = something;
    }

    void perturb(int x, int y, float something) {
        //float *curr = mesh[whichGrid_];  // "mesh" doesn't exist, we assume grid is meant
        assert(x < N);
        assert(y == 0);
        float *curr = grid[whichGrid_];
        curr[x * (1+y)] = something;
    }

    void update() {
        //float *curr = mesh[whichGrid_];  // "mesh" doesn't exist, we assume grid is meant
        float *curr = grid[whichGrid_];
        float *next = grid[whichGrid_ ^= 1];  // next/last mesh

        // // Set the first derivative of the boundaries to zero:
        // curr[0] = curr[1];
        // curr[N + 1] = curr[N];

        // // Ensure the boundaries are zero:
        // curr[0] = 0;
        // curr[N + 1] = 0;


        const float something = 0.5f;  // TOOD: Change this
        for (size_t i = 1; i < N + 1; i++) {
            float f = -next[i] + 2.0f*curr[i] +
                      courantSq_*(curr[i + 1] - 2.0f*curr[i] + curr[i - 1]);
            next[i] = f - (f / pow(2.0, something));
        }
    }

    float getGridValue(size_t x, size_t y) const {
        if (x >= N || y >= 1) {
            throw std::out_of_range("Grid indices out of range");
        }
        return grid[y][x];
    }

private:
    size_t whichGrid_ = 0;
    float grid[2][N + 2]{0.0f};
    float courantSq_ = SOMETHING;
};

#endif // GRID_HPP
