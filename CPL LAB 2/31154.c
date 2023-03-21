#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

#define MAX_DIGITS 1024

typedef struct BigInt {
    int digits[MAX_DIGITS];
    int size;
    int is_negative;
} BigInt;

void initBigInt(BigInt *n);
BigInt create_bigint(int num);
void setBigInt(BigInt *n, char *str);
void printBigInt(BigInt n);
int compareBigInt(BigInt a, BigInt b);
void addBigInt(BigInt a, BigInt b, BigInt *result);
void subtractBigInt(BigInt a, BigInt b, BigInt *result);
void multiplyBigInt(BigInt a, BigInt b, BigInt *result);
void shiftLeft(BigInt *n);
void divideBigInt(BigInt a, BigInt b, BigInt *result);
void moduloBigInt(BigInt a, BigInt b, BigInt *result);
BigInt factorial(int n);
BigInt divideByTwo(BigInt num);
BigInt multiply(BigInt a, BigInt b);
BigInt power(BigInt base, BigInt exponent);
BigInt subtract(BigInt a, BigInt b);
BigInt add(BigInt a, BigInt b);
int compare(BigInt a, BigInt b);
BigInt sqrtBig(BigInt n);


void initBigInt(BigInt *n) {
    memset(n->digits, 0, sizeof(n->digits));
    n->size = 0;
    n->is_negative = 0;
}

void setBigInt(BigInt *n, char *str) {
    int len = strlen(str);
    initBigInt(n);
    n->is_negative = 0;
    int start = 0;
    if(str[0] == '-')
    {
        n->is_negative = 1;
        start = 1;
    }    
    for (int i = len - 1, j = 0; i >= start; i--, j++) {
        n->digits[j] = str[i] - '0';
    }
    n->size = len - start;
}

void printBigInt(BigInt n) 
{
    if(n.is_negative == 1)
    {
        printf("-");
    }
    for (int i = n.size - 1; i >= 0; i--) {
        printf("%d", n.digits[i]);
    }
    printf("\n");
}

int compareBigInt(BigInt a, BigInt b) 
{
    if(a.is_negative && !b.is_negative)
    {
        return -1;
    }
    if(!a.is_negative && b.is_negative)
    {
        return 1;
    }
    if(a.is_negative && b.is_negative)
    {
        a.is_negative = 0;
        b.is_negative = 0;
        int res = compareBigInt(b, a);
        a.is_negative = 1;
        b.is_negative = 1;
        return res;
    }
    if (a.size > b.size) 
    {
        return 1;
    } else if (a.size < b.size) 
    {
        return -1;
    } else 
    {
        for (int i = a.size - 1; i >= 0; i--) {
            if (a.digits[i] > b.digits[i]) {
                return 1;
            } else if (a.digits[i] < b.digits[i]) {
                return -1;
            }
        }
        return 0;
    }
}

void subtractBigInt(BigInt a, BigInt b, BigInt *result);

void addBigInt(BigInt a, BigInt b, BigInt *result) 
{
    if(a.is_negative == 1 && b.is_negative == 0)
    {
        a.is_negative = 0;
        subtractBigInt(b, a, result);
        a.is_negative =1;
        return;
    }
    if(a.is_negative == 0 && b.is_negative == 1)
    {
        b.is_negative = 0;
        subtractBigInt(a, b, result);
        b.is_negative = 1;
        return;
    }
    if(a.is_negative == 1 && b.is_negative == 1)
    {
        a.is_negative = 0;
        b.is_negative = 0;
        addBigInt(a, b, result);
        result->is_negative = 1;
        a.is_negative = 1;
        b.is_negative = 1;
    }
    int carry = 0;
    result->is_negative = 0;
    for (int i = 0; i < MAX_DIGITS; i++) {
        int sum = a.digits[i] + b.digits[i] + carry;
        result->digits[i] = sum % 10;
        carry = sum / 10;
    }
    result->size = MAX_DIGITS;
    while (result->size > 1 && result->digits[result->size - 1] == 0) {
        result->size--;
    }
}

