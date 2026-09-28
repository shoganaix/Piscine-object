# Piscine Object — Module 00: Encapsulation

Implementation of **Piscine Object, Module 00 — Encapsulation** (42 school).

The subject is included in this repository as [`po_module00_subject.pdf`](po_module00_subject.pdf).
This README is not only a manual: the peer evaluation asks *why* each encapsulation
decision was made, so [§6](#6-guía-de-encapsulación) and [§8](#8-decisiones-que-hay-que-defender)
are the parts to study before the defense.

---

## 1. Requirements

| Tool | Version used | Notes |
|------|---------------|-------|
| `c++` (g++) | 11.4.0 | Any C++98 capable compiler works |
| GNU `make` | 4.3 | The Makefiles use `:=` and pattern rules |

No external library is used. No Boost. Nothing to install.

Both exercises are compiled with the exact flags the subject mandates:

```
-Wall -Wextra -Werror -std=c++98
```

## 2. Build and run

```bash
make            # builds ex00 and ex01
./ex00/ex00     # runs the bank
./ex01/ex01     # runs the graph, must be run from the ex01 folder (it reads points.txt)
```

Other goals, available at the root and inside each exercise:

| Goal | Effect |
|------|--------|
| `make` / `make all` | Compiles and links |
| `make clean` | Removes the `.o` files, keeps the executables |
| `make fclean` | Removes the `.o` files **and** the executables |
| `make re` | `fclean` followed by `all`, a full rebuild |

The root `Makefile` does not build anything itself. It only forwards each goal to
`ex00/Makefile` and `ex01/Makefile`, which is where the real rules live. Each of
those keeps its own `$(NAME)`, `all`, `clean`, `fclean` and `re`, as the subject requires.

`make` never relinks when nothing changed:

```
$ make
--- ex00 : all ---
make[1]: Nothing to be done for 'all'.
```

> **Note about `ex01`:** it reads `points.txt` for the file-loading bonus, so it must
> be launched from inside `ex01/`, or the file will not be found. A missing file is
> reported as a clean error, not a crash.

## 3. Repository layout

```
.
├── Makefile                  # Root Makefile, forwards every goal to ex00 and ex01
├── README.md                 # This file
├── .gitignore                # Keeps the .o files and the executables out of git
├── po_module00_subject.pdf   # The subject
│
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

There is deliberately **no `Account.hpp`** in `ex00`. The bonus requires the account
to be *internal* to the bank, and a nested class must be declared inside the body of
its enclosing class, so it cannot live in a header of its own. It is declared at the
top of [`ex00/Bank.hpp:38`](ex00/Bank.hpp).

## 4. Exercise 00 — the bank

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

## 5. Exercise 01 — the graph

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

## 6. Guía de encapsulación

This is the heart of the exercise. Each row is a change from the subject's code and
the reason behind it.

### ex00 — the bank

| Before (subject) | After | Why |
|---|---|---|
| `struct Account { int id; int value; };` | class with `_id` and `_value` **private** | With public attributes anyone could write `account.value = 1000000` and print money from thin air. |
| `Account accountA = Account();` then `accountA.id = 0;` | `bank.createAccount(0, 100);` | The client can no longer invent an account. Creation is an operation the bank performs and controls. |
| `accountA.value += 400;` | `bank.deposit(0, 400);` | The 5% commission lives inside `deposit()`. The client cannot add money without the bank taking its cut, because this is the **only** method that credits an account. |
| `bank.liquidity -= 200;` | no setter at all | A public `setLiquidity` would let anyone mint the bank's money. The liquidity only moves through `deposit()` and `giveLoan()`. |
| `std::vector<Account *> clientAccounts;` with `&accountA` | `std::map<int, Account> _accounts;` | Two reasons. The subject's version stored pointers to stack variables with no `delete` anywhere: a leak waiting to happen. And a vector indexed by position renumbers every account after a deletion, so a client holding the id `1` would silently get a *different* account once account `0` is closed. A map keyed by the id keeps ids stable for the whole life of the account, which is the only correct model for a bank. |
| nothing | `Account(int, int)` **private** + `friend class Bank` | Even with the data private, a public constructor would let a user build a fake account. Only the bank may create one. |
| nothing | copy constructor and assignment declared, never defined | Copying a bank would duplicate its account ids, so two banks would hold accounts sharing the same id, which the subject explicitly forbids. Declaring them private turns an attempt to copy into a compile error instead of a logic bug. |
| nothing | validation + `throw`, handled in `main` | The subject's program indexed an empty vector when given a bad id and crashed. Now it reports the problem and carries on. |
| none | `const Account& getAccount(int) const` | The subject says const getters by copy are not accepted. Returning a reference means the caller inspects the real account, and `const` means the caller cannot touch it. |

### ex01 — the graph

| Before | After | Why |
|---|---|---|
| `int x; int y;` in a `struct` | `_x` and `_y` **private** in a class | The subject asks this question directly. A `Vector2` is a pair of coordinates that carries no invariant of its own, so a standalone one does get public setters. But the moment it lives inside a `Graph`, the setters become dangerous. See below. |
| `std::vector<Vector2> points;` public | private, reachable only through `addPoint()` | **This is the most important decision in ex01.** If the list were public, a user could do `graph.getPoints().push_back(Vector2(99, 99))`. The graph would store a point that does not exist in it, and `display()` would then walk a grid that does not contain that point, so the point would either be invisible or the render loop would index out of range and crash. Only the `Graph` can enforce the invariant "every point is inside the grid". |
| `Vector2 size;` public | private, exposed by `const Vector2& getSize() const` | A user may *read* the size but must never *resize* a graph that already holds points: the stored points would instantly become out of bounds. Returning a `const` reference closes the door. |
| nothing | `checkInside()` called by `addPoint()` | The invariant is enforced at the boundary, before anything is stored. A point at `x == 6` is rejected in a graph of width 6, because the upper bound is exclusive: a 6x6 graph holds columns 0 to 5. |
| `push_back` on every call | `addPoint()` is idempotent, adding an existing point is a no-op | A graph is a **set** of points, not a list. This was found while testing: without it, two segments meeting on a shared corner stored that cell twice. It is also what makes `addLine` compose correctly. The cost is that `addPoint` becomes O(n), which is irrelevant for an ASCII grid and cheaper than the render loop, which already does one lookup per cell. |
| none | `addLine()` reused by `addPoint()` | Every cell of a segment goes through `addPoint()`, so the line feature inherits the bounds validation instead of reimplementing it. |

## 7. Where each requirement is met

### ex00 — the mandatory requirements

| Subject requirement | Where |
|---|---|
| The bank receives 5% of each money inflow | [`ex00/Bank.cpp:138`](ex00/Bank.cpp) `Bank::deposit` |
| The accounts never have two identical IDs | [`ex00/Bank.cpp:116`](ex00/Bank.cpp) `Bank::createAccount` rejects a taken id, and the map key makes it structurally impossible |
| The attributes are not modifiable from the outside | [`ex00/Bank.hpp:54-55`](ex00/Bank.hpp) and [`ex00/Bank.hpp:89-90`](ex00/Bank.hpp), both `private` |
| The bank creates, deletes and modifies accounts | [`ex00/Bank.hpp:73-75`](ex00/Bank.hpp) `createAccount` / `deleteAccount` / `deposit` |
| The bank gives a loan within the limits of its funds | [`ex00/Bank.cpp:152`](ex00/Bank.cpp) `Bank::giveLoan` |
| Impossible to add money without going through the bank | `Account::_value` is private and `Bank` is its only friend; `deposit` is the only method that credits it |
| Getters, not accepted by copy | [`ex00/Bank.hpp:69`](ex00/Bank.hpp) `getAccount` returns `const Account&` |
| Mandatory const getters | [`ex00/Bank.hpp:68-71`](ex00/Bank.hpp), every accessor is marked `const` |

### ex00 — the "Divide and Govern" bonus

| Bonus requirement | Where |
|---|---|
| `operator[]` by id, with no `for` or `while` | [`ex00/Bank.cpp:89`](ex00/Bank.cpp). The body is one expression, the tree descent happens inside `std::map` |
| No methods in `Account` other than const getters | [`ex00/Bank.hpp:42-43`](ex00/Bank.hpp). Only `getId` and `getValue`, both `const` |
| `Account` internal to `Bank` | [`ex00/Bank.hpp:38`](ex00/Bank.hpp), declared inside the body of `Bank` |
| Error handling through `throw`, handled in `main` | [`ex00/main.cpp`](ex00/main.cpp), one `try`/`catch` per misuse plus a final safety net |

### ex01 — the mandatory requirements

| Subject requirement | Where |
|---|---|
| `Vector2` holds an `x` and a `y` | [`ex01/Vector2.hpp:51-52`](ex01/Vector2.hpp) `_x` and `_y`, both `float` |
| A `Graph` with a size and a list of points | [`ex01/Graph.hpp:41-42`](ex01/Graph.hpp) |
| The user can add a point | [`ex01/Graph.cpp:103`](ex01/Graph.cpp) `Graph::addPoint` |
| The user can print the graph on the console | [`ex01/Graph.cpp:149`](ex01/Graph.cpp) `Graph::display` |
| The encapsulation decision must be explainable | [§6](#ex01--the-graph) above |

### ex01 — the "What am i looking at ?!" bonus

| Bonus requirement | Where |
|---|---|
| Add a line feature | [`ex01/Graph.cpp:117`](ex01/Graph.cpp) `Graph::addLine` |
| Read an input file containing a list of points | [`ex01/Graph.cpp:29`](ex01/Graph.cpp) `Graph::fromFile`, sample in [`ex01/points.txt`](ex01/points.txt) |
| A PNG rendering of the graph | **Not done.** Producing a PNG requires an image library, and the subject forbids any external library. See [§8](#8-decisiones-que-hay-que-defender) for the reasoning. |

## 8. Decisiones que hay que defender

These are the points the evaluator is most likely to push on. Each one is a
deliberate trade-off, not an accident.

**Why `std::map` instead of a `std::vector` for the accounts.**
The first draft used a vector with the id equal to the position in the vector, which
made `operator[]` a direct subscript. It was wrong. Deleting an account renumbers
every account after it, so a client holding the id `1` would silently start talking
to a different account the moment account `0` was closed. A bank must never renumber
a live account. The map is O(log n) instead of O(1), and the subject's bonus defines
"PERFECT" as *"works without malfunctioning"*, so correctness wins over the constant
factor. The bonus only forbids writing a `for` or a `while` loop, which is satisfied:
the method body is a single expression and the descent happens inside the standard
library.

**Why `giveLoan` both returns a `bool` and can throw.**
Two different failure modes. An id that does not exist is a *programming error*: the
caller violated a precondition, so it throws. A loan larger than the liquidity is an
*expected business outcome*: the bank simply declines, and the caller is meant to
handle it, so it returns `false` and the caller is expected to test it. Throwing for
a refusal would force `try`/`catch` around a normal decision.

**Why there is no setter anywhere in `Bank`, not even for the liquidity.**
The subject says a setter is mandatory *"if it makes sense"*. It does not make sense
here. A public `setLiquidity` would hand the client a way to mint the bank's money,
which is exactly the hole this module exists to close. The liquidity moves only
through `deposit()` (the 5% commission) and `giveLoan()` (money actually lent out),
and both of those enforce their own rules. The same reasoning removes the setters
from `Account`.

**Why `Vector2` has setters but `Graph` does not expose any way to mutate a point.**
A standalone `Vector2` carries no invariant, so mutating it is harmless and a setter
is justified. A `Vector2` inside a `Graph` is different: the graph has a size, and a
point outside that size corrupts the object. So `Graph::getPoints()` returns
`const std::vector<Vector2>&`, and every `Vector2` reached through it is const. The
setter exists in the class, but the encapsulation of `Graph` makes it unreachable.

**Why the copy constructor of `Bank` is declared but never defined.**
It looks like dead code and it is deliberate. The subject forbids two accounts with
the same id, but it says nothing about two *banks*. If copying a bank were allowed,
`Bank b2 = b1;` would silently produce two banks holding accounts with identical ids,
which is the exact bug the requirement is trying to prevent. Declaring the special
members private and leaving them undefined makes the compiler reject the copy at
compile time, which is the cheapest possible place to catch it.

**Why `Account` has no `Account.hpp`.**
The bonus requires the account to be internal to the bank. A nested class must be
declared inside the body of its enclosing class in the same translation unit, so it
cannot be declared in a separate header. Splitting it out would have meant dropping
the bonus.

**Why `operator[]` on `Bank` returns the balance rather than the account.**
Returning `const Account&` would require the return type to name `Bank::Account`
before the nested class is complete. `operator[]` returning the balance is the
idiomatic C++98 choice, and `getAccount()` is right there when the account itself is
needed. Note that `std::map::operator[]` cannot be used internally either: it would
require `Account` to be default constructible, which would mean exposing a public
default constructor and giving up the private constructor.

**Why `addLine` refuses a diagonal.** A graph is a grid of discrete cells, so a
diagonal has no meaningful cells to fill and any "thickness" would be an arbitrary
choice. Rather than silently pick a rule, `addLine` throws. Two axis-aligned calls
draw a corner, which is what the demonstration does.

**Why the PNG bonus was skipped.** Producing a PNG needs an image library, and the
subject forbids any external library. Writing an encoder from scratch means a
deflate compressor plus a CRC32, which is a sizeable project in itself and would pull
the submission away from the encapsulation the module is actually about. The ASCII
renderer already demonstrates that the graph holds the right data, which is the part
being graded.

## 9. Error handling

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

## 10. References

- The subject: [`po_module00_subject.pdf`](po_module00_subject.pdf)
- `DivideAndRule.cpp`, the unencapsulated starting point, was moved to
  [`ex00/main.cpp`](ex00/main.cpp) and rewritten. The `git mv` keeps the history
  readable.
