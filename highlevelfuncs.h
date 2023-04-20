#include <string>

//Special variables
const int nullCell = 0;
const int cellBuffer = 1;
const int leftParam = 2;
const int rightParam = 3;
const int SpecialVarEnd = 4;

#define movRight(delta) std::string(delta, '>')
#define movLeft(delta) std::string(delta, '<')
#define mov(delta) if(delta>0) std::string(delta, '>'); else std::string(-delta, '<');
#define movRel(position, relPosition) movLeft(relPosition) + movRight(position)
#define movTmp(scope, delta) movRight(delta) + scope + movLeft(delta)
#define inc(amount) std::string(amount, '+')
#define dec(amount) std::string(amount, '-')


#define zeroCell(cell) movTmp("[-]", cell)
#define copyCell(from, to) zeroCell(to) + movTmp("[-", from) + movTmp("+", cellBuffer) + movTmp("+", to) + movTmp("]", from) \
                           + movTmp("[-", cellBuffer) + movTmp("+", from) + movTmp("]", cellBuffer)
#define copySum(leftCell, rightCell) copyCell(leftCell, leftParam) + copyCell(rightCell, rightParam)

#define addCell(from, to) \
        movTmp("[-", from) + movTmp("+", cellBuffer) + movTmp("]", from) + \
        movTmp("[-", cellBuffer) + movTmp("+", from) + movTmp("+", to) + movTmp("]", cellBuffer)
#define subCell(from, to) \
        movTmp("[-", from) + movTmp("+", cellBuffer) + movTmp("]", from) + \
        movTmp("[-", cellBuffer) + movTmp("+", from) + movTmp("-", to) + movTmp("]", cellBuffer)
#define mulCell(from, to) copySum(from, to) + zeroCell(to) + \
        movTmp("[-" + addCell(leftParam, to) + "]", rightParam) + zeroCell(leftParam)

#define addConst(value, to) movTmp(inc(value), to)
#define subConst(value, to) movTmp(dec(value), to)
#define mulConst(value, to) copyCell(to, leftParam) + addConst(rightParam, value) + zeroCell(to) + \
        movTmp("[-" + addCell(leftParam, to) + "]", rightParam) + zeroCell(leftParam)
