#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include <utils.hpp>

/**
 * @brief Virtual shape class
 */
class shape {
public:
    /**
     * @brief Test if a position is within the shape
     *
     * Test if the given position as input belongs to the shape or not.
     * @param {double} x; x coordinate
     * @param {double} y; y coordinate
     * @return Return true if the given position is within the shape.
     */
    virtual bool is_within(double x, double y) const = 0;

    shape* clone() const {
        return do_clone(); // implemented by private virtual function
    }

    virtual shape * do_clone() const = 0;
};

inline shape* new_clone(const shape& sh) {
    return sh.clone();
}

#endif // SHAPE_HPP_
