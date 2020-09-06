#ifndef GRID_HPP
#define GRID_HPP

class Grid{
public:
    int food;
    int height;
    int move_cost;
    int fertility;
    int nat_regrow_period;
    int time_to_regrow = 0;

    //
    // Setters
    //
    void setFood(int food){ this->food = food;}
    void addFood(int food){ this->food += food;}
    void setHeight(int height){ this->height = height;}
    void setMoveCost(int move_cost){this->move_cost = move_cost;}
    
};

#endif //GRID_H