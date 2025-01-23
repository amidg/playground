class Node:
    def __init__(self):
        self.children = {}
        self.end = False

class PrefixTree:
    def __init__(self):
        self.root = Node()

    def insert(self, word: str) -> None:
        cur = self.root
        for c in word:
            if c not in cur.children:
                # if not in the dictionary
                # add new node with with key of character
                cur.children[c] = Node()
            cur =  cur.children[c]
        cur.end = True

    def search(self, word: str) -> bool:
        # returns true if string word was inserted before
        cur = self.root
        for c in word:
            if c not in cur.children:
                return False
            cur = cur.children[c]
        return cur.end
        

    def startsWith(self, prefix: str) -> bool:
        # returns true if there is a string word that was inserted
        # before that has prefix
        # do should return True if dog is inserted
        cur = self.root
        for c in prefix:
            if c not in cur.children:
                return False
            cur = cur.children[c]
        return True


if __name__ == "__main__":
    trie = PrefixTree()
    trie.insert("dog")
    print(trie.search("do"))
    print(trie.startsWith("do"))

    trie.insert("Hello")
    print(trie.startsWith("Hell"))
    print(trie.search("Hell"))
