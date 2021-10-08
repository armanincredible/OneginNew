#include "dora.h"

int make_array_adress (char* buffer, const int str_amount, Onegin* line)
{
    if (buffer == NULL)
    {
        printf ("ERROR in function : %s \nbuffer has zero adress\n", __func__);
        return -1;
    }
    
    for (int ind = 0; ind < str_amount; ind++)
    {
        line[ind].adress = (char*) calloc (1, sizeof (char*));
        if (line[ind].adress == NULL)
        {
            printf ("ERROR in function : %s \nhaven't memory for struct line\n", __func__);
            return -1;
        }
    }

    char *adress_new_str = buffer;
    char *past_adress = buffer;
    line[0].adress = buffer;
    int amount = 1;
    int one = 1; // ammount for work with symbols and strings

    for (; amount != str_amount; amount++)
    {
        adress_new_str = strchr (adress_new_str, '\n') + one;

        line [amount      ].adress = adress_new_str;
        line [amount - one].length = adress_new_str - past_adress - one;

        past_adress = adress_new_str;
    }

    adress_new_str = strchr (adress_new_str, '\0') + one;
    *(adress_new_str - one) = '\n';

    line[amount      ].adress = adress_new_str;
    line[amount - one].length = line[amount].adress - line[amount - one].adress - one;

    return 1;
}

int size_file (FILE *filestream)
{
    if (filestream == NULL)
    {
        printf ("ERROR in function : %s \nfilestream have zero adress\n", __func__);
        return -1;
    }

    fseek (filestream, 0, SEEK_END);

    int size = ftell (filestream);

    fseek (filestream, 0, SEEK_SET);
    
    return (size);
}

char* remove_trash_and_copy_in_buffer (int* str_amount)
{
    if (str_amount == NULL)
    {
        printf ("ERROR in function : %s \nstr_amount have zero adress\n", __func__);
        return NULL;
    }

    FILE *file = NULL;
    if ((file = fopen ("dora.txt", "r")) == NULL)
    {
        printf ("ERROR in function : %s \ndora.txt didnt open\n", __func__);
        return NULL;
    }

    int size_buffer = size_file (file);

    char* buffer = (char*)calloc (size_buffer, sizeof (char));
    if (buffer == NULL)
    {
        printf ("ERROR in function : %s \nhaven't memory for buffer open\n", __func__);
        return NULL;
    }
    
    int amount_of_necessary_symbols = 0;
    char curSymbol = '0';
    char symbol_past = '\n';
    bool new_string = true;
    *str_amount = 1;

    for (;(curSymbol = getc (file)) != EOF;)
    {
        if ((new_string == 1) && (curSymbol == ' '))
        {
            continue;
        }
        new_string = false;

        if ((curSymbol == symbol_past) &&
           ((curSymbol == ' ') || (curSymbol == '\t') || 
            (curSymbol == ',') || (curSymbol == '\n')))
        {
            continue;
        }

        if (curSymbol == '\n')
        {
            new_string = true;
            if (symbol_past != '\n')
            {
                *str_amount = *str_amount + 1;
            }
        }

        symbol_past = curSymbol;
        buffer [amount_of_necessary_symbols] = curSymbol;
        amount_of_necessary_symbols++;
    }

    for (int amount = amount_of_necessary_symbols; amount < size_buffer; amount++)
    {
        buffer [amount] = '\0';
    }

    if (ferror(file))
    {
        printf ("ERROR in function : %s \nreading file falled\n", __func__);
        return NULL;
    }
    
    fclose (file);
    
    return buffer;
}

void qsort (void* line, size_t size, int left, int right, int (*comp)(const void*, const void*))
{
    int one = 1;
    int ind = 0;
    int last = 0;
    
    if (left >= right)
    {
        return;
    }

    last = left;

    for (ind = left + one; ind <= right; ind++)
    {
        if ( (*comp)((char*)line + size*ind, (char*)line + size * left) > 0)
        {
            swap((char*) line, size, ++last, ind);
        }
    }
    
    swap((char*)line, size, left, last);

    qsort((char*)line, size, left, last - one, comp);
    qsort((char*)line, size, last + one, right, comp);
}

void swap (void* line, size_t size, int i, int j)
{
    char *tmp = (char*) calloc(1, size);
    if (tmp == NULL)
    {
        printf ("ERROR in function : %s \nhaven't memory for tmp\n", __func__);
        return ;
    }
    memcpy(tmp, (char*) line + size * i, size);
    memcpy((char*) line + size * i, (char*) line + size * j, size);
    memcpy((char*) line + size * j, tmp, size);
    free(tmp);
}

