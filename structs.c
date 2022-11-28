#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
} Date;

typedef struct
{
    char name[255];
    Date birthdate;
    double salary;
    unsigned int weekly_hours;
} Employee;

unsigned int readu(char *msg);
double readd(char *msg);
Date read_date(char *msg);
Employee read_employee(char *msg);

unsigned int random_u(const unsigned int min, const unsigned int max);

void print_date(const Date date);
void print_employee(const Employee employee);
void print_employees(const Employee employees[], const size_t size);

void increase_salary(Employee *employee, const unsigned int amount);
void hr(Employee employees[], const size_t size, const bool end_of_year);

double hourly_rate(const Employee employee);
size_t highest_salary(const Employee employees[], const size_t size);
size_t highest_hourly_rate(const Employee employees[], const size_t size);
double average_salary(const Employee employees[], const size_t size);
size_t oldest_employee(const Employee employees[], const size_t size);
int cmp_date(const Date date1, const Date date2);
size_t search(const Employee employees[], const size_t size, const char *search);
void swap_employees(Employee *restrict emp1, Employee *restrict emp2);
void sort_by_salary(Employee employees[], const size_t size);
void sort_by_age(Employee employees[], const size_t size);
void sort_by_name(Employee employees[], const size_t size);

int main(int argc, char const *argv[])
{
    const size_t size = readu("How many employees do you have?");
    Employee employees[size];
    for (size_t i = 0; i < size; i++)
    {
        char msg[255];
        sprintf(msg, "Employee #%lu:", i + 1);
        employees[i] = read_employee(msg);
    }

    print_employees(employees, size);

    puts("\nHighest salary:");
    print_employee(employees[highest_salary(employees, size)]);
    puts("\nHighest hourly rate:");
    print_employee(employees[highest_hourly_rate(employees, size)]);
    printf("Average salary:\n%lf\n", average_salary(employees, size));
    puts("\nOldest employee:");
    print_employee(employees[oldest_employee(employees, size)]);

    char name[255];
    puts("Who are you looking for?");
    scanf(" %[^\n]", name);
    const size_t found = search(employees, size, name);
    if (found < size)
    {
        print_employee(employees[found]);
    }
    else
    {
        printf("%s is not an employee here.\n", name);
    }

    sort_by_salary(employees, size);
    print_employees(employees, size);

    sort_by_age(employees, size);
    print_employees(employees, size);

    sort_by_name(employees, size);
    print_employees(employees, size);

    hr(employees, size, true);
    print_employees(employees, size);

    return 0;
}

unsigned int readu(char *msg)
{
    bool ok;
    unsigned int num;
    do
    {
        ok = true;
        puts(msg);
        if (scanf(" %u", &num) != 1)
        {
            while (getchar() != '\n')
                ;
            ok = false;
            puts("Bad input format!");
        }
    } while (!ok);
    return num;
}

double readd(char *msg)
{
    bool ok;
    double num;
    do
    {
        ok = true;
        puts(msg);
        if (scanf(" %lf", &num) != 1 || num < 0)
        {
            while (getchar() != '\n')
                ;
            ok = false;
            puts("Bad input format!");
        }
    } while (!ok);
    return num;
}

Date read_date(char *msg)
{
    bool ok;
    Date date;
    do
    {
        ok = true;
        puts(msg);
        if (scanf("%4u.%2u.%2u", &date.year, &date.month, &date.day) != 3)
        {
            puts("Bad date format! (YYYY:MM:DD)");
            while (getchar() != '\n')
                ;
            ok = false;
        }
    } while (!ok);
    return date;
}

Employee read_employee(char *msg)
{
    Employee employee;
    puts(msg);
    puts("Name:");
    scanf(" %[^\n]", &employee.name);
    employee.birthdate = read_date("Birthdate:");
    employee.salary = readd("Salary:");
    employee.weekly_hours = readu("Weekly hours:");
    return employee;
}

unsigned int random_u(const unsigned int min, const unsigned int max)
{
    return rand() % (max - min) + min;
}

void print_date(const Date date)
{
    printf("%u.%2u.%2u", date.year, date.month, date.day);
}

void print_employee(const Employee employee)
{
    printf("Name: %s\n", employee.name);
    printf("Birthdate: ");
    print_date(employee.birthdate);
    printf("\nSalary (celery): %lf\n", employee.salary);
    printf("Weekly hours: %u\n", employee.weekly_hours);
    printf("Hourly rate: %.2lf\n", hourly_rate(employee));
}

