#pragma once
#ifndef SPF_MATH_H
#define SPF_MATH_H

#include <glm/glm.hpp>

namespace math {
    glm::mat4 get_transform_matrix(const glm::vec3 &pos, const glm::vec3 &rotation);
    glm::mat3 get_rotation_between(const glm::vec3 &origin, const glm::vec3 &end);
    glm::vec3 get_angle_between(const glm::vec3 &origin, const glm::vec3 &end, const glm::vec3 &axis);

    /*
     * The reason for this struct exists is simple: glm::vec2 waste memory than,
     * this struct, an UI needs better performance possible.
     */
    struct point {
        float x {};
        float y {};

        point() = default;
        point(float a, float b) {
            this->x = a;
            this->y = b;
        }
    };

    struct rect {
        float x {};
        float y {};
        float w {};
        float h {};

        rect() = default;
        rect(float a, float b) {
            this->x = a;
            this->y = b;
        }

        rect(float a, float b, float aa, float bb) {
            this->x = a;
            this->y = b;
            this->w = aa;
            this->h = bb;
        }

        bool collide_with_aabb(const rect &rect) const;
        bool collide_with_ab(float a, float b) const;

        math::rect operator + (const rect &rect) const;
    };
}

#endif