#define MAXVAR 26

static double vars[MAXVAR];

double getvar(int c);
void setvar(int c, double val);

double getvar(int c) 
{
    return vars[c - 'a'];
}

void setvar(int c, double val)
{
    vars[c - 'a'] = val;
}
