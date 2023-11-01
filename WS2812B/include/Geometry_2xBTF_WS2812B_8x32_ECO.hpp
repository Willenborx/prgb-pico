/**
 * @file Geometry_2xBTF_WS2812B_8x32_ECO.hpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief This implements the geometry of 2 cascaded BTF WS2812B_8x32_ECO LED arrays
 * @version 0.5
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PRGB_Geometry_2xBTF_WS2812B_8x32_ECO_HPP
#define PRGB_Geometry_2xBTF_WS2812B_8x32_ECO_HPP

#include <Geometry.hpp>

namespace prgb {
    /** 
     * @brief Implements the geometry of 2 8x32 W2812B ECO arrays produced by BTF
    */
    class Geometry_2xBTF_WS2812B_8x32_ECO : public Geometry
    {
    public:
        //! The BTF array is weirdly arranged in a zig-zag pattern
        uint16_t get_index_from_coords(Coordinate xP, Coordinate yP) {
            Coordinate x = xP % get_x_count();
            Coordinate y = yP % get_y_count();
            uint16_t idx;
            
            if (y < 8) { // first array
                idx = x*8;
                if ((x & 1) == 0) { // odd
                    idx += 7-y;
                } else { // even
                    idx += y;
                }

            } else { // second array 
                idx = 256+8*(31-x);
                if ((x & 1) == 0) { // odd
                    idx += 7-(y % 8);
                } else { // even
                    idx += (y % 8);
                }
            }
            return idx;
        };

        //! returns 512 for two BTF 8x32 arrays
        uint16_t get_length() { return 512; }
        uint16_t get_max_framerate() { return 60; }
        Dimension get_x_count() { return 32; }
        Dimension get_y_count() { return 16; }
        
    };

};
#endif