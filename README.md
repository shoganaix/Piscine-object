# Piscine Object

Implementation of the 42 **Piscine Object** subjects, in C++98.

| Module | Subject | Subject file | Exercises |
|---|---|---|---|
| 00 | Encapsulation | [`module00/po_module00_subject.pdf`](module00/po_module00_subject.pdf) | `ex00`, `ex01` |
| 01 | Relationship | [`module01/po_module01_subject.pdf`](module01/po_module01_subject.pdf) | `ex00` |

Both are built with `-Wall -Wextra -Werror -std=c++98`, use no library outside
the standard one, and never allocate raw memory.

## Build and run

The three Makefiles form a chain, each one forwarding the goals to the level
below. `make` at the root builds everything:

```
make (root)  ->  make (moduleXX)  ->  make (moduleXX/exYY)
```

```bash
make                       # builds every exercise of every module
make -C module01           # only module 01
make -C module01/ex00      # only one exercise
make re                    # fclean + all, forces a full rebuild
make clean                 # removes the .o files
make fclean                # clean + removes the executables

./module00/ex00/ex00       # runs the bank
./module00/ex01/ex01       # runs the graph
./module01/ex00/ex00       # runs the relationships
```

> **Note about `module00/ex01`:** it reads `points.txt` for the file-loading
> bonus, so it must be launched from inside `ex01/`, or the file will not be
> found. A missing file is reported as a clean error, not a crash.

## Repository layout

```
.
├── Makefile                  # Root, forwards every goal to the module Makefiles
├── module00/Makefile         # Forwards every goal to its own exercises
├── module01/Makefile         # Same, for module 01
├── README.md                 # This file
├── .gitignore                # Keeps the .o files and the executables out of git
│
├── module00
│   ├── po_module00_subject.pdf   # The subject
│   ├── ex00/                     # Exercise 00 — "Divide and conquer"
│   │   ├── Makefile              # NAME = ex00
│   │   ├── main.cpp              # Demonstration, all the error handling lives here
│   │   ├── Bank.hpp              # Bank and, nested inside it, the Account class
│   │   └── Bank.cpp              # Implementation of the above
│   │
│   └── ex01/                     # Exercise 01 — "I don't know what I'm doing!"
│       ├── Makefile              # NAME = ex01
│       ├── main.cpp              # Demonstration, all the error handling lives here
│       ├── Vector2.hpp           # A point or a size, two float components
│       ├── Vector2.cpp           # Implementation of the above
│       ├── Graph.hpp             # A grid holding a set of points
│       ├── Graph.cpp             # Implementation of the above
│       └── points.txt            # Input file for the file-loading bonus
│
└── module01
    ├── po_module01_subject.pdf   # The subject
    └── ex00/                     # The only exercise, built in four stages
        ├── Makefile              # NAME = ex00
        ├── main.cpp              # The demonstration, one block per section
        ├── Position.hpp/.cpp     # IV.1 — the composition ingredient #1
        ├── Statistic.hpp/.cpp    # IV.1 — the composition ingredient #2
        ├── Tool.hpp/.cpp         # IV.3 — the abstract base, IV.2 — the shared counter
        ├── Shovel.hpp/.cpp       # IV.2/IV.3 — a Tool that digs
        ├── Hammer.hpp/.cpp       # IV.3 — a second Tool, so the base is justified
        ├── Worker.hpp/.cpp       # IV.1 composition + IV.2 aggregation + IV.4 association
        └── Workshop.hpp/.cpp     # IV.4 association + the two tool related bonuses
```

---

# Module 00: Encapsulation

`Account` and `Bank` come from the subject's `DivideAndRule.cpp`, where every
attribute was public. The exercise is to seal both classes so that no illogical
action is possible.

## M00.1 Exercise 00 — the bank

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

> **Note about `ex00`:** There is deliberately **no `Account.hpp`** in `ex00`.
> The bonus requires the account to be *internal* to the bank, and a nested class
> must be declared, so it cannot live in a header of its own.
> It is declared at the top of [`module00/ex00/Bank.hpp:38`](module00/ex00/Bank.hpp).

## M00.2 Exercise 01 — the graph

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

And the file-loading bonus, from [`module00/ex01/points.txt`](module00/ex01/points.txt):

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

## M00.3 Error handling

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
| `std::overflow_error` | An account balance or a bank liquidity that would overflow |

---

