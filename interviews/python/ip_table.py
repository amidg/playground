"""
Part 1:
We want to construct an IP routing table that matches binary IP addresses with a port based on the longest matching prefix.

It needs to support 2 functions:
- insert (ipPrefix: string, port: number): void
- lookup(ip: string): number.

IP addresses are represented as binary strings that are 6 characters long.

Examples:
IP Prefix   Port
ニニニニニニニ
10       => 3
010      => 4
1111     => 5
ニニニニニニ
lookup ("010110" ) = 4
lookup ("111111")  = 5

Part 2:
- Add default port to be 999
- Return -1 if lookup cannot find port and no default port provided
  otherwise provide default port


Part 3:
Support real IPv4 addresses:
IP Prefix    Port
==================
34.126       => 301
34.120       => 120

lookup("34.126.7.1") = 301
lookup("34.120.8.1") = 120
lookup("192.168.0.1") = -1
"""

class Node:
    def __init__(self):
        self.children = {}
        self.port = 0

class PrefixTree:
    def __init__(self):
        self.root = Node()
        self.defaultPort = self.root.port

    def setDefaultPort(self, port: int):
        self.defaultPort = port

    def insert(self, ipPrefix: str, port: int) -> None:
        # check if is IPv4
        ipPrefix = ipPrefix.replace('.', '')
        cur = self.root
        for c in ipPrefix:
            if c not in cur.children:
                cur.children[c] = Node()
            cur =  cur.children[c]
        cur.port = port

    def lookup(self, ip: str) -> int:
        cur = self.root
        # check for IPv4
        # we don't start IPs with .
        ip = ip.replace('.', '')

        if ip[0] == ".":
            raise ValueError("Wrong entry")

        for c in ip:
            if c not in cur.children:
                if self.defaultPort:
                    return self.defaultPort
                return -1
            cur = cur.children[c]
            if cur.port:
                return cur.port

if __name__ == "__main__":
    ip_table = PrefixTree()

    # Part 1
    ip_table.insert("10", 3)
    ip_table.insert("010", 4)
    ip_table.insert("1111", 5)
    print(ip_table.lookup("101110"))
    print(ip_table.lookup("010110"))
    print(ip_table.lookup("111111"))

    # Part 2
    print(ip_table.lookup("000000")) # -1
    ip_table.setDefaultPort(999)
    print(ip_table.lookup("000000")) # 999

    # Part 3
    ip_table.insert("34.126", 301)
    ip_table.insert("34.120", 120)
    ip_table.setDefaultPort(0)
    print(ip_table.lookup("34.126.7.1")) # = 301
    print(ip_table.lookup("34.120.8.1")) # = 120
    print(ip_table.lookup("192.168.0.1")) # = -1
