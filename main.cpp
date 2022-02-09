/// @file main.cpp
/// @author Chris Catechis
/// @date 2/6/2022
/// @brief This program tests the knightType class.

#include <iostream>
#include <cstdlib>
#include "knightType.hpp"

int main() {
    knightType* tour = NULL;
    int dim;
    int r, c;
    char yesOrNo;

    do
    {
        std::cout << "Enter board dimension: ";
        std::cin >> dim;

        tour = new knightType(dim);

        tour->outputTour();

        std::cout << "Enter initial knight position: ";
        std::cin >> r >> c;

        if (tour->knightTour(r - 1, c - 1))
        {
            std::cout << "Knight FTW" << std::endl;
            tour->outputTour();
        }

        delete tour;
        tour = NULL;
        
        std::cout << "Another go? ";
        std::cin >> yesOrNo;
        std::cout << '\n';
    }
    while (std::tolower(yesOrNo) == 'y');
    



    return 0;
}