void print_employees(const Employee employees[], const size_t size)
{
    printf("\n| %30s | %10s | %7s | %4s |\n", "Name", "BDate", "Salary", "WHrs");
    for (size_t i = 0; i < size; i++)
    {
        printf("| %30s | ", employees[i].name);
        print_date(employees[i].birthdate);
        printf(" | %6.2lf | %4u |\n", employees[i].salary, employees[i].weekly_hours);
    }
}

void increase_salary(Employee *employee, const unsigned int amount)
{
    employee->salary += amount;
}

void hr(Employee employees[], const size_t size, const bool end_of_year)
{
    if (end_of_year)
    {
        for (size_t i = 0; i < size; i++)
        {
            increase_salary(employees + i, random_u(0, 1000));
        }
    }
}

double hourly_rate(const Employee employee)
{
    return employee.salary / employee.weekly_hours;
}

size_t highest_salary(const Employee employees[], const size_t size)
{
    size_t highest = 0;
    for (size_t i = 1; i < size; i++)
    {
        if (employees[highest].salary < employees[i].salary)
        {
            highest = i;
        }
    }
    return highest;
}

size_t highest_hourly_rate(const Employee employees[], const size_t size)
{
    size_t highest = 0;
    for (size_t i = 1; i < size; i++)
    {
        if (hourly_rate(employees[highest]) < hourly_rate(employees[i]))
        {
            highest = i;
        }
    }
    return highest;
}

double average_salary(const Employee employees[], const size_t size)
{
    double avg = 0;
    for (size_t i = 0; i < size; i++)
    {
        avg += employees[i].salary;
    }
    return avg / size;
}

size_t oldest_employee(const Employee employees[], const size_t size)
{
    size_t oldest = 0;

    for (size_t i = 1; i < size; i++)
    {
        if (cmp_date(employees[oldest].birthdate, employees[i].birthdate) == 1)
        {
            oldest = i;
        }
    }

    return oldest;
}

int cmp_date(const Date date1, const Date date2)
{
    if (date1.year > date2.year)
    {
        return 1;
    }
    else if (date1.year == date2.year)
    {
        if (date1.month > date2.month)
        {
            return 1;
        }
        else if (date1.month == date2.month)
        {
            if (date1.day > date2.day)
            {
                return 1;
            }
            else if (date1.day == date2.day)
            {
                return 0;
            }
        }
    }
    return -1;
}

size_t search(const Employee employees[], const size_t size, const char *search)
{
    size_t found;
    for (found = 0; found < size; found++)
    {
        if (strcmp(employees[found].name, search) == 0)
        {
            break;
        }
    }
    return found;
}

void swap_employees(Employee *restrict emp1, Employee *restrict emp2)
{
    Employee temp = *emp1;
    *emp1 = *emp2;
    *emp2 = temp;
}

void sort_by_salary(Employee employees[], const size_t size)
{

    for (size_t i = 0; i < size - 1; i++)
    {
        size_t max_index = i;
        for (size_t j = i + 1; j < size; j++)
        {
            if (employees[j].salary > employees[i].salary)
            {
                max_index = j;
            }
        }

        if (max_index != i)
        {
            swap_employees(&employees[i], &employees[max_index]);
        }
    }
}

void sort_by_age(Employee employees[], const size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
    {
        size_t oldest_index = i;
        for (size_t j = i + 1; j < size; j++)
        {
            if (cmp_date(employees[oldest_index].birthdate, employees[j].birthdate))
            {
                oldest_index = j;
            }
        }

        if (oldest_index != i)
        {
            swap_employees(&employees[i], &employees[oldest_index]);
        }
    }
}

void sort_by_name(Employee employees[], const size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
    {
        size_t max_index = i;
        for (size_t j = i + 1; j < size; j++)
        {
            int cmp = strcmp(employees[j].name, employees[max_index].name);
            if (cmp < 0)
            {
                max_index = j;
            }
            else if (cmp == 0)
            {
                // sort by age if names are equal
                if (cmp_date(employees[max_index].birthdate, employees[j].birthdate))
                {
                    max_index = j;
                }
            }
        }

        if (max_index != i)
        {
            swap_employees(&employees[i], &employees[max_index]);
        }
    }
}
