# wordcount

Counts occurrences of words in a text

Requirements
============
* Requires the following libraries:
  crypto ssl boost_regex boost_iostreams boost_context

* Requires cmake3 (I have version 3.17.3)

* The python script requires python3.6

Build instructions:
==================
Go to your git root (i.e. above wordcount) and:
  
git clone https://github.com/pgholm61/wordcount.git
cd wordcount
mkdir BUILD
cd BUILD
cmake3 ..
make

