#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 309

typedef struct BigInt
{
    int length;     // number of digits in the integer
    int isNegative; // 1 if this BigInt is negative, zero otherwise.
    int *digits;    // array of digits, stored in reverse order (e.g. "123" is stored as [3, 2, 1])
} BigInt;

// Converts input string to BigInt data type
BigInt stringToBigInt(char *s);
// Prints the BigInt
void printBigInt(BigInt *b);
// Returns -1 if a < b, 0 if a == b, 1 if a > b
int BigInt_compare(BigInt *a, BigInt *b);
// Comparing the digits only
int BigInt_compare_digits(BigInt *a, BigInt *b);
// Addition of 2 BigInts
BigInt *addBigInts(BigInt *b1, BigInt *b2);
// Subtraction of 2 BigInts
BigInt *subtractBigInts(BigInt *b1, BigInt *b2);
// subtracts the digits without regard for the sign of either parameter.  Does (b1 - b2) assuming b1 is larger.
BigInt *BigInt_subtract_digits(BigInt *b1, BigInt *b2);
// Multiplication of 2 BigInts
BigInt *multBigInts(BigInt *b1, BigInt *b2);

BigInt stringToBigInt(char *s)
{
    BigInt b;
    int size = strlen(s);
    if (s[0] == '-')
    {
        b.isNegative = 1;
    }
    else
    {
        b.isNegative = 0;
    }
    b.length = size - b.isNegative;
    b.digits = (int *)malloc(sizeof(int) * (b.length));
    int *i;
    i = b.digits;
    size--; // last index
    while (size >= b.isNegative)
    {
        *(i) = s[size] - '0';
        i++;
        size--;
    }
    return b;
}

void printBigInt(BigInt *b)
{
    if (b->isNegative == 1)
    {
        printf("-");
    }
    for (int i = b->length - 1; i >= 0; i--)
    {
        printf("%d", b->digits[i]);
    }
}

int BigInt_compare(BigInt *a, BigInt *b)
{
    // Return if one is negative and the other isn't
    if (a->isNegative && !b->isNegative)
    {
        return -1;
    }
    else if (!a->isNegative && b->isNegative)
    {
        return 1;
    }

    if (a->isNegative == 1)
    {
        return BigInt_compare_digits(b, a);
    }
    else
    {
        return BigInt_compare_digits(a, b);
    }
}

int BigInt_compare_digits(BigInt *a, BigInt *b)
{
    // Not looking at the sign here, comparing the digits only.

    // Return if one number has more digits than the other
    if (a->length > b->length)
    {
        return 1;
    }
    else if (a->length < b->length)
    {
        return -1;
    }

    // Both have the same number of digits, so we actually have to loop through until we
    // find one that doesn't match.
    for (int i = a->length - 1; i >= 0; i--)
    {
        if (a->digits[i] > b->digits[i])
        {
            return 1;
        }
        else if (a->digits[i] < b->digits[i])
        {
            return -1;
        }
    }

    // All digits match; numbers are equal
    return 0;
}

