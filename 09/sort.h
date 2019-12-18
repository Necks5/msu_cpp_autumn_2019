#include <iostream>
#include <algorithm>
#include <atomic>
#include <thread>
#include <string.h>
#include <cmath>

using namespace std;
using intl = uint64_t;
using FILE_pointer = unique_ptr<FILE, int(*)(FILE*)>;
const int amount = 1 << 18;
const char *temp_file = "temp_file.bin";
const char *input_file = "in.bin";
const char *name_output = "out.bin";
const char *output_first_file = "outputtmp1.bin";
const char *output_second_file = "outputtmp2.bin";

int first_sort()
{
    unique_ptr<intl[]> data(new intl [amount]);
    FILE_pointer  input  (fopen(input_file, "rb"), fclose);
    FILE_pointer  output  (fopen(temp_file, "wb"), fclose);

    int read_numbers = 0, i = 0;
    do
    {
        read_numbers = fread(data.get(), sizeof(intl), amount, input.get());
        i++;
        sort(data.get(), data.get() + read_numbers);
        fwrite(data.get(), sizeof(intl), read_numbers, output.get());
    }
    while (read_numbers == amount);
    return i;
}

void merge_files(const int beg, const int size_of_chunk, const char *name_output, FILE* const output)
{
    FILE_pointer  input1 (fopen(name_output, "rb"),fclose);
    FILE_pointer  input2 (fopen(name_output, "rb"), fclose);

    unique_ptr<intl []> first_inp  (new intl [amount]);
    unique_ptr<intl []> second_inp (new intl [amount]);
    unique_ptr<intl []> merge      (new intl [2 * amount]);

    fread(first_inp.get(),  sizeof(intl), amount * beg, input1.get());
    fread(second_inp.get(), sizeof(intl), amount * beg, input2.get());

    for (int i = 0; i <= size_of_chunk; i++)
    {
        fread(second_inp.get(), sizeof(intl), amount, input2.get());
    }

    fread(first_inp.get(), sizeof(intl), amount, input1.get());
    int numbers_read = fread(second_inp.get(), sizeof(intl), amount, input2.get());

    intl *first_cur = first_inp.get(), *sec_cur = second_inp.get();
    int carried_out_1 = 1, carried_out_2 = 1; // количество считанных чанков
    bool finished = 0;
    do
    {
        if (*first_cur - *(first_inp.get()) == amount)
        {
            carried_out_1++;
            fread(first_inp.get(), sizeof(intl), amount, input1.get());
            first_cur = first_inp.get();
        }
        if (*sec_cur  - *(second_inp.get()) == numbers_read)
        {
            carried_out_2++;
            numbers_read = fread(second_inp.get(), sizeof(intl), amount, input2.get());
            sec_cur  = second_inp.get();
        }
        int how_comp = 0, cur1 = 0, cur2 = 0;
        while (cur1 < amount && cur2 < amount)
        {
            auto merge_array = merge.get();
            if (*first_cur < *sec_cur)
            {
                merge_array[how_comp++] = *(first_cur++);
                cur1++;
            }
            else
            {
                merge_array[how_comp++] = *(sec_cur ++);
                cur2++;
            }
        }
        finished = cur1 != amount;

        fwrite(merge.get(), sizeof(intl), how_comp, output);
    }
    while (!finished && !(carried_out_1 == size_of_chunk && !finished)
           && !(carried_out_2 == size_of_chunk && finished));

    if (finished)
    {
        fwrite(sec_cur , sizeof(intl), amount, output);
        while (carried_out_2 != size_of_chunk)
        {
            fread(second_inp.get(), sizeof(intl), amount, input2.get());
            carried_out_2++;
            fwrite(second_inp.get(), sizeof(intl), amount, output);
        }
    }
    else
    {
        fwrite(first_cur , sizeof(intl), amount - (*first_cur - *(first_inp.get())), output);
        while (carried_out_1 != size_of_chunk)
        {
            fread(first_inp.get(), sizeof(intl), amount, input1.get());
            carried_out_1++;
            fwrite(first_inp.get(), sizeof(intl), amount, output);
        }
    }
    return;
}

void merge_in_one(bool flag)
{
    char *name1 = strdup(output_first_file), *name2 = strdup(output_second_file);
    if (!flag)
        swap(name1, name2);

    FILE_pointer  output1 (fopen(name1, "rb"),fclose);
    FILE_pointer  output2 (fopen(name2, "rb"),fclose);
    FILE_pointer  output (fopen(name_output, "wb"),fclose);

    unique_ptr<intl []> buf (new intl [amount]);
    int how_read = 1;
    while (how_read)
    {
        how_read = fread(buf.get(), sizeof(intl), amount, output1.get());
        fwrite(buf.get(), sizeof(intl), how_read, output.get());
    }
    how_read = 1;
    while (how_read)
    {
        how_read = fread(buf.get(), sizeof(intl), amount, output2.get());
        fwrite(buf.get(), sizeof(intl), how_read, output.get());
    }
    return;
}