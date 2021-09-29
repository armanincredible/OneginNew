#include "dora.h"

int main ()
{
    char *buffer = NULL;
    int str_amount = 0;

    buffer = remove_trash_and_copy_in_buffer (&str_amount);

    struct Onegin* line = (Onegin*) calloc (str_amount, sizeof (line));
    if (line == NULL)
    {
        printf ("ERROR in function : %s \nhaven't memory for struct line\n", __func__);
        return -1;
    }

    make_array_adress (buffer, str_amount, line);
    if (sort_and_output_text (line, str_amount, buffer) == -1)
    {
        printf ("ERROR in function : %s \nerror in funct sort_and_output_text\n", __func__);
    }

    free (buffer);
    for (int ind = 0; ind < str_amount - 1; ind++)
    {
        free (line[ind].adress);
    }

    return 0;
}