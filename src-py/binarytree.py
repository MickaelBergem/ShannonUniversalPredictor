"""
Binary Tree
"""


class Node(object):
    parent = None
    value = None

    def __init__(self, parent, value):
        self.parent = parent
        self.value = value
