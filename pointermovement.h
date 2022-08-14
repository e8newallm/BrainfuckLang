#ifndef POINTERMOVEMENT_H
#define POINTERMOVEMENT_H

#include <string>

class PointerMovement
{
    public:
    std::string movePointer(int position);
    std::string relativePointer(int delta);
    int getPointer();

    private:
    int currentPos = 0;
};

#endif //POINTERMOVEMENT_H