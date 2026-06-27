#include <stdio.h>

static char daytab[2][13] = {
    { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

int day_of_year(int year, int month, int day)
{
    if (year < 0 || month < 1 || month > 12)
        return -1;
    int i, leap;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    if (day < 1 || day > daytab[leap][month])
        return -1;
    for (i = 1; i < month; i++)
        day += daytab[leap][i];
    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
    if (pmonth == NULL || pday == NULL)
        return;
    if (year < 0) {
        *pmonth = -1;
        *pday = -1;
        return;
    }
    int i, leap, maxday;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    maxday = leap ? 366 : 365;
    if (yearday < 1 || yearday > maxday) {
        *pmonth = -1;
        *pday = -1;
        return;
    }
    for (i = 1; yearday > daytab[leap][i]; i++)
        yearday -= daytab[leap][i];
    *pmonth = i;
    *pday = yearday;
}

int main()
{
    int year, month, yearday, day;
    year = 2026;
    yearday = 33;
    month = 0;
    day = 0;
    month_day(year, yearday, &month, &day);
    printf("Year: %d, Yearday: %d. Calculdated month: %d day: %d\n", year, yearday, month, day);
    year = -1;
    month_day(year, yearday, &month, &day);
    printf("Year: %d, Yearday: %d. Calculdated month: %d day: %d\n", year, yearday, month, day);
    year = 2000;
    yearday = 400;
    month_day(year, yearday, &month, &day);
    printf("Year: %d, Yearday: %d. Calculdated month: %d day: %d\n", year, yearday, month, day);
    return 0;
}