# Module 01: Relationship

One exercise, `ex00`, built in four stages. Each stage introduces one relationship
between objects, and the four of them end up living in the same `Worker` class.

| § | What the subject asks | Relationship | How it is written |
|---|---|---|---|
| IV.1 | A `Worker` **contains** a `Position` and a `Statistic` | **Composition** | Two members stored **by value** |
| IV.2 | A `Shovel` is given to a worker, taken back, and **survives** the worker | **Aggregation** | A `Tool*` the worker only **borrows** |
| IV.3 | `Shovel` and `Hammer` share a `Tool` base with a pure virtual `use()` | **Inheritance** | `class Tool` abstract, two concrete children |
| IV.4 | A `Workshop` holds workers who sign up and leave freely | **Association** | `std::vector<Worker*>` in both classes |

The difference between the first two rows is the whole point of the module, and the
program prints it: a worker **destroys** his position and his statistic, and
**leaves behind** his shovel.

## M01.1 What the program does

Every constructor, destructor and method prints, because the subject uses that
output during the evaluation to follow which code runs and in which order. The
block titles are the section numbers of the subject, so any line of the trace maps
back to a requirement.

### IV.1 Composition

```
=== IV.1 Composition ===
[Position] ctor   a worker stands at (1, 2, 3)
[Statistic] ctor  born at level 3 with 250 exp
[Worker  ] ctor   worker(1, 2, 3) is born, level 3, 250 exp
Alice is worker(1, 2, 3), level 3, 250 exp
She is handed 0 tool(s) and joined 0 workshop(s): a worker starts with nothing
[Worker  ] dtor   worker(1, 2, 3) is leaving
```

The position and the statistic are built before the worker body runs, in the order
they are declared, and a worker cannot be constructed without both of them.

### IV.2 Aggregation

```
=== IV.2 Aggregation ===
[Tool    ] ctor   a brand new tool, 0 use so far
[Shovel  ] ctor   a shovel is leaning against the wall
[Position] ctor   a worker stands at (4, 5, 6)
[Statistic] ctor  born at level 1 with 10 exp
[Worker  ] ctor   worker(4, 5, 6) is born, level 1, 10 exp
[Worker  ] give   worker(4, 5, 6) holds 1 tool(s)
[Shovel  ] use    digs a hole, use #1
[Worker  ] dtor   worker(4, 5, 6) is leaving
[Worker  ] dtor   worker(4, 5, 6) leaves the shovel behind, it was never his to destroy
The shovel outlived the worker, uses so far : 1
```

That last line of the destructor is the proof: **nothing is deleted**, the tool
belongs to `main`, and the worker may vanish while holding one. Handing the same
shovel to a second worker removes it from the first one:

```
[Worker  ] give   worker(7, 8, 9) holds 1 tool(s)
[Worker  ] give   the shovel is taken away from worker(7, 8, 9)
[Worker  ] take   worker(7, 8, 9) put the shovel back, 0 tool(s) left
[Worker  ] give   worker(0, 0, 0) holds 1 tool(s)
```

The tool knows its holder, so the hand-over is resolved without any global registry
of workers.

### IV.3 Inheritance

```
[Tool    ] ctor   a brand new tool, 0 use so far
[Hammer  ] ctor   a hammer is hanging on the board
[Worker  ] give   worker(5, 5, 5) holds 2 tool(s)
Erin holds 2 tool(s) at once
[Hammer  ] use    breaks a rock, use #1
[Shovel  ] use    digs a hole, use #2
[Worker  ] take   worker(5, 5, 5) put the hammer back, 1 tool(s) left
```

The two `use` lines come from the **same** call on the same `Tool*` array, the loop
cannot tell the two tools apart, and each one still behaves as itself.

### IV.4 Association

```
[Workshop] ctor   "forest camp" opens, any worker is welcome
[Workshop] ctor   "mine" opens, any worker is welcome
[Worker  ] work   worker(1, 1, 1) is signed up nowhere, he stays home
Frank works before signing up, work() returns : 0
[Workshop] enrol  worker(1, 1, 1) joins "forest camp", 1 worker(s) inside
[Workshop] enrol  worker(1, 1, 1) joins "mine", 1 worker(s) inside
Frank is now inside 2 workshops at once
[Workshop] day    "forest camp" opens its doors for 1 worker(s)
[Worker  ] work   worker(1, 1, 1) is heading to 2 workshop(s)
[Workshop] day    "forest camp" closes, the work day is over
[Workshop] day    "mine" opens its doors for 1 worker(s)
[Workshop] day    "mine" closes, the work day is over
[Workshop] release worker(1, 1, 1) leaves "forest camp", 0 worker(s) left
Frank is down to 1 workshop(s)
[Worker  ] dtor   worker(1, 1, 1) is leaving
[Workshop] release worker(1, 1, 1) leaves "mine", 0 worker(s) left
```

