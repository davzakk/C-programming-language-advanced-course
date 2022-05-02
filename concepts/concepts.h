#include <type_traits>
#include <iterator>
#include <concepts>

template <class P, class T>
concept Predicate = requires(P p, T t) {
    { p(t) } -> std::same_as<bool>;
};

template <typename T>
concept IsNotVoid = !std::is_void_v<T>;

template <class T>
concept Indexable = requires(T s, size_t i) {
    { s[i] } -> IsNotVoid;
}
|| requires(T s) {
    std::begin(s) + 3;
    std::end(s) - 3;
};
