//
// Created by Gyebro on 2016. 08. 01..
//

#ifndef MCSUITESANDBOX_DEXEL2_H
#define MCSUITESANDBOX_DEXEL2_H


#include <vector>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace vpm {

    enum class DexelDirection2 {
        X,
        Y,
        Z
    };

    /**
     * Operation byte for dexel subtraction
     * this represents the fact, that the current point should be added to the result in the following cases
     * Bit      op_right    rs      ls      result
     * 0        0           0       0       1
     * 1        0           0       1       1
     * 2        0           1       0       0
     * 3        0           1       1       0
     * 4        1           0       0       0
     * 5        1           0       1       1
     * 6        1           1       0       0
     * 7        1           1       1       1
     * i.e. when we operate on the left and rs = false OR when we operate on the right and ls = true
     */
    const uint8_t operationByteSubtract = 0b10100011;

    /**
     * Operation byte for dexel union
     * this represents the fact, that the current point should be added to the result in the following cases
     * Bit      op_right    rs      ls      result
     * 0        0           0       0       1
     * 1        0           0       1       1
     * 2        0           1       0       0
     * 3        0           1       1       0
     * 4        1           0       0       1
     * 5        1           0       1       0
     * 6        1           1       0       1
     * 7        1           1       1       0
     * i.e. when we operate on the left and rs = false OR when we operate on the right and ls = true
     */
    const uint8_t operationByteUnion = 0b01010011;

    /**
     * \brief Dexel2 class
     * This dexel variant maintains an ordered list of points representing intervals
     * Invariant properties:
     * - holds 2*k points
     * - points are in ascending order
     */
    template <class T>
    class Dexel2 {
    private:
        vector<T> p;
    public:
        Dexel2() {
            p.resize(0);
        }
        Dexel2(vector<T> points) {
            p = points;
        }
        const vector<T> & getPoints() const { return p; }
        void addDexel(const Dexel2<T>& that) {
            p = dexelOperate(*this, that, operationByteUnion);
        }
        void subtractDexel(const Dexel2<T>& that) {
            p = dexelOperate(*this, that, operationByteSubtract);
        }
    };

    

    // Operations

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
    bool dexelOperationQuery(const bool& op_right, const bool& ls, const bool& rs, const uint8_t& operation);

    const double globalDexelTolerance = 0.000001;

    template <class T>
    vector<T> dexelOperate(const Dexel2<T>& l, const Dexel2<T>& r, const uint8_t operation) {
        // Sweep line over the points of both dexels
        vector<T> res;
        res.resize(0);
        size_t li=0, ri=0;
        size_t lsize = l.getPoints().size();
        size_t rsize = r.getPoints().size();
        bool ls = false;
        bool rs = false;
        const vector<T>& lp = l.getPoints();
        const vector<T>& rp = r.getPoints();
        bool sweeping = true;
        bool op_left = false;
        bool op_right = false;
        bool l_ok, r_ok, b_ok;
        size_t iter = 0;
        while (sweeping) {
            l_ok = (li < lsize); r_ok = (ri < rsize);
            b_ok = l_ok && r_ok;
            op_left = l_ok && !r_ok;    // Check if one of the containers has ran out
            op_right = !l_ok && r_ok;
            op_left = op_left || (b_ok && (lp[li] < rp[ri])); // Regular check of next
            op_right = op_right || (b_ok && (lp[li] >= rp[ri]));
            // TODO: op_left and op_right cannot be true at the same time, use this to simplify
            if (op_left) {
                ls = !ls;
                if (dexelOperationQuery(op_right, ls, rs, operation)) res.push_back(lp[li]);
                li++; op_left = false;
            } else if (op_right) {
                rs = !rs;
                if (dexelOperationQuery(op_right, ls, rs, operation)) res.push_back(rp[ri]);
                ri++; op_right = false;
            }
            sweeping = (li < lsize) || (ri < rsize);    // Terminate while if necessary
            iter++;
            if (iter > 10000) {
                cout << "Problem here!\n";
            }
        } // end while
        // TODO: Poor man's reduce
        vector<T> red; red.resize(0);
        T rstart, rend;
        for (size_t i=0; i<res.size(); i+=2) {
            rstart = res[i];
            rend = res[i+1];
            if ((rend-rstart) > globalDexelTolerance) {
                red.push_back(rstart); red.push_back(rend);
            } else {
                //cout << "x";
            }
        }
        return red;
    }

    template <class T>
    Dexel2<T> dexelSubtract(const Dexel2<T>& l, const Dexel2<T>& r) {
        return Dexel2<T>(dexelOperate(l, r, operationByteSubtract));
    }

    template <class T>
    Dexel2<T> dexelUnion(const Dexel2<T>& l, const Dexel2<T>& r) {
        return Dexel2<T>(dexelOperate(l, r, operationByteUnion));
    }


}

#endif //MCSUITESANDBOX_DEXEL2_H