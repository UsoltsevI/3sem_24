# Terminal emulator
Computer technologies, task 1, Terminal emulator.

## Author
Усолцьев Иван Алексеевич, студент 2-го гурса ФРКТ МФТИ, группа Б01-307.

## Usage
To build the project go to [UsoltsevI/task1/](UsoltsevI/task1/) folder and run the following command:
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
```

Then use it:
```
VERBOSE=1 cmake --build ./build
```

Finally, to run the program, tap:
```
./build/uso-terminal
```

*note:* You _have to_ use cmake to build the project if you don't want to dealing with hemorrhoids.
