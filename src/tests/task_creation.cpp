#include"CM_datatypes.hpp"
#include"CM_task.hpp"

void resize_vector(tasks_array& tasks, const int size);
int main()
{
    const int size = 12;
    tasks_array tasks;
    std::cout << "Tasks size (before): " << tasks.size() << std::endl;
    resize_vector(tasks, size);
    std::cout << "Tasks size (after): " << tasks.size() << std::endl;

}

void resize_vector(tasks_array& tasks, const int size)
{
     tasks.resize(size);
}