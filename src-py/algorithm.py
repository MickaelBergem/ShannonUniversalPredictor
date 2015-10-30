"""
Base algorithm
"""
import sys
from binarytree import Node
from prompt_console import prompt_bit as prompt_bit_console
from prompt_file import prompt_file as prompt_bit_file
from prompt_random import prompt_bit as prompt_bit_random
from naive_algorithm import find_longest_suffix as find_longest_suffix_naive


def prompt_fake():
    """ Fake a prompter and returned a predefined string """
    fixture = list("0101110110")
    for bit in fixture:
        yield int(bit)


def start_algorithm(prompter, longest_suffix_algorithm):
    """ Run the given algorithm with the given prompter """

    # List to store the history of bits
    history = []

    prediction = None
    predictions_sucess = 0
    predictions_total = 0

    for input_bit in prompter():
        history.append(input_bit)

        if prediction is not None:
            print("\tPrediction was {}, ".format(prediction), end="")
            if prediction == input_bit:
                predictions_sucess += 1
            print("success_rate {}%".format(
                round(predictions_sucess * 100. / predictions_total)))

        (max_depth, marker_index, best_suffix) = longest_suffix_algorithm(history)

        print("\tDepth: {}, marker_index={} suffix={}".format(
            max_depth, marker_index, best_suffix))

        # print("New history: {}".format(history))

        if marker_index is not None:
            prediction = history[marker_index + 1]
            predictions_total += 1


if __name__ == '__main__':
    import argparse

    algorithms = {
        'naive': find_longest_suffix_naive,
    }

    prompters = {
        'console': prompt_bit_console,
        'fixture': prompt_fake,
        'file': prompt_bit_file,
        'random': prompt_bit_random,
    }

    parser = argparse.ArgumentParser(description="Shannon Universal Predictor.")
    parser.add_argument('--algorithm', '-a', default='naive',
                        choices=algorithms.keys(),
                        help='the filename of the communities PNG graph'
                             ' to be written')
    parser.add_argument('--prompter', '-p', default='console',
                        choices=prompters.keys(),
                        help='the prompter to use')
    parser.add_argument('--input-file', '-i', help='the file to read from '
                        'when using the '
                        'file prompter')
    args = parser.parse_args()

    longest_suffix_algorithm = algorithms[args.algorithm]

    if args.prompter == 'file':
        if not args.input_file:
            print("Please provide a file with the --input-file parameter")
            sys.exit(1)
        prompter = prompters[args.prompter](args.input_file)
    else:
        prompter = prompters[args.prompter]

    start_algorithm(prompter, longest_suffix_algorithm)
