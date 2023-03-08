float ft_pow(float base, int power)
{
    while (--power)
    {
        base *= base;
    }
    return base;
}

float ft_abs(float num)
{
    return (num < 0) ? -num : num;
}

float ft_sqrt(float num)
{
    float acc = 0.0000001;
    float guess = 1.0;
    while (ft_abs(num/guess -guess) > acc )
        guess = (guess+num/guess)/2.0;
    return guess;
}