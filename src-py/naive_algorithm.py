
def find_longest_suffix(history):
    """
    Finds the longest suffix

    Naive implementation.
    """

    length_history = len(history)

    max_depth = 0
    marker_index = None
    best_suffix = None

    # Enumerate possible suffixes
    for index in range(1, length_history):
        suffix = history[index:]

        for offset in range(length_history-len(suffix)):
            # Compare to find if the suffix matches
            proposed_suffix = history[offset:offset+len(suffix)]
            if suffix == proposed_suffix and len(suffix) > max_depth:
                max_depth = len(suffix)
                best_suffix = suffix
                marker_index = offset + len(suffix) - 1

    return (max_depth, marker_index, best_suffix)
