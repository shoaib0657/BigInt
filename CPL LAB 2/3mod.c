#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 1024

typedef struct BigInt {
    int digits[MAX_DIGITS];
    int size;
} BigInt;

void initBigInt(BigInt *n) {
    memset(n->digits, 0, sizeof(n->digits));
    n->size = 0;
}

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

int main() {
    BigInt a, b, result;
    initBigInt(&a);
    initBigInt(&b);
    initBigInt(&result);
    setBigInt(&a, "81");
    setBigInt(&b, "8");
    moduloBigInt(a, b, &result);
    // divideBigInt(a, b, &result);
    printBigInt(result);
    return 0;
}
