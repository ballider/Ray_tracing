#include "head.hpp"

int main() {
    Parametrs param;              // параметры камеры, экрана, света
    std::vector<Figure*> Figures; // фигуры для рисования 

    std::string file;

    std::cout << "Enter file with parametrs: ";
    std::cin >> file;
    
    bool res1 = ParametrsFromFile(file, param);
    if(!res1) {
        return -1;
    }

    std::cout << "Enter file with objects: ";
    std::cin >> file;
    
    bool res2 = FiguresFromFile(file, Figures);
    if(!res2) {
        return -1;
    }
    
    render(Figures, param);
    
    return 0;
}