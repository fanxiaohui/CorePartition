#//               GNU GENERAL PUBLIC LICENSE
#//                Version 3, 29 June 2007
#//
#//Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
#//Everyone is permitted to copy and distribute verbatim copies
#//of this license document, but changing it is not allowed.
#//
#//Preamble
#//
#//The GNU General Public License is a free, copyleft license for
#//software and other kinds of works.
#//
#//The licenses for most software and other practical works are designed
#//to take away your freedom to share and change the works.  By contrast,
#//the GNU General Public License is intended to guarantee your freedom to
#//share and change all versions of a program--to make sure it remains free
#//software for all its users.  We, the Free Software Foundation, use the
#//GNU General Public License for most of our software; it applies also to
#//any other work released this way by its authors.  You can apply it to
#//your programs, too.
#//
#// See LICENSE file for the complete information

CC=gcc
src = $(wildcard *.c)
srcpp = $(wildcard *.cpp)
obj = $(src:.c=.o) $(srcpp:.cpp=.o)

TARGET = CorePartition
LDFLAGS = -lm -L /opt/local/lib -I /opt/local/include

$(TARGET): $(obj)
	$(CC) -c $@ $^ (LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(TARGET)