void subtractBigInt(BigInt a, BigInt b, BigInt *result) 
{
    if(a.is_negative == 0 && b.is_negative == 1)
    {
        b.is_negative = 0;
        addBigInt(a, b, result);
        result->is_negative = 0;
        b.is_negative = 1;
        return;
    }
    if(a.is_negative == 1 && b.is_negative == 0)
    {
        a.is_negative = 0;
        addBigInt(a, b, result);
        result -> is_negative = 1;
        a.is_negative = 1;
        return;
    }
    if(a.is_negative == 1 && b.is_negative == 1)
    {
        a.is_negative = 0;
        b.is_negative = 0;
        subtractBigInt(b, a, result);
        a.is_negative = 1;
        b.is_negative = 1;
        return;
    }
    if(compareBigInt(a, b) < 0)
    {
        result -> is_negative = 1;
        BigInt temp = a;
        a = b;
        b = temp;
    }
    else
    {
        result ->is_negative = 0;
    }

    int borrow = 0;
    for (int i = 0; i < MAX_DIGITS; i++) {
        int diff = a.digits[i] - b.digits[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = diff;
    }
    result->size = MAX_DIGITS;
    while (result->size > 1 && result->digits[result->size - 1] == 0) {
        result->size--;
    }
}

void multiplyBigInt(BigInt a, BigInt b, BigInt *result) 
{
    if(a.size == 1 && a.digits[0] == 0)
    {
        result->is_negative = 0;
        result->size = 1;
        result->digits[0] = 0;
        return;
    }
    if(b.size == 1 && b.digits[0] == 0)
    {
        result -> is_negative = 0;
        result -> size = 1;
        result -> digits[0] = 0;
        return;
    }

    if(a.is_negative == 1 && b.is_negative == 0)
    {
        result -> is_negative = 1;
    }
    else if(a.is_negative == 0 && b.is_negative == 1)
    {
        result -> is_negative = 1;
    }
    else if(a.is_negative == 1 && b.is_negative == 1)
    {
        result -> is_negative = 0;
    }
    else
    {
        result -> is_negative = 0;
    }
    BigInt temp;
    initBigInt(&temp);
    temp.is_negative = 0;
    a.is_negative = 0;
    b.is_negative = 0;
    for (int i = 0; i < a.size; i++) {
        int carry = 0;
        for (int j = 0; j < b.size; j++) {
            int prod = a.digits[i] * b.digits[j] + carry;
            temp.digits[i + j] = prod % 10;
            carry = prod / 10;
        }
        temp.size = a.size + b.size;
        while (temp.size > 1 && temp.digits[temp.size - 1] == 0) {
            temp.size--;
        }
    }
    BigInt sum;
    initBigInt(&sum);
    sum.is_negative = 0;
    for (int i = 0; i < b.size; i++) {
        initBigInt(&temp);
        temp.is_negative = 0;
        int carry = 0;
        for (int j = 0; j < a.size; j++) {
            int prod = b.digits[i] * a.digits[j] + carry;
            temp.digits[i + j] = prod % 10;
            carry = prod / 10;
        }
        temp.size = a.size + b.size;
        while (temp.size > 1 && temp.digits[temp.size - 1] == 0) {
            temp.size--;
        }
        addBigInt(sum, temp, &sum);
    }
    *result = sum;
}

void shiftLeft(BigInt *n) {
    for (int i = n->size - 1; i >= 0; i--) {
        n->digits[i + 1] = n->digits[i];
    }
    n->digits[0] = 0;
    n->size++;
}

void divideBigInt(BigInt a, BigInt b, BigInt *result) 
{
    if(b.size == 1 && b.digits[0] == 0)
    {
        printf("FUCK OFF\n");
        return;
    }
    if(a.size == 1 && a.digits[0] == 0)
    {
        result -> is_negative = 0;
        result->size = 1;
        result->digits[0] = 0;
        return;
    }

    if(a.is_negative == 1 && b.is_negative == 0)
    {
        result -> is_negative = 1;
    }
    else if(a.is_negative == 0 && b.is_negative == 1)
    {
        result -> is_negative = 1;
    }
    else if(a.is_negative == 1 && b.is_negative == 1)
    {
        result -> is_negative = 0;
    }
    else
    {
        result -> is_negative = 0;
    }
    a.is_negative = 0;
    b.is_negative = 0;
    BigInt temp, remainder;
    initBigInt(&temp);
    initBigInt(&remainder);
    temp.is_negative = 0;
    remainder.is_negative = 0;
    result->size = a.size;
    for (int i = a.size - 1; i >= 0; i--) 
    {
        shiftLeft(&remainder);
        remainder.digits[0] = a.digits[i];
        result->digits[i] = 0;
        remainder.is_negative = 0;
        while (compareBigInt(remainder, b) >= 0) 
        {
            result->digits[i]++;
            remainder.is_negative = 0;
            temp.is_negative = 0;
            subtractBigInt(remainder, b, &temp);
            temp.is_negative = 0;
            remainder = temp;
        }
    }
    while (result->size > 1 && result->digits[result->size - 1] == 0) {
        result->size--;
    }
}

void moduloBigInt(BigInt a, BigInt b, BigInt *result) 
{
    int asign = a.is_negative;
    int bsign = b.is_negative;
    a.is_negative = 0;
    b.is_negative = 0;
    BigInt quotient;
    initBigInt(&quotient);
    quotient.is_negative = 0;
    divideBigInt(a, b, &quotient);
    quotient.is_negative = 0;
    multiplyBigInt(b, quotient, result);
    a.is_negative = 0;
    result->is_negative = 0;
    subtractBigInt(a, *result, result);
    if(asign == 1 && bsign == 0)
    {
        result -> is_negative = 1;
    }
    else if(asign == 0 && bsign == 1)
    {
        result -> is_negative = 1;
    }
    else if(asign == 1 && bsign == 1)
    {
        result -> is_negative = 0;
    }
    else
    {
        result -> is_negative = 0;
    }
}

BigInt factorial(int n) { //correct
    BigInt result;
    result.size = 1;
    result.digits[0] = 1;

    for (int i = 1; i <= n; i++) {
        int carry = 0;
        for (int j = 0; j < result.size; j++) {
            int prod = result.digits[j] * i + carry;
            result.digits[j] = prod % 10;
            carry = prod / 10;
        }
        while (carry > 0) {
            result.digits[result.size] = carry % 10;
            carry /= 10;
            result.size++;
        }
    }

    return result;
}

BigInt create_bigint(int num) {
    BigInt result;
    initBigInt(&result);
    if (num < 0) {
        num = -num;
    }
    int i = 0;
    while (num > 0) {
        int rem = num % 10;
        result.digits[i] = rem;
        num = num/10;
        i++; 
    }
    result.size = i;
    return result;
}

BigInt divideByTwo(BigInt num);
BigInt multiply(BigInt a, BigInt b);

BigInt power(BigInt base, BigInt exponent) 
{
    if (exponent.size == 1 && exponent.digits[0] == 0) {
        BigInt result;
        result.size = 1;
        result.digits[0] = 1;
        return result;
    }

    BigInt temp = power(base, divideByTwo(exponent));

    if (exponent.digits[0] % 2 == 0) {
        return multiply(temp, temp);
    } else {
        return multiply(base, multiply(temp, temp));
    }
}

BigInt divideByTwo(BigInt num) {
    BigInt result;
    result.size = num.size;
    int carry = 0;
    for (int i = num.size - 1; i >= 0; i--) {
        int digit = num.digits[i] + carry * 10;
        result.digits[i] = digit / 2;
        carry = digit % 2;
    }
    while (result.size > 1 && result.digits[result.size - 1] == 0) {
        result.size--;
    }
    return result;
}

BigInt multiply(BigInt a, BigInt b) {
    BigInt result;
    result.size = a.size + b.size;
    // fill(result.digits, result.digits + result.size, 0);
    memset(result.digits, 0, sizeof(result.digits));

    for (int i = 0; i < a.size; i++) {
        int carry = 0;
        for (int j = 0; j < b.size; j++) {
            int prod = a.digits[i] * b.digits[j] + carry + result.digits[i + j];
            result.digits[i + j] = prod % 10;
            carry = prod / 10;
        }
        result.digits[i + b.size] = carry;
    }

    while (result.size > 1 && result.digits[result.size - 1] == 0) {
        result.size--;
    }

    return result;
}

BigInt subtract(BigInt a, BigInt b) {
    BigInt result;
    result.size = a.size;
    memset(result.digits, 0, sizeof(result.digits));

    int borrow = 0;
    for (int i = 0; i < result.size; i++) {
        int diff = borrow + a.digits[i];
        if (i < b.size) {
            diff -= b.digits[i];
        }
        if (diff < 0) {
            diff += 10;
            borrow = -1;
        } else {
            borrow = 0;
        }
        result.digits[i] = diff;
    }

    while (result.size > 1 && result.digits[result.size - 1] == 0) {
        result.size--;
    }

    return result;
}

BigInt add(BigInt a, BigInt b) {
    BigInt result;
    // result.size = max(a.size, b.size);
    if(a.size > b.size)
    {
        result.size = a.size;
    }
    else
    {
        result.size = b.size;
    }
    memset(result.digits, 0, sizeof(result.digits));

    int carry = 0;
    for (int i = 0; i < result.size; i++) {
        int sum = carry;
        if (i < a.size) {
            sum += a.digits[i];
        }
        if (i < b.size) {
            sum += b.digits[i];
        }
        result.digits[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry > 0) {
        result.digits[result.size] = carry;
        result.size++;
    }

    return result;
}

int compare(BigInt a, BigInt b) {
    if (a.size != b.size) {
        return a.size - b.size;
    }

    for (int i = a.size - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i]) {
            return a.digits[i] - b.digits[i];
        }
    }

    return 0;
}

