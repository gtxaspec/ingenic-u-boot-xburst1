int sort(int *data, int num)
{
    int i = 0;
    int j = 0;
    for(i =0;i < num - 1;i++)
    {
        for(j = (num - 2);j >= i;j--)
        {
            if(data[j] > data[j+1])
            {
                int temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
    return 0;
}

#define TESTCODE                                                                    \
{                                                                                   \
    int i = 0;                                                                      \
    int data1[10] = {0,2,1,3,6,5,4,9,8,7};                                          \
    sort(data1, 10);                                                                \
    for (i = 0; i < 10; i++) {                                                      \
        if (i != data1[i]) {                                                        \
            printf("sort error: i = %d, data[%d] = %d\n", i , i, data1[i]);         \
        }                                                                           \
    }                                                                               \
    int data2[10] = {9,8,7,6,5,4,3,2,1,0};                                          \
    sort(data2, 10);                                                                \
    for (i = 0; i < 10; i++) {                                                      \
        if (i != data2[i]) {                                                        \
            printf("sort error: i = %d, data[%d] = %d\n", i , i, data2[i]);         \
        }                                                                           \
    }                                                                               \
    int data3[10] = {0,5,6,4,3,2,9,1,7,8};                                          \
    sort(data3, 10);                                                                \
    for (i = 0; i < 10; i++) {                                                      \
        if (i != data3[i]) {                                                        \
            printf("sort error: i = %d, data[%d] = %d\n", i , i, data3[i]);         \
        }                                                                           \
    }                                                                               \
    int data4[10] = {5,2,9,1,8,0,4,3,6,7};                                          \
    sort(data4, 10);                                                                \
    for (i = 0; i < 10; i++) {                                                      \
        if (i != data4[i]) {                                                        \
            printf("sort error: i = %d, data[%d] = %d\n", i , i, data4[i]);         \
        }                                                                           \
    }                                                                               \
}

#define TEST_CODES  \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE        \
    TESTCODE

#if 0
int main(int argc, unsigned char *argv[])
{
    TEST_CODES
    return 0;
}
#endif
