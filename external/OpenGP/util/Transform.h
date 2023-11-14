// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/types.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class Transform {
public:

    Vec3 position = Vec3::Zero();
    Vec3 scale = Vec3::Ones();
    Quaternion rotation = Quaternion::Identity();

    Vec3 right() const {
        return rotation * Vec3(1, 0, 0);
    }

    void set_right(Vec3 right) {
        right.normalize();
        Vec3 axis = this->right().cross(right);
        float angle = axis.norm();
        rotation = rotation * Quaternion(Eigen::AngleAxisf(std::asin(angle), axis / angle));
    }

    Vec3 up() const {
        return rotation * Vec3(0, 1, 0);
    }

    void set_up(Vec3 up) {
        up.normalize();
        Vec3 axis = this->up().cross(up);
        float angle = axis.norm();
        rotation = rotation * Quaternion(Eigen::AngleAxisf(std::asin(angle), axis / angle));
    }

    Vec3 forward() const {
        return rotation * Vec3(0, 0, 1);
    }

    void set_forward(Vec3 forward) {
        forward.normalize();
        Vec3 axis = this->forward().cross(forward);
        float angle = axis.norm();
        rotation = rotation * Quaternion(Eigen::AngleAxisf(std::asin(angle), axis / angle));
    }

    Mat4x4 get_transformation_matrix() const {

        Mat4x4 t = get_translation_matrix();
        Mat4x4 r = get_rotatation_matrix();
        Mat4x4 s = get_scale_matrix();

        return t * r * s;

    }

    void set_transformation_matrix(const Mat4x4 &mat) {
        auto t = decompose_matrix(mat);
        position = t.position;
        rotation = t.rotation;
        scale = t.scale;
    }

    void set_transformation(const Transform &t) {
        position = t.position;
        rotation = t.rotation;
        scale = t.scale;
    }

    Mat4x4 get_translation_matrix() const {

        Mat4x4 t = Mat4x4::Identity();
        t(0, 3) = position(0);
        t(1, 3) = position(1);
        t(2, 3) = position(2);

        return t;

    }

    void set_translation_matrix(const Mat4x4 &mat) {
        position = decompose_matrix_translation(mat);
    }

    void set_translation(const Vec3 &v) {
        position = v;
    }

    void apply_translation_matrix(const Mat4x4 &mat) {
        apply_translation(decompose_matrix_translation(mat));
    }

    void apply_translation(const Vec3 &v) {
        position += v;
    }

    Mat4x4 get_rotatation_matrix() const {

        Mat4x4 r = Mat4x4::Identity();
        r.block<3, 3>(0, 0) = rotation.matrix();

        return r;

    }

    void set_rotation_matrix(const Mat4x4 &mat) {
        rotation = decompose_matrix_rotation(mat);
    }

    void apply_rotation_matrix(const Mat4x4 &mat) {
        apply_rotation(decompose_matrix_rotation(mat));
    }

    void apply_rotation(const Quaternion &q) {
        rotation = q * rotation;
    }

    Mat4x4 get_scale_matrix() const {

        Mat4x4 s = Mat4x4::Identity();
        s(0, 0) = scale(0);
        s(1, 1) = scale(1);
        s(2, 2) = scale(2);

        return s;

    }

    void set_scale_matrix(const Mat4x4 &mat) {
        scale = decompose_matrix_scale(mat);
    }

    void set_scale(const Vec3 &v) {
        scale = v;
    }

    void apply_scale_matrix(const Mat4x4 &mat) {
        apply_scale(decompose_matrix_scale(mat));
    }

    void apply_scale(const Vec3 &v) {
        scale[0] *= v[0];
        scale[1] *= v[1];
        scale[2] *= v[2];
    }

    static Vec3 decompose_matrix_translation(const Mat4x4 &mat) {
        return mat.block<3, 1>(0, 3);
    }

    static Quaternion decompose_matrix_rotation(const Mat4x4 &mat) {
        Mat4x4 lmat = mat;
        Vec3 scale = Vec3(
            lmat.block<3, 1>(0, 0).norm(),
            lmat.block<3, 1>(0, 1).norm(),
            lmat.block<3, 1>(0, 2).norm()
        );

        lmat.block<3, 1>(0, 0) /= scale(0);
        lmat.block<3, 1>(0, 1) /= scale(1);
        lmat.block<3, 1>(0, 2) /= scale(2);

        return Quaternion(lmat.block<3, 3>(0, 0));
    }

    static Vec3 decompose_matrix_scale(const Mat4x4 &mat) {
        return Vec3(
            mat.block<3, 1>(0, 0).norm(),
            mat.block<3, 1>(0, 1).norm(),
            mat.block<3, 1>(0, 2).norm()
        );
    }

    static Transform decompose_matrix(const Mat4x4 &mat) {
        Transform t;

        Mat4x4 lmat = mat;
        t.position = lmat.block<3, 1>(0, 3);

        t.scale = Vec3(
            lmat.block<3, 1>(0, 0).norm(),
            lmat.block<3, 1>(0, 1).norm(),
            lmat.block<3, 1>(0, 2).norm()
        );

        lmat.block<3, 1>(0, 0) /= t.scale(0);
        lmat.block<3, 1>(0, 1) /= t.scale(1);
        lmat.block<3, 1>(0, 2) /= t.scale(2);

        t.rotation = Quaternion(lmat.block<3, 3>(0, 0));

        return t;
    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
