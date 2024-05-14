#include <iostream>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

std::mutex blockListDishes;

enum Menu
{
    pizza, soup, steak, salad, sushi
};

void waitingTime(int& inNumber,  int inTime1, int inTime2)
{
    blockListDishes.lock();
    inNumber = rand() % inTime2 + inTime1;
    blockListDishes.unlock();
    std::cout << "inTime3: " << inNumber << " " << std::endl; 
}

void printDish(int& inOrder)
{
    if (inOrder == Menu::pizza)
    {
        std::cout << "pizza " << Menu::pizza << std::endl;
    }
    else if (inOrder == Menu::soup)
    {
        std::cout << "soup " << Menu::soup << std::endl;
    }
    else if (inOrder == Menu::steak)
    {
        std::cout << "steak " << Menu::steak << std::endl;
    }
    else if (inOrder == Menu::salad)
    {
        std::cout << "salad " << Menu::salad << std::endl;
    }
    else if (inOrder == Menu::sushi)
    {
        std::cout << "sushi " << Menu::sushi << std::endl;
    }
}

void onlineOrder(int inTime)
{
    waitingTime(inTime, 5, 6);
    //std::cout << "inTime2: " <<  inTime << " " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(inTime));

}

void listOfDishes(std::vector<int>& inListDishes, int& inOrder)
{
    blockListDishes.lock();
    inListDishes.insert(inListDishes.begin(), inOrder);
    blockListDishes.unlock();
}

int main()
{
    std::srand(time(NULL));

    int order;
    int time;
    std::vector<int> listDishes;
    

   

    for (int i = 0; i < 10; ++i)
    {
        waitingTime(order, 0, 5);
        waitingTime(time, 5, 6);
        std::cout << "inTime1: " << time << " " << std::endl;
        std::cout << "The waiter took the order: ";
        printDish(order);
        std::thread orderFood(onlineOrder, time);

        orderFood.join();

        std::thread dish(listOfDishes, std::ref(listDishes), std::ref(order));

        dish.join();
       
        
       
    }
    


    
    for (int i = 0; i < listDishes.size(); ++i)
        {
            std::cout << listDishes[i] << " ";
        }

    
    
    return 0;
}