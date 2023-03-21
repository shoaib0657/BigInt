#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 1024

typedef struct BigInt {
    int digits[MAX_DIGITS];
    int size;
    int is_negative;
} BigInt;

BigInt one;
BigInt two;
BigInt ten;

BigInt create_bigint(int num) {
    BigInt result = {0};
    if (num < 0) {
        result.is_negative = 1;
        num = -num;
    }
    while (num > 0) {
        result.digits[result.size++] = num % 10;
        num /= 10;
    }
    return result;
}

// one = create_bigint(1);
// two = create_bigint(2);

BigInt parse_bigint(char* str) {
    BigInt result = {0};
    int len = strlen(str);
    int i, j;
    if (str[0] == '-') {
        result.is_negative = 1;
        i = 1;
    } else {
        i = 0;
    }
    for (j = len - 1; j >= i; j--) {
        result.digits[result.size++] = str[j] - '0';
    }
    return result;
}

void print_bigint(BigInt num) {
    int i;
    if (num.is_negative) {
        printf("-");
    }
    for (i = num.size - 1; i >= 0; i--) {
        printf("%d", num.digits[i]);
    }
}

int compare(BigInt a, BigInt b) {
    int i;
    if (a.is_negative && !b.is_negative) {
        return -1;
    } else if (!a.is_negative && b.is_negative) {
        return 1;
    } else if (a.size < b.size) {
        return -1;
    } else if (a.size > b.size) {
        return 1;
    } else {
        for (i = a.size - 1; i >= 0; i--) {
            if (a.digits[i] < b.digits[i]) {
                return -1;
            } else if (a.digits[i] > b.digits[i]) {
                return 1;
            }
        }
        return 0;
    }
}

BigInt subtract(BigInt a, BigInt b);

BigInt add(BigInt a, BigInt b) {
    BigInt result = {0};
    int carry = 0;
    int i;
    if (a.is_negative && !b.is_negative) {
        b.is_negative = 1;
        return subtract(a, b);
    } else if (!a.is_negative && b.is_negative) {
        a.is_negative = 1;
        return subtract(b, a);
    } else if (a.is_negative && b.is_negative) {
        result.is_negative = 1;
    }
    for (i = 0; i < a.size || i < b.size || carry; i++) {
        int sum = carry;
        if (i < a.size) {
            sum += a.digits[i];
        }
        if (i < b.size) {
            sum += b.digits[i];
        }
        result.digits[i] = sum % 10;
        carry = sum / 10;
        result.size++;
    }
    return result;
}

BigInt subtract(BigInt a, BigInt b) {
    BigInt result = {0};
    int borrow = 0;
    int i;
    if (a.is_negative && !b.is_negative) {
        b.is_negative = 1;
        return add(a, b);
    } else if (!a.is_negative && b.is_negative) {
        b.is_negative = 0;
        return add(a, b);
    } else if (a.is_negative && b.is_negative) {
        b.is_negative = 0;
    a.is_negative = 0;
    return subtract(b, a);
} else if (compare(a, b) < 0) {
    result.is_negative = 1;
    BigInt tmp = a;
    a = b;
    b = tmp;
}
for (i = 0; i < a.size; i++) {
    int diff = a.digits[i] - borrow;
    if (i < b.size) {
        diff -= b.digits[i];
    }
    if (diff < 0) {
        diff += 10;
        borrow = 1;
    } else {
        borrow = 0;
    }
    result.digits[i] = diff;
    if (result.digits[i] != 0) {
        result.size = i + 1;
    }
}
if (result.size == 0) {
    result.size = 1;
}
return result;
}
BigInt multiply(BigInt a, BigInt b) {
BigInt result = {0};
int i, j, carry;
if (a.is_negative && !b.is_negative) {
a.is_negative = 0;
result = multiply(a, b);
a.is_negative = 1;
result.is_negative = 1;
return result;
} else if (!a.is_negative && b.is_negative) {
b.is_negative = 0;
result = multiply(a, b);
b.is_negative = 1;
result.is_negative = 1;
return result;
} else if (a.is_negative && b.is_negative) {
a.is_negative = 0;
b.is_negative = 0;
}
for (i = 0; i < b.size; i++) {
carry = 0;
for (j = 0; j < a.size || carry; j++) {
int product = result.digits[i + j] + carry;
if (j < a.size) {
product += a.digits[j] * b.digits[i];
}
result.digits[i + j] = product % 10;
carry = product / 10;
if (i + j + 1 > result.size && (result.digits[i + j] != 0 || carry != 0)) {
result.size = i + j + 1;
}
}
}
if (result.size == 0) {
result.size = 1;
}
return result;
}

