import sys


def prompt_bit():
    """
    Prompt one bit from the terminal from the user
    """

    valid = {"1": 1, "0": 0}
    prompt = "Enter new bit: "

    while True:
        sys.stdout.write(prompt)
        choice = input()

        if choice in valid:
            yield valid[choice]
        else:
            sys.stdout.write("Please respond with '0' or '1'.\n")
