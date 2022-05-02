# Concepts

Реализуйте 2 концепта:

1. Концепт Predicate, который означает, что переданный тип P является унарным предикатом относительно типа T, т.е.
P(T) возвращает bool. Обратите внимание, что возвращаться должен **в точности** тип bool.

2. Используя концепт Sequence, определите концепт Indexable, который означает, что для последовательности S:

    * либо определены функции `std::begin/std::end`, возвращающие random-access итератор (тип которого должен быть определен в
        `S::iterator`).
    * либо для S определен оператор [], принимающий `size_t` и возвращающий какое-то значение (не void).

### Ссылки

* https://en.cppreference.com/w/cpp/language/constraints
* https://www.youtube.com/watch?v=XgrjybKaIV8
* https://www.youtube.com/watch?v=d_E-VLyUnzc
