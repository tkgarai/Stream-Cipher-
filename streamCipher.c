#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_setbits(long long int n)
{
    int count = 0;
    while(n)
    {
        int val = n & (-n);
        n -= val;
        count++;
    }
    return count;
}

int lfsr(int n, long int pol, long int seed)
{
    static int j = 1;
    long int padder = 0b1 << n-1;
    long int temp = seed & pol;
    int counter = num_setbits(temp);
    if(counter % 2 == 0)
    {
        seed = seed >> 1;
    }
    else
    {
        seed = (seed >> 1) | padder;
    }
    return seed;
}

void main()
{
    FILE *cpr, *key, *msg;
    int ch;
    cpr = fopen("cipher.txt", "w+");
    key = fopen("key.txt", "w+");
    msg = fopen("message.txt", "r");
    long int lfsr1 = 0b0110100, lfsr2 = 0b01100111010, decider = 0b0011100100010;// these are the seeds of the 3 lfsr's
    int keybit;
    if(decider & 1)
        keybit = lfsr2 & 1;
    else
        keybit = lfsr1 & 1;
    fprintf(key, "%d", keybit);
    ch = getc(msg);
    fprintf(cpr, "%d", (ch - 48) ^ keybit);
    for(int i = 0; i < (1 << 20) - 1; i++)
    {
        lfsr1 = lfsr(7, 0b0000011, lfsr1);//middle argument denote the polynomial 1 + x^6 + x^7
        lfsr2 = lfsr(11, 0b00000000101, lfsr2);//middle argument denote the polynomial 1 + x^9 + x^11
        decider = lfsr(13, 0b0000000011011, decider);//middle argument denote the polynomial 1 + x^9 + x^10 + x^12 + x^13
        if(decider & 1)
            keybit = lfsr2 & 1;
        else
            keybit = lfsr1 & 1;
        fprintf(key, "%d", keybit);
        ch = getc(msg);
        fprintf(cpr, "%d", (ch - 48) ^ keybit);//since 0 and 1 has ascii value 48 and 49 respectively
    }
    fclose(cpr);
    fclose(key);
    fclose(msg);
}