int comp_typical (const void* first_line_void, const void* second_line_void)
{
    Onegin* first_line = (Onegin*) first_line_void;
    Onegin* second_line = (Onegin*) second_line_void;

    int first_ind = 0;
    int second_ind = 0;
    int first_length = first_line->length - 1;
    int second_length = second_line->length - 1;
    char* first_adress = first_line->adress;
    char* second_adress = second_line->adress;

    for (; first_ind > -1; first_ind++, second_ind++)
    {
        while (!isalpha(*(first_adress + first_ind)))
        {
            first_ind++;
            if (first_ind == first_length)
            {
                break;
            }
        }
        while (!isalpha(*(second_adress + second_ind)))
        {
            second_ind++;
            if (second_ind == first_length)
            {
                break;
            }
        }

        if (tolower (*(first_adress  + first_ind)) == 
            tolower (*(second_adress + second_ind)))
        {
            if ((second_ind == second_length) && 
                (first_ind !=  first_length))
            {
                return -1;
            }
            else if ((second_ind != second_length) && 
                     (first_ind  == first_length))
            {
                return 1;
            }
            else if ((second_ind == second_length) &&
                     (first_ind  == first_length))
            {
                return  first_line->length - second_line->length;
            }
            continue;
        }
        else
        {
            return compare_char (*(second_adress + second_ind),
                                 *(first_adress + first_ind));
        }
    }
    return -1;
}

int comp_reverse (const void* first_line_void, const void* second_line_void)
{
    Onegin* first_line = (Onegin*) first_line_void;
    Onegin* second_line = (Onegin*) second_line_void;

    int first_ind  = 0;
    int second_ind = 0;
    
    int first_length    = first_line-> length - 1;
    int second_length   = second_line->length - 1;

    char* first_adress  = first_line-> adress;
    char* second_adress = second_line->adress;

    for (; first_ind > -1; first_ind++, second_ind++)
    {   
        while (!isalpha(*(first_adress + first_length - first_ind)))
        {
            first_ind++;
            if (first_ind == first_length)
            {
                break;
            }
        }
        while (!isalpha(*(second_adress + second_length - second_ind)))
        {
            second_ind++;
            if (second_ind == second_length)
            {
                break;
            }
        }

        if (tolower (*(first_adress + first_length - first_ind)) == 
            tolower (*(second_adress + second_length - second_ind)))
        {
            if ((second_ind == second_length) && 
                (first_ind != first_length))
            {
                return 1;
            }
            else if ((second_ind != second_length) && 
                     (first_ind == first_length))
            {
                return -1;
            }
            else if ((second_ind == second_length) && 
                     (first_ind == first_length))
            {
                return  first_line->length - second_line->length;
            }
            continue;
        }
        else 
        {
            return compare_char (*(second_adress + second_length - second_ind),
                                 *(first_adress + first_length - first_ind));
        }
    }
    return 1;
}

int sort_and_output_text (Onegin* line, const int str_amount, char *buffer)
{
    int one = 1;

    if (buffer == NULL)
    {
        printf ("ERROR in function : %s \nbuffer have zero adress\n", __func__);
        return -1;
    }

    FILE* fileoutput = NULL;
    fileoutput = fopen ("NewText.txt", "w+");
    if (fileoutput == NULL)
    {
        printf ("ERROR in function : %s \nNewText.txt didnt open\n", __func__);
        return -1;
    }

    qsort (line, sizeof(Onegin), 0, str_amount - one, comp_typical);
    output (line, str_amount, fileoutput);
    

    qsort (line, sizeof(Onegin), 0, str_amount - one, comp_reverse);
    output (line, str_amount, fileoutput);
    

    fprintf (fileoutput, "%s", buffer);
    if (ferror (fileoutput))
    {
        printf ("ERROR in function : %s \nwrite in file falled\n", __func__);
        return -1;
    }
    fclose (fileoutput);
    return 1;   
}

int compare_char (const char first_symb, const char second_symb) //!!!!!!!!!!!!!!!comp do universal and better without if and swap//
{
    return (tolower (first_symb) - tolower (second_symb));
}

int output (Onegin* line, const int str_amount, FILE* fileoutput)
{
    if (fileoutput == NULL)
    {
        printf ("ERROR in function : %s \nfileoutput has zero adress\n", __func__);
        return -1;
    }
    for (int ind = 0; ind < str_amount - 1; ind++)
    {
        putsn (line[ind].adress, fileoutput);
    }
    fprintf (fileoutput, "\n----------------------------------------\n");
    return 1;
}
