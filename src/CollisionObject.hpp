#ifndef COLLISIONOBJECT_HPP_GUARD
#define COLLISIONOBJECT_HPP_GUARD

class CollisionObject {
public:
    void onHit() { doOnHit(); }

private:
    virtual void doOnHit() = 0;
};

#endif
