#include "Exceptions.h"

ostream& operator<<(ostream& f, UTComputerException& e) {
    f<<e.getMessage();
    return f;
}