BigInt divide(BigInt a, BigInt b) {
BigInt result = {0};
BigInt current_value = {0};
int i, j;
if (b.size == 1 && b.digits[0] == 0) {
printf("Division by zero error.\n");
return result;
}
if (a.is_negative && !b.is_negative) {
a.is_negative = 0;
result = divide(a, b);
a.is_negative = 1;
result.is_negative = 1;
return result;
} else if (!a.is_negative && b.is_negative) {
b.is_negative = 0;
result = divide(a, b);
b.is_negative = 1;
result.is_negative = 1;
return result;
} else if (a.is_negative && b.is_negative) {
a.is_negative = 0;
b.is_negative = 0;
}
for (i = a.size - 1; i >= 0; i--) {
    current_value = multiply(current_value, ten);
    current_value.digits[0] = a.digits[i];
if (compare(current_value, b) < 0) {
result.digits[i] = 0;
if (result.size == 0 && i == a.size - 1) {
result.size = 0;
}
} else {
int x = 1;
int y = 10;
while (y <= 10) {
y = y * 10 + 1;
x = x * 10;
}
y = y / 10;
x = x / 10;
    while (y > 0) 
    {
        BigInt xb = create_bigint(x);
        while (compare(multiply(b, xb), current_value) <= 0) {
        subtract(current_value, multiply(b, xb));
        result.digits[i] += x;
    }
y = y / 10;
x = x / 10;
}
if (result.digits[i] != 0 && result.size == 0) {
result.size = i + 1;
}
}
}
if (result.size == 0) {
result.size = 1;
}
return result;
}

BigInt power(BigInt a, BigInt b) {
BigInt result = {0};
BigInt current = a;
result.digits[0] = 1;
result.size = 1;
while (compare(b, one) > 0) {
if (b.digits[0] % 2 == 1) {
result = multiply(result, current);
}
current = multiply(current, current);
divide(b, two);
}
return result;
}
// BigInt modulo(BigInt a, BigInt b) {
// BigInt result = {0};
// result.is_negative = a.is_negative;
// for (int i = a.size - 1; i >= 0; i--) {
// shift_left(result);
// result.digits[0] = a.digits[i];
// while (compare(result, b) >= 0) {
// subtract(result, b);
// }
// }
// if (result.size == 0) {
// result.size = 1;
// }
// return result;
// }

// int is_prime(BigInt n) {
// if (compare(n, {2}) < 0) {
// return 0;
// }
// if (n.digits[0] % 2 == 0) {
// return compare(n, {2}) == 0;
// }
// BigInt d = subtract(n, {1});
// int s = 0;
// while (d.digits[0] % 2 == 0) {
// divide(d, {2});
// s++;
// }
// for (int i = 0; i < 10; i++) {
// BigInt a = {0};
// int k = rand() % (n.size - 1) + 1;
// for (int j = 0; j < k; j++) {
// a.digits[j] = rand() % 10;
// }
// a.size = k;
// BigInt x = power(a, d);
// if (compare(x, {1}) == 0 || compare(x, subtract(n, {1})) == 0) {
// continue;
// }
// int prime = 0;
// for (int j = 0; j < s - 1; j++) {
// x = multiply(x, x);
// x = modulo(x, n);
// if (compare(x, {1}) == 0) {
// return 0;
// }
// if (compare(x, subtract(n, {1})) == 0) {
// prime = 1;
// break;
// }
// }
// if (prime == 0) {
// return 0;
// }
// }
// return 1;
// }
// void print_bigint(BigInt a) {
// if (a.is_negative) {
// printf("-");
// }
// for (int i = a.size - 1; i >= 0; i--) {
// printf("%d", a.digits[i]);
// }
// printf("\n");
// }

int main() {
    one = create_bigint(1);
    two = create_bigint(2);
    ten = create_bigint(10);
    // srand(time(NULL));
    // BigInt a = {0};
    // BigInt b = {0};
    // BigInt c = {0};
    // BigInt d = {0};
    // a.digits[0] = 1;
    // a.size = 1;
    // for (int i = 0; i < 1023; i++) {
    //     a = multiply(a, two);
    // }
    // print_bigint(a);
    // b.digits[0] = 2;
    // b.size = 1;
    // c = add(a, b);
    // print_bigint(c);
    // d = divide(c, b);
    // print_bigint(d);
    // // if (is_prime(c)) {
    // // printf("c is prime\n");
    // // } else {
    // // printf("c is not prime\n");
    // // }
    BigInt a = create_bigint(289);
    BigInt b = create_bigint(87);
    BigInt d = divide(a, b);
    print_bigint(d);
    return 0;
}
