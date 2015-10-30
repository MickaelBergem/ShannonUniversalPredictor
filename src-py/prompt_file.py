def prompt_file(filename):
    """
    Read the input from a file, one bit after another, without any spacer
    """

    with open(filename, 'r') as file:
        fixture = list(file.read().strip())

    print("Imported {}".format(fixture))

    def prompt_bit():
        for bit in fixture:
            yield int(bit)

    return prompt_bit
