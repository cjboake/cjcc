int test(int x)
{
    int y = x + 2;
    return y; 
}


int test2(int argc, int argv)
{
    int x = argc + argv;
    return test(3);
}
