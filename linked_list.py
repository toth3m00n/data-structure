class Node:
    def __init__(self, data: int):
        self.data = data
        self.next = None

    def __str__(self) -> str:
        return str(self.data) + " --> "


class LinkedList:
    def __init__(self, data: int = None):
        if data is not None:
            self.size = 1
            self.head = Node(data)
        else:
            self.size = 1
            self.head = None

    def push_back(self, data: int):
        if self.head is None:
            self.head = Node(data)
        else:
            current = self.head
            while current.next is not None:
                current = current.next
            current.next = Node(data)
        self.size += 1

    def push_front(self, data: int):
        current = Node(data)
        current.next = self.head
        self.head = current
        self.size += 1

    def pop_front(self):
        if self.head is None:
            pass
        else:
            self.head = self.head.next
            self.size -= 1

    def pop_back(self):
        if self.head is None:
            pass
        elif self.head.next is None:
            self.head = None
        else:
            current = self.head
            previous = self.head
            while current.next is not None:
                previous = current
                current = current.next
            previous.next = None
            current = None
        self.size -= 1

    def print_linked_list(self):
        current = self.head
        while current is not None:
            print(current, end="")
            current = current.next
        print("None")

        
    def construct_linked_list(self, lst: list):
        if len(lst) == 0:
            self.head = None
        else:
            self.head = Node(lst[0])
            current = self.head
            for i in range(len(lst) - 1):
                current.next = Node(lst[i + 1])
                current = current.next

def print_lst_by_head(node: Node):
    current = node
    while current:
        print(current, end="")
        current = current.next


if __name__ == "__main__":
    first = LinkedList()
    # enter_in_list = [2, 99, 0, 4]

    # for element in enter_in_list:
    #   first.push_back(element)

    first.print_linked_list()

    print("After: ")
    first.pop_front()
    first.print_linked_list()
