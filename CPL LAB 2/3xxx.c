#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIGITS 1024

typedef struct BigInt 
{
    int digits[MAX_DIGITS];
    int size;
    int is_negative;
} BigInt;

void initBigInt(BigInt *n) //correct
{
    memset(n->digits, 0, sizeof(n->digits));
    n->size = 1; //check
    n->is_negative = 0;
}

// Returns a pointer to a new BigInt initialized to the specified value.
// Caller is responsible for freeing the new BigInt with a
// corresponding call to BigInt_free.
BigInt* BigInt_construct(int value)  //correct
{
    BigInt* new_big_int = (BigInt*)malloc(sizeof(BigInt));

    if(value == 0)
    {
        initBigInt(new_big_int);
        return new_big_int;
    }    
    else if(value < 0) 
    {
        new_big_int->is_negative = 1;
        value *= -1;
    } 
    else 
    {
        new_big_int->is_negative = 0;
    }

    new_big_int->size = floor(log10(value)) + 1;

    // Special case for 0
    if(new_big_int->size == 0) 
    {
        new_big_int->size = 1;
    }

    int i;
    for(i = 0; i < new_big_int->size; i++) {
        new_big_int->digits[i] = value % 10;
        value /= 10;
    }

    return new_big_int;
}

void setBigInt(BigInt *n, char *str) //correct
{
    int len = strlen(str);
    memset(n->digits, 0, sizeof(n->digits));
    n->size = 0;
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

void BigInt_assign(BigInt* target, const BigInt* source)
{
    // memset(n->digits, 0, sizeof(n->digits));
    int i;
    for(i = 0; i < source->size; ++i) {
        target->digits[i] = source->digits[i];
    }

    target->is_negative = source->is_negative;
    target->size = source->size;
}

void printBigInt(BigInt* n)  //correct
{
    if(n->is_negative)
    {
        printf("-");
    }
    for (int i = n->size - 1; i >= 0; i--) {
        printf("%d", n->digits[i]);
    }
    printf("\n");
}

// Returns -1 if a < b, 0 if a == b, 1 if a > b 
int compareBigInt(BigInt* a, BigInt* b) //correct
{
    // Quick return if one is negative and the other isn't
    if(a->size > 0 || a->digits[0] > 0 || b->size > 0 || b->digits[0] > 0) 
    {
        if (a->is_negative && !b->is_negative) 
        {
            return -1;
        } else if (!a->is_negative && b->is_negative) 
        {
            return 1;
        }
    }

    if (a->size > b->size) 
    {
        return 1;
    } 
    else if (a->size < b->size) 
    {
        return -1;
    } 
    else 
    {
        for (int i = a->size - 1; i >= 0; i--) 
        {
            if (a->digits[i] > b->digits[i]) 
            {
                return 1;
            } else if (a->digits[i] < b->digits[i]) 
            {
                return -1;
            }
        }
        return 0;
    }
}

void subtractBigInt(BigInt *a, BigInt *b, BigInt *result);

void addBigInt(BigInt* a, BigInt* b, BigInt *result) //correct
{    
    if(a->is_negative == b->is_negative)
    {
        memset(result->digits, 0, sizeof(result->digits));
        result -> is_negative = a -> is_negative;
        int carry = 0;
        for (int i = 0; i < MAX_DIGITS; i++) {
            int sum = a->digits[i] + b->digits[i] + carry;
            result->digits[i] = sum % 10;
            carry = sum / 10;
        }
        result->size = MAX_DIGITS;
        while (result->size > 1 && result->digits[result->size - 1] == 0) {
            result->size--;
        }
    }
    else if(a->is_negative && !b->is_negative)
    {
        a->is_negative = 0;
        subtractBigInt(b, a, result);
        a->is_negative = 1;
        return;
    }
    else if(!a->is_negative && b->is_negative)
    {
        b->is_negative = 0;
        subtractBigInt(a, b, result);
        b->is_negative = 1;
        return;
    }
}

void subtractBigInt(BigInt *a, BigInt *b, BigInt *result) //correct
{
    int borrow = 0;

    memset(result->digits, 0, sizeof(result->digits));
    result -> size = 0;
    result -> is_negative = 0;

    // Check the signs of a and b
    if (a->is_negative && !b->is_negative) 
    {
        // Add |a| + b if a is negative and b is positive
        a->is_negative = 0;
        addBigInt(a, b, result);
        result->is_negative = 1;
        a->is_negative = 1;
        return;
    } 
    else if (!a->is_negative && b->is_negative) 
    {
        // Add a + |b| if a is positive and b is negative
        b->is_negative = 0;
        addBigInt(a, b, result);
        result->is_negative = 0;
        b->is_negative = 1;
        return;
    } 
    else if (a->is_negative && b->is_negative) 
    {
        // Subtract |b| - |a| if a and b are both negative
        a->is_negative = 0;
        b->is_negative = 0;
        subtractBigInt(b, a, result);
        a->is_negative = 1;
        b->is_negative = 1;
        return;
    }

    // If both a and b are positive, subtract them normally
    if (compareBigInt(a, b) < 0) {
        // Swap a and b if a is smaller than b
        BigInt *temp = a;
        a = b;
        b = temp;
        result->is_negative = 1;
    }

    for (int i = 0; i < MAX_DIGITS; i++) {
        int diff = a->digits[i] - b->digits[i] - borrow;
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

void multiplyBigInt(BigInt* a, BigInt* b, BigInt *result) //correct
{
    memset(result->digits, 0, sizeof(result->digits));
    result->size = 0;

    if(a->digits[0] == 0 || b->digits[0] == 0)
    {
        result->is_negative = 0;
        initBigInt(result);
        return;
    }

    BigInt *temp = (BigInt*)malloc(sizeof(BigInt));
    memset(temp->digits, 0, sizeof(temp->digits));
    temp->size = 0;

    int i, j;
    int carry = 0;
    for(i = 0; i < b->size; ++i) {

        if(i > 0) {
            temp->size = i;
            temp->digits[i - 1] = 0;
        }

        for(j = 0; j < a->size || carry > 0; ++j) {
            if(j + i == temp->size) {
                ++temp->size;
            }

            // assert(digits_needed >= j + 1);
           
            int total; 
            if(j < a->size) {
                total = (a->digits[j] * b->digits[i]) + carry;
            } else {
                total = carry;
            }

            temp->digits[i + j] = total % 10;
            carry = total / 10;
        }

        addBigInt(result, temp, result);

        result->is_negative = a->is_negative != b->is_negative;
    }

    BigInt *sum = (BigInt*)malloc(sizeof(BigInt));
    memset(sum->digits, 0, sizeof(sum->digits));
    sum->size = 0;

    for (int i = 0; i < b->size; i++) 
    {
        memset(temp->digits, 0, sizeof(temp->digits));
        temp->size = 0;
        int carry = 0;
        for (int j = 0; j < a->size; j++) 
        {
            int prod = b->digits[i] * a->digits[j] + carry;
            temp->digits[i + j] = prod % 10;
            carry = prod / 10;
        }
        temp->size = a->size + b->size;
        while (temp->size > 1 && temp->digits[temp->size - 1] == 0) {
            temp->size--;
        }
        addBigInt(sum, temp, sum);
    }
    result = sum;
}

void shiftLeft(BigInt *n) //correct
{
    for (int i = n->size - 1; i >= 0; i--) 
    {
        n->digits[i + 1] = n->digits[i];
    }
    n->digits[0] = 0;
    n->size++;
}

// void divideBigInt(BigInt* dividend, BigInt* divisor, BigInt *quotient)
// {
//     // Check for divide by zero error
//     if (divisor->size == 1 && divisor->digits[0] == 0) {
//         printf("Error: Divide by zero");
//         exit(1);
//     }

//     memset(quotient->digits, 0, sizeof(quotient->digits));
//     quotient -> size = dividend->size;;
//     quotient -> is_negative = 0;

//     BigInt *temp = (BigInt*)malloc(sizeof(BigInt));
//     BigInt *remainder = (BigInt*)malloc(sizeof(BigInt));

//     memset(temp->digits, 0, sizeof(temp->digits));
//     temp->size = 0;
//     temp->is_negative = 0;

//     memset(remainder->digits, 0, sizeof(remainder->digits));
//     remainder->size = 0;
//     remainder->is_negative = 0;

//     // initBigInt(&temp);
//     // initBigInt(&remainder);

//     // Store the signs of the dividend and divisor
//     int dividend_sign = dividend->is_negative;
//     int divisor_sign = divisor->is_negative;

//     // Convert both dividend and divisor to their absolute values
//     dividend->is_negative = 0;
//     divisor->is_negative = 0;

//     // quotient->size = dividend->size;
//     for (int i = dividend->size - 1; i >= 0; i--) 
//     {
//         shiftLeft(remainder);
//         remainder->digits[0] = dividend->digits[i];
//         quotient->digits[i] = 0;
//         while (compareBigInt(remainder, divisor) >= 0) 
//         {
//             quotient->digits[i]++;
//             subtractBigInt(remainder, divisor, temp);
//             remainder = temp;
//         }
//     }
//     while (quotient->size > 1 && quotient->digits[quotient->size - 1] == 0) 
//     {
//         quotient->size--;
//     }

//     // Determine the sign of the quotient based on the signs of the dividend and divisor
//     if (dividend_sign != divisor_sign) {
//         quotient->is_negative = 1;
//     }

//     // Restore the signs of the dividend and divisor
//     dividend->is_negative = dividend_sign;
//     divisor->is_negative = divisor_sign;
// }

void divideBigInt(BigInt a, BigInt b, BigInt *result) {
    BigInt temp, remainder;
    initBigInt(&temp);
    initBigInt(&remainder);
    result->size = a.size;
    for (int i = a.size - 1; i >= 0; i--) {
    shiftLeft(&remainder);
    remainder.digits[0] = a.digits[i];
    result->digits[i] = 0;
    while (compareBigInt(&remainder, &b) >= 0) {
    result->digits[i]++;
    subtractBigInt(&remainder, &b, &temp);
    remainder = temp;
    }
    }
    while (result->size > 1 && result->digits[result->size - 1] == 0) {
    result->size--;
    }
}

void digit_shift(BigInt *n, int d)		/* multiply n by 10^d */
{
	int i;				/* counter */

	if ((n->size - 1 == 0) && (n->digits[0] == 0)) return;

	for (i=n->size - 1; i>=0; i--)
		n->digits[i+d] = n->digits[i];

	for (i=0; i<d; i++) n->digits[i] = 0;

	n->size= n->size + d;
}

void zero_justify(BigInt *n)
{
	while ((n->size > 1) && (n->digits[ n->size - 1 ] == 0))
	    (n->size) --;

    if ((n->size-1 == 0) && (n->digits[0] == 0))
    n->is_negative = 0;	/* hack to avoid -0 */
}

void divide_bignum(BigInt *a, BigInt *b, BigInt *c)
{
    BigInt row;                     /* represent shifted row */
    BigInt tmp;                     /* placeholder bignum */
    int asign, bsign;		        /* temporary signs */
    int i,j;                        /* counters */

	initBigInt(c);

	c->is_negative = a->is_negative ^ b->is_negative;

	asign = a->is_negative;
	bsign = b->is_negative;

	a->is_negative = 0;
    b->is_negative = 0;

	// initialize_bignum(&row);
	// initialize_bignum(&tmp);
    initBigInt(&row);
    initBigInt(&tmp);

	c->size = a->size;

	for (i=a->size-1; i>=0; i--) 
    {
		digit_shift(&row,1);
		row.digits[0] = a->digits[i];
		c->digits[i] = 0;
		while (compareBigInt(&row,b) != 1) {
			c->digits[i]++;
			subtractBigInt(&row,b,&tmp);
			row = tmp;
		}
	}

	zero_justify(c);

	a->is_negative = asign;
	b->is_negative = bsign;
}

void moduloBigInt(BigInt a, BigInt b, BigInt *result) 
{
    memset(result->digits, 0, sizeof(result->digits));
    result->size = 1;
    printBigInt(result);
    BigInt *quotient = (BigInt*)malloc(sizeof(BigInt));
    // memset(quotient->digits, 0, sizeof(quotient->digits));
    // quotient->size = 0;
    divideBigInt(a, b, quotient);
    printBigInt(quotient);
    multiplyBigInt(&b, quotient, result);
    printBigInt(result);
    subtractBigInt(&a, result, result);
    printBigInt(result);
}

void divide_by_2(BigInt *a) //correct
{
    int add = 0;
    for (int i = a->size - 1; i >= 0;i--){
        int digit = (a->digits[i] >> 1) + add;
        add = ((a->digits[i] & 1) * 5);
        a->digits[i] = digit;
    }
    while(a->size > 1 && a->digits[a->size-1] == 0)
        a->size--;
}


// BigInt* sqrtBigInt(BigInt *a)
// {
//     // BigInt left(1), right(a), v(1), mid, prod;
//     BigInt *left, *right, *v, *mid, *prod, *one;
//     left = (BigInt*)malloc(sizeof(BigInt));
//     right = (BigInt*)malloc(sizeof(BigInt));
//     v = (BigInt*)malloc(sizeof(BigInt));
//     mid = (BigInt*)malloc(sizeof(BigInt));
//     prod = (BigInt*)malloc(sizeof(BigInt));
//     one = (BigInt*)malloc(sizeof(BigInt));

//     left = BigInt_construct(1);
//     // right = a;
//     BigInt_assign(right, a);
//     v = BigInt_construct(1);
//     mid = BigInt_construct(0);
//     prod = BigInt_construct(0);
//     one = BigInt_construct(1);

//     divide_by_2(right);
//     while(compareBigInt(left, right) <= 0){
//         // mid += left;
//         addBigInt(mid, left, mid);
//         // mid += right;
//         addBigInt(mid, right, mid);
//         divide_by_2(mid);
//         // prod = (mid * mid);
//         multiplyBigInt(mid, mid, prod);
//         if(compareBigInt(prod, a) <= 0)
//         {
//             // v = mid;
//             BigInt_assign(v, mid);
//             // ++mid;
//             addBigInt(mid, one, mid);
//             // left = mid;
//             BigInt_assign(left, mid);
//         }
//         else{
//             // --mid;
//             subtractBigInt(mid, one, mid);
//             // right = mid;
//             BigInt_assign(right, mid);
//         }
//         // mid = BigInt();
//         mid = BigInt_construct(0);
//     }
//     return v;
// }

BigInt* sqrtBigInt(BigInt* x) 
{
    // Initialize variables
    // BigInt lo = BigInt("0");
    // BigInt hi = x;
    // BigInt mid = BigInt("0");

    BigInt* one = (BigInt*)malloc(sizeof(BigInt));
    one = BigInt_construct(1);

    BigInt *lo = (BigInt*)malloc(sizeof(BigInt));
    lo = BigInt_construct(0);
    BigInt *hi = (BigInt*)malloc(sizeof(BigInt));
    BigInt_assign(hi, x);
    BigInt *mid = (BigInt*)malloc(sizeof(BigInt));
    mid = BigInt_construct(0);

    // Use binary search to find the square root
    while (compareBigInt(lo, hi) <= 0) 
    {
        // mid = (lo + hi) / BigInt("2");
        addBigInt(lo, hi, mid);
        divide_by_2(mid);
        // BigInt mid_squared = mid * mid;
        BigInt* mid_squared = (BigInt*)malloc(sizeof(BigInt));
        multiplyBigInt(mid, mid, mid_squared);
        if (compareBigInt(mid_squared, x) == 0) 
        {
            return mid;
        } 
        else if (compareBigInt(mid_squared, x) == -1) 
        {
            // lo = mid + BigInt("1");
            addBigInt(mid, one, lo);
        } 
        else 
        {
            // hi = mid - BigInt("1");
            addBigInt(mid, one, hi);
        }
    }

    // // Return the closest integer to the square root
    // if (mid * mid > x) 
    // {
    //     return mid - BigInt("1");
    // } else 
    // {
        return mid;
    // }
}

BigInt bigIntSqrt(BigInt *num) {
    BigInt res = { {0}, 0, 0 };
    int i, j;

    // If num is negative, return zero
    if (num->is_negative) {
        return res;
    }

    // Determine the size of the result
    int size = (num->size + 1) / 2;
    res.size = size;

    // Initialize the result to zero
    for (i = 0; i < size; i++) {
        res.digits[i] = 0;
    }

    // Calculate the square root
    for (i = size - 1; i >= 0; i--) {
        int digit = 0;
        int tmp = 0;

        // Find the largest digit that fits into the current position
        for (j = 9; j >= 0; j--) {
            tmp = digit * 20 + j * j;
            if (tmp <= num->digits[i]) {
                break;
            }
        }

        // Store the digit in the result and subtract it from num
        res.digits[i] = j;
        tmp *= 10;
        num->digits[i] -= tmp;
        if (i > 0) {
            num->digits[i - 1] -= (j * j) / 10;
        }
    }

    // Remove leading zeros from the result
    while (res.size > 1 && res.digits[res.size - 1] == 0) {
        res.size--;
    }

    return res;
}

BigInt sqrt1(BigInt x) {
    BigInt one;
    setBigInt(&one, "1");
    BigInt left = {0}, right = x, result = {0};
    while (compareBigInt(&left, &right) <= 0) {
        // BigInt mid = divide(add(left, right), 2);
        BigInt mid;
        addBigInt(&left, &right, &mid);
        divide_by_2(&mid);
        // BigInt mid_squared = multiply(mid, mid);
        BigInt mid_squared;
        multiplyBigInt(&mid, &mid, &mid_squared);
        if (compareBigInt(&mid_squared, &x) <= 0) {
            result = mid;
            // left = add(mid, 1);
            addBigInt(&mid, &one, &left);
        } else {
            // right = subtract(mid, 1);
            subtractBigInt(&mid, &one, &right);
        }
    }
    return result;
}

BigInt powerr(BigInt a,const BigInt b){
    BigInt Exponent;
    BigInt Base = a;
    Exponent = b;
    setBigInt(&a, "1");
    while(Exponent.digits[0] != 0){
        if(Exponent.digits[0] & 1)
            // a *= Base;
            multiplyBigInt(&a, &Base, &a);
        // Base *= Base;
        multiplyBigInt(&Base, &Base, &Base);
        divide_by_2(&Exponent);
    }
    return a;
}

BigInt Factorial(int n)
{
    BigInt f;
    initBigInt(&f);
    setBigInt(&f, "1");
    for (int i = 2; i <= n;i++)
    {
        BigInt *temp = (BigInt*)malloc(sizeof(BigInt));
        initBigInt(temp);
        temp = BigInt_construct(i);
        // f *= i;
        multiplyBigInt(&f, temp, &f);
    }
    return f;
}

int main() 
{
    BigInt a, b, result;
    initBigInt(&a);
    initBigInt(&b);
    initBigInt(&result);
    setBigInt(&a, "254");
    setBigInt(&b, "14");
    // moduloBigInt(a, b, &result);
    // printBigInt(result);

    // BigInt* temp = BigInt_construct(2);
    // printBigInt(temp);

    // BigInt* n = (BigInt*)malloc(sizeof(BigInt));
    // initBigInt(n);
    // printBigInt(n);

    // setBigInt(n, "-145782172248258");
    // printBigInt(n);

    // BigInt *num1 = (BigInt*)malloc(sizeof(BigInt));
    // BigInt *num2 = (BigInt*)malloc(sizeof(BigInt));
    // BigInt *q = (BigInt*)malloc(sizeof(BigInt));
    // setBigInt(num1, "16");
    // setBigInt(num2, "17");
    // divide_bignum(num1, num2, q);
    // printBigInt(q);

    // BigInt* sq = sqrtBigInt(num1);
    // sq -> size = 2;
    // printBigInt(sq);

    // divide_by_2(num1);
    // printBigInt(num1);

    // num1 = BigInt_construct(0);
    // printf("%d", num1->size);
    // // printBigInt(num1);

    // int cmp = compareBigInt(num1, num2);
    // printf("%d", cmp);

    // multiplyBigInt(num1, num2, q);
    // // printf("%d", q->size);
    // printBigInt(q);

    // BigInt num;
    // setBigInt(&num, "225");

    // BigInt sq = sqrt1(num);
    // printBigInt(&sq);

    // BigInt p = powerr(a, b);
    // printBigInt(&p);

    BigInt f = Factorial(5);
    printBigInt(&f);

    return 0;
}
