#pragma once
#include <cmath>
#include <iostream>

class Vector3D {
public:
    float x, y, z;

    // Constructores
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    // Operadores básicos
    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    Vector3D operator*(float escalar) const {
        return Vector3D(x * escalar, y * escalar, z * escalar);
    }

    Vector3D& operator=(const Vector3D& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }

    // Módulo
    float modulo() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalización
    Vector3D normalizado() const {
        float m = modulo();
        if (m == 0) return Vector3D(0, 0, 0);
        return Vector3D(x / m, y / m, z / m);
    }

    // Producto escalar
    float dot(const Vector3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Para debug
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }
};
