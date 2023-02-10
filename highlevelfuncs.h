#include <string>

//Special variables
const int nullCell = 0;
const int cellBuffer = 1;
const int numBuffer = 2;
const int SpecialVarEnd = 6;

#define movRight(delta) std::string(delta, '>')
#define movLeft(delta) std::string(delta, '<')
#define mov(delta) if(delta>0) std::string(delta, '>'); else std::string(-delta, '<');
#define movRel(position, relPosition) movLeft(relPosition) + movRight(position)
#define movTmp(scope, delta) movRight(delta) + scope + movLeft(delta)
#define inc(amount) std::string(amount, '+')
#define dec(amount) std::string(amount, '-')

#define zeroCell(cell) movTmp("[-]", cell)
#define zeroNum(num) zeroCell(num+0) + zeroCell(num+1) + zeroCell(num+2) + zeroCell(num+3)
#define copyCell(from, to) zeroCell(to) + \
    movTmp("[-", from) + movTmp("+", cellBuffer) + movTmp("+", to) + movTmp("]", from) + \
    movTmp("[-", cellBuffer) + movTmp("+", from) + movTmp("]", cellBuffer)
#define copyNum(from, to) copyCell(from+0, to+0) + copyCell(from+1, to+1) + copyCell(from+2, to+2) + copyCell(from+3, to+3)

#define addCell(from, to) \
    movTmp("[-", from) + movTmp("+", cellBuffer) + movTmp("+", to) + movTmp("]", from) + \
    movTmp("[-", cellBuffer) + movTmp("+", from) + movTmp("]", cellBuffer)
#define addCellWithCarry(from, to) \
    movTmp("[-", from) + movTmp("+", cellBuffer) + movTmp("+[<+>+]-", to) + movTmp("]", from) + \
    movTmp("[-", cellBuffer) + movTmp("+", from) + movTmp("]", cellBuffer)
#define copyAdd(from, to) addCell(from+0, to+0) + addCellWithCarry(from+1, to+1) + addCellWithCarry(from+2, to+2) + addCellWithCarry(from+3, to+3) 

#define addConstNum(to, value) movTmp(inc((valSource >> 24) & 0xFF) + ">" + inc((valSource >> 16) & 0xFF) \
                                + ">" + inc((valSource >> 8) & 0xFF)+ ">" + inc((valSource >> 0) & 0xFF) + "<<<", to)
