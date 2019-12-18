#include <chrono>
#include <iostream>


#include "sort.h"

int main()
{
    thread thread1, thread2;
    int n = first_sort();
    int i = (int)log2(n) + 1, size = 1;
    n /= 2;
    for (int j = 0; j < i; j++)
    {
        FILE_pointer  output1 (fopen(output_first_file, "wb"), fclose);
        FILE_pointer  output2 (fopen(output_second_file, "wb"), fclose);

        for (int k = 1; k <= n; k += 2)
        {
            thread1 = thread(merge_files,  2 * k    * size, size, temp_file, output1.get());
            thread2 = thread(merge_files,  2 *(k-1) * size, size, temp_file, output2.get());
            thread1.join();
            thread2.join();
        }
        size <<= 1;
        merge_in_one(!(n & 1));
    }
    return 0;
}