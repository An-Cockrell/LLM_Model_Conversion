#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <cmath>


double calculateDistance(int x1, int y1, int x2, int y2);
std::vector<std::pair<int, int>> generateCoordinates(int number_to_generate, int center_x, int center_y, int radius_from_center, int buffer_zone_between_points);

#endif