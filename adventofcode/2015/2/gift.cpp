#include "gift.h"
#include <sstream>
#include <algorithm>

Gift::Gift(const std::string& text_dim): entry(text_dim) {
    // fragment string text entry into the integral side entries
    std::stringstream stream(entry);
    std::string segment;
    for (int i = 0; std::getline(stream, segment, 'x'); i++) {
        sides[i] = std::stoi(segment);
    }

    // areas
    areas[0] = sides[0]*sides[1];
    areas[1] = sides[0]*sides[2];
    areas[2] = sides[1]*sides[2];

    // perimeters
    perimeters[0] = 2*sides[0]+2*sides[1];
    perimeters[1] = 2*sides[0]+2*sides[2];
    perimeters[2] = 2*sides[1]+2*sides[2];

    // get gift smallest dimensions
    get_gift_smallest_dimensions();
}


/*
 * arrays decay to pointer when passed to a function by reference
 */
static int array_min_value_index(const std::array<int, 3>& arr) {
    return std::distance(arr.begin(), std::min_element(arr.begin(), arr.end()));
}

void Gift::get_gift_smallest_dimensions() {
    // fill array with areas
    smallest_area = areas[array_min_value_index(areas)];

    // get perimeter measurements
    smallest_perimeter = perimeters[array_min_value_index(perimeters)];
}

int Gift::get_gift_area() {
    return 2*areas[0]+2*areas[1]+2*areas[2]+smallest_area;
}

int Gift::get_ribbon_length() {
    return (sides[0]*sides[1]*sides[2])+smallest_perimeter;
}
