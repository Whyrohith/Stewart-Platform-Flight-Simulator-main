#include "math.hpp"
#include <iostream>
#include <string>
#include <glm/ext/quaternion_trigonometric.hpp>
#include "log.hpp"

glm::mat4 math::get_transform_matrix(const glm::vec3 &pos, const glm::vec3 &rotation) {
    glm::mat4 mat;
    return mat;
}

glm::mat3 math::get_rotation_between(const glm::vec3 &origin, const glm::vec3 &end) {
    glm::vec3 up {0, 0, 0};
    glm::vec3 dir = end - origin;
    glm::mat3 transform {};

    if (dir.x == 0 && dir.z == 0) {
        if (dir.y < 0) {
            transform = glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }
    } else {
        glm::vec3 y = glm::normalize(dir);
        glm::vec3 z = glm::normalize(glm::cross(y, glm::vec3(0, 1, 0)));
        glm::vec3 x = glm::normalize(glm::cross(y, z));

        transform = glm::mat3(x, y, z);
    }

    return transform;
}

glm::vec3 math::get_angle_between(const glm::vec3 &origin, const glm::vec3 &end, const glm::vec3 &axis) {
    glm::vec3 final_vec {};
    glm::vec3 axis_vec {};

    if (axis.x == 1) {
        axis_vec = glm::vec3(end.x, origin.y, origin.z);

        glm::vec3 a = glm::normalize(end - origin);
        glm::vec3 b = glm::normalize(axis_vec - origin);

        final_vec.x = glm::degrees(glm::acos(glm::dot(a, b)));
    }

    if (axis.y == 1) {
        axis_vec = glm::vec3(origin.x, end.y, origin.z);

        glm::vec3 a = glm::normalize(end - origin);
        glm::vec3 b = glm::normalize(axis_vec - origin);

        final_vec.y = glm::degrees(glm::acos(glm::dot(a, b)));
    }

    if (axis.z == 1) {
        axis_vec = glm::vec3(origin.x, origin.y, end.z);

        glm::vec3 a = glm::normalize(end - origin);
        glm::vec3 b = glm::normalize(axis_vec - origin);

        final_vec.z = glm::degrees(glm::acos(glm::dot(a, b)));
    }

    return final_vec;
}

bool math::rect::collide_with_aabb(const math::rect &rect) const {
    // Not implemented.
    return this->x < rect.x + rect.w && this->y < rect.y + rect.h && this->x + this->w > rect.x && this->y + this->h > rect.y;
}

bool math::rect::collide_with_ab(float a, float b) const {
    return a > this->x && a < this->x + this->w && b > this->y && b < this->y + this->h;
}

math::rect math::rect::operator+(const math::rect &rect) const {
    math::rect copy;

    copy.x = this->x + rect.x;
    copy.y = this->y + rect.y;
    copy.w = this->w;
    copy.h = this->h;

    return copy;
}
