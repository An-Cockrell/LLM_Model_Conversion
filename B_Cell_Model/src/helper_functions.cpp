#include "helper_functions.h"

// Function to calculate Euclidean distance between two points
double calculateDistance(int x1, int y1, int x2, int y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// Function to generate coordinates within a radius without buffer zone
std::vector<std::pair<int, int>> generateCoordinates(int number_to_generate, int center_x, int center_y, int radius_from_center, int buffer_zone_between_points) {
  std::size_t n = number_to_generate;
  int x = center_x;
  int y = center_y;
  int r = radius_from_center;
  int b = buffer_zone_between_points;
  
    std::vector<std::pair<int, int>> coordinates;

    // Generate random coordinates within the radius
    while (coordinates.size() < n) {
        int newX = std::rand() % (2 * r + 1) + (x - r);
        int newY = std::rand() % (2 * r + 1) + (y - r);
        
        bool isValid = true;
        if (calculateDistance(newX, newY, x, y) > r){isValid=false;}
        for (const auto& coord : coordinates) {
            // Check if the new coordinate is within the buffer zone
            if (!isValid){break;}
            if (calculateDistance(newX, newY, coord.first, coord.second) <= b) {
                isValid = false;
            }
        }

        if (isValid) {
            coordinates.emplace_back(newX, newY);
        }
    }

    return coordinates;
}