BigInt sqrtBig(BigInt n) { //correct
    BigInt left, right, mid;
    left.size = 1;
    left.digits[0] = 0;
    right = n;

    BigInt one;
    one.size = 1;
    one.digits[0] = 1;

    while (compare(left, right) <= 0) {
        mid = divideByTwo(add(left, right));
        BigInt square = multiply(mid, mid);

        if (compare(square, n) == 0) {
            return mid;
        } else if (compare(square, n) < 0) {
            left = add(mid, one);
        } else {
            right = subtract(mid, one);
        }
    }

    return right;
}

void printMod(BigInt n, int x)
{
    if(n.is_negative == 1)
    {
        printf("-");
    }
    if(x < 0)
    {
        x = 0;
    }
    for (int i = n.size - x-1; i >= 0; i--) {
        printf("%d", n.digits[i]);
    }
    printf("\n");
}

void printPower(BigInt n, int sign)
{
    if(sign == 1)
    {
        printf("-");
    }
    for (int i = n.size - 1; i >= 0; i--) {
        printf("%d", n.digits[i]);
    }
    printf("\n");
}

int main() {
    BigInt a, b, result;
    initBigInt(&a);
    initBigInt(&b);
    initBigInt(&result);
    setBigInt(&a, "2");
    // printBigInt(a);
    setBigInt(&b, "10");
    // int x = a.size-b.size;
    // moduloBigInt(a, b, &result);
    // printMod(result, x);
    // divideBigInt(a, b, &result);
    // printBigInt(result);

    // BigInt cr;
    // initBigInt(&cr);
    // cr = create_bigint(145);
    // printBigInt(cr);

    // BigInt f;
    // initBigInt(&f);
    // f = factorial(11);
    // printBigInt(f);

    // multiplyBigInt(a, b, &result);
    // printBigInt(result);

    BigInt p = power(a, b);
    int isa = 0;
    int sign = 0;
    if(a.is_negative == 1)
    {
        isa = 1;
    }
    if(isa == 1 && b.digits[b.size - 1] % 2 != 0)
    {
        sign = 1;
    }
    printPower(p, sign);

    // BigInt sq = sqrtBig(a);
    // printBigInt(sq);

    return 0;
}