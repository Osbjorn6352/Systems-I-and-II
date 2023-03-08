// Homework 4

  // Problem 3.62
  // Fill in the switch statement below

typedef enum { MODE_A, MODE_B, MODE_C, MODE_D, MODE_E } mode_t;


long switch3 (long *p1, long*p2, mode_t action)
{

  long result = 0;
  switch (action) {

  case MODE_A:
    result = *p2;
    action = *p1;
    *p2 = action;
    break;

  case MODE_B:
    result = *p1;
    result += *p2;
    *p1 = result;
    break;

  case MODE_C:
    *p1 = 59;
    result = *p2;
    break;

  case MODE_D:
    result = *p2;
    *p1 = result;
    result = 27;
    break;

  case MODE_E:
    result = 27;
    break;

  default:
    result = 12;

  }
  return result;
}

// Problem 3.65

/*

A.

%rdx stores a pointer to A[i][j]
This can be seen because the first argument assigns what's pointed to to variable t.
It is then referenced again when being set equal to %rsi which holds the value of (%rax).


B.

%rax stores a pointer to A[j][i]
This can be seen as when we assign (%rax) to %rsi and then assign %rsi to (%rdx).
In the program this corresponds to the single step of setting A[i][j] = A[j][i].
This is the steps taken by assembly to set (%rdx) = (%rax).


C.

M = 15
The adding of 8 to %rdx indicates the incrementation of j by 8 bytes (size of the data type)
From this, we can infer that addq $120, %rax is the equivalent of A[j+1][i], or 8*M.
Therfore M is equal to 15.

 */
