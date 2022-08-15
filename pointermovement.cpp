#include "pointermovement.h"

#include <string>

std::string PointerMovement::movePointer(int position)
{
    if(currentPos > position)
    {
        int distance = currentPos - position;
        currentPos -= distance;
        return std::string(distance, '<');
    }
    else
    {
        int distance = position - currentPos;
        currentPos += distance;
        return std::string(distance, '>');
    }
}

std::string PointerMovement::relativePointer(int delta)
{
    if(delta < 0)
    {
        currentPos -= delta;
        return std::string(delta, '<');
    }
    else if(delta > 0)
    {
        currentPos += delta;
        return std::string(delta, '>');
    }
    return "";
}

int PointerMovement::getPointer()
{
    return currentPos;
}