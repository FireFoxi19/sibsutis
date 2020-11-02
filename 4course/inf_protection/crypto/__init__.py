"""Криптографическая библиотека"""

__author__ = "Dmitry Samsonov"

import math
import random
import sys
import typing


def fast_pow(base: int, power: int, mod: int = None) -> int:
    """
    Быстрое возведение числа в степень по модулю.

    Реализация алгоритмма имеет трудоёмкость O(log2(power)).
    Показатель не может быть отрицательным, модуль не может быть равным нулю.
    Если модуль не был дан, проводит простое быстрое возведения в степень.

    Пример:

    fast_pow(5, 2206, 22) -> 5
    """
    if mod == 0:
        raise ValueError("Модуль не может быть равен нулю")
    if power < 0:
        raise ValueError("Показатель не может быть отрицательным")
    result = 1
    if mod:
        base %= mod
        result %= mod  # Для правильного вычисления, когда показатель равен 0, а модуль отрицателен
    while power:
        power, remainder = divmod(power, 2)
        if remainder:
            result *= base
            if mod:
                result %= mod
        base *= base
        if mod:
            base %= mod
    return result


def extgcd(a: int, b: int) -> typing.Tuple[int, int, int]:
    """
    Обобщённый алгоритм Евклида.

    Возвращает наибольший общий делитель и их коэффициенты Безу.
    Оба числа должны быть натуральными.

    Пример:

    egcd(12, 8) -> (4, 1, -1), при том 4 = 1 * 12 - 1 * 8
    """
    if a <= 0 or b <= 0:
        raise ValueError("Числа могут быть только натуральными")
    # if a < b:
    #     a, b = b, a  # Ломает алгоритм
    u1, u2, u3 = a, 1, 0
    v1, v2, v3 = b, 0, 1
    while v1:
        q = u1 // v1
        t1, t2, t3 = u1 % v1, u2 - q * v2, u3 - q * v3
        u1, u2, u3 = v1, v2, v3
        v1, v2, v3 = t1, t2, t3
    return u1, u2, u3


def is_prime(n: int) -> bool:
    """Возвращает True, если число n простое, иначе False"""
    if n < 2:
        return False
    return all(n % i for i in range(2, math.isqrt(n) + 1))


def gen_p(a: int, b: int) -> int:
    """Генерирует простое число в диапазоне [a, b]"""
    while True:
        p = random.randint(a, b)
        if is_prime(p):
            return p


def gen_safe_p(a: int, b: int) -> int:
    """Генерирует безопасное простое число в диапазоне [a, b]"""
    while True:
        q = gen_p(a // 2, (b - 1) // 2)
        if is_prime(q * 2 + 1):
            return q * 2 + 1


def gen_g(mod: int) -> int:
    while True:
        g = random.randrange(2, mod)
        if pow(g, (mod - 1) // 2, mod) != 1:
            return g


def gen_public(private_key: int, mod: int):
    """Генерирует открытый ключ из закрытого по модулю"""
    return pow(gen_g(mod), private_key, mod)


def gen_common(secret_key: int, public_key: int, mod: int) -> int:
    """Генерирует общий ключ из закрытого и открытого по модулю"""
    return pow(public_key, secret_key, mod)


def shanks(y: int, a: int, mod: int) -> typing.Union[int, None]:
    """Вычисляет x для выражения y = a ** x % mod"""
    if y >= mod:
        raise ValueError("y не может быть больше или равным mod")
    m = k = math.ceil(math.sqrt(mod))
    seq1 = {pow(a, j, mod) * y % mod: j for j in range(m)}
    seq2 = (pow(a, i * m, mod) for i in range(1, k + 1))
    for i, vel in enumerate(seq2, 1):
        if (j := seq1.get(vel)) is not None:
            return i * m - j
    return None


def gen_mutually_prime(a):
    while True:
        b = random.randrange(2, a)
        if math.gcd(a, b) == 1:
            return b


def write_bytes_to_file(bytes, file):
    for byte in bytes:
        file.write(byte.to_bytes(1, sys.byteorder))


def gen_c_d(p):
    c = gen_mutually_prime(p)
    gcd, d, _ = extgcd(c, p)
    assert gcd == 1
    while d < 0:
        d += p
    return c, d