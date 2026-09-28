# Piscine Object
## Module 00: Encapsulation

Implementation of **Piscine Object, Module 00 — Encapsulation** (42 school)

The subject is included in this repository as [`po_module00_subject.pdf`](po_module00_subject.pdf).

## 1. Build and run

```bash
make            # builds ex00 and ex01
./ex00/ex00     # runs the bank
./ex01/ex01     # runs the graph, must be run from the ex01 folder (it reads points.txt)
```

> **Note about `ex01`:** it reads `points.txt` for the file-loading bonus, so it must
> be launched from inside `ex01/`, or the file will not be found. A missing file is
> reported as a clean error, not a crash.

## 2. Repository layout

```
.
├── Makefile                  # Root Makefile, forwards every goal to ex00 and ex01
├── README.md                 # This file
├── .gitignore                # Keeps the .o files and the executables out of git
├── module00
    ├── po_module00_subject.pdf   # The subject
    ├── ex00/                     # Exercise 00 — "Divide and conquer"
    │   ├── Makefile              # NAME = ex00
    │   ├── main.cpp              # Demonstration, all the error handling lives here
    │   ├── Bank.hpp              # Bank and, nested inside it, the Account class
    │   └── Bank.cpp              # Implementation of the above
    │
    └── ex01/                     # Exercise 01 — "I don't know what I'm doing!"
        ├── Makefile              # NAME = ex01
        ├── main.cpp              # Demonstration, all the error handling lives here
        ├── Vector2.hpp           # A point or a size, two float components
        ├── Vector2.cpp           # Implementation of the above
        ├── Graph.hpp             # A grid holding a set of points
        ├── Graph.cpp             # Implementation of the above
        └── points.txt            # Input file for the file-loading bonus
```

> **Note about `ex00`:** There is deliberately **no `Account.hpp`** in `ex00`.
> The bonus requires the account to be *internal* to the bank, and a nested class must be declared, so it cannot live in a header of its own.
> It is declared at the top of [`ex00/Bank.hpp:38`](ex00/Bank.hpp).

## 3. Exercise 00 — the bank

`Account` and `Bank` come from the subject's `DivideAndRule.cpp`, where every
attribute was public. The exercise is to seal both classes so that no illogical
action is possible.

### What the program does

```
=== Creation of the bank ===
Bank informations :
Liquidity : 999

=== Account creation ===
Bank informations :
Liquidity : 999
[0] - [100]
[1] - [100]

=== Deposit, the bank keeps 5% ===
Alice now owns : 480
Bank liquidity : 1019
Alice id is still : 0
=== Loans ===
Loan of 200 granted : 1
Bob now owns : 300
Loan of 100000 granted : 0
Liquidity untouched : 819
=== Account deletion ===
Accounts left : 1
Bob id after the deletion : 1
Bob now owns : 300
Bank informations :
Liquidity : 819
[1] - [300]

=== Refused operations ===
Caught : Bank: unknown account id
Caught : Bank: amount must be strictly positive
Caught : Bank: this account id is already taken
Bob was not altered : 300
```

The arithmetic is the proof that the encapsulation works: a deposit of 400 leaves
the client with `100 + 380 = 480` and gives the bank `999 + 20 = 1019`, because the
5% commission lives inside `deposit()` and there is no way to reach it from outside.

### The public surface

```cpp
class Bank
{
    class Account
    {
    public:                            // Read-only, const getters only
        int getId() const;
        int getValue() const;
    private:                           // Bank is the only class allowed in here
        Account(int id, int value);
        int _id;
        int _value;
        friend class Bank;
    };

    explicit Bank(int liquidity);
    Bank(const Bank&);                 // Declared, never defined: a bank cannot be copied
    Bank& operator=(const Bank&);

    int                operator[](int id) const;
    const Account&     getAccount(int id) const;
    int                getLiquidity() const;
    size_t             getNbAccounts() const;

    void createAccount(int id, int value);
    void deleteAccount(int id);
    void deposit(int id, int amount);
    bool giveLoan(int id, int amount);
    void print(std::ostream& os) const;
};
```

## 4. Exercise 01 — the graph

`Vector2` is a container for two `float` components. `Graph` holds a size and a list
of points, and renders them as ASCII art.

### What the program does

The first block is the exact example from the subject, produced byte for byte:

```
>& 0 1 2 3 4 5
>& 0 X . . . . .
>& 1 . . . . . .
>& 2 . . X . X .
>& 3 . . . . . .
>& 4 . . X . . .
>& 5 . . . . . .
```

The line bonus, an horizontal segment followed by a vertical one:

```
>& 0 1 2 3 4 5 6 7
>& 0 . . . . . . . .
>& 1 X X X X X X X .
>& 2 . . . . . . X .
>& 3 . . . . . . X .
```

And the file-loading bonus, from [`ex01/points.txt`](ex01/points.txt):

```
>& 0 1 2 3 4 5 6 7 8 9
>& 0 X . . . X X . . . X
>& 1 . . . . X X . . . .
>& 2 . . . . X X . . . .
>& 3 . . . . X X . . . .
>& 4 . . . . X X . . . .
>& 5 X . . . X X . . . X
```

The file format is one point per line as `x y`. Blank lines are skipped, and
everything after a `#` is a comment. Every point still goes through `addPoint()`, so
the bounds validation applies exactly as if the points had been typed in by hand.

### The public surface

```cpp
class Graph
{
public:
    Graph();
    explicit Graph(const Vector2& size);
    static Graph fromFile(const char* path, const Vector2& size);

    const Vector2&              getSize() const;
    const std::vector<Vector2>& getPoints() const;
    size_t                      getNbPoints() const;

    void addPoint(const Vector2& point);
    void addLine(const Vector2& from, const Vector2& to);
    void display(std::ostream& os) const;

private:
    bool hasPoint(float x, float y) const;
    void checkInside(const Vector2& point) const;

    Vector2                 _size;
    std::vector<Vector2>    _points;
};
```

## 5. Error handling

The subject requires that the program never crashes, even when it runs out of memory.
Three things were done to make that true:

1. **No raw memory management.** There is no `new` and no `delete` anywhere. Every
   object is stored by value inside a standard container, which frees itself. An
   out-of-memory condition still throws `std::bad_alloc`, and it is caught by the
   outermost `catch` in `main` like any other standard exception.
2. **Every external input is validated at the boundary.** An unknown id, a
   non-positive amount, a negative opening balance, a point outside the grid, a
   missing file: all of them are rejected with a specific exception carrying a
   readable message.
3. **Every exception is handled.** Each misuse is wrapped in its own `try`/`catch`
   so the demonstration keeps going, and a final `catch` in `main` turns anything
   that slips through into a clean message and a non-zero exit code.

The exceptions used are the standard ones, from `<stdexcept>`:

| Exception | Thrown when |
|---|---|
| `std::out_of_range` | An account id or a graph point that does not exist |
| `std::invalid_argument` | A non-positive amount, a negative id or size, a taken id, a missing file |

