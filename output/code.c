int add(int a, int b){
    return a + b;
}
int mul(int a, int b){
    return a * b;
}
int stradd(char *a, char *b){
    int alen = 0;
    int blen = 0;
    while (1)
    {
        if (a[alen] == '\0')
        {
            break;
        }
        alen++;
    }
    while (1)
    {
        if (b[blen] == '\0')
        {
            break;
        }
        blen++;
    }
    char *res = malloc(alen + blen + 1);
    for (int i = 0; i < alen; i++)
    {
        res[i] = a[i];
    }
    for (int i = 0; i < blen; i++)
    {
        res[alen + i] = b[i];
    }
    res[alen + blen] = '\0';
    
}

int cmpStrLen(char *a, char *b){
    int alen = 0;
    int blen = 0;
    while (1)
    {
        if (a[alen] == '\0')
        {
            break;
        }
        alen++;
    }
    while (1)
    {
        if (b[blen] == '\0')
        {
            break;
        }
        blen++;
    }
    if (alen == blen)
    {
        return 0;
    }
    else if (alen > blen)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

int main(){
    int a = 5;
    int b = 10;
    int c = add(a, b);
    int d = mul(a, b);
    char str1[7] = "Hello, ";
    char str2[7] = "World!";
    char *str3 = stradd(str1, str2);
    return 0;
}