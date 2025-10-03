#include "RenderUtils.h" // Asumiendo que aquí están RenderItem y CreateShape
#include <PxPhysicsAPI.h> // Para PhysX

class Particle {
public:
    Particle(const physx::PxGeometry& geometry, const physx::PxVec3& pos,
        const Vector3& vel, const Vector3& accel, float damping, const Vector4& color)
        : position(pos), velocity(vel), acceleration(accel), damping(damping)
    {
        // Crear la forma
        shape = CreateShape(geometry);
        transform = new physx::PxTransform(pos); // Inicializa PhysX transform
        renderItem = new RenderItem(shape, transform, color);
    }

    ~Particle() {
        // Eliminar RenderItem
        delete renderItem;
        delete transform;
        // Liberar shape si corresponde
        shape->release();
    }

    void integrate(float dt) {
        // Euler semi-implícito (más estable que el Euler simple)
        velocity += acceleration * dt;
        velocity *= damping;   // aplica damping
        position += velocity * dt;

        // Actualizar el RenderItem
        transform->p = physx::PxVec3(position.x, position.y, position.z);
    }

    // Getters (si los necesitas)
    Vector3 getPosition() const { return position; }
    Vector3 getVelocity() const { return velocity; }

private:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float damping;

    physx::PxShape* shape;
    physx::PxTransform* transform;
    RenderItem* renderItem;
};

