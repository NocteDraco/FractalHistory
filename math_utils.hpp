#ifndef UTILS_HPP
#define UTILS_HPP

float eucl_dist_grid(int, int, int z = 0);
int mRandMove();

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}


#endif