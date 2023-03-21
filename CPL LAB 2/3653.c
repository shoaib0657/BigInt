#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

#define MAX_DIGITS 1024

typedef struct BigInt {
    int digits[MAX_DIGITS];
    int size;
} BigInt;

void initBigInt(BigInt *n) {
    memset(n->digits, 0, sizeof(n->digits));
    n->size = 0;
}

BigInt create_bigint(int num);

void setBigInt(BigInt *n, char *str) {
    int len = strlen(str);
    initBigInt(n);
    for (int i = len - 1, j = 0; i >= 0; i--, j++) {
        n->digits[j] = str[i] - '0';
    }
    n->size = len;
}

void printBigInt(BigInt n) {
    for (int i = n.size - 1; i >= 0; i--) {
        printf("%d", n.digits[i]);
    }
    printf("\n");
}

int compareBigInt(BigInt a, BigInt b) {
    if (a.size > b.size) {
        return 1;
    } else if (a.size < b.size) {
        return -1;
    } else {
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

void addBigInt(BigInt a, BigInt b, BigInt *result) {
    int carry = 0;
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

void subtractBigInt(BigInt a, BigInt b, BigInt *result) {
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

void multiplyBigInt(BigInt a, BigInt b, BigInt *result) {
    BigInt temp;
    initBigInt(&temp);
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
    for (int i = 0; i < b.size; i++) {
        initBigInt(&temp);
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

void divideBigInt(BigInt a, BigInt b, BigInt *result) {
    BigInt temp, remainder;
    initBigInt(&temp);
    initBigInt(&remainder);
    result->size = a.size;
    for (int i = a.size - 1; i >= 0; i--) {
    shiftLeft(&remainder);
    remainder.digits[0] = a.digits[i];
    result->digits[i] = 0;
    while (compareBigInt(remainder, b) >= 0) {
    result->digits[i]++;
    subtractBigInt(remainder, b, &temp);
    remainder = temp;
    }
    }
    while (result->size > 1 && result->digits[result->size - 1] == 0) {
    result->size--;
    }
}

void moduloBigInt(BigInt a, BigInt b, BigInt *result) {
    BigInt quotient;
    initBigInt(&quotient);
    divideBigInt(a, b, &quotient);
    multiplyBigInt(b, quotient, result);
    subtractBigInt(a, *result, result);
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
        // result.digits[result.size++] = num % 10;
        // num /= 10;
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

BigInt power(BigInt base, BigInt exponent) {
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

BigInt sqrtBig(BigInt n) {
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

void printMod(BigInt n)
{
    for (int i = n.size - 2; i >= 0; i--) {
        printf("%d", n.digits[i]);
    }
    printf("\n");
}

int main() {
    BigInt a, b, result;
    initBigInt(&a);
    initBigInt(&b);
    initBigInt(&result);
    setBigInt(&a, "47551");
    setBigInt(&b, "7475");
    moduloBigInt(a, b, &result);
    printMod(result);
    divideBigInt(a, b, &result);
    // printBigInt(result);

    // BigInt cr;
    // initBigInt(&cr);
    // cr = create_bigint(145);
    // printBigInt(cr);

    // BigInt f;
    // initBigInt(&f);
    // f = factorial(11);
    // printBigInt(f);

    multiplyBigInt(a, b, &result);
    // printBigInt(result);

    // BigInt p = power(a, b);
    // printBigInt(p);

    // BigInt sq = sqrtBig(b);
    // printBigInt(sq);

    return 0;
}