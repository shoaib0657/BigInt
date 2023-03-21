#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIGITS 1024
#define BASE 10000

typedef struct {
    int digits[MAX_DIGITS];
    int length;
} BigInt;

// Helper function to initialize BigInt to zero
void initialize(BigInt *num) {
    int i;
    for (i = 0; i < MAX_DIGITS; i++) {
        num->digits[i] = 0;
    }
    num->length = 1;
}

// Helper function to copy a BigInt
void copy(BigInt *source, BigInt *destination) {
    int i;
    for (i = 0; i < MAX_DIGITS; i++) {
        destination->digits[i] = source->digits[i];
    }
    destination->length = source->length;
}

// Helper function to convert a simple integer to a BigInt
void from_int(BigInt *num, int n) {
    int i = 0;
    while (n > 0) {
        num->digits[i] = n % BASE;
        n /= BASE;
        i++;
    }
    num->length = i;
}

// Helper function to convert a BigInt to a simple integer
int to_int(BigInt *num) {
    int i, n = 0;
    for (i = num->length - 1; i >= 0; i--) {
        n = n * BASE + num->digits[i];
    }
    return n;
}

// Helper function to print a BigInt
void print(BigInt *num) {
    int i;
    printf("%d", num->digits[num->length-1]);
    for (i = num->length - 2; i >= 0; i--) {
        printf("%04d", num->digits[i]);
    }
    printf("\n");
}

// Helper function to compare two BigInts
int compare(BigInt *a, BigInt *b) {
    int i;
    if (a->length != b->length) {
        return a->length - b->length;
    }
    for (i = a->length - 1; i >= 0; i--) {
        if (a->digits[i] != b->digits[i]) {
            return a->digits[i] - b->digits[i];
        }
    }
    return 0;
}

// Helper function to add two BigInts
void add(BigInt *a, BigInt *b, BigInt *result) {
    int carry = 0, i;
    for (i = 0; i < MAX_DIGITS; i++) {
        int sum = a->digits[i] + b->digits[i] + carry;
        result->digits[i] = sum % BASE;
        carry = sum / BASE;
    }
    result->length = MAX_DIGITS;
    while (result->digits[result->length-1] == 0 && result->length > 1) {
        result->length--;
    }
}

// Helper function to subtract two BigInts
void subtract(BigInt *a, BigInt *b, BigInt *result) {
    int borrow = 0, i;
    for (i = 0; i < MAX_DIGITS; i++) {
        int diff = a->digits[i] - b->digits[i] - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = diff;
    }
    result->length = MAX_DIGITS;
    while (result->digits[result->length-1] == 0 && result->length > 1) {
result->length--;
}
}

// Helper function to multiply two BigInts
void multiply(BigInt *a, BigInt *b, BigInt *result) {
int i, j;
initialize(result);
for (i = 0; i < a->length; i++) {
int carry = 0;
for (j = 0; j < b->length; j++) {
int prod = a->digits[i] * b->digits[j] + carry + result->digits[i+j];
result->digits[i+j] = prod % BASE;
carry = prod / BASE;
}
result->digits[i+b->length] = carry;
}
result->length = MAX_DIGITS;
while (result->digits[result->length-1] == 0 && result->length > 1) {
result->length--;
}
}

// Helper function to divide two BigInts and return the remainder
void divide(BigInt *a, BigInt *b, BigInt *quotient, BigInt *remainder) {
int i;
initialize(quotient);
copy(a, remainder);
while (compare(remainder, b) >= 0) {
BigInt temp;
initialize(&temp);
temp.length = remainder->length;
for (i = remainder->length - 1; i >= 0; i--) {
temp.digits[i] = remainder->digits[i];
}
int num_digits = temp.length - b->length + 1;
if (num_digits < 1) {
num_digits = 1;
}
BigInt factor;
from_int(&factor, pow(BASE, num_digits));
multiply(b, &factor, &factor);
while (compare(&temp, &factor) < 0) {
divide(&factor, &BASE, &factor, &temp);
}
while (compare(&temp, b) >= 0) {
subtract(&temp, b, &temp);
add(quotient, &factor, quotient);
}
divide(&factor, &BASE, &factor, remainder);
}
}

// Helper function to calculate the square root of a BigInt
void square_root(BigInt *num, BigInt *result) {
BigInt low, high, mid, temp1, temp2;
initialize(&low);
initialize(&high);
initialize(&mid);
initialize(&temp1);
initialize(&temp2);
int i;
high.digits[(num->length+1)/2] = 1;
high.length = (num->length+1)/2;
while (compare(&low, &high) <= 0) {
add(&low, &high, &temp1);
divide(&temp1, &BASE, &temp1, &temp2);
copy(&temp1, &mid);
multiply(&mid, &mid, &temp1);
if (compare(&temp1, num) > 0) {
copy(&mid, &high);
subtract(&high, &low, &temp1);
divide(&temp1, &BASE, &temp1, &temp2);
copy(&temp1, &high);
} else {
copy(&mid, &low);
subtract(num, &mid, &temp1);
add(&mid, &BASE, &temp2);
divide(&temp2, &BASE, &temp2, &temp1);
copy(&temp2, &low);
}
}
copy(&mid, result);
}

// Function to raise a BigInt to a power
void power(BigInt *base, BigInt *exp, BigInt *result) {
if (exp->digits[0] == 0 && exp->length == 1) {
from_int(result, 1);
return;
}
BigInt temp1, temp2;
initialize(&temp1);
initialize(&temp2);
if (exp->digits[0] % 2 == 0) {
divide(exp, &BASE, &temp1, &temp2);
power(base, &temp1, result);
square_root(result, &temp1);
multiply(&temp1, &temp1, result);
} else {
subtract(exp, &ONE, &temp1);
divide(&temp1, &TWO, &temp1, &temp2);
power(base, &temp1, result);
multiply(result, result, &temp1);
multiply(base, &temp1, result);
}
}

// Function to convert a simple integer to a BigInt
void from_int(BigInt *num, int n) {
initialize(num);
while (n > 0) {
num->digits[num->length++] = n % BASE;
n /= BASE;
}
if (num->length == 0) {
num->length = 1;
}
}

// Function to calculate the factorial of a BigInt
void factorial(BigInt *num, BigInt *result) {
from_int(result, 1);
BigInt temp;
initialize(&temp);
while (compare(num, &ZERO) > 0) {
multiply(result, num, &temp);
copy(&temp, result);
subtract(num, &ONE, &temp);
copy(&temp, num);
}
}

// Main function to test the BigInt operations
int main() {
BigInt a, b, c, d, e;
initialize(&a);
initialize(&b);
initialize(&c);
initialize(&d);
initialize(&e);
from_int(&a, 123456789);
from_int(&b, -987654321);
from_int(&c, 1000000000);
from_int(&d, 2);
from_int(&e, 0);
// Modulo of two BigInts
printf("a mod c = ");
print(&a);
printf(" mod ");
print(&c);
printf(" = ");
modulo(&a, &c, &d);
print(&d);
printf("\n");

// Square root of a BigInt
printf("sqrt(c) = ");
square_root(&c, &d);
print(&d);
printf("\n");

// BigInt raised to a power
printf("c^10 = ");
power(&c, &d, &e);
print(&e);
printf("\n");

// Simple integer to a BigInt
printf("123456789 = ");
print(&a);
printf("\n");

// Factorial of a BigInt
printf("50! = ");
from_int(&a, 50);
factorial(&a, &b);
print(&b);
printf("\n");

return 0;
}
