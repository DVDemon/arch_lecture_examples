#include "../rabbit/SimplePocoHandler.h"
#include "../database/pizza_order.h"
#include "../database/queue.h"


int main(void)
{
    database::Queue::get().subscribe(
        [](const std::string & val) {
            try{
                database::PizzaOrder order = database::PizzaOrder::fromJSON(val);
                order.add();
            }catch(std::exception &ex){
                std::cout << "# error - " << ex.what() << std::endl;
            }    
         }
    );

    std::cin.get();

    return 0;
}