#!/usr/bin/python
import sys
import os
import random
import string
import sqlite3

CREATE = """
CREATE TABLE IF NOT EXISTS hugetable (
   id   INTEGER PRIMARY KEY AUTOINCREMENT,
   weirdtext   TEXT,
   crazynumber REAL
);
"""

def main():
    if len(sys.argv) != 2:
        sys.exit("please specify the db filename")

    with sqlite3.connect(sys.argv[1]) as c:
        c.executescript(CREATE)

        rowcount = 1000000
        for i in range(rowcount):
            text = "".join( [random.choice(string.ascii_letters) for i in range(200)] )
            num = random.random() * random.randint(0, 2930)
            c.execute("INSERT INTO hugetable(weirdtext, crazynumber) VALUES ( :t, :n);", {"t": text, "n": num})
            if i % 1000 == 0:
                print("inserted", i, "of", rowcount)
        c.commit()


if __name__ == "__main__":
    main()
