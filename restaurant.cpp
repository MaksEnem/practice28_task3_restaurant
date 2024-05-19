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
    //waitingTime(inTime, 5, 6);
    //std::cout << "inTime2: " <<  inTime << " " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(inTime));

}

void listOfDishes(std::vector<int>& inListDishes, int& inOrder)
{
    blockListDishes.lock();
    inListDishes.insert(inListDishes.begin(), inOrder);
    blockListDishes.unlock();
}


void listOfCooking(std::vector<int>& inListDishes, std::vector<int>& inMealDeliveryList, int inTime)
{    
    blockListDishes.lock();
    std::cout << "The food is being prepared: ";
    printDish(inListDishes.back());
    inMealDeliveryList.push_back(inListDishes.back());
    inListDishes.pop_back();
    blockListDishes.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(inTime));
    std::cout << "Dish prepared: ";
    printDish(inMealDeliveryList.back());
}

void listMealDelivery(std::vector<int>& inMealDeliveryList, int& inCount)
{
    std::this_thread::sleep_for(std::chrono::seconds(30));
    std::cout << "The following dishes are delivered: ";

    for (int i = 0; i < inMealDeliveryList.size(); ++i)
    {
        printDish(inMealDeliveryList[i]);
    }
    inMealDeliveryList.clear();
    ++inCount;
}

int main()
{
    std::srand(time(NULL));

    int order;
    int time;
    int count = 0;
    std::vector<int> listDishes;
    std::vector<int> mealDeliveryList;

   

    while (true)
    {
        waitingTime(order, 0, 5);
        waitingTime(time, 5, 6);
        
        std::cout << "The waiter took the order: ";
        printDish(order);
        std::thread orderFood(onlineOrder, time);
        orderFood.join();

        std::thread dish(listOfDishes, std::ref(listDishes), std::ref(order));
        dish.join();

        waitingTime(time, 5, 11);
        std::thread cooking(listOfCooking, std::ref(listDishes), std::ref(mealDeliveryList), time);        
        cooking.detach();
        
        std::thread mealDelivery(listMealDelivery, std::ref(mealDeliveryList), std::ref(count));
        mealDelivery.detach();
       
        if (count == 9)
        {
            return 0;
        }
    }   
}