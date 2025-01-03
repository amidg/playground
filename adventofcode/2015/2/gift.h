#pragma once
#ifndef GIFT_H
#define GIFT_H

#include<string>
#include<array>

class Gift {
    public:
        Gift(const std::string& text_dim);

        int get_gift_area();
        int get_ribbon_length();
        int get_width() const {return sides[0];};
        int get_length() const {return sides[1];};
        int get_height() const {return sides[2];};
        //std::array<int, 3> get_perimeters() const {return {perimeters[0], perimeters[1], perimeters[2]};};
        std::string get_text_entry() const {return entry;};

    private:
        std::string entry;
        std::array<int, 3> sides; // w, l, h
        std::array<int, 3> areas; // wl, wh, lh
        std::array<int, 3> perimeters; // w, l, h
        int smallest_area;
        int smallest_perimeter;
        void get_gift_smallest_dimensions();
};

#endif