BigInt *addBigInts(BigInt *b1, BigInt *b2)
{
    int n1 = b1->length;
    int n2 = b2->length;
    int carry;
    BigInt *sum = (BigInt *)malloc(sizeof(BigInt));
    if (n2 > n1)
    {
        sum->digits = (int *)malloc(sizeof(int) * (n2 + 1));
    }
    else
    {
        sum->digits = (int *)malloc(sizeof(int) * (n1 + 1));
    }

    // IF BOTH THE NUMBERS ARE OF SAME SIGN
    if (b1->isNegative == b2->isNegative)
    {
        sum->isNegative = b1->isNegative;

        carry = 0;
        int i = 0, digit;
        while (i < n1 && i < n2)
        {
            digit = (b1->digits[i]) + (b2->digits[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->digits[i] = digit;
            i++;
        }
        while (i < n1)
        {
            digit = (b1->digits[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->digits[i] = digit;
            i++;
        }
        while (i < n2)
        {
            digit = (b2->digits[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->digits[i] = digit;
            i++;
        }
        while (carry != 0)
        {
            sum->digits[i] = (carry % 10);
            carry /= 10;
            i++;
        }
        sum->length = i;
    }
    // IF BOTH NUMBERS ARE OF DIFFERENT SIGN
    else
    {
        if (b1->isNegative == 1)
        {
            b1->isNegative = 0;
            sum = subtractBigInts(b2, b1);
            b1->isNegative = 1;
        }
        else
        {
            b2->isNegative = 0;
            sum = subtractBigInts(b1, b2);
            b2->isNegative = 1;
        }
    }
    return sum;
}

// subtracts the digits without regard for the sign of either parameter.  Does (b1 - b2) assuming b1 is larger.
BigInt *BigInt_subtract_digits(BigInt *b1, BigInt *b2)
{
    BigInt *diff = (BigInt *)malloc(sizeof(BigInt));
    diff->digits = (int *)malloc(sizeof(int) * (b1->length));
    int borrow = 0;
    int i = 0, digit;
    while (i < b2->length)
    {
        digit = (b1->digits[i]) - (b2->digits[i]) - borrow;
        if (digit < 0)
        {
            borrow = 1;
            digit += 10;
        }
        else
        {
            borrow = 0;
        }
        diff->digits[i] = digit;
        i++;
    }
    while (i < b1->length)
    {
        digit = (b1->digits[i]) - borrow;
        if (digit < 0)
        {
            borrow = 1;
            digit += 10;
        }
        else
        {
            borrow = 0;
        }
        diff->digits[i] = digit;
        i++;
    }
    diff->length = i;
    return diff;
}

// Subtraction
BigInt *subtractBigInts(BigInt *b1, BigInt *b2)
{
    int n1 = b1->length;
    int n2 = b2->length;
    BigInt *diff = (BigInt *)malloc(sizeof(BigInt));
    if (n2 > n1)
    {
        diff->digits = (int *)malloc(sizeof(int) * n2);
    }
    else
    {
        diff->digits = (int *)malloc(sizeof(int) * n1);
    }

    // if the signed bits are not eqaul
    if (b1->isNegative != b2->isNegative)
    {
        if (b2->isNegative == 1)
        {
            b2->isNegative = 0;
            diff = addBigInts(b1, b2);
            b2->isNegative = 1;
            diff->isNegative = 0;
        }
        else
        {
            b2->isNegative = 1;
            diff = addBigInts(b1, b2);
            b2->isNegative = 0;
            diff->isNegative = 1;
        }
    }
    else
    {
        if (BigInt_compare_digits(b1, b2) > 0) // magnitude of b1>b2
        {
            diff = BigInt_subtract_digits(b1, b2); // subtraction of magnitudes
            diff->isNegative = b1->isNegative;     // b1-b2------gets sign of b1
        }
        else
        {
            diff = BigInt_subtract_digits(b2, b1);
            if (b1->isNegative == 0) // b1-b2 and b1<b2 ---> diff has opp sign of b1
            {
                diff->isNegative = 1;
            }
            else
            {
                diff->isNegative = 0;
            }
        }
    }
    return diff;
}

// MULTIPLICATION
BigInt *multBigInts(BigInt *b1, BigInt *b2)
{
    BigInt *mult = (BigInt *)malloc(sizeof(BigInt));
    mult->length = b1->length + b2->length; // maximum size of the result
    mult->digits = (int *)malloc(sizeof(int) * (mult->length));

    // carry, prod;

    for (int i = 0; i < mult->length; i++) // initialising
        mult->digits[i] = 0;

    for (int i = 0; i < b1->length; i++)
    {
        int carry = 0;
        int j;
        for (j = 0; j < b2->length; j++)
        {
            int prod = (b1->digits[i] * b2->digits[j]) + carry;
            carry = prod / 10;
            mult->digits[i + j] += (prod % 10);
            if (mult->digits[i + j] > 9)
            {
                mult->digits[i + j + 1] += 1;
                (mult->digits[i + j]) %= 10;
            }
        }
        while (carry != 0)
        {
            mult->digits[i + j] += (carry % 10);
            if (mult->digits[i + j] > 9)
            {
                mult->digits[i + j + 1] += 1;
                (mult->digits[i + j]) %= 10;
            }
            carry /= 10;
            j++;
        }
    }

    if (b1->isNegative == b2->isNegative)
    {
        mult->isNegative = 0;
    }
    else
    {
        mult->isNegative = 1;
    }

    int i = mult->length - 1;
    while (i >= 0 && mult->digits[i] == 0)
    {
        i--;
        (mult->length)--;
    }
    if (mult->length == 0) // if input in 0, product is 0
    {
        mult->isNegative = 0; // if mult by negative number, - sign comes
        mult->length = 1;
    }

    return mult;
}

int main()
{
    BigInt num1, num2;
    char *n1, *n2;
    n1 = (char *)malloc(sizeof(char) * SIZE); // DYNAMIC ALLOCATION
    n2 = (char *)malloc(sizeof(char) * SIZE);

    printf("Enter number 1 : ");
    scanf("%s", n1);
    printf("Enter number 2 : ");
    scanf("%s", n2);

    num1 = stringToBigInt(n1); // CONVERTING STRING TO BIG INT
    num2 = stringToBigInt(n2);

    BigInt *ans = addBigInts(&num1, &num2);
    printf("SUM OF 2 NUMBERS : ");
    printBigInt(ans);

    ans = subtractBigInts(&num1, &num2);
    printf("\nDIFFERENCE OF 2 NUMBERS : ");
    printBigInt(ans);

    ans = multBigInts(&num1, &num2);
    printf("\nPRODUCT OF 2 NUMBERS : ");
    printBigInt(ans);
    return 0;
}