One worker is inside two workshops at the same time, `work()` refuses to do
anything while he is registered nowhere, and the last two lines are the destructor
cleanup: a dying worker tells every workshop it belongs to, so no workshop is left
holding a pointer to freed memory.

## M01.2 The public surface

```cpp
struct Position                              // IV.1, plain structure as the subject asks
{
    int x, y, z;
    Position(int x, int y, int z);
};

struct Statistic
{
    int level, exp;
    Statistic(int level, int exp);
};

class Tool                                   // IV.3, abstract, cannot be instantiated
{
public:
    virtual            ~Tool();             // VIRTUAL, mandatory to delete through Tool*
    virtual void       use(void) = 0;       // pure virtual
    virtual std::string getToolName(void) const = 0;

    int        getNumberOfUses(void) const;
    Worker*    getHolder(void) const;

protected:
    Tool(void);                             // only a derived tool may be built
    void       _countUse(void);             // the only writer of the counter

private:
    Tool(const Tool&);                      // declared, never defined: a tool cannot
    Tool& operator=(const Tool&);           // be copied, it has an identity

    friend class Worker;
    void       _setHolder(Worker* holder);

    int         _numberOfUses;
    Worker*     _holder;                    // 0 = the tool is on the shelf
};

class Shovel : public Tool { /* ... */ };    // IV.2 and IV.3
class Hammer : public Tool { /* ... */ };    // IV.3, exists to justify the base class

class Worker                                 // the four relationships meet here
{
public:
    Worker(const Position& coordonnee, const Statistic& stat);
    Worker(const Worker&);                  // declared, never defined
    Worker& operator=(const Worker&);
    ~Worker();                              // clears the back references, deletes nothing

    const Position& getCoordonnee(void) const;
    const Statistic& getStat(void) const;
    std::string     getLabel(void) const;

    void   giveTool(Tool* tool);            // AGGREGATION, steals it if held
    bool   takeTool(Tool* tool);
    size_t getNbTools(void) const;
    bool   hasTool(const std::string& name) const;

    template <typename ToolType>            // BONUS 1, body must stay in the header
    ToolType* getTool(void);                // first tool of that type, or 0

    bool   isRegisteredTo(const Workshop* workshop) const;
    size_t getNbWorkshops(void) const;
    bool   work(void);                      // does nothing when registered nowhere

private:
    friend class Workshop;                   // the other half of the association
    void        _forgetWorkshop(Workshop* workshop);
    void        _checkWorkshops(void);       // BONUS 3
    std::string _label(void) const;

    const Position           _coordonnee;   // COMPOSITION, by value, const
    const Statistic          _stat;         // COMPOSITION, by value, const
    std::vector<Tool*>       _tools;        // AGGREGATION, non owning
    std::vector<Workshop*>   _workshops;    // ASSOCIATION, non owning
};

class Workshop                               // IV.4
{
public:
    Workshop(const std::string& name);
    Workshop(const std::string& name, const std::string& requiredTool);   // BONUS 2
    Workshop(const Workshop&);              // declared, never defined
    Workshop& operator=(const Workshop&);
    ~Workshop();                            // clears the back references, deletes nothing

    const std::string& getName(void) const;
    const std::string& getRequiredTool(void) const;
    size_t             getNbWorkers(void) const;
    bool               hasWorker(const Worker* worker) const;

    bool enrolWorker(Worker* worker);        // refuses without the required tool
    bool releaseWorker(Worker* worker);
    void executeWorkDay(void);

private:
    friend class Worker;                     // the other half of the association
    bool        _findWorker(const Worker* worker, size_t& index) const;
    bool        _hasRequiredTool(const Worker& worker) const;
    void        _checkWorker(Worker& worker);   // BONUS 3

    std::string             _name;
    std::string             _requiredTool;  // empty means anybody is welcome
    std::vector<Worker*>    _workers;       // non owning
};
```

