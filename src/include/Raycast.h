#pragma once
#include "Camera.h"
#include "Utils.h"
#include "Object.h"

class Raycast{
public:
    Raycast(const Utils::point3f startPosition, const Utils::rotation3f startRotation);
    ~Raycast(){};
    void cast(float length);
    Object* getCollision(){return collisionObj;};
    bool render();

    Utils::point3f getRayEnd(){return rayEndPos;};


private:
    const Utils::rotation3f startRot;
    const Utils::point3f startPos;

    Utils::point3f rayEndPos = {0,0,0};

    Object* collisionObj = nullptr;
    bool hasCast = false;
};