import random


def prompt_bit():
    """
    Return a random bit
    """
    for _ in range(1000):
        yield random.getrandbits(1)
