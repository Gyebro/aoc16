//
// Created by Gyebro on 2016. 09. 14.
// Definition of non-template functions for Dexel2
//

#include "dexel2.h"

namespace vpm {

    /**
     * During the sweeping of two dexels, we either operate on a point of the left or the right operand (op_right)
     * and the dexel's state are stored in ls and rs indicators.
     * Based on these 3 boolean values, 8 possible scenarios can happen, therefore each operation can be stored in
     * a byte.
     * Bit      op_right    rs      ls
     * 0        0           0       0
     * 1        0           0       1
     * 2        0           1       0
     * 3        0           1       1
     * 4        1           0       0
     * 5        1           0       1
     * 6        1           1       0
     * 7        1           1       1
     */
    bool dexelOperationQuery(const bool& op_right, const bool& ls, const bool& rs, const uint8_t& operation) {
        uint8_t shift = 0;
        if(op_right) shift += 4;
        if(rs) shift += 2;
        if(ls) shift += 1;
        return ((operation >> shift) & 0x01);
    }
}