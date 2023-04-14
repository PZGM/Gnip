int ft_strlen(const char *s){
    int x = 0;
    while(s && s[x])
        x++;
    return x;
}