## M01.3 The three bonuses

All three are done. The subject only assesses them if the mandatory part is perfect.

### Bonus 1 — `GetTool<ToolType>()`

Implemented as a template member in [`module01/ex00/Worker.hpp:104`](module01/ex00/Worker.hpp),
`dynamic_cast` on every tool of the toolbox, first match wins, `0` when there is
none. Its body **must** live in the header, a template is compiled where it is
declared:

```
getTool<Shovel>() returned her shovel, getTool<Hammer>() returned nothing
```

### Bonus 2 — a workshop only accepts workers with a given tool

`Workshop` keeps the name of the tool it demands and refuses the others at the door.
The comparison is done on the **name** and not on a `typeid`, because a workshop
holding a `Tool*` prototype would have to instantiate an abstract class to compare
against. RTTI is used only where the caller asked for a type, inside `getTool<T>()`:

```
[Workshop] ctor   "kitchen garden" opens, only a worker with a shovel may come in
[Workshop] enrol  worker(2, 2, 2) is turned away, "kitchen garden" only takes workers with a shovel
[Worker  ] give   worker(2, 2, 2) holds 1 tool(s)
[Workshop] enrol  worker(2, 2, 2) joins "kitchen garden", 1 worker(s) inside
[Workshop] enrol  worker(3, 3, 3) is turned away, "kitchen garden" only takes workers with a shovel
```

### Bonus 3 — a worker who loses his tool is released automatically

Every worker keeps a back reference to the workshops it belongs to, and every change
to the toolbox asks each of them to re-check the worker. The implementation is in
[`module01/ex00/Workshop.hpp:100`](module01/ex00/Workshop.hpp) and the erasure of
both sides happens there, on the vector of the caller, to avoid invalidating the
iteration:

```
[Worker  ] take   worker(2, 2, 2) put the shovel back, 0 tool(s) left
[Workshop] check  worker(2, 2, 2) no longer has a shovel, released from "kitchen garden"
Grace is down to 0 workshop(s), the garden let her go on its own
```

Nobody calls `releaseWorker()` there, and she still leaves.

## M01.4 Design notes

**Nobody owns a tool.** `main` creates the `Shovel` and the `Hammer` on its own
stack, the workers only receive pointers, and neither the worker destructor nor the
workshop destructor ever calls `delete`. There is exactly one owner per object and it
is always the creator.

**The back references are what keep the lists consistent.** `Worker::_workshops` and
`Workshop::_workers` are two halves of one relationship, and each class edits **its
own** vector through a private method guarded by `friend`. Neither side gets a public
setter, and the two lists can never drift apart. That is why the two classes are
friends of each other.

**Two loops iterate backwards, on purpose.** `Worker::~Worker()` and
`Worker::_checkWorkshops()` both walk `_workshops` while a `releaseWorker()` or a
`_forgetWorkshop()` may erase the element the loop is standing on. Going backwards
makes the erasure safe, a forward iterator would be invalidated.

**A copy is refused by a link error.** `Worker`, `Tool` and `Workshop` all declare a
copy constructor and a copy assignment and never define them. Nothing in this exercise
ever needs a second man with the same coordinates, the same tools and the same
workshops, so the operation is closed rather than silently done wrong.

## M01.5 Error handling

The same approach as module 00, applied to the new objects:

1. **No raw memory.** No `new`, no `delete`, no owning pointer. A `std::bad_alloc`
   from a growing `std::vector` is caught by the outermost `catch` in `main` like
   any other standard exception.
2. **Every input is validated where it enters.** A negative coordinate, a negative
   level, an empty required tool name, a null tool, a null worker: each one is
   rejected at the boundary by the object that owns the data.
3. **Every exception is caught individually**, so the demonstration always runs to
   the end:

```
=== Refused operations ===
Caught : Position: coordinates cannot be negative
Caught : Statistic: level and exp cannot be negative
Caught : Workshop: the required tool cannot be an empty name
Caught : Worker: cannot be given a null tool
[Worker  ] take   worker(9, 9, 9) was not holding that tool, refused
Caught : Workshop: cannot enrol a null worker
```

A `takeTool()` for a tool the worker does not hold is **not** an exception: the
requested state is already the current one, so it returns `false` and prints a line.

| Exception | Thrown when |
|---|---|
| `std::invalid_argument` | A negative coordinate, level or exp, an empty required tool name, a null tool, a null worker |
