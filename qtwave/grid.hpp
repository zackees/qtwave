#ifndef GRID_HPP
#define GRID_HPP

#include <stdio.h>
#include <cmath>
#include <cstddef>

// Note that this is a 1-dimensional wave, but the api here is a 2D grid. This is because
// we intend to upgrade this to a 2D wave simulation.
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

    float get(size_t x, size_t y) const {
        if (x >= N || y >= 1) {
            printf("Out of range.\n");
            return 0.0f;
        }
        //return grid[y][x];
        return grid[whichGrid_][x+1];
    }

    // value => {-1,1}
    void set(int x, int y, float value) {
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


private:
    size_t whichGrid_ = 0;
    float grid[2][N + 2]= {{0.0f}, {0.0f}};  // Two extra for the boundary condition.
    float curr_grid[N+2] = {0.0f};
    float courantSq_ = 0.16f;
    float dampening = 6.0f;
};

#endif // GRID